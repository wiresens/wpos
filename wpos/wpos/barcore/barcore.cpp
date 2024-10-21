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
#include <libbslxml/xmlconfig.h>

#include <QString>
#include <QFile>
#include <QDateTime>
#include <QDataStream>
#include <string>

#include <iostream>
using namespace std;

extern AuthCore *authCore;
extern QString CASHBOX_DEVICE;
extern QString CASHBOX_TYPE;

// BarCoreDB BarCore::m_db{"BarCoreConnection", cfg::xmlFileByKey(cfg::XMLKey::Database)};

BarCore::BarCore( QObject *parent, const QString& name):
    QObject(parent),
    m_db{ new BarCoreDB(QString("BarCoreConnection"),
    cfg::xmlFileByKey(cfg::XMLKey::Database))}
{
    m_extra_core = new ExtraCore(this, "ExtraCore");
    m_receipt_querier = new DBusReceiptQuerier(this);
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
    if (! m_db->connect()) qDebug() << m_db->lastError();
}

BarCore::~BarCore(){
    if (m_xml){
        m_xml->delDomain();
        delete m_xml;
    }    
    m_db->disConnect();
    delete m_db;
}

void BarCore::reInitialise(bool flush_last){
    if (!m_db->isConnected())
        qDebug() << "Error trying to establish " << m_db->connectionName();
    delete m_xml;
    m_xml = 0;

    if (flush_last){
        m_last_employee_id.clear();
        m_last_start_time.clear();
    }

    m_xml = new XmlConfig();

    setEmployeeInfo(*m_xml);
    setLocationInfo(*m_xml);

    //set the first timestamp at the date and time that this command was created
    setTimeStamp("start");
    setTimeStamp("last");

    emit orderChanged(m_xml);
}

void BarCore::resetCore(){
    if (m_xml){
        m_xml->delDomain();
        delete m_xml;
        m_xml = 0;
    }

    m_last_employee_id.clear();
    m_last_start_time.clear();
}


/**
*    should get from the autentication module the employee info
*    nowdays it should only fill the employee section
*/
void BarCore::setEmployeeInfo(XmlConfig &xml){

    xml.pushDomain();
    xml.delDomain();
    while (xml.howManyTags("employee") != 0)
        xml.deleteElement("employee");

    QString name = authCore->userName()+" "+authCore->userLastName();

    xml.createElementSetDomain("employee");
    xml.createElement("name", name);
    xml.createElement("dni", authCore->userId());

    xml.delDomain();
    xml.popDomain();
}

/**
*    Should get from the location method (table location and place location) info
*    nowdays it should only fill the location section
*/
void BarCore::setLocationInfo(XmlConfig &xml){

    xml.pushDomain();
    xml.delDomain();

    while ( xml.howManyTags("location") != 0 )
        xml.deleteElement("location");

    xml.createElementSetDomain("location");
    xml.createElement("lounge", "venta directa");
    xml.createElement("table", "0");
    xml.delDomain();
    xml.popDomain();
}

void BarCore::setTimeStamp(const QString& name){

    if ( !m_xml->setDomain("timestamps") )
        m_xml->createElementSetDomain("timestamps");

    while (m_xml->howManyTags(name))
        m_xml->deleteElement(name);

    auto timestamp = currentDateTimeString();
    m_xml->createElement(name, timestamp);
    m_xml->releaseDomain("timestamps");
}

void BarCore::receiveProduct(XmlConfig *xml){
    //prepare the product reception.
    //After working with _xml we should free the mem (product class get mem and here we free it).
    if (!xml)  return;

    XmlConfig *product = xml;
    if (setProduct(product)) emit orderChanged(m_xml);
    else  emit genericSignal(GSIGNAL::WRONG_PRODUCT);
}

void BarCore::receiveProduct(XmlConfig *xml, int number){
    XmlConfig *product{};
    int count;

    //prepare the product reception.
    //After working with _xml we should free the mem (product class get mem and here we free it).
    if (!xml) return;

    product = xml;
    if (!setProduct(product)){
        emit genericSignal(GSIGNAL::WRONG_PRODUCT);
        return;
    }

    //make the copy from an
    product = new XmlConfig();
    product->delDomain();
    m_xml->delDomain();
    m_xml->setDomain("products");
    count = m_xml->howManyTags("product");
    product->copy(m_xml, "product["+QString::number(count - 1)+"]","product");
    product->delDomain();
    m_xml->delDomain();
    //product copy to the aux and now rollback
    for (int i=0 ; i < number - 1; i++){
        m_xml->copy(product, "product","products.product["+QString::number(count + i)+"]");
    }
    emit orderChanged(m_xml);
    delete product;
}

bool BarCore::setProduct(XmlConfig *product){
    QString product_code;
    m_xml->pushDomain();
    product->delDomain();
    //if the xml have no product or no product.articles delete the xml and return.
    if ( !product->setDomain("product") ||
        !product->howManyTags("articles")){
        return false;
    }

    //set the last timestamp to the actual date
    setTimeStamp("last");

    m_xml->delDomain();
    if (!m_xml->setDomain("products")){
        m_xml->createElementSetDomain("products");
    }

    m_xml->createElementSetDomain("product");
    setTimeStamp("ordertime");

    product_code = getProductCode(product);

    /* @benes
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
        m_xml->releaseDomain("product");
        last = m_xml->howManyTags("product");
        m_xml->deleteElement("product["+QString::number(last-1)+"]");
        m_xml->delDomain();
        m_extra_core->clearOptionList();
        return false;
    }

    if (!checkOffers(m_xml,product_code)){
        int last;
        m_xml->releaseDomain("product");
        last = m_xml->howManyTags("product");
        m_xml->deleteElement("product["+QString::number(last-1)+"]");
        m_xml->delDomain();
        m_extra_core->clearOptionList();
        return false;
    }

    //get the price by checking the product and all the options.
    setPrice(product, product_code);

    //copy the product description
    m_xml->copy(product,"articles","articles");
    m_xml->releaseDomain("products");
    m_xml->delDomain();

    return true;
}

void BarCore::addProductDefaultOption(XmlConfig *_xml){
    if (!_xml) return;

    delete m_xml;
    m_xml = new XmlConfig();
    m_xml->readXmlFromString(_xml->toString());

    initExtras();
    m_xml->pushDomain();
    m_xml->delDomain();
    m_xml->setDomain("products");
    if ( m_xml->setDomain("defaultoptions") ){
        for( auto i=0; i < m_xml->howManyTags("option"); i++){
            auto type = m_xml->readString("option["+QString::number(i)+"].type");
            auto value = m_xml->readString("option["+QString::number(i)+"].value");
            if ( !type.isEmpty() && !value.isEmpty()){
                XmlConfig optionXml;
                optionXml.createElement("type", type);
                optionXml.createElement("value", value);
                m_extra_core->addProductOption(&optionXml);
            }
        }
    }
    m_xml->delDomain();
    m_xml->popDomain();
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
*    all this method fill the m_xml with the product specification.
*    most of them have m_db access. and set the data to the
*    proper fields at the m_xml
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

    aux = m_db->getName(product_code);

    if (aux.isEmpty())
        m_xml->createElement("name", product_code);
    else
        m_xml->createElement("name",aux);

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


    option_list = m_extra_core->getFixedOptionsNames();
    count = option_list.count();
    for(i=0;i<count;i++){
        option_type = option_list[i];
        option_value = m_extra_core->getFixedOptionValue(option_type);
        if (!m_db->checkOption(product_code,option_type,option_value))
            continue;
        for(int j=0;j<(int) product->howManyTags("option");j++){
            aux = product->readString("option["+QString::number(j)+"].type");
            if (aux == option_type){
                product->deleteElement("option["+QString::number(j)+"].value");
                product->createElement("option["+QString::number(j)+"].value",option_value);
            }
        }//product for
    }//option for

    option_list = m_extra_core->getOptionsNames();
    count = option_list.count();
    for(i=0;i<count;i++){
        option_type = option_list[i];
        option_value = m_extra_core->getOptionValue(option_type);
        if (!m_db->checkOption(product_code,option_type,option_value))
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
    m_xml->copy(product,"options","options");
    m_extra_core->clearOptionList();
    return true;
}

bool BarCore::checkOffers(XmlConfig *_xml, const QString& product_code){
    XmlConfig *product = _xml;
    if (!product)  return false;
    //this method should only try to connect to the database and if there are offers at
    //extra core it should check if the product_code has the extra_offer.
    OfferData *offer = m_extra_core->getOffer();

    //if there are no offers all's ok
    if (!offer)  return true;

    OfferData offer_db =  m_db->getOffer(product_code, offer->offer_type, offer->offer_name);
    //now check that offer for that product
    if (offer_db.isNull())
        return false;
    else{
        product->createElement("offer.type",offer->offer_type);
        product->createElement("offer.name",offer->offer_name);
    }
    return true;
}

void BarCore::setPrice(XmlConfig *_xml,const QString& product_code){
    int i,count;
    ProductPriceData *prod_price = 0;
    XmlConfig *product;
    QString mod,option_value,option_type;
    double modifier;
    double r_price;
    product = _xml;
    if (!product)
        return;

    //should check the customizable price and taxes
    if ( !( prod_price = m_extra_core->getPrice() ) ){
        //if there are no customizable price, the price is set at the db
        //and is modified by the option of the product
        prod_price = new ProductPriceData;
        prod_price->price = m_db->getPrice(product_code);
        if ((prod_price->price).isEmpty())
            prod_price->price = "0.0";
        prod_price->tax_type = m_db->getTaxName(product_code);
        prod_price->tax = m_db->getTax(product_code);
        if (prod_price->tax.isEmpty())
            prod_price->tax="0.00";

        //should check the options
        r_price = (prod_price->price).toDouble();
        m_xml->setDomain("options");
        count = m_xml->howManyTags("option");
        for(i=0;i<count;i++){
            option_type = m_xml->readString("option["+QString::number(i)+"].type");
            option_value = m_xml->readString("option["+QString::number(i)+"].value");
            mod = this->m_db->getOptionModifier(product_code,option_type,option_value);
            if (!mod.isEmpty()){
                modifier = mod.toDouble();
                r_price += modifier;
            }
        }
        m_xml->releaseDomain("options");
        prod_price->price = QString::number(r_price);

        //also the special offers at the extra Core should be readed
        OfferData* tmp = m_extra_core->getOffer();
        if ( tmp){
            OfferData offer_data = m_db->getOffer(product_code, tmp->offer_type, tmp->offer_name);
            if (offer_data.offer_mode == "x"){
                double discount = (offer_data.offer_discount).toDouble();
                //FIXME   FIX THE ROUND TO 2 DECIMALS
                prod_price->price = QString::number((double) (prod_price->price).toDouble() * discount );
            }
            else if (offer_data.offer_mode == "="){
                prod_price->price = offer_data.offer_discount;
            }
            m_extra_core->clearOffer();
        }
    }

    r_price = (prod_price->price).toDouble();
    r_price = m_extra_core->getModifiedPrice(r_price);
    prod_price->price = QString::number(r_price);

    m_xml->createElement("billing.price",prod_price->price);
    m_xml->createElement("billing.currency","euro");
    m_xml->createElement("billing.tax",prod_price->tax);
    if (!prod_price->tax_type.isEmpty())
        m_xml->createAttribute("billing.tax","type",prod_price->tax_type);


    //be sure to clean the customizable price
    m_extra_core->clearPrice();
}

bool BarCore::hasProducts() const{
    return hasProducts(*m_xml);
}

//bool BarCore::hasProducts(XmlConfig &xml) const{
//    int count;

//    xml.pushDomain();
//    xml.delDomain();

//    if (!xml.setDomain("products")){
//        xml.popDomain();
//        return false;
//    }

//    count = xml.howManyTags("product");
//    if (!count){
//        xml.popDomain();
//        return false;
//    }
//    xml.popDomain();
//    return true;
//}

bool BarCore::hasProducts(XmlConfig &xml) const{
    xml.pushDomain();
    xml.delDomain();
    auto res = xml.setDomain("products") && xml.howManyTags("product");
    xml.popDomain();
    return res;
}

void BarCore::setDescription(const QString& description){
    m_xml->pushDomain();
    m_xml->delDomain();
    m_xml->doWrite("description",description);
    m_xml->popDomain();
    emit orderChanged(m_xml);
}

double BarCore::total(){
    m_xml->pushDomain();
    m_xml->delDomain();

    double sum{};
    if ( m_xml->setDomain("products")){
        bool ok;
        double parcial{};
        for( auto i=0; i < m_xml->howManyTags("product"); i++){
            parcial = m_xml->readString("product["+QString::number(i)+"].billing.price").toDouble(&ok);
            if (!ok)  continue;
            sum += parcial;
        }
    }
    m_xml->popDomain();
    return sum;
}

void BarCore::processCore(const QString& pay_type){

    m_xml->delDomain();
    if (! m_xml->howManyTags("product", "products") ) return;

    //set the last timestamp to the actual timestamp
    setTimeStamp("last");

    m_xml->delDomain();
    m_xml->createElement("paytype", pay_type);

    //delete the command from the receipt module
    m_xml->pushDomain();
    m_xml->delDomain();
    auto employee_id = m_xml->readString("employee.dni");
    auto start_time = m_xml->readString("timestamps.start");
    m_xml->delDomain();
    m_xml->popDomain();

    if ( m_receipt_querier->receiptExist(employee_id, start_time))
        m_receipt_querier->deleteReceipt(employee_id, start_time);

    emit ticket(m_xml);
    reInitialise();

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
    m_extra_core->clearExtras();
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
    m_xml->pushDomain();
    m_xml->delDomain();
    m_last_employee_id = m_xml->readString("employee.dni");
    m_last_start_time = m_xml->readString("timestamps.start");
    m_xml->delDomain();
    m_xml->popDomain();
}

void BarCore::setLastReceipt(){

    if ( m_last_employee_id.isEmpty()  ||
        m_last_start_time.isEmpty() )
        return;

    if ( m_receipt_querier->receiptState(m_last_employee_id, m_last_start_time) )
    {
        reInitialise();
        initExtras();
        return;
    }

    auto xml_receipt = m_receipt_querier->getReceipt(m_last_employee_id,m_last_start_time);
    addProductDefaultOption(&xml_receipt);
    m_receipt_querier->lockRemoteReceipts(m_last_employee_id, m_last_start_time);
    emit orderChanged(m_xml);
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

    m_xml->pushDomain();
    m_xml->delDomain();
    employee_id = m_xml->readString("employee.dni");
    start_time = m_xml->readString("timestamps.start");
    m_xml->delDomain();
    m_xml->popDomain();

    exist = m_receipt_querier->receiptExist(employee_id, start_time);
    if (!exist){
        if ( !hasProducts() ) return false;
        ret = m_receipt_querier->createReceipt(m_xml);
    }
    else
        ret = m_receipt_querier->saveReceipt(m_xml);

    saveLastReceipt();
    m_receipt_querier->unlockRemoteReceipts(employee_id, start_time);
    return ret;
}

void BarCore::genericSignalSlot(const QString& signal){
    if (signal == GSIGNAL::BARCORE_EXIT_AND_SAVE_RECEIPT){
        exitAndSaveReceipt();
        reInitialise(false);
        initExtras();
    }
    else if ( signal ==GSIGNAL::BARCORE_SET_LAST_RECEIPT){
        reInitialise(false);
        initExtras();
        setLastReceipt();
    }
    else if ( signal ==GSIGNAL::BARCORE_DELETE_ACTUAL_RECEIPT){
        QString employee_id;
        QString start_time;
        m_xml->pushDomain();
        m_xml->delDomain();
        employee_id = m_xml->readString("employee.dni");
        start_time = m_xml->readString("timestamps.start");
        m_xml->delDomain();
        m_xml->popDomain();
        m_receipt_querier->deleteReceipt(employee_id,start_time);
    }
}

void BarCore::genericDataSignalSlot(const QString& signal, XmlConfig *xml){
    if ( signal == GDATASIGNAL::USER_CHANGED ){
        setEmployeeInfo(*m_xml);
        emit orderChanged(m_xml);
    }
    else if ( signal == GDATASIGNAL::BARCORE_PROCESS_CORE ){
        m_xml->delDomain();
        auto pay_type = xml->readString("pay_type");
        processCore(pay_type);
    }
    else if (signal == GDATASIGNAL::BARCORE_SEND_PRODUCT){
        receiveProduct(xml);
        emit orderChanged(m_xml);
    }
    else if (signal == GDATASIGNAL::BARCORE_CHANGE_XML){
        addProductDefaultOption(xml);
        emit orderChanged(m_xml);
    }
    else if (signal == GDATASIGNAL::BARCORE_CHANGE_ORDER_DESCRIPTION){
        QString aux;
        xml->pushDomain();
        xml->delDomain();
        aux = xml->readString("description");
        if (!aux.isEmpty())
            this->setDescription(aux);
        xml->popDomain();
    }
    else if (signal == GDATASIGNAL::BARCORE_PRINT_ORDER_AT_SPECIAL_PRINTER){
        QString aux;
        xml->pushDomain();
        xml->delDomain();
        aux = xml->readString("type");
        if (!aux.isEmpty())
            sendCurrentOrderToKitchen();
        xml->popDomain();
    }
}

void BarCore::setProductPrinterSection(const QString& product_code){
    if (m_db->getProductAtPrinter(product_code, "kitchen")){
        m_xml->createElementSetDomain("printer");
        m_xml->createAttributeHere("type","kitchen");
        m_xml->releaseDomain("printer");
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
    if (!hasProducts()) return;
    m_xml->pushDomain();

    XmlConfig kitchenXml;
    kitchenXml.readXmlFromString(m_xml->toString());
    kitchenXml.delDomain();
    kitchenXml.setDomain("products");

    QStringList printers;
    for ( auto i=0; i < kitchenXml.howManyTags("product"); i++){
        kitchenXml.setDomain("product["+QString::number(i)+"]");
        /**
        FIXME At the moment there is only a printer type so, we should only check for this tag
        In the future, with a lot of printer tags, this method must check all printer tags and check
        that a printer tag has the kitchen attribute;
*/
        if (kitchenXml.howManyTags("printer")==0)
            printers.prepend(QString::number(i));
        kitchenXml.releaseDomain("product");
    }

    for (const QString& item : printers)
        kitchenXml.deleteElement("product["+item+"]");

    kitchenXml.delDomain();
    if (! hasProducts(kitchenXml)) return;

    m_xml->delDomain();
    m_xml->setDomain("products");
    for (auto i=0; i < m_xml->howManyTags("product"); i++){
        m_xml->setDomain("product["+QString::number(i)+"]");
        /**
        FIXME At the moment there is only a printer type so, we should only check for this tag
        In the future, with a lot of printer tags, this method must check all printer tags and check
        that a printer tag has the kitchen attribute;
        */
        if (kitchenXml.howManyTags("printer")==0)
            m_xml->createAttribute("printer","printed","true");

        m_xml->releaseDomain("product");
    }
    m_xml->delDomain();
    m_xml->popDomain();

    emit orderChanged(m_xml);

    PrinterManager printer;
    printer.printKitchenOrder(kitchenXml);
}

void BarCore::debug(){
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "                          XML DEL CORE                "<< endl;
    cout << m_xml->toString().toStdString();
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
}
