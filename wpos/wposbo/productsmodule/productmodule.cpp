/***************************************************************************
                          product.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "productmodule.h"
#include "productmodule_adaptor.h"

#include "database/productsmoduledb.h"
#include "database/productoptionsmoduledb.h"

#include <QSqlQuery>
#include <QSqlError>

#include <iostream>
using namespace std;

static const QString& DCOP_OBJECT_INTERFACE_NAME = "ProductsModule";

const QString& XML_BAR_PATH = "/etc/ntpv/bar.xml";
const QString& PRODUCT_PATH_FILE = "/etc/ntpv/bar_database.xml";
const QString& PRODUCT_MODULE_ICON_PATH = "/usr/share/ntpv/logos/";
const QString& PRODUCT_CONNECTION_NAME = "ProductModule";
static const QString& PRODUCTS_LIST_DTD = "/etc/ntpv_backoffice/dtds/products_productslist.dtd";
static const QString& FAMILIES_LIST_DTD = "/etc/ntpv_backoffice/dtds/products_familieslist.dtd";
static const QString& TAXES_LIST_DTD = "/etc/ntpv_backoffice/dtds/products_taxeslist.dtd";
static const QString& COMPOSITION_DTD = "/etc/ntpv_backoffice/dtds/products_composition.dtd";

const QString ProductModule::DBusObjectPath  = QString{"/wpos/wposbo/DBusBOProduct"};

ProductModule::ProductModule(QObject *parent, const QString& name):
    QObject(parent)
{
    setObjectName(name);
    new DBusBOProductAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(ProductModule::DBusObjectPath, this);
}

bool ProductModule::isUnitaryProduct(QString product_code){
    ProductsModuleDB *db = 0;

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);

    if(db->isUnitaryProduct(product_code)){
        delete db;
        return true;
    }
    else{
        delete db;
        return false;
    }
}

bool ProductModule::deleteProduct(const QString& product_code){
    ProductsModuleDB *db = 0;
    bool deleted = false;

    if(product_code.isEmpty()){
        cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    db = new ProductsModuleDB("ProductModule", "/etc/ntpv/bar_database.xml");
    deleted = db->deleteProduct(product_code);
    delete db;

    if(deleted)
        emit productDeleted(product_code);

    return deleted;
}

QString ProductModule::getLogo(const QString& product_name){
    XmlConfig *xml = 0;
    QString xml_string;
    ProductsModuleDB *db = 0;
    ProductData *product = 0;

    xml_string = "";

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    product = db->getLogo(product_name);
    delete db;

    xml = new XmlConfig();
    xml->delDomain();
    xml->createElementSetDomain("products");
    xml->createElementSetDomain("product");
    xml->createElement("code", product->code);
    xml->createElement("name", product->name);
    xml->createElement("logo", product->logo);
    xml->releaseDomain("product", false);
    xml->releaseDomain("products", true);

    xml_string = xml->toString();
    delete xml;

    return xml_string;
}

QString ProductModule::getLogoFromProductCode(const QString& product_code){
    QString ret = "";
    ProductsModuleDB *db = 0;
    ProductData *product = 0;

    if (product_code.isEmpty())
        return ret;

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    product = db->getProduct(product_code);
    delete db;

    if (!product)
        return ret;

    return product->logo;
}

QString ProductModule::getLogos(const QString& xml_string){
    QString string_return;
    XmlConfig *xml = 0;
    QStringList products_name;
    ProductsModuleDB *db = 0;
    QPtrList<ProductData> *products = 0;
    ProductData *product = 0;
    int index, count;

    string_return = "";
    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert the sitring into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return string_return;
    }

    if(!xml->validateXmlWithDTD(PRODUCTS_LIST_DTD)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return string_return;
    }
    xml->delDomain();
    xml->setDomain("products[0]");

    count = xml->howManyTags("product");
    for(index = 0; index < count; index++)
        products_name.append(xml->readString("product[" + QString::number(index) + "].name"));

    delete xml;

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    products = db->getLogos(products_name);
    delete db;

    xml = new XmlConfig();
    xml->createElementSetDomain("products");

    count = products->count();
    for(index = 0; index < count; index++){
        product = 0;
        product = products->at(index);

        xml->createElementSetDomain("product");
        xml->createElement("code", product->code);
        xml->createElement("name", product->name);
        xml->createElement("logo", product->logo);
        xml->releaseDomain("product", false);
    }
    xml->releaseDomain("products", true);

    if(!xml->validateXmlWithDTD(PRODUCTS_LIST_DTD)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return string_return;
    }

    xml->delDomain();
    string_return = xml->toString();
    delete xml;
    delete product;
    delete products;

    return string_return;
}

QString ProductModule::getProduct(const QString& product_code){
    QString xml_string;
    XmlConfig *xml = 0;
    ProductsModuleDB *db = 0;
    ProductData *product = 0;

    xml_string = "";

    if(product_code.isEmpty()){
        cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    product = db->getProduct(product_code);
    delete db;

    if(!product){
        cerr << "there are not product with this product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("products");
    xml->createElementSetDomain("product");
    xml->createElement("code", product->code);
    xml->createElement("name", product->name);
    xml->createElement("price", QString::number(product->price));
    xml->createElement("tax", product->tax);
    xml->createElement("logo", product->logo);
    xml->createElement("description", product->description);
    xml->releaseDomain("product", true);
    xml->releaseDomain("products", true);
    delete product;
    //     xml->debug();

    if(!xml->validateXmlWithDTD(PRODUCTS_LIST_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return QString();
    }

    xml_string = xml->toString();
    delete xml;

    return xml_string;
}

QString ProductModule::getProductName(const QString& product_code){
    QString string="";
    ProductsModuleDB *db = 0;
    ProductData *product = 0;

    if(product_code.isEmpty()){
        cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return string;
    }

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    product = db->getProduct(product_code);
    delete db;

    if(!product){
        //          cerr << "there are not product with this product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return string;
    }
    string = product->name;
    delete product;
    return string;
}

QString ProductModule::getProductLike(const QString& product_name){
    QString code;
    ProductsModuleDB *db = 0;

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    code = db->getProductLike(product_name);
    delete db;

    return code;
}

QString ProductModule::getFamilies(){
    QString xml_string;
    XmlConfig *xml = 0;
    QStringList *families = 0;
    ProductsModuleDB *db = 0;
    int index, count;

    xml_string = "";
    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    families = db->getFamilies();
    delete db;

    if(!families){
        cerr << "there are not families " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }
    xml = new XmlConfig();

    xml->delDomain();
    xml->createElementSetDomain("families");

    if(!families){
        xml->releaseDomain("families");

        if(!xml->validateXmlWithDTD(FAMILIES_LIST_DTD, true)){
            cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
            xml->debug();
            delete families;
            delete xml;
            return xml_string;
        }

        xml_string = xml->toString();
        delete families;
        delete xml;
        return xml_string;
    }
    count = families->count();

    for(index = 0; index < count; index++)
        xml->createElement("family", (*families)[index]);

    xml->releaseDomain("families", true);
    xml->delDomain();
    delete families;

    if(!xml->validateXmlWithDTD(FAMILIES_LIST_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    delete xml;
    return xml_string;
}

QString ProductModule::getTaxes(){
    QString xml_string;
    XmlConfig *xml = 0;
    QStringList *taxes_list = 0;
    ProductsModuleDB *db = 0;
    int index, count;

    xml_string = "";
    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    taxes_list = db->getTaxes();
    delete db;

    if(!taxes_list){
        cerr << "there are no taxes " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }
    xml = new XmlConfig();

    xml->delDomain();
    xml->createElementSetDomain("taxes");

    if(taxes_list)
        count = taxes_list->count();
    else
        count = 0;

    for(index=0; index < count; index++)
        xml->createElement("tax", (*taxes_list)[index]);

    xml->releaseDomain("taxes", true);
    xml->delDomain();
    delete taxes_list;

    if(!xml->validateXmlWithDTD(TAXES_LIST_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    delete xml;
    return xml_string;
}

QString ProductModule::getProductsExtend(){
    QString xml_string;
    XmlConfig *xml = 0;
    QPtrList<ProductData> *products = 0;
    ProductsModuleDB *db = 0;
    int index, count;
    ProductData *product = 0;

    xml_string = "";

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    products = db->getProductsExtend();
    delete db;

    if(!products){
        cerr << "there are no products " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("products");
    count = products->count();

    for(index = 0; index < count; index++){
        //          cout << "                                                   " << index << endl;
        product = products->at(index);
        xml->createElementSetDomain("product");

        xml->createElement("code", product->code);
        xml->createElement("name", product->name);
        xml->createElement("price", QString::number(product->price));
        xml->createElement("tax", product->tax);
        xml->createElement("logo", product->logo);
        xml->createElement("description", product->description);

        xml->releaseDomain("product");
    }
    xml->releaseDomain("products");
    delete products;
    delete product;

    if(!xml->validateXmlWithDTD(PRODUCTS_LIST_DTD)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    //     cout << "                                                                                       Final xml" << endl;
    delete xml;

    return xml_string;
}

QString ProductModule::getProducts(){
    QString xml_string;
    XmlConfig *xml = 0;
    QPtrList<ProductData> *products = 0;
    ProductsModuleDB *db = 0;
    int index, count;
    ProductData *product = 0;

    xml_string = "";

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    products = db->getProducts();
    delete db;

    if(!products){
        cerr << "there are no products " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("products");
    count = products->count();

    for(index = 0; index < count; index++){
        //          cout << "                                                   " << index << endl;
        product = products->at(index);
        xml->createElementSetDomain("product");

        xml->createElement("code", product->code);
        xml->createElement("name", product->name);
        xml->createElement("logo", product->logo);

        xml->releaseDomain("product");
    }
    xml->releaseDomain("products");
    delete products;
    delete product;

    if(!xml->validateXmlWithDTD(PRODUCTS_LIST_DTD)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    //     cout << "                                                                                       Final xml" << endl;
    delete xml;
    return xml_string;
}

QString ProductModule::getUnitaryProducts(){
    QString xml_string;
    XmlConfig *xml = 0;
    QPtrList<ProductData> *products_list = 0;
    ProductData *product = 0;
    ProductsModuleDB *db = 0;
    int index, count;

    xml_string = "";

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    products_list = db->getUnitaryProducts();
    delete db;

    if(!products_list){
        cerr << "there are no products " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    //ok we can get the the list of unitary products
    xml = new XmlConfig();
    xml->delDomain();

    xml->createElementSetDomain("products");
    count = products_list->count();
    for(index = 0; index < count; index++){
        xml->createElementSetDomain("product");

        product = products_list->at(index);
        xml->createElement("code", product->code);
        xml->createElement("name", product->name);
        xml->createElement("price", QString::number(product->price));
        xml->createElement("tax", product->tax);
        xml->createElement("logo", product->logo);
        xml->createElement("description", product->description);

        xml->releaseDomain("product", false);
    }
    xml->releaseDomain("products", true);
    xml->delDomain();
    delete product;
    delete products_list;

    if(!xml->validateXmlWithDTD(PRODUCTS_LIST_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    delete xml;

    return xml_string;
}

bool ProductModule::insertProduct(const QString& xml_string){
    XmlConfig *xml = 0;
    ProductData *product = 0;
    ProductsModuleDB *db = 0;

    xml = new XmlConfig();
    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert the sitring into xml " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(PRODUCTS_LIST_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    product = new ProductData;
    xml->setDomain("products[0]");
    xml->setDomain("product[0]");

    if(xml->readString("code").isEmpty()){
        cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        delete product;
        return false;
    }
    product->code = xml->readString("code");

    if(xml->readString("name").isEmpty()){
        cerr << "Nombre de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        delete product;
        return false;
    }
    product->name = xml->readString("name");
    product->price = (xml->readString("price")).toDouble();
    product->tax = xml->readString("tax");
    product->description = xml->readString("description");
    product->logo = xml->readString("logo");
    delete xml;

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);

    if(db->insertProduct(product)){
        emit productCreated(product->code);
        delete product;
        delete db;
        return true;
    }
    else{
        delete product;
        delete db;
        return false;
    }
}

bool ProductModule::updateProduct(const QString& xml_string){
    XmlConfig *xml = 0;
    ProductData *product = 0;
    ProductsModuleDB *db = 0;
    bool updated = false;

    xml = new XmlConfig();
    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert the sitring into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(PRODUCTS_LIST_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    product = new ProductData;
    xml->setDomain("products[0]");
    xml->setDomain("product[0]");

    if(xml->readString("code").isEmpty()){
        cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        delete product;
        return false;
    }
    product->code = xml->readString("code");

    if(xml->readString("name").isEmpty()){
        cerr << "Nombre de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        delete product;
        return false;
    }
    product->name = xml->readString("name");
    product->price = (xml->readString("price")).toDouble();
    product->tax = xml->readString("tax");
    product->description = xml->readString("description");
    product->logo = xml->readString("logo");
    delete xml;

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);

    if(!db->existProduct(product->code))
        updated = db->insertProduct(product);
    else
        updated = db->updateProduct(product);

    if(updated)
        emit productUpdated(product->code);

    delete product;
    delete db;
    return updated;
}

QString ProductModule::getCompositions(){
    QString xml_string;
    XmlConfig *xml = 0;
    QPtrList<ProductData> *products_list = 0;
    ProductData *product = 0;
    ProductsModuleDB *db = 0;
    int index, count;

    xml_string = "";

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    products_list = db->getCompositions();
    delete db;

    if(!products_list){
        cerr << "there are no products " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    xml = new XmlConfig();

    xml->delDomain();
    xml->createElementSetDomain("products");
    count = products_list->count();
    for(index = 0; index < count; index++){
        product = 0;
        product = products_list->at(index);

        if(!product)
            continue;

        xml->createElementSetDomain("product");
        xml->createElement("code", product->code);
        xml->createElement("name", product->name);
        xml->createElement("price", QString::number(product->price));
        xml->createElement("tax", product->tax);
        xml->createElement("logo", product->logo);
        xml->createElement("description", product->description);
        xml->releaseDomain("product", false);
    }
    xml->releaseDomain("products", true);
    xml->delDomain();
    delete product;
    delete products_list;

    if(!xml->validateXmlWithDTD(PRODUCTS_LIST_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return QString();
    }

    xml_string = xml->toString();
    delete xml;
    return xml_string;
}

bool ProductModule::insertProductComposition(const QString& xml_string){
    QString composition_code, ingredient_code;
    XmlConfig *xml = 0;
    int index, count = 0;
    ProductsModuleDB *db = 0;
    double quantity;

    xml = new XmlConfig();
    xml->delDomain();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "No se pudo convertir en xml el string " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    if(!xml->validateXmlWithDTD(COMPOSITION_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return false;
    }

    xml->setDomain("composition[0]");
    composition_code = xml->readString("composition_code");
    count = xml->howManyTags("ingredient");
    for(index = 0; index < count; index++){
        xml->setDomain("ingredient[" + QString::number(index) + "]");

        ingredient_code = xml->readString("ingredient_code");
        if(ingredient_code.isEmpty()){
            cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml->debug();
            xml->releaseDomain("ingredient", true);
            continue;
        }
        if(xml->readString("quantity").isEmpty()){
            cerr << "empty product cuantity " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml->releaseDomain("ingredient", true);
            continue;
        }
        quantity = xml->readString("quantity").toDouble();

        db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
        if(!db->insertProductComposition(composition_code, ingredient_code,quantity,index)){
            delete xml;
            delete db;
            return false;
        }
        delete db;

        xml->releaseDomain("ingredient", true);
    }

    delete xml;
    return true;
}

bool ProductModule::updateProductComposition(const QString& xml_string){
    QString composition_code, ingredient_code;
    XmlConfig *xml = 0;
    int index, count;
    ProductsModuleDB *db = 0;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert string into xml " << __PRETTY_FUNCTION__ << ": " << endl;
        delete xml;
        return false;
    }
    //     xml->debug();

    xml->delDomain();
    xml->setDomain("composition[0]");
    composition_code = xml->readString("composition_code");
    if(composition_code.isEmpty()){
        cerr << "empty composition code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    count = xml->howManyTags("ingredient");
    for(index = 0; index < count; index ++){
        xml->setDomain("ingredient[" + QString::number(index) + "]");

        ingredient_code = xml->readString("ingredient_code");
        if(ingredient_code.isEmpty()){
            cerr << "component code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml->releaseDomain("ingredient", true);
            continue;
        }

        if(xml->readString("quantity").isEmpty()){
            cerr << "component cuantity " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml->releaseDomain("ingredient", true);
            continue;
        }

        db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
        if(!db->updateProductComposition(composition_code, ingredient_code,
                                         (xml->readString("quantity").toDouble()))){
            delete xml;
            delete db;
            return false;
        }

        xml->releaseDomain("ingredient", true);
    }

    delete xml;
    return true;
}

bool ProductModule::existProductComposition(const QString& xml_string){
    QString composition_code, ingredient_code;
    XmlConfig *xml = 0;
    int index, count;
    ProductsModuleDB *db = 0;

    xml = new XmlConfig();

    if(!xml->readXmlFromString(xml_string)){
        cerr << "cannot convert the sitring into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    xml->delDomain();
    xml->setDomain("composition[0]");
    composition_code = xml->readString("composition_code");
    if(composition_code.isEmpty()){
        cerr << "empty composition code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return false;
    }

    count = xml->howManyTags("ingredient");
    for(index = 0; index < count; index++){
        xml->setDomain("ingredient[" + QString::number(index) + "]");
        ingredient_code = xml->readString("ingredient_code");

        if(ingredient_code.isEmpty()){
            cerr << "empty component code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml->releaseDomain("ingredient", true);
            continue;
        }

        db = 0;
        db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
        if(!db->existProductComposition(composition_code, ingredient_code)){
            delete xml;
            delete db;
            return false;
        }
        delete db;
    }

    delete xml;
    return true;
}

QString ProductModule::getIngredients(const QString& product_code){
    QString xml_string;
    XmlConfig *xml = 0;
    QPtrList<IngredientData> *ingredient_list = 0;
    IngredientData *ingredient = 0;
    int index, count;
    ProductsModuleDB *db = 0;

    xml_string = "";
    db = 0;
    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    ingredient_list = db->getIngredients(product_code);
    delete db;

    if(!ingredient_list){
        cerr << "product " << product_code.toStdString() << " does not have ingredients " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    if(!ingredient_list)
        return xml_string;

    if(ingredient_list->isEmpty()){
        delete ingredient_list;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->createElementSetDomain("composition");
    xml->createElement("composition_code", product_code);

    count = ingredient_list->count();
    for(index = 0; index < count; index++){
        ingredient = 0;
        ingredient = ingredient_list->at(index);

        xml->createElementSetDomain("ingredient");
        xml->createElement("ingredient_code", ingredient->ingredient_code);
        xml->createElement("quantity", QString::number(ingredient->quantity));
        xml->releaseDomain("ingredient");
    }
    xml->releaseDomain("composition");
    delete ingredient_list;
    delete ingredient;

    if(!xml->validateXmlWithDTD(COMPOSITION_DTD, true)){
        cerr << "xml does not validate against DTD " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }
    xml_string = xml->toString();
    delete xml;

    return xml_string;
}

QString ProductModule::getCompositionsWithIngredient(const QString& ingredient_code){
    XmlConfig *xml = 0;
    QString xml_string;
    QPtrList<ProductData> *products_list{} ;
    int index, count;
    ProductData *p = 0;
    ProductsModuleDB *db = 0;

    xml_string = "";
    db = 0;
    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    //FIXME
    products_list = db->getCompositionsWithIngredient(ingredient_code);
    delete db;

    if(!products_list){
        cerr << "there are no combinations " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return xml_string;
    }

    xml = new XmlConfig();
    xml->createElementSetDomain("products");

    if(!products_list){
        xml->releaseDomain("products");
        xml_string = xml->toString();
        return xml_string;
    }

    count = products_list->count();
    for(index = 0; index < count; index++){
        p = products_list->at(index);
        xml->createElementSetDomain("product");
        xml->createElement("code",p->code);
        xml->createElement("name",p->name);
        xml->releaseDomain("product", true);
    }

    products_list->clear();
    delete products_list;

    xml->releaseDomain("products", true);

    if(!xml->validateXmlWithDTD(PRODUCTS_LIST_DTD, true)){
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml->debug();
        delete xml;
        return xml_string;
    }

    xml_string = xml->toString();
    delete xml;
    return xml_string;
}

QString ProductModule::getProductCodeByProductName(const QString& product_name){
    QString xml_string;
    ProductsModuleDB *db = 0;
    QString name;

    xml_string = "";
    db = 0;
    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);

    name = db->getProductCodeByProductName(product_name);
    return name;
}

int ProductModule::getNumberOfProducts(){
    int ret;
    ProductsModuleDB *db = 0;
    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    ret = db->getNumberOfProducts();
    delete db;
    return ret;
}




void ProductModule::saveBarXml(){
    int i, count,j,ing_count;
    QPtrList<ProductData> *products = 0;
    QPtrList<ProductOptionData> *options = 0;
    QPtrList<IngredientData> *ingredients = 0;
    ProductData *product = 0;
    ProductData *aux_product = 0;
    ProductOptionData *option = 0;
    IngredientData *ingredient = 0;
    ProductsModuleDB *db = 0;
    ProductOptionsModuleDB *odb = 0;
    XmlConfig *xml = 0;
    int position=0;

    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    odb = new ProductOptionsModuleDB(PRODUCT_CONNECTION_NAME+"option_module",PRODUCT_PATH_FILE);

    xml = new XmlConfig(XML_BAR_PATH);
    xml->delDomain();
    while (xml->howManyTags("products"))
        xml->deleteElement("products");
    xml->createElementSetDomain("products");


    products = db->getProducts();

    if (!products){
        xml->save();
        delete xml;
        delete db;
        return;
    }

    count = products->count();
    for (i=0;i<count;i++){
        product = products->at(i);
        emit productAtXml(i,product->name);
        options = odb->getProductOptions(product->code);
        ingredients = db->getIngredients(product->code);
        if (!ingredients)
            continue;
        ing_count = ingredients->count();
        ingredient = ingredients->at(0);
        position = productPosition(ingredient->ingredient_code,xml);
        if (position<0){
            aux_product = db->getProduct(ingredient->ingredient_code);
            xml->createElementSetDomain("product");
            xml->createElement("name",ingredient->ingredient_code);
            if (!aux_product->logo.isEmpty())
                xml->createElement("icon",PRODUCT_MODULE_ICON_PATH+aux_product->logo);
            delete aux_product;
        }
        else{
            xml->setDomain("product["+QString::number(position)+"]");
        }
        if (ing_count>1){
            position = productPosition("solo",xml);
            if (position<0)
                xml->createElement("product.name","solo");
        }
        for (j=1;j<ing_count;j++){
            ingredient = ingredients->at(j);
            position = productPosition(ingredient->ingredient_code,xml);
            if (position>=0){
                xml->setDomain("product["+QString::number(j)+"]");
                continue;
            }
            else{
                xml->createElementSetDomain("product");
                xml->createElement("name", ingredient->ingredient_code);
            }
        }
        this->writeOptions(options, xml);
        xml->releaseDomain("product");
        if (options){
            options->clear();
            delete options;
        }
        if (ingredients){
            ingredients->clear();
            delete ingredients;
        }
    }

    products->clear();
    delete products;
    xml->delDomain();
    xml->save();
    delete xml;
}


void ProductModule::writeOptions(QList<ProductOptionData*>* options, XmlConfig *xml){
    ProductOptionData *option = 0;
    int index, count, i, j;

    if (!options)
        return;

    count = options->count();
    if (!count)
        return;
    xml->pushDomain();
    if(!xml->setDomain("options"))
        xml->createElementSetDomain("options");
    count = options->count();
    for(index = 0; index < count; index++){
        option = 0;
        option = options->at(index);
        j = xml->howManyTags("option");
        for(i = 0; i < j; i++){
            xml->setDomain("option[" + QString::number(i) + "]");
            if(xml->readString("type") == option->option_type)
                break;
            xml->releaseDomain("option", false);
        }
        if(i == j){
            xml->createElementSetDomain("option");
            xml->createElement("type", option->option_type);
        }
        j = xml->howManyTags("value");
        for(i = 0; i < j; i++)
            if(xml->readString("value") == option->option_name)
                break;
        if(i == j)
            xml->createElement("value", option->option_name);
        if(option->is_default)
            xml->createAttribute("value[" + QString::number(i) + "]", "type", "default");
        xml->releaseDomain("option", false);
    }
    xml->releaseDomain("options");
    xml->popDomain();
}





int ProductModule::productPosition(const QString& name, XmlConfig *xml){
    //if the xml haven't this product's name, return a negative number
    int position = -1;
    int i, count;
    QString auxiliar;

    count = xml->howManyTags("product");
    for(i = 0; i < count; i++){
        //Read the product's name
        auxiliar = xml->readString("product["+QString::number(i)+"].name");

        if (auxiliar == name){
            position = i;
            break;
        }
    }

    return position;
}

void ProductModule::setProductAtPrinter(const QString& product_name, const QString& printer,bool on){
    ProductsModuleDB *db = 0;
    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    db->setProductAtPrinter(product_name,printer,on);
    delete db;
}

bool ProductModule::getProductAtPrinter(const QString& product_name, const QString& printer){
    bool ret = false;
    ProductsModuleDB *db = 0;
    db = new ProductsModuleDB(PRODUCT_CONNECTION_NAME, PRODUCT_PATH_FILE);
    ret = db->getProductAtPrinter(product_name,printer);
    delete db;
    return ret;
}














