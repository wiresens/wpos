/***************************************************************************
                          barcore.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
***************************************************************************/

#include "barcore.h"
#include <string>

//the database module for barcore
#include "database/barcoredb.h"
#include "extracore.h"

#include "auth/authcore.h"
#include "barcore/dbusreceiptquerier.h"
#include "printer/printermanager.h"

#include <wposcore/utility.h>
#include <wposcore/config.h>
#include <wposcore/genericsignalmanager.h>
#include <xmlconfig.h>

#include <QString>
#include <QFile>
#include <QDateTime>
#include <QDataStream>
#include <string>

//extern "C"{
//     #include <unistd.h>
#include <cstdlib>
#include <cstdio>
//}

#include <iostream>
using namespace std;

extern AuthCore *auth;

extern QString CASHBOX_DEVICE;
extern QString CASHBOX_TYPE;

#define TMP_FILE_CORE "/tmp/bar_core.xml"

BarCore::BarCore(QObject *parent, const QString& name):
    QObject(parent)
{
    extra_core = new ExtraCore(this, "extra_core");
    receipt_com = new DBusReceiptQuerier(this);
    setObjectName(name);

    auto gsm = GenericSignalManager::instance();

        gsm->subscribeToGenericDataSignal(GDATASIGNAL::USER_CHANGED, this);
        gsm->subscribeToGenericDataSignal(GDATASIGNAL::BARCORE_PROCESS_CORE, this);
        gsm->subscribeToGenericDataSignal(GDATASIGNAL::BARCORE_SEND_PRODUCT, this);
        gsm->subscribeToGenericDataSignal(GDATASIGNAL::BARCORE_CHANGE_XML, this);
        gsm->subscribeToGenericDataSignal(GDATASIGNAL::BARCORE_PRINT_ORDER_AT_SPECIAL_PRINTER, this);

        gsm->subscribeToGenericSignal(GSIGNAL::BARCORE_EXIT_AND_SAVE_RECEIPT, this);
        gsm->subscribeToGenericSignal(GSIGNAL::BARCORE_SET_LAST_RECEIPT, this);
        gsm->subscribeToGenericSignal(GSIGNAL::BARCORE_DELETE_ACTUAL_RECEIPT, this);

        gsm->publishGenericSignal(GSIGNAL::WRONG_PRODUCT, this);

}

BarCore::~BarCore(){
    resetCore();
    delete db;
    delete receipt_com;
}

void BarCore::initCore(bool flush_last){
    if (db){
        db->disConnect();
        delete db;
        db = nullptr;
    }
    db = new BarCoreDB("BarCoreConnection", Files::configFilePath("database"));
    if (!db->connect())
        cerr << "Error trying to establish connection"<< endl;
    delete xml;
    xml = 0;

    if (flush_last){
        last_employee_id ="";
        last_start_time = "";
    }

    xml = new XmlConfig();

    setEmployeeInfo();
    setLocationInfo();

    //set the first timestamp at the date and time that this command was created
    setTimeStamp("start");
    setTimeStamp("last");

    emit dataChanged(xml);
}

bool BarCore::resetCore(){
    if (xml){
        xml->delDomain();
        delete xml;
        xml = 0;
    }
    db->disConnect();
    delete db;
    db=0;

    last_employee_id ="";
    last_start_time = "";

    return true;
}


/**
*    should get from the autentication module the employee info
*    nowdays it should only fill the employee section
*/
void BarCore::setEmployeeInfo(){

    xml->pushDomain();
    xml->delDomain();
    while (xml->howManyTags("employee") != 0)
        xml->deleteElement("employee");

    QString name = auth->getUserName()+" "+auth->getUserLastName();

    xml->createElementSetDomain("employee");
    xml->createElement("name", name);
    xml->createElement("dni", auth->getUserId());

    xml->delDomain();
    xml->popDomain();
}

/**
*    Should get from the location method (table location and place location) info
*    nowdays it should only fill the location section
*/
void BarCore::setLocationInfo(){

    xml->pushDomain();
    xml->delDomain();

    while ( xml->howManyTags("location") != 0 )
        xml->deleteElement("location");

    xml->createElementSetDomain("location");
    xml->createElement("lounge", "venta directa");
    xml->createElement("table", "0");
    xml->delDomain();
    xml->popDomain();
}

void BarCore::setTimeStamp(const QString& name){

    if ( !xml->setDomain("timestamps") )
        xml->createElementSetDomain("timestamps");

    while (xml->howManyTags(name))
        xml->deleteElement(name);

    auto timestamp = currentDateTimeString();
    xml->createElement(name, timestamp);
    xml->releaseDomain("timestamps");
}

void BarCore::receiveProduct(XmlConfig *_xml){


    //prepare the product reception.
    //After working with _xml we should free the mem (product class get mem and here we free it).
    if (!_xml)  return;

    XmlConfig *product = _xml;
    if (setProduct(product)) emit dataChanged(xml);
    else  emit genericSignal(GSIGNAL::WRONG_PRODUCT);
}

void BarCore::receiveProduct(XmlConfig *_xml,int number){
    XmlConfig *product = 0;
    int count;

    //prepare the product reception.
    //After working with _xml we should free the mem (product class get mem and here we free it).
    if (!_xml) return;

    product = _xml;
    if (!setProduct(product)){
        emit genericSignal(GSIGNAL::WRONG_PRODUCT);
        return;
    }


    //make the copy from an
    product = new XmlConfig();
    product->delDomain();
    xml->delDomain();
    xml->setDomain("products");
    count = xml->howManyTags("product");
    product->copy(xml,"product["+QString::number(count-1)+"]","product");
    product->delDomain();
    xml->delDomain();
    //product copy to the aux and now rollback
    for (int i=0 ; i < number - 1; i++){
        xml->copy(product,"product","products.product["+QString::number(count+i)+"]");
    }
    emit dataChanged(xml);
    delete product;
}

bool BarCore::setProduct(XmlConfig *product){
    QString product_code;
    xml->pushDomain();
    product->delDomain();
    //if the xml have no product or no product.articles delete the xml and return.
    if ((!product->setDomain("product"))||(!product->howManyTags("articles"))){
        return false;
    }

    //set the last timestamp to the actual date
    this->setTimeStamp("last");

    xml->delDomain();
    if (!xml->setDomain("products")){
        xml->createElementSetDomain("products");
    }


    xml->createElementSetDomain("product");
    this->setTimeStamp("ordertime");

    product_code = getProductCode(product);

    /*
    //This function calls a function
    // BareCoreDB::getProductAtPrinter(product, printer) which makes access to a the table
    // "product_printer_types" which is not present in the database we downloaded.
    //This version of code version 1.2rc2 is not provided with the appropriate database, but with the
    //database of code version 1.2rc1 which does not have that table. We will investigate in the future
    //what was the rationale for that. Presently we will just disable this line of code.
    the call sequence is:
                    -> BareCore::setProductPrinterSection(const QString& product)
                        -> BareCoreDB::getProductAtPrinter(const QString& product, const QString& printer)
    */
    //setProductPrinterSection(product_code);

    //get the product name
    setProductName(product,product_code);

    if (!setOptions(product,product_code)){
        int last;
        xml->releaseDomain("product");
        last = xml->howManyTags("product");
        xml->deleteElement("product["+QString::number(last-1)+"]");
        xml->delDomain();
        extra_core->clearOptionList();
        return false;
    }

    if (!checkOffers(xml,product_code)){
        int last;
        xml->releaseDomain("product");
        last = xml->howManyTags("product");
        xml->deleteElement("product["+QString::number(last-1)+"]");
        xml->delDomain();
        extra_core->clearOptionList();
        return false;
    }

    //get the price by checking the product and all the options.
    this->setPrice(product,product_code);

    //copy the product description
    xml->copy(product,"articles","articles");
    xml->releaseDomain("products");
    xml->delDomain();

    return true;
}

void BarCore::dataChangedSlot(XmlConfig *_xml){
    int count,i;
    XmlConfig *aux_xml=0;
    if (!_xml)
        return;

    delete xml;

    xml = new XmlConfig();
    xml->readXmlFromString(_xml->toString());

    this->initExtras();
    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("products");
    if (xml->setDomain("defaultoptions")){
        count = xml->howManyTags("option");
        for(i=0;i<count;i++){
            QString type,value;
            type = xml->readString("option["+QString::number(i)+"].type");
            value = xml->readString("option["+QString::number(i)+"].value");
            if ((!type.isEmpty())&&(!value.isEmpty())){
                aux_xml = new XmlConfig();
                aux_xml->createElement("type",type);
                aux_xml->createElement("value",value);
                extra_core->readFixedOptionFromXml(aux_xml);
                delete aux_xml;
            }
        }
    }
    xml->delDomain();
    xml->popDomain();
}

QString BarCore::getProductCode(XmlConfig *xml){

    //check the name at the list of articles
    xml->pushDomain();

    auto count = xml->howManyTags("article","articles");
    QString sort_aux;
    for (auto i = 0 ; i < count; i++){
        sort_aux+= xml->readString("articles.article["+QString::number(i)+"].name");
        if (i != count-1 ) sort_aux+=" ";
    }

    xml->popDomain();
    return sort_aux.simplified();
}

/**
*    all this method fill the xml with the product specification.
*    most of them have db access. and set the data to the
*    proper fields at the xml
*/
void BarCore::setProductName(XmlConfig *_xml,const QString& product_code){
    QString aux;
    XmlConfig *product = 0;
    //first we should check the DB product to see if it has an alias
    //or name, if it's empty or it has no alias, a shorted list with each
    //article should be provided

    //check the name at the list of articles
    product = _xml;
    product->pushDomain();

    aux = db->getName(product_code);

    if (aux.isEmpty())
        xml->createElement("name", product_code);
    else
        xml->createElement("name",aux);

    product->popDomain();
}

/**
*    should check the following things in the following order
*    customizable prize (from the customizable modules)
*    normal price (from the database)
*    options (to add at the price if the normal price has been selected)
*    should set taxes.
*/


bool BarCore::setOptions(XmlConfig *_xml,const QString& product_code){
    int count,i;
    XmlConfig *product=0;
    QStringList option_list;
    QString option_type;
    QString option_value;
    QString aux;

    //merge the options of the product received by the xml  and received by the
    //customizable class and set it at the xml.
    product = _xml;
    product->setDomain("options");


    option_list = extra_core->getFixedOptionsNames();
    count = option_list.count();
    for(i=0;i<count;i++){
        option_type = option_list[i];
        option_value = extra_core->getFixedOptionValue(option_type);
        if (!db->checkOption(product_code,option_type,option_value))
            continue;
        for(int j=0;j<(int) product->howManyTags("option");j++){
            aux = product->readString("option["+QString::number(j)+"].type");
            if (aux == option_type){
                product->deleteElement("option["+QString::number(j)+"].value");
                product->createElement("option["+QString::number(j)+"].value",option_value);
            }
        }//product for
    }//option for

    option_list = extra_core->getOptionsNames();
    count = option_list.count();
    for(i=0;i<count;i++){
        option_type = option_list[i];
        option_value = extra_core->getOptionValue(option_type);
        if (!db->checkOption(product_code,option_type,option_value))
            return false;
        for(int j=0;j<(int) product->howManyTags("option");j++){
            aux = product->readString("option["+QString::number(j)+"].type");
            if (aux == option_type){
                product->deleteElement("option["+QString::number(j)+"].value");
                product->createElement("option["+QString::number(j)+"].value",option_value);
            }
        }//product for
    }//option for
    product->releaseDomain("options");
    xml->copy(product,"options","options");
    extra_core->clearOptionList();
    return true;
}

bool BarCore::checkOffers(XmlConfig *_xml,const QString& product_code){
    OfferData *offer = 0;
    OfferData *offer_db = 0;
    XmlConfig *product=0;
    product = _xml;
    if (!product)
        return false;
    //this method should only try to connect to the database and if there are offers at
    //extra core it should check if the product_code has the extra_offer.
    offer = extra_core->getOffer();

    //if there are no offers all's ok
    if (!offer)
        return true;

    //now check that offer for that product
    if (!(offer_db = db->getOffer(product_code,offer->offer_type,offer->offer_name)))
        return false;
    else{
        product->createElement("offer.type",offer->offer_type);
        product->createElement("offer.name",offer->offer_name);
        delete offer_db;
    }
    return true;
}

void BarCore::setPrice(XmlConfig *_xml,const QString& product_code){
    int i,count;
    ProductPriceData *prod_price = 0;
    OfferData *offer_data = 0;
    XmlConfig *product;
    QString mod,option_value,option_type;
    double modifier;
    double r_price;
    product = _xml;
    if (!product)
        return;

    //should check the customizable price and taxes
    if ( !( prod_price = extra_core->getPrice() ) ){
        //if there are no customizable price, the price is set at the db
        //and is modified by the option of the product
        prod_price = new ProductPriceData;
        prod_price->price = db->getPrice(product_code);
        if ((prod_price->price).isEmpty())
            prod_price->price = "0.0";
        prod_price->tax_type = db->getTaxName(product_code);
        prod_price->tax = db->getTax(product_code);
        if (prod_price->tax.isEmpty())
            prod_price->tax="0.00";

        //should check the options
        r_price = (prod_price->price).toDouble();
        xml->setDomain("options");
        count = xml->howManyTags("option");
        for(i=0;i<count;i++){
            option_type = xml->readString("option["+QString::number(i)+"].type");
            option_value = xml->readString("option["+QString::number(i)+"].value");
            mod = this->db->getOptionModifier(product_code,option_type,option_value);
            if (!mod.isEmpty()){
                modifier = mod.toDouble();
                r_price += modifier;
            }
        }
        xml->releaseDomain("options");
        prod_price->price = QString::number(r_price);

        //also the special offers at the extra Core should be readed
        if ((offer_data=extra_core->getOffer())){
            offer_data = db->getOffer(product_code,offer_data->offer_type,offer_data->offer_name);
            if (offer_data->offer_mode == "x"){
                double discount = (offer_data->offer_discount).toDouble();
                //FIXME   FIX THE ROUND TO 2 DECIMALS
                prod_price->price = QString::number((double) (prod_price->price).toDouble() * discount );
            }
            else if (offer_data->offer_mode == "="){
                prod_price->price = offer_data->offer_discount;
            }
            extra_core->clearOffer();
        }
    }

    r_price = (prod_price->price).toDouble();
    r_price = extra_core->getModifiedPrice(r_price);
    prod_price->price = QString::number(r_price);


    xml->createElement("billing.price",prod_price->price);
    xml->createElement("billing.currency","euro");
    xml->createElement("billing.tax",prod_price->tax);
    if (!prod_price->tax_type.isEmpty())
        xml->createAttribute("billing.tax","type",prod_price->tax_type);


    //be sure to clean the customizable price
    extra_core->clearPrice();
}

bool BarCore::hasProducts(){
    return hasProducts(xml);
}

bool BarCore::hasProducts(XmlConfig *_xml){
    int count;

    if (!_xml)
        return false;

    _xml->pushDomain();
    _xml->delDomain();

    if (!_xml->setDomain("products")){
        _xml->popDomain();
        return false;
    }

    count = _xml->howManyTags("product");
    if (!count){
        _xml->popDomain();
        return false;
    }
    _xml->popDomain();
    return true;
}

void BarCore::setDescription(const QString& description){
    xml->pushDomain();
    xml->delDomain();
    xml->doWrite("description",description);
    xml->popDomain();
    emit dataChanged(xml);
}


double BarCore::getBillingSum(){
    int count,i;
    double ret = 0;
    double parcial;
    QString aux;
    bool ok;
    xml->pushDomain();
    xml->delDomain();

    if (!xml->setDomain("products")){
        xml->popDomain();
        return ret;
    }

    count = xml->howManyTags("product");
    if (count==0){
        xml->popDomain();
        return ret;
    }
    else
        ret = 0;

    for(i=0;i<count;i++){
        aux = xml->readString("product["+QString::number(i)+"].billing.price");
        parcial = aux.toDouble(&ok);
        if (!ok)
            continue;
        ret += parcial;
    }
    xml->popDomain();
    return ret;
}

void BarCore::processCore(const QString& pay_type){

    xml->delDomain();
    if (! xml->howManyTags("product", "products") ) return;

    //set the last timestamp to the actual timestamp
    setTimeStamp("last");

    xml->delDomain();
    xml->createElement("paytype", pay_type);

    //delete the command from the receipt module
    xml->pushDomain();
    xml->delDomain();
    auto employee_id = xml->readString("employee.dni");
    auto start_time = xml->readString("timestamps.start");
    xml->delDomain();
    xml->popDomain();

    if ( receipt_com->receiptExist(employee_id, start_time) )
        receipt_com->deleteReceipt(employee_id, start_time);

    emit ticket(xml);
    initCore();

    const char* device_node = CASHBOX_DEVICE.toStdString().c_str();
    FILE *device = fopen(device_node, "w");
    auto error_msg = "BarCore::processCore(" + pay_type.toStdString() + ")\n";
    error_msg += std::string("Problem while opening the Cash Box device at node " + std::string(device_node));
    if (!device) std::perror(error_msg.c_str());
    else{
        if ( CASHBOX_TYPE == "cash_drawer"  || CASHBOX_TYPE == "p_samsung_350" ){
            char c[] = { 0x1b, 0x70, 0x00, 0x30, 0x30 };
            fwrite( c, sizeof(char), 5 , device );
        }
        else{
            char c[] = { 0x0A };
            fwrite(c, sizeof(char) , 1, device);
        }

        fflush(device);
        fclose(device);
        qDebug("BarCore::processCore : Cash Box Opened and Closed Succesfully");
    }
}

void BarCore::initExtras(){
    extra_core->clearExtras();
}

void BarCore::saveLastReceipt(){
//    DCOPClient *client= 0;
//    client = kapp->dcopClient();
    //only to use at the moment.
    //this code is going to be deleted from here

//    if (!client->isAttached())
//        client->attach();

    //if the dbusreceipt is not running  exit without saving
//    if (!client->isApplicationRegistered("dbusreceipt"))
//        return;

    //get some data of the receipt
    xml->pushDomain();
    xml->delDomain();
    last_employee_id = xml->readString("employee.dni");
    last_start_time = xml->readString("timestamps.start");
    xml->delDomain();
    xml->popDomain();
}

void BarCore::setLastReceipt(){
    XmlConfig *aux_xml = 0;

    if ((last_employee_id.isEmpty()) || (last_start_time.isEmpty()))
        return;

    if (receipt_com->receiptState(last_employee_id, last_start_time)){
        this->initCore();
        this->initExtras();
        return;
    }

    aux_xml = receipt_com->getReceipt(last_employee_id,last_start_time);
    if (!aux_xml){
        this->initCore();
        this->initExtras();
        return;
    }
    this->dataChangedSlot(aux_xml);
    delete aux_xml;
    receipt_com->lockRemoteReceipts(last_employee_id, last_start_time);
    emit dataChanged(xml);
}

bool BarCore::exitAndSaveReceipt(){
//    DCOPClient *client= 0;
    bool exist = false;
    bool ret = false;
    QString employee_id;
    QString start_time;

//    client = kapp->dcopClient();
//    if (!client->isAttached())
//        client->attach();
//    if (!client->isApplicationRegistered("dbusreceipt"))
//        return false;

    xml->pushDomain();
    xml->delDomain();
    employee_id = xml->readString("employee.dni");
    start_time = xml->readString("timestamps.start");
    xml->delDomain();
    xml->popDomain();

    exist = receipt_com->receiptExist(employee_id, start_time);
    if (!exist){
        if ( !hasProducts() ) return false;
        ret = receipt_com->createReceipt(xml);
    }
    else
        ret = receipt_com->saveReceipt(xml);

    saveLastReceipt();
    receipt_com->unlockRemoteReceipts(employee_id, start_time);
    return ret;
}

void BarCore::genericSignalSlot(const QString& signal_name){
    if (signal_name == GSIGNAL::BARCORE_EXIT_AND_SAVE_RECEIPT){
        this->exitAndSaveReceipt();
        this->initCore(false);
        this->initExtras();
    }
    else if ( signal_name ==GSIGNAL::BARCORE_SET_LAST_RECEIPT){
        this->initCore(false);
        this->initExtras();
        this->setLastReceipt();
    }
    else if ( signal_name ==GSIGNAL::BARCORE_DELETE_ACTUAL_RECEIPT){
        QString employee_id;
        QString start_time;
        xml->pushDomain();
        xml->delDomain();
        employee_id = xml->readString("employee.dni");
        start_time = xml->readString("timestamps.start");
        xml->delDomain();
        xml->popDomain();
        receipt_com->deleteReceipt(employee_id,start_time);
    }
}

void BarCore::genericDataSignalSlot(const QString& signal_name, XmlConfig *_xml){
    if ( signal_name == GDATASIGNAL::USER_CHANGED ){
        setEmployeeInfo();
        emit dataChanged(xml);
    }
    else if ( signal_name == GDATASIGNAL::BARCORE_PROCESS_CORE ){
        xml->delDomain();
        auto pay_type = _xml->readString("pay_type");
        processCore(pay_type);
    }
    else if (signal_name == GDATASIGNAL::BARCORE_SEND_PRODUCT){
        receiveProduct(_xml);
        emit dataChanged(xml);
    }
    else if (signal_name == GDATASIGNAL::BARCORE_CHANGE_XML){
        dataChangedSlot(_xml);
        emit dataChanged(xml);
    }
    else if (signal_name == GDATASIGNAL::BARCORE_CHANGE_ORDER_DESCRIPTION){
        QString aux;
        _xml->pushDomain();
        _xml->delDomain();
        aux = _xml->readString("description");
        if (!aux.isEmpty())
            this->setDescription(aux);
        _xml->popDomain();
    }
    else if (signal_name == GDATASIGNAL::BARCORE_PRINT_ORDER_AT_SPECIAL_PRINTER){
        QString aux;
        _xml->pushDomain();
        _xml->delDomain();
        aux = _xml->readString("type");
        if (!aux.isEmpty())
            sendCurrentOrderToKitchen();
        _xml->popDomain();
    }
}

void BarCore::setProductPrinterSection(const QString& product_code){
    if (db->getProductAtPrinter(product_code, "kitchen")){
        xml->createElementSetDomain("printer");
        xml->createAttributeHere("type","kitchen");
        xml->releaseDomain("printer");
    }
}

/**
        -create a copy of the current order
        -delete from the copy all the products without <printer type="kitchen" \> tag
        -tag at the current order all the product with the printer tag with a new attribute "printed=true"
        -create a printer
        -use the printer->kitchenMethod

*/
void BarCore::sendCurrentOrderToKitchen(){
    XmlConfig *kitchen = 0;
    int i,count;
    //int j,attr_count;
    QStringList num_list;
    PrinterManager *printer=0;

    if (!this->hasProducts())
        return;

    xml->pushDomain();

    kitchen = new XmlConfig();
    kitchen->readXmlFromString(xml->toString());

    kitchen->delDomain();
    kitchen->setDomain("products");
    count =kitchen->howManyTags("product");
    for (i=0;i<count;i++){
        kitchen->setDomain("product["+QString::number(i)+"]");
        /**
        FIXME At the moment there is only a printer type so, we should only check for this tag
        In the future, with a lot of printer tags, this method must check all printer tags and check
        that a printer tag has the kitchen attribute;
*/
        if (kitchen->howManyTags("printer")==0){
            num_list.prepend(QString::number(i));
        }
        kitchen->releaseDomain("product");
    }

    count = num_list.count();
    for (i=0;i<count;i++){
        kitchen->deleteElement("product["+num_list[i]+"]");
    }

    kitchen->delDomain();
    if (!(hasProducts(kitchen)))
        return;

    xml->delDomain();
    xml->setDomain("products");
    count =xml->howManyTags("product");
    for (i=0;i<count;i++){
        xml->setDomain("product["+QString::number(i)+"]");
        /**
        FIXME At the moment there is only a printer type so, we should only check for this tag
        In the future, with a lot of printer tags, this method must check all printer tags and check
        that a printer tag has the kitchen attribute;
*/
        if (kitchen->howManyTags("printer")==0){
            xml->createAttribute("printer","printed","true");
        }
        xml->releaseDomain("product");
    }
    xml->delDomain();
    xml->popDomain();

    emit dataChanged(xml);

    printer = new PrinterManager(this);
    printer->printKitchenOrder(kitchen);
    delete printer;
    delete kitchen;

}

void BarCore::debug(){
    QString aux;
    aux = xml->toString();
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "                          XML DEL CORE                "<< endl;
    cout << aux.toStdString();
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
}
