/***************************************************************************
                          ProductOptionModule.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "productoptionmodule.h"
#include "productoptionmodule_adaptor.h"

#include <QtDBus/QDBusConnection>

const QString& OPTION_CONNECTION_NAME = "ProductOptionModuleConnectionName";
const QString& OPTION_PATH_FILE = "/etc/ntpv/bar_database.xml";
static const QString& OPTIONS_DTD = "/etc/ntpv_backoffice/dtds/products_optionslist.dtd";

#include <iostream>
using namespace std;

const QString ProductOptionModule::DBusObjectPath  = QString{"/wpos/wposbo/DBusBOProductOption"};

ProductOptionModule::ProductOptionModule(QObject *parent, const QString& name):
    QObject(parent)
{
    setObjectName(name);
    new DBusBOProductOptionAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(ProductOptionModule::DBusObjectPath, this);
}

bool ProductOptionModule::deleteOption(const QString& xml_string){
    XmlConfig xml ;
    ProductOptionsModuleDB *db = 0;

    if(!xml.readXmlFromString(xml_string)){
        cerr << "Cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(!xml.validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return false;
    }

    xml.delDomain();
    xml.setDomain("options[0].option[0]");
    if(xml.readString("option_type").isEmpty()){
        cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(xml.readString("option_name").isEmpty()){
        cerr << "Empty option name " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    if(db->deleteOption(xml.readString("option_type"), xml.readString("option_name"))){
        delete db;

        return true;
    }
    delete db;

    return false;
}

bool ProductOptionModule::deleteOptionType(const QString& option_type){
    ProductOptionsModuleDB *db = 0;

    if(option_type.isEmpty()){
        cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);

    if(db->deleteOptionType(option_type)){
        delete db;
        return true;
    }
    else{
        delete db;
        return false;
    }
}

bool ProductOptionModule::existOption(const QString& xml_string){
    ProductOptionsModuleDB *db = 0;
    XmlConfig *xml = 0;

    xml = new XmlConfig();
    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "El xml no tiene una estructura correcta " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    xml->delDomain();
    xml->setDomain("options[0]");
    xml->setDomain("option[0]");

    if(xml->readString("option_type").isEmpty()){
        cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(xml->readString("option_name").isEmpty()){
        cerr << "empty option name " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);

    if(db->existOption(xml->readString("option_type"), xml->readString("option_name"))){
        delete db;
        delete xml;
        return true;
    }
    delete db;
    delete xml;
    return false;
}

bool ProductOptionModule::existOptionType(const QString& option_type){
    ProductOptionsModuleDB *db = 0;

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);

    if(db->existOptionType(option_type)){
        delete db;
        return true;
    }
    else{
        delete db;
        return false;
    }
}

QString ProductOptionModule::getDescriptionOption(const QString& xml_string){
    QString description;
    XmlConfig *xml = 0;
    ProductOptionsModuleDB *db = 0;

    description = "";
    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot conver xml into string" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return description;
    }

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return description;
    }
    xml->delDomain();
    xml->setDomain("options[0].option[0]");

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    description = db->getDescriptionOption(xml->readString("option_type"),
                                           xml->readString("option_name"));
    delete db;
    delete xml;
    return description;
}

QString ProductOptionModule::getDescriptionOptionType(const QString& option_type){
    QString description;
    ProductOptionsModuleDB *db = 0;

    description = "";

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    description = db->getDescriptionOptionType(option_type);
    delete db;

    return description;
}

QString ProductOptionModule::getOption(const QString& xml_string){
    QString xml_return;
    XmlConfig *xml = 0;
    ProductOptionData *option = 0;
    ProductOptionsModuleDB *db = 0;

    xml_return = "";
    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return xml_return;
    }

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return QString{};
    }

    xml->delDomain();
    xml->setDomain("options[0].option[0]");
    if(xml->readString("option_type").isEmpty()){
        cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return xml_return;
    }

    if(xml->readString("option_name").isEmpty()){
        cerr << "empty option name " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return xml_return;
    }

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    option = db->getOption(xml->readString("option_type"), xml->readString("option_name"));
    delete db;
    delete xml;

    if(!option){
        cerr << "Option does not exists " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "Option type: " << xml->readString("option_type").toStdString() << endl;
        cerr << "Option name: " << xml->readString("option_name").toStdString() << endl;
        return xml_return;
    }

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("options");

    if(!option){
        xml->releaseDomain("options", true);
        xml->delDomain();
        xml_return = xml->toString();
        delete xml;
        return xml_string;
    }

    xml->createElementSetDomain("option");
    xml->createElement("option_type", option->option_type);
    xml->createElement("option_name", option->option_name);
    xml->createElement("description_option", option->description_option);
    xml->releaseDomain("option", true);
    xml->releaseDomain("options", true);

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return QString{};
    }

    xml_return = xml->toString();
    delete xml;
    return xml_return;
}

QString ProductOptionModule::getOptions(const QString& option_type){
    QString xml_string;
    XmlConfig *xml = 0;
    QStringList *options = 0;
    ProductOptionsModuleDB *db = 0;
    int index, count;

    xml_string = "";

    if(option_type.isEmpty()){
        cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    options = db->getOptions(option_type);
    delete db;

    if(!options){
        cerr << "there are no options for this option type " <<__PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "Option type: " << option_type.toStdString() << endl;
        return xml_string;
    }
    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("options");

    if(!options){
        xml->releaseDomain("options");
        xml->delDomain();
        xml_string = xml->toString();
        delete xml;
        delete options;
        return xml_string;
    }

    count = options->count();
    for(index = 0; index < count; index++){
        xml->createElementSetDomain("option");
        xml->createElement("option_type");
        xml->createElement("option_name", (*options)[index]);
        xml->releaseDomain("option");
    }
    xml->releaseDomain("options", true);
    delete options;

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    delete xml;

    return xml_string;
}

QString ProductOptionModule::getAllOptions(){
    int i,count;
    QString xml_string;
    XmlConfig *xml = 0;
    QList<ProductOptionData*> *options = 0;
    ProductOptionData *option = 0;
    ProductOptionsModuleDB *db = 0;

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    options = db->getAllOptions();
    delete db;

    if (!options)
        return xml_string;

    xml = new XmlConfig();
    count = options->count();
    xml->createElementSetDomain("options");
    for (i=0;i<count;i++){
        option = options->at(i);
        xml->createElementSetDomain("option");
        xml->createElement("option_type", option->option_type);
        xml->createElement("option_name",option->option_name);
        xml->releaseDomain("option");
    }
    xml->delDomain();

    xml_string = xml->toString();
    delete xml;
    options->clear();
    delete options;

    return xml_string;
}



QString ProductOptionModule::getOptionType(const QString& option_type){
    QString xml_string;
    XmlConfig *xml = 0;
    ProductOptionData *option = 0;
    ProductOptionsModuleDB *db = 0;

    xml_string = "";

    if(option_type.isEmpty()){
        cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    option = db->getOptionType(option_type);
    delete db;

    if(!option){
        cerr << "option type does not exists " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "Option type: " << option_type.toStdString() << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("options");
    xml->createElementSetDomain("option");
    xml->createElement("option_type", option->option_type);
    xml->createElement("description_type", option->description_type);
    xml->createElement("option_name");
    xml->releaseDomain("option", true);
    xml->releaseDomain("options", true);
    delete option;

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    delete xml;

    return xml_string;
}

QString ProductOptionModule::getOptionTypes(){
    QString xml_string;
    XmlConfig *xml = 0;
    QStringList *options = 0;
    ProductOptionsModuleDB *db = 0;
    int index, count;

    xml_string = "";

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    options = db->getOptionTypes();
    delete db;

    if(!options){
        cerr << "there are no option types" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->createElement("product_code");
    xml->createElementSetDomain("options");

    count = options->count();
    for(index = 0; index < count; index++){
        xml->createElementSetDomain("option");
        xml->createElement("option_type", (*options)[index]);
        xml->createElement("option_name");
        xml->releaseDomain("option", true);
    }
    xml->releaseDomain("option_types", true);
    delete options;

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }
    xml_string = xml->toString();
    delete xml;
    return xml_string;
}

bool ProductOptionModule::insertOption(const QString& xml_string){
    QString aux;
    XmlConfig *xml = 0;
    ProductOptionsModuleDB *db = 0;
    bool inserted = false;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    xml->delDomain();
    xml->setDomain("options.option");

    if(xml->readString("option_type").isEmpty()){
        cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(xml->readString("option_name").isEmpty()){
        cerr << "empty option name " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    if(db->insertOption(xml->readString("option_type"), xml->readString("option_name"),
                        xml->readString("description_option")))
        inserted = true;

    delete db;
    delete xml;
    return inserted;
}

bool ProductOptionModule::insertOptionType(const QString& xml_string){
    QString aux;
    XmlConfig *xml = 0;
    ProductOptionsModuleDB *db = 0;
    bool inserted = false;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__  << ": " <<__LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    xml->delDomain();
    xml->setDomain("options[0].option");

    if(xml->readString("option_type").isEmpty()){
        cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    inserted = db->insertOptionType(xml->readString("option_type"), xml->readString("description_type"));
    delete db;

    delete xml;
    return inserted;
}

bool ProductOptionModule::updateOption(const QString& xml_string){
    XmlConfig *xml = 0;
    ProductOptionsModuleDB *db = 0;
    bool updated = false;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert xml into string " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    xml->delDomain();
    xml->setDomain("options[0].option[0]");

    if(xml->readString("option_type").isEmpty()){
        cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(xml->readString("option_name").isEmpty()){
        cerr << "empty option name " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    updated = db->updateOption(xml->readString("option_type"), xml->readString("option_name"),
                               xml->readString("description_option"));
    delete db;
    delete xml;
    return updated;
}

bool ProductOptionModule::updateOptionType(const QString& xml_string){
    XmlConfig *xml = 0;
    ProductOptionsModuleDB *db = 0;
    bool updated = false;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert xml into string " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }
    xml->delDomain();
    xml->setDomain("options[0].option[0]");

    if(xml->readString("option_type").isEmpty()){
        cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    updated = db->updateOptionType(xml->readString("option_type"), xml->readString("description_type"));

    delete db;
    delete xml;
    return updated;
}

bool ProductOptionModule::existProductAndOption(const QString& xml_string){
    bool exist = false;
    QString product_code;
    XmlConfig *xml = 0;
    ProductOptionsModuleDB *db = 0;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return exist;
    }

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    xml->delDomain();
    product_code = xml->readString("product_code[0]");
    if(product_code.isEmpty()){
        cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return exist;
    }

    xml->setDomain("options[0].option[0]");
    if(xml->readString("option_type").isEmpty()){
        cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return exist;
    }

    if(xml->readString("option_name").isEmpty()){
        cerr << "empty option name " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return exist;
    }

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    exist = db->existProductAndOption(product_code, xml->readString("option_type"),
                                      xml->readString("option_name"));
    delete db;
    delete xml;
    return exist;
}

QString ProductOptionModule::getProductOptions(const QString& product_code){
    QString xml_string;
    XmlConfig *xml = 0;
    QList<ProductOptionData*> *options = 0;
    ProductOptionData *option = 0;
    ProductOptionsModuleDB *db = 0;
    int index, count;

    xml_string = "";
    if(product_code.isEmpty()){
        cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    db = 0;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    options = db->getProductOptions(product_code);
    delete db;

    if(!options){
        cerr << "the product does not have options " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("options");

    count = options->count();
    for(index = 0; index < count; index++){
        option = options->at(index);
        xml->createElementSetDomain("option");

        xml->createElement("option_type", option->option_type);
        xml->createElement("description_type", option->description_type);
        xml->createElement("option_name", option->option_name);
        xml->createElement("description_option", option->description_option);
        xml->createElement("value", QString::number(option->value));

        if(option->is_default)
            xml->createElement("default", "true");
        else
            xml->createElement("default", "false");

        xml->releaseDomain("option", false);
    }
    xml->releaseDomain("options", true);
    delete options;
    delete option;

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return QString{};
    }
    xml_string = xml->toString();
    delete xml;

    return xml_string;
}

bool ProductOptionModule::insertOptionsToProduct(const QString& xml_string){
    QString sql, product_code;
    XmlConfig *xml = 0;
    ProductOptionsModuleDB *db = 0;
    int index, count;

    xml = new XmlConfig();
    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    xml->delDomain();
    product_code = xml->readString("product_code");
    xml->setDomain("options");
    count = xml->howManyTags("option");

    for(index = 0; index < count; index++){
        xml->setDomain("option[" + QString::number(index) + "]");

        if(xml->readString("option_type").isEmpty()){
            xml->releaseDomain("option", true);
            continue;
        }

        if(xml->readString("option_name").isEmpty()){
            xml->releaseDomain("option", true);
            continue;
        }

        if(xml->readString("value").isEmpty()){
            xml->releaseDomain("value", true);
            continue;
        }

        if(product_code.isEmpty()){
            xml->releaseDomain("option", true);
            continue;
        }

        if(db){
            db = 0;
            delete db;
        }
        db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);

        if(!db->existOptionType(xml->readString("option_type")))
            if(!db->insertOptionType(xml->readString("option_type"), xml->readString("description_type"))){
                delete db;
                delete xml;
                return false;
            }



        if(!db->existOption(xml->readString("option_type"), xml->readString("option_name")))
            if(!db->insertOption(xml->readString("option_type"), xml->readString("option_name"),
                                 xml->readString("description_option"))){
                delete db;
                delete xml;
                return false;
            }

        if(!db->insertOptionToProduct(product_code, xml->readString("option_type"),
                                      xml->readString("option_name"),
                                      (xml->readString("value")).toDouble(),
                                      xml->readString("default"))){
            delete db;
            delete xml;
            return false;
        }

        delete db;
        xml->releaseDomain("option");
    }
    xml->releaseDomain("options", false);
    delete xml;
    return true;
}

bool ProductOptionModule::updateOptionsToProduct(const QString& xml_string){
    QString product_code, aux;
    XmlConfig *xml = 0;
    ProductOptionsModuleDB *db = 0;
    int index, count;

    xml = new XmlConfig();
    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }
    xml->delDomain();

    product_code = xml->readString("product_code");
    if(product_code.isEmpty()){
        cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__  << endl;
        delete xml;
        return false;
    }
    xml->setDomain("options");

    count = xml->howManyTags("option");
    for(index = 0; index < count; index++){
        xml->setDomain("option[" + QString::number(index) + "]");

        if(xml->readString("option_type").isEmpty()){
            cerr << "empty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml->releaseDomain("option", true);
            continue;
        }

        if(xml->readString("option_name").isEmpty()){
            cerr << "empty option name " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml->releaseDomain("option", true);
            continue;
        }

        if(xml->readString("value").isEmpty()){
            cerr << "empty option value " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml->releaseDomain("option", true);
            continue;
        }

        db = 0;
        db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);

        if(!db->existOptionType(xml->readString("option_type")))
            if(!db->insertOptionType(xml->readString("option_type"), xml->readString("description_type"))){
                delete xml;
                delete db;
                return false;
            }



        if(!db->existOption(xml->readString("option_type"), xml->readString("option_name")))
            if(!db->insertOption(xml->readString("option_type"), xml->readString("option_name"),
                                 xml->readString("description_option"))){
                delete xml;
                delete db;
                return false;
            }


        if(!db->updateOptionToProduct(product_code, xml->readString("option_type"),
                                      xml->readString("option_name"), xml->readString("value").toDouble(),
                                      xml->readString("default"))){
            delete xml;
            delete db;
            return false;
        }

        delete db;
    }
    delete xml;
    return true;
}

bool ProductOptionModule::deleteOptionsToProduct(const QString& product_code, const QString& option_type, const QString& option_name){
    bool ret = false;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    ret = db->deleteOptionToProduct(product_code,option_type,option_name);
    delete db;
    return ret;
}


QString ProductOptionModule::getStringXmlOption(ProductOptionData *option){
    XmlConfig *xml = 0;
    QString xml_string;

    xml_string = "";

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("options");
    xml->createElementSetDomain("option");
    xml->createElement("option_type", option->option_type);
    xml->createElement("description_type", option->description_type);
    xml->createElement("option_name", option->option_name);
    xml->createElement("description_option", option->description_option);
    xml->createElement("value", QString::number(option->value));

    if(option->is_default)
        xml->createElement("default", "t");
    else
        xml->createElement("default", "f");

    xml->releaseDomain("option", true);
    xml->releaseDomain("option", true);

    if(!xml->validateXmlWithDTD(OPTIONS_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    delete xml;
    return xml_string;
}


void ProductOptionModule::setAllProductOptionValue(const QString& option_type, const QString& option_name, const QString& value){
    bool ok;
    ProductOptionsModuleDB *db{} ;
    //check if the value is correct.
    value.toDouble(&ok);
    if (!ok)
        return;

    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    db->setAllProductOptionValue(option_type,option_name,value);
    delete db;
}

void ProductOptionModule::updateAllProductOptionValue(const QString& option_type, const QString& option_name, const QString& value){
    bool ok;
    ProductOptionsModuleDB *db{} ;
    //check if the value is correct.
    value.toDouble(&ok);
    if (!ok)
        return;
    db = new ProductOptionsModuleDB(OPTION_CONNECTION_NAME, OPTION_PATH_FILE);
    db->updateAllProductOptionValue(option_type,option_name,value);
    delete db;
}
