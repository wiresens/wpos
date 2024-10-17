/***************************************************************************
                          offer.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "productoffermodule.h"
#include "productoffermodule_adaptor.h"

#include "database/productsmoduledb.h"
#include "database/productoffersmoduledb.h"

#include <wposcore/basicdatabase.h>
#include <libbslxml/xmlconfig.h>

#include <iostream>
using namespace std;

static const QString OFFER_CONNECTION_NAME = "OfferConnection";
static const QString OFFER_PATH_FILE = "/etc/ntpv/bar_database.xml";
static const QString& OFFERS_DTD = "/etc/ntpv_backoffice/dtds/products_offerslist.dtd";

const QString ProductOfferModule::DBusObjectPath  = QString{"/wpos/wposbo/DBusBOProductOffer"};

ProductOfferModule::ProductOfferModule(QObject *parent, const QString& name):
    QObject(parent){

    setObjectName(name);
    new DBusBOProductOfferAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(ProductOfferModule::DBusObjectPath, this);
}

bool ProductOfferModule::deleteOffer(const QString& xml_string){
    XmlConfig *xml = 0;
    ProductOffersModuleDB *db = 0;

    xml = new XmlConfig();
    if(!xml)
        return false;

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }
    xml->delDomain();
    xml->setDomain("offers[0].offer[0]");

    if(xml->readString("offer_type").isEmpty()){
        cerr << "empty offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(xml->readString("offer_name").isEmpty()){
        cerr << "empty offer name " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    if(!db){
        cerr << "cannot connect with database " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete db;
        return false;
    }

    if(db->deleteOffer(xml->readString("offer_type"), xml->readString("offer_name"))){
        delete xml;
        delete db;
        return true;
    }
    delete xml;
    delete db;
    return false;
}

bool ProductOfferModule::deleteOfferType(const QString& offer_type){
    ProductOffersModuleDB *db = 0;

    if(offer_type.isEmpty()){
        cerr << "empty offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    if(db->deleteOfferType(offer_type)){
        delete db;
        return true;
    }
    else{
        delete db;
        return false;
    }
}

bool ProductOfferModule::existOffer(const QString& xml_string){
    ProductOffersModuleDB *db = 0;
    XmlConfig *xml = 0;

    xml = new XmlConfig();
    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD)){
        cerr << "xml does not validate against DTD " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    xml->delDomain();
    xml->setDomain("offers[0]");
    xml->setDomain("offer[0]");

    db = 0;
    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);

    if(db->existOffer(xml->readString("offer_type"), xml->readString("offer_name"))){
        delete db;
        delete xml;
        return true;
    }
    delete db;
    delete xml;
    return false;
}

bool ProductOfferModule::existOfferType(const QString& offer_type){
    ProductOffersModuleDB *db = 0;

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    if(db->existOfferType(offer_type)){
        delete db;
        return true;
    }
    else{
        delete db;
        return false;
    }
}

QString ProductOfferModule::getDescriptionOffer(const QString& xml_string){
    XmlConfig *xml = 0;
    ProductOffersModuleDB *db = 0;
    QString description_offer;

    description_offer = "";
    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return description_offer;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return description_offer;
    }
    xml->delDomain();
    xml->setDomain("offers[0].offer[0]");

    db = 0;
    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    description_offer = db->getDescriptionOffer(xml->readString("offer_type"), xml->readString("offer_name"));

    delete db;
    delete xml;
    return description_offer;
}

QString ProductOfferModule::getDescriptionOfferType(const QString& offer_type){
    QString description_type;
    ProductOffersModuleDB *db = 0;

    description_type = "";

    if(offer_type.isEmpty())
        return description_type;

    db = 0;
    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    description_type = db->getDescriptionOfferType(offer_type);

    delete db;
    return description_type;
}

QString ProductOfferModule::getCppOperator(const QString& offer_type){
    ProductOffersModuleDB *db = 0;
    QString cpp_operator;

    cpp_operator = "";

    db = 0;
    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    cpp_operator = db->getCppOperator(offer_type);

    delete db;
    return cpp_operator;
}

QString ProductOfferModule::getLogoOffer(const QString& xml_string){
    ProductOffersModuleDB *db = 0;
    XmlConfig *xml = 0;
    QString logo;

    logo = "";
    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return logo;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return logo;
    }

    xml->delDomain();
    xml->setDomain("offers[0].offer[0]");

    if(xml->readString("offer_type").isEmpty()){
        cerr << "empty offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return logo;
    }

    if(xml->readString("offer_name").isEmpty()){
        cerr << "empty offer name " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return logo;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    logo = db->getLogoOffer(xml->readString("offer_type"), xml->readString("offer_name"));
    delete db;
    delete xml;
    return logo;
}

QString ProductOfferModule::getLogo(const QString& type, const QString& name){
    ProductOffersModuleDB *db = 0;
    ProductOfferData *offer = 0;
    QString ret;


    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    offer = db->getOffer(type,name);
    delete db;

    ret = offer->logo;
    delete offer;
    return ret;
}


QString ProductOfferModule::getOffer(const QString& xml_string){
    QString xml_return;
    XmlConfig *xml = 0;
    ProductOfferData *offer = 0;
    ProductOffersModuleDB *db = 0;

    xml_return = "";

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return xml_return;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_return;
    }

    xml->setDomain("offers[0].offer[0]");

    if(xml->readString("offer_type").isEmpty()){
        cerr << "empty offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return  xml_return;
    }

    if(xml->readString("offer_name").isEmpty()){
        cerr << "empty offer name " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return xml_return;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    offer = db->getOffer(xml->readString("offer_type"), xml->readString("offer_name"));
    delete db;

    if(!offer){
        cerr << "the offer does not exists " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "Offer type: " << xml->readString("offer_type").toStdString() << endl;
        cerr << "Offer name: " << xml->readString("offer_name").toStdString() << endl;
        delete xml;
        return xml_string;
    }

    delete xml;
    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("offers");
    xml->createElementSetDomain("offer");
    xml->createElement("offer_type", offer->offer_type);
    xml->createElement("offer_name", offer->offer_name);
    xml->createElement("description_offer", offer->description_offer);
    xml->createElement("cpp_operator",offer->cpp_operator);
    xml->createElement("logo", offer->logo);

    xml->releaseDomain("offer", true);
    xml->releaseDomain("offers", true);
    delete offer;

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_return;
    }

    xml_return = xml->toString();
    delete xml;
    return xml_return;
}

QString ProductOfferModule::getAllOffers(){
    QString xml_string;
    XmlConfig *xml = 0;
    QPtrList<ProductOfferData> *offers = 0;
    ProductOfferData *offer = 0;
    int index, count;
    ProductOffersModuleDB *db = 0;

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    offers = db->getAllOffers();
    delete db;

    if(!offers){
        cerr << "there are not offers for this offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("offers");
    count = offers->count();
    for(index = 0; index < count; index++){
        offer = offers->at(index);
        xml->createElementSetDomain("offer");
        xml->createElement("offer_type", offer->offer_type);
        xml->createElement("offer_name", offer->offer_name);
        xml->createElement("logo", offer->logo);
        xml->releaseDomain("offer", false);
    }
    xml->releaseDomain("offers", true);

    delete offer;
    delete offers;

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }
    xml_string = xml->toString();
    delete xml;
    return xml_string;
}

QString ProductOfferModule::getOffers(const QString& offer_type){
    QString xml_string;
    XmlConfig *xml = 0;
    QPtrList<ProductOfferData> *offers = 0;
    ProductOfferData *offer = 0;
    int index, count;
    ProductOffersModuleDB *db = 0;

    xml_string = "";

    if(offer_type.isEmpty()){
        cerr << "empty offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    offers = db->getOffers(offer_type);
    delete db;

    if(!offers){
        cerr << "there are not offers for this offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "Offer type: " << offer_type.toStdString() << endl;
        return xml_string;
    }

    if(offers->isEmpty())
        return xml_string;

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("offers");
    count = offers->count();
    for(index = 0; index < count; index++){
        offer = offers->at(index);

        xml->createElementSetDomain("offer");
        xml->createElement("offer_type", offer->offer_type);
        xml->createElement("description_type", offer->description_type);
        xml->createElement("offer_name", offer->offer_name);
        xml->createElement("description_offer", offer->description_offer);
        xml->createElement("cpp_operator", offer->cpp_operator);
        xml->createElement("logo", offer->logo);

        xml->releaseDomain("offer", false);
    }
    xml->releaseDomain("offers", true);

    delete offer;
    delete offers;

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    delete xml;
    return xml_string;
}

QString ProductOfferModule::getOfferType(const QString& offer_type){
    QString xml_string;
    XmlConfig *xml = 0;
    ProductOfferData *offer = 0;
    ProductOffersModuleDB *db = 0;

    xml_string = "";

    if(offer_type.isEmpty()){
        cerr << "empty offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    offer = db->getOfferType(offer_type);
    delete db;

    if(!offer){
        cerr << "offer type does not exists" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "Offer type: " << offer_type.toStdString() << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("offers");
    xml->createElementSetDomain("offer");
    xml->createElement("offer_type", offer->offer_type);
    xml->createElement("description_type", offer->description_type);
    xml->createElement("offer_name");
    xml->createElement("cpp_operator", offer->cpp_operator);
    xml->createElement("logo", offer->logo);
    xml->releaseDomain("offer");
    xml->releaseDomain("offers", true);

    delete offer;
    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    delete xml;
    return xml_string;
}

QString ProductOfferModule::getOfferTypes(){
    QString xml_string;
    XmlConfig *xml = 0;
    QStringList *offer_types = 0;
    ProductOffersModuleDB *db = 0;
    int index, count;

    xml_string = "";

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    offer_types = db->getOfferTypes();
    delete db;

    if(!offer_types){
        cerr << "offer type does not exists" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("offers");
    count = offer_types->count();
    for(index = 0; index < count; index++){
        xml->createElementSetDomain("offer");
        xml->createElement("offer_type", (*offer_types)[index]);
        xml->createElement("offer_name");
        xml->releaseDomain("offer", true);
    }
    xml->releaseDomain("offers", true);

    delete offer_types;

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    delete xml;
    return xml_string;
}

bool ProductOfferModule::insertOffer(const QString& xml_string){
    QString aux;
    XmlConfig *xml = 0;
    ProductOffersModuleDB *db = 0;
    bool inserted = false;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return inserted;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return inserted;
    }

    xml->setDomain("offers[0].offer[0]");
    if(xml->readString("offer_type").isEmpty()){
        cerr << "empty offer type" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return inserted;
    }

    if(xml->readString("offer_name").isEmpty()){
        cerr << "empty offer name" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return inserted;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);

    if(!db->existOfferType(xml->readString("offer_type"))){
        db->insertOfferType(xml->readString("offer_type"), xml->readString("description_type"), xml->readString("cpp_operator"));
    }

    if(db->insertOffer(xml->readString("offer_type"), xml->readString("offer_name"),
                       xml->readString("description_offer"),xml->readString("logo")))
        inserted = true;

    delete db;
    delete xml;
    return inserted;
}

bool ProductOfferModule::insertOfferType(const QString& xml_string){
    QString aux;
    XmlConfig *xml = 0;
    ProductOffersModuleDB *db = 0;
    bool inserted = false;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return inserted;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return inserted;
    }

    xml->setDomain("offers[0].offer[0]");
    if(xml->readString("offer_type").isEmpty()){
        cerr << "empty offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return inserted;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    if(db->insertOfferType(xml->readString("offer_type"), xml->readString("description_type"),
                           xml->readString("cpp_operator")))
        inserted = true;

    delete db;
    delete xml;
    return inserted;
}

int ProductOfferModule::getNumOfferTypes(){
    ProductOffersModuleDB *db = 0;
    int num;

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    num = db->getNumOfferTypes();
    delete db;

    return num;
}

bool ProductOfferModule::updateOffer(const QString& xml_string){
    QString aux;
    XmlConfig *xml = 0;
    ProductOffersModuleDB *db = 0;
    bool updated = false;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    xml->delDomain();
    xml->setDomain("offers[0].offer[0]");
    if(xml->readString("offer_type").isEmpty()){
        cerr << "empty offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(xml->readString("offer_name").isEmpty()){
        cerr << "empty offer name" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    if(db->updateOffer(xml->readString("offer_type"), xml->readString("offer_name"),
                       xml->readString("description_offer"),xml->readString("logo")));
    updated = true;

    delete db;
    delete xml;

    return updated;
}

bool ProductOfferModule::updateOfferType(const QString& xml_string){
    XmlConfig *xml = 0;
    bool updated = false;
    ProductOffersModuleDB *db = 0;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    xml->setDomain("offers[0].offer[0]");
    if(xml->readString("offer_type").isEmpty()){
        cerr << "empty offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    if(db->updateOfferType(xml->readString("offer_type"), xml->readString("description_type"),
                           xml->readString("cpp_operator")))
        updated = true;

    delete db;
    delete xml;

    return updated;
}

QString ProductOfferModule::getProductOffers(const QString& product_code){
    QString xml_string;
    XmlConfig *xml = 0;
    QPtrList<ProductOfferData> *offers = 0;
    ProductOfferData *offer = 0;
    ProductOffersModuleDB *db = 0;
    int index, count;

    xml_string = "";

    if(product_code.isEmpty()){
        cerr << "empty product code" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    offers = db->getProductOffers(product_code);
    delete db;

    if(!offers){
        cerr << "product does not have offers " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "Product Code: " << product_code.toStdString() << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("offers");
    count = offers->count();
    for(index = 0; index < count; index++){
        offer = offers->at(index);
        xml->createElementSetDomain("offer");

        xml->createElement("offer_type", offer->offer_type);
        xml->createElement("description_type", offer->description_type);
        xml->createElement("offer_name", offer->offer_name);
        xml->createElement("description_offer", offer->description_offer);
        xml->createElement("cpp_operator", offer->cpp_operator);
        xml->createElement("value", QString::number(offer->value));

        if(offer->is_default)
            xml->createElement("default", "t");
        else
            xml->createElement("default", "f");
        xml->createElement("logo", offer->logo);

        xml->releaseDomain("offer", false);
    }
    xml->releaseDomain("offers", true);

    delete offer;
    delete offers;

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return QString{};
    }

    xml_string = xml->toString();
    delete xml;
    return xml_string;
}

bool ProductOfferModule::existProductAndOffer(const QString& xml_string){
    bool exist = false;
    QString product_code;
    XmlConfig *xml = 0;
    ProductOffersModuleDB *db = 0;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return exist;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return exist;
    }

    product_code = xml->readString("product_code[0]");
    if(product_code.isEmpty()){
        cerr << "empty product code" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return exist;
    }

    xml->setDomain("offers[0].offer[0]");
    if(xml->readString("offer_type").isEmpty()){
        cerr << "empty offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return exist;
    }

    if(xml->readString("offer_name").isEmpty()){
        cerr << "empty offer name" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return exist;
    }

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    if(db->existProductAndOffer(product_code, xml->readString("offer_type"), xml->readString("offer_name")))
        exist = true;

    delete db;
    delete xml;
    return exist;
}

bool ProductOfferModule::insertOffersToProduct(const QString& xml_string){
    QString product_code, aux;
    XmlConfig *xml = 0;
    ProductOffersModuleDB *db = 0;
    int index, count;
    bool inserted = false;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return inserted;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return inserted;
    }
    xml->delDomain();

    product_code = xml->readString("product_code");
    if(product_code.isEmpty()){
        cerr << "empty product code" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return inserted;
    }

    inserted = true;
    xml->setDomain("offers");
    count = xml->howManyTags("offer");
    for(index = 0; index < count; index++){
        xml->setDomain("offer[" + QString::number(index) + "]");

        db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);

        if(!db->existOfferType(xml->readString("offer_type")))
            if(!db->insertOfferType(xml->readString("offer_type"), xml->readString("description_type"),
                                    xml->readString("cpp_operator")))
                return false;

        if(!db->existOffer(xml->readString("offer_type"), xml->readString("offer_name")))
            if(!db->insertOffer(xml->readString("offer_type"), xml->readString("offer_name"),
                                xml->readString("description_offer")))
                return false;

        if(!db->insertOfferToProduct(product_code, xml->readString("offer_type"), xml->readString("offer_name"),
                                     (xml->readString("value")).toDouble(), "f")){
            delete db;
            inserted = false;
            break;
        }

        delete db;
        xml->releaseDomain("offer", false);
    }

    delete xml;
    return inserted;
}

bool ProductOfferModule::updateOffersToProduct(const QString& xml_string){
    QString product_code, aux;
    XmlConfig *xml = 0;
    ProductOffersModuleDB *db = 0;
    int index, count;
    bool updated = true;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    product_code = xml->readString("product_code[0]");
    if(product_code.isEmpty()){
        cerr << "empty product code" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    xml->setDomain("offers");
    count = xml->howManyTags("offer");

    for(index = 0; index < count; index++){
        xml->setDomain("offer[" + QString::number(index) + "]");

        if(xml->readString("offer_type").isEmpty()){
            cerr << "empty offer type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            continue;
        }

        if(xml->readString("offer_name").isEmpty()){
            cerr << "empty offer name " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            continue;
        }

        db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
        if(!db->updateOfferToProduct(product_code, xml->readString("offer_type"), xml->readString("offer_name"),
                                     (xml->readString("value")).toDouble(), "f")){
            delete db;
            updated = false;
            break;
        }

        delete db;
        xml->releaseDomain("offer", true);
    }

    delete xml;
    return updated;
}

bool ProductOfferModule::deleteOffersToProduct(const QString& product_code, const QString& offer_type,const QString& offer_name){
    bool ret = false;
    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    ret = db->deleteOfferToProduct(product_code,offer_type,offer_name);
    delete db;
    return ret;
}


QString ProductOfferModule::getStringXmlOffer(ProductOfferData *offer){
    XmlConfig *xml = 0;
    QString xml_string;

    xml_string = "";

    if(!offer){
        cerr << "empty offer " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("offers");
    xml->createElementSetDomain("offer");
    xml->createElement("offer_type", offer->offer_type);
    xml->createElement("description_type", offer->description_type);
    xml->createElement("offer_name", offer->offer_name);
    xml->createElement("description_offer", offer->description_offer);
    xml->createElement("cpp_operator", offer->cpp_operator);
    xml->createElement("value", QString::number(offer->value));

    if(offer->is_default)
        xml->createElement("default", "t");
    else
        xml->createElement("default", "f");
    xml->createElement("logo", offer->logo);

    xml->releaseDomain("offer", true);
    xml->releaseDomain("offers", true);

    if(!xml->validateXmlWithDTD(OFFERS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    delete xml;
    return xml_string;
}


void ProductOfferModule::setAllProductOfferValue(const QString& offer_type, const QString& offer_name, const QString& value){
    bool ok;
    double v;
    ProductOffersModuleDB *db{} ;
    //check if the value is correct.
    v = value.toDouble(&ok);
    if (!ok)
        return;

    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    db->setAllProductOfferValue(offer_type,offer_name,value);
    delete db;
}

void ProductOfferModule::updateAllProductOfferValue(const QString& offer_type, const QString& offer_name, const QString& value){
    bool ok;
    double v;
    ProductOffersModuleDB *db{} ;
    //check if the value is correct.
    v = value.toDouble(&ok);
    if (!ok)
        return;
    db = new ProductOffersModuleDB(OFFER_CONNECTION_NAME, OFFER_PATH_FILE);
    db->updateAllProductOfferValue(offer_type,offer_name,value);
    delete db;
}








