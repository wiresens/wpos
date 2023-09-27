/***************************************************************************
                          product.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "productsmoduledb.h"

#include <libbslxml/xmlconfig.h>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <iostream>
using namespace std;

ProductsModuleDB::ProductsModuleDB(
        QString _name_connection,
        QString _path_connection,
        QString _database,
        QString _username,
        QString _passwd)
    : BasicDatabase(_name_connection,
                    _path_connection,
                    _database,
                    _username,
                    _passwd){}

ProductsModuleDB::ProductsModuleDB(
        const QString& _connection_name,
        XmlConfig *xml)
    :BasicDatabase(_connection_name,xml){}

ProductsModuleDB::ProductsModuleDB(
        const QString& _connection_name,
        const QString& configuration_path)
    :BasicDatabase(_connection_name,configuration_path){}

bool ProductsModuleDB::existProduct(const QString& product_code){
    QSqlQuery *query = 0;
    QString sql;
    bool exist = false;

    if(product_code.isEmpty()){
        cerr << "Codigo del producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    sql  = "SELECT COUNT(product_code) FROM products ";
    sql += "WHERE product_code = '" + product_code + "';";
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->next()){
        delete query;
        return false;
    }

    if(query->value(0).toInt() > 0)
        exist = true;

    delete query;
    return exist;
}

bool ProductsModuleDB::deleteProduct(const QString& product_code){
    QSqlQuery *query = 0;
    QSqlError error;
    QString sql;

    if(product_code.isEmpty()){
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    sql = "DELETE FROM products WHERE product_code = '" + product_code + "';";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->getDB());

    error = query->lastError();

    delete query;
    disConnect();

    if(error.type() == QSqlError::NoError)
        return true;
    else
        return false;
}

ProductData* ProductsModuleDB::getProduct(const QString& product_code){
    QSqlQuery *query = 0;
    QString sql, xml_string;
    ProductData *product = 0;

    if(product_code.isEmpty()){
        cerr << "Codigo del producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return product;
    }

    sql    = "SELECT product_code, product, price, tax, logo, description ";
    sql += "FROM products WHERE product_code = '" + product_code + "';";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return product;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        delete query;
        disConnect();
        return product;
    }

    if(query->first()){
        product = new ProductData;

        product->code = query->value(0).toString();
        product->name = query->value(1).toString();
        product->price = query->value(2).toDouble();
        product->tax = query->value(3).toString();
        product->logo = query->value(4).toString();
        product->description = query->value(5).toString();
    }

    delete query;
    disConnect();

    return product;
}

QString ProductsModuleDB::getProductLike(const QString& product_name){
    QSqlQuery *query = 0;
    QString sql, code;

    code = "";
    if(product_name.isEmpty())
        return code;

    sql    = "SELECT product_code ";
    sql += "FROM products ";
    sql += "WHERE product LIKE '" + product_name + "%' ORDER BY product;";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return code;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        delete query;
        disConnect();
        return code;
    }

    if(!query->size()){
        delete query;
        disConnect();
        return code;
    }

    if(query->first())
        code =query->value(0).toString();

    delete query;
    disConnect();
    return code;
}

QStringList* ProductsModuleDB::getFamilies(){
    QString sql;
    QSqlQuery *query = 0;
    QStringList *families = 0;

    sql = "";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return families;
    }

    query = new QSqlQuery(sql, this->getDB());

    families = new QStringList();
    if(!query->isActive()){
        delete query;
        disConnect();
        return families;
    }

    while(query->next())
        families->append(query->value(0).toString());

    delete query;
    disConnect();

    return families;
}

ProductData* ProductsModuleDB::getLogo(const QString& product_name){
    QString sql;
    QSqlQuery *query = 0;
    ProductData *product = 0;

    sql    = "SELECT product_code, product, logo ";
    sql += "FROM products ";
    sql += "WHERE product = '" + product_name + "';";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return product;
    }

    query = new QSqlQuery(sql, this->getDB());

    product = new ProductData;
    if(!query->isActive()){
        delete query;
        disConnect();
        return product;
    }

    if(query->next()){
        product->code = query->value(0).toString();
        product->name = query->value(1).toString();
        product->logo = query->value(2).toString();
    }

    delete query;
    disConnect();
    return product;
}

QPtrList<ProductData>* ProductsModuleDB::getLogos(QStringList products_name){
    QPtrList<ProductData>* products = 0;
    ProductData *product = 0;
    int index, count;
    QString name;

    products = new QPtrList<ProductData>;
    count = products_name.count();
    for(index = 0; index < count; index++){
        product = 0;
        product = getLogo(products_name[index]);
        if(product)
            products->append(product);
    }

    return products;
}

QStringList* ProductsModuleDB::getTaxes(){
    QString aux;
    QSqlQuery *query;
    QString sql;
    QStringList* taxes_list = 0;

    sql    = "SELECT tax ";
    sql += "FROM taxes ";
    sql += "ORDER BY tax;";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return taxes_list;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        delete query;
        disConnect();
        return taxes_list;
    }

    taxes_list = new QStringList();
    while(query->next())
        taxes_list->append(query->value(0).toString());

    delete query;
    disConnect();

    return taxes_list;
}

QPtrList<ProductData>* ProductsModuleDB::getProductsExtend(){
    QPtrList<ProductData> *products_list = 0;
    ProductData *product = 0;
    QString sql, aux;
    QSqlQuery *query = 0;

    sql    = "SELECT product_code, product, price, tax, logo, description ";
    sql += "FROM products ";
    sql += "ORDER BY product;";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        disConnect();
        return products_list;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        delete query;
        disConnect();
        return products_list;
    }

    products_list = new QPtrList<ProductData>;
    while(query->next()){
        product = 0;
        product = new ProductData;

        product->code = query->value(0).toString();
        product->name = query->value(1).toString();
        product->price = (query->value(2).toString()).toDouble();
        product->tax = query->value(3).toString();
        product->logo = query->value(4). toString();
        product->description = query->value(5).toString();

        products_list->append(product);
    }

    delete query;
    disConnect();
    return products_list;
}

QPtrList<ProductData>* ProductsModuleDB::getProducts(){
    QPtrList<ProductData> *products_list = 0;
    ProductData *product = 0;
    QString sql, aux;
    QSqlQuery *query = 0;

    sql    = "SELECT product_code, product, logo ";
    sql += "FROM products ";
    sql += "ORDER BY product;";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        disConnect();
        return products_list;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        delete query;
        disConnect();
        return products_list;
    }

    products_list = new QPtrList<ProductData>;
    while(query->next()){
        product = 0;
        product = new ProductData;

        product->code = query->value(0).toString();
        product->name = query->value(1).toString();
        product->logo = query->value(2). toString();

        products_list->append(product);
    }

    delete query;
    disConnect();
    return products_list;
}

QPtrList<ProductData>* ProductsModuleDB::getUnitaryProducts(){
    QSqlQuery *query = 0;
    QString sql;
    QPtrList<ProductData> *products_list = 0;
    ProductData *product = 0;

    sql    = "SELECT p.product_code, p.product, p.price, p.tax, p.logo, p.description ";
    sql += "FROM products p, prod_composition c ";
    sql += "WHERE p.product_code = c.product_code ";
    sql +=      "AND c.product_code = c.ingredient_code ";
    sql += "ORDER BY product;";

    connect();
    if (!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return products_list;
    }

    query = new QSqlQuery(sql, this->getDB());

    if (!query->isActive()){
        delete query;
        disConnect();
        return products_list;
    }

    //ok we can get the the list of unitary products
    products_list = new QPtrList<ProductData>;
    while (query->next()){
        product = new ProductData;

        product->code = query->value(0).toString();
        product->name = query->value(1).toString();
        product->price = query->value(2).toDouble();
        product->tax = query->value(3).toString();
        product->logo = query->value(4).toString();
        product->description = query->value(5).toString();

        products_list->append(product);
    }

    delete query;
    disConnect();

    return products_list;
}

bool ProductsModuleDB::isUnitaryProduct(const QString& product_code){
    QSqlQuery *query = 0;
    QString sql;
    int num_ingredient;

    num_ingredient = 0;
    if(product_code.isEmpty()){
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    sql = "SELECT COUNT(ingredient_code) ";
    sql += "FROM prod_composition ";
    sql += "WHERE product_code = '" + product_code + "';";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion en la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        disConnect();
        delete query;
        return false;
    }

    if(query->next())
        num_ingredient = query->value(0).toInt();

    delete query;
    disConnect();
    if(num_ingredient == 0)
        return false;

    if(num_ingredient == 1)
        return true;

    if(num_ingredient > 1)
        return false;

    return false;
}

bool ProductsModuleDB::insertProduct(ProductData *product){
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if((product->code).isEmpty()){
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if((product->name).isEmpty()){
        cerr << "Nombre de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(existProduct(product->code))
        return updateProduct(product);

    sql    = "INSERT INTO products (product_code, product, price, tax, description, logo) ";
    sql += "VALUES ('"+ product->code +"', '" + product->name +"',";
    sql +=                 " " + QString::number(product->price) + ", '" + product->tax + "',";
    sql +=                 " '" + product->description + "', '" + product->logo + "');";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion en la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

    //     cout << sql.toStdString() << endl;
    query = new QSqlQuery(sql, this->getDB());

    error = query->lastError();
    //     cout << "Error " << error.type() << endl;
    delete query;
    disConnect();

    switch(error.type()){
    case QSqlError::NoError:
        return true;
        break;
    case QSqlError::StatementError:
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
    default:
        return false;
        break;
    }
}

bool ProductsModuleDB::updateProduct(ProductData *product){
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if((product->code).isEmpty()){
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if((product->name).isEmpty()){
        cerr << "Nombre de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }
    //anadir la actualizacion del logo
    sql    = "UPDATE products ";
    sql += "SET product = '" + product->name + "', ";
    sql +=        "description = '" + product->description + "', ";
    sql +=        "price = " + QString::number(product->price) + ", ";
    sql +=        "tax = '" + product->tax + "', ";
    sql +=        "logo = '" + product->logo+ "' ";
    sql += "WHERE product_code = '" + product->code + "';";

    if(!existProduct(product->code))
        return insertProduct(product);

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

    //     cout << sql.toStdString() << endl;
    query = new QSqlQuery(sql, this->getDB());

    error = query->lastError();
    delete query;
    disConnect();

    switch(error.type()){
    case QSqlError::NoError:
        return true;
        break;
    case QSqlError::StatementError:
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
    default:
        return false;
        break;
    }
}

QPtrList<ProductData>* ProductsModuleDB::getCompositions(){
    QSqlQuery *query = 0;
    QString sql;
    QPtrList<ProductData> *products_list = 0;
    ProductData *product = 0;

    sql    = "SELECT DISTINCT p.product_code, p.product, p.price, p.tax, p.logo, p.description ";
    sql += "FROM products p, prod_composition c ";
    sql += "WHERE NOT(c.ingredient_code = c.product_code) ";
    sql +=               "AND c.product_code = p.product_code ";
    sql += "ORDER BY product;";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return products_list;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        delete query;
        disConnect();
        return products_list;
    }

    products_list = new QPtrList<ProductData>;
    while(query->next()){
        product = new ProductData;

        product->code = query->value(0).toString();
        product->name = query->value(1).toString();
        product->price = query->value(2).toDouble();
        product->tax = query->value(3).toString();
        product->logo = query->value(4).toString();
        product->description = query->value(5).toString();

        products_list->append(product);
    }

    disConnect();
    return products_list;
}

bool ProductsModuleDB::insertProductComposition(const QString& composition_code,
                                                const QString& ingredient_code,
                                                double quantity,
                                                int ordering){
    QSqlError error;
    QString sql;
    QSqlQuery *query = 0;

    if(existProductComposition(composition_code, ingredient_code))
        return updateProductComposition(composition_code, ingredient_code, quantity,ordering);

    if(composition_code.isEmpty()){
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(ingredient_code.isEmpty()){
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    //     if(quantity == 0.00){
    //          cerr << "Cantidad de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
    //          return false;
    //     }

    sql   = "INSERT INTO prod_composition (product_code, ingredient_code, quantity,ordering) ";
    sql += "VALUES ('"+ composition_code + "', '" + ingredient_code + "', ";
    sql += QString::number(quantity, 'f', 2) + ","+QString::number(ordering)+");";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

    //     cout << sql.toStdString() << endl;
    query = new QSqlQuery(sql, this->getDB());

    error = query->lastError();
    delete query;
    disConnect();

    switch(error.type()){
    case QSqlError::NoError:
        return true;
        break;
    case QSqlError::StatementError:
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
    default:
        return false;
        break;
    }
}

bool ProductsModuleDB::updateProductComposition(const QString& composition_code,
                                                const QString& ingredient_code,
                                                double quantity,
                                                int ordering){
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(!existProductComposition(composition_code, ingredient_code))
        return insertProductComposition(composition_code, ingredient_code, quantity,ordering);

    if(composition_code.isEmpty()){
        cerr << "Codigo de composicion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(ingredient_code.isEmpty()){
        cerr << "Codigo de ingrediente vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(quantity == 0.00){
        cerr << "Cantidad de ingrediente igual a 0" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    sql    = "UPDATE prod_composition ";
    sql += "SET quantity = " + QString::number(quantity, 'f', 2) + ", ";
    sql += "ordering="+QString::number(ordering)+" ";
    sql += "WHERE product_code = '" + composition_code + "' AND ";
    sql +=               "ingredient_code = '" + ingredient_code + "';";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    cout << sql.toStdString() << endl;
    query = new QSqlQuery(sql, this->getDB());

    error = query->lastError();
    delete query;
    disConnect();

    switch(error.type()){
    case QSqlError::NoError:
        return true;
        break;
    case QSqlError::StatementError:
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
    default:
        return false;
        break;
    }
}

bool ProductsModuleDB::existProductComposition(const QString& composition_code,
                                               const QString& ingredient_code){
    QSqlQuery *query = 0;
    QString sql;

    if(composition_code.isEmpty()){
        cerr << "Codigo de composicion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(ingredient_code.isEmpty()){
        cerr << "Codigo de ingrediente vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    sql    = "SELECT product_code ";
    sql += "FROM prod_composition ";
    sql += "WHERE product_code = '" + composition_code;
    sql += "' AND ingredient_code = '" + ingredient_code + "';";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        delete query;
        disConnect();
        return false;
    }

    if(!query->next()){
        delete query;
        disConnect();
        return false;
    }

    delete query;
    disConnect();
    return true;
}

QPtrList<IngredientData>* ProductsModuleDB::getIngredients(const QString& product_code){
    QSqlQuery *query = 0;
    QString sql;
    QPtrList<IngredientData> *ingredients_list = 0;
    IngredientData *ingredient = 0;

    if(product_code.isEmpty())
        return ingredients_list;

    sql    = "SELECT ingredient_code, quantity ";
    sql += "FROM prod_composition ";
    sql += "WHERE (product_code = '" + product_code + "') ORDER BY ordering;";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return ingredients_list;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        cerr << "No esta activa " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete query;
        disConnect();
        return ingredients_list;
    }

    if(!query->size()){
        cerr << "tamano incorrecto " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete query;
        disConnect();
        return ingredients_list;
    }

    ingredients_list = new QPtrList<IngredientData>;
    while(query->next()){
        ingredient = 0;
        ingredient = new IngredientData;

        ingredient->ingredient_code = query->value(0).toString();
        ingredient->quantity = query->value(1).toDouble();

        if(ingredient)
            ingredients_list->append(ingredient);
    }

    delete query;
    disConnect();
    return ingredients_list;
}

QPtrList<ProductData>* ProductsModuleDB::getCompositionsWithIngredient(const QString& ingredient_code){
    QString sql;
    QPtrList<ProductData> *product_list = 0;
    ProductData *p=0;
    QSqlQuery *query = 0;
    QSqlError error;

    sql = "SELECT  p.product_code, p.product, p.logo  ";
    sql += "FROM products p, prod_composition c ";
    sql += "WHERE c.ingredient_code = '" + ingredient_code + "' AND ";
    sql +=               "NOT c.ingredient_code = c.product_code AND ";
    sql +=               "c.product_code = p.product_code ";
    sql += "ORDER BY p.product_code;";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return product_list;
    }

    query = new QSqlQuery(sql, this->getDB());
    error = query->lastError();
    switch(error.type()){
    case QSqlError::NoError:
        break;
    case QSqlError::StatementError:
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        break;
    default:
        break;
    }

    product_list = new QPtrList<ProductData>;
    while(query->next()){
        p = new ProductData;
        p->code = query->value(0).toString();
        p->name = query->value(1).toString();
        p->logo = query->value(2).toString();
        product_list->append(p);
    }

    delete query;
    disConnect();

    return product_list;
}


QString ProductsModuleDB::getProductCodeByProductName(const QString& product_name){
    QString sql;
    QString ret="";
    QSqlQuery *query = 0;

    sql   = "SELECT product_code from products where product='"+product_name+"' ; ";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return ret;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->size()){
        delete query;
        disConnect();
        return ret;
    }

    query->first();
    ret =query->value(0).toString();

    delete query;
    disConnect();
    return ret;
}

int ProductsModuleDB::getNumberOfProducts(){
    QString sql;
    int ret=-1;
    QSqlQuery *query = 0;
    sql = "SELECT count(product_code) FROM products where product_code!='varios';";

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return ret;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->size()){
        delete query;
        disConnect();
        return ret;
    }

    query->first();
    ret =query->value(0).toInt();

    delete query;
    disConnect();
    return ret;
}


void ProductsModuleDB::setProductAtPrinter(const QString& product_name,const QString& printer,bool on){
    QString sql;
    QSqlQuery *query = 0;
    QSqlError error;
    QString product_code;

    product_code = this->getProductCodeByProductName(product_name);

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return;
    }
    if (on){
        sql = "INSERT INTO product_printer_types (product_code,printer_type) ";
        sql += "VALUES ('"+product_code+ "','" + printer +"') ;";
    }
    else{
        sql = "DELETE FROM product_printer_types WHERE ";
        sql += "product_code='"+product_code+"' AND printer_type='"+printer+"' ;";
    }

    query = new QSqlQuery(sql, this->getDB());

    error = query->lastError();
    switch(error.type()){
    case QSqlError::NoError:
        break;
    case QSqlError::StatementError:
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        break;
    default:
        break;
    }

    delete query;
    disConnect();
}

bool ProductsModuleDB::getProductAtPrinter(const QString& product_name,const QString& printer){
    bool ret = false;
    QString sql;
    QSqlQuery *query = 0;
    QSqlError error;
    QString product_code;

    product_code = this->getProductCodeByProductName(product_name);

    connect();
    if(!connect()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }
    sql = "SELECT product_code FROM product_printer_types ";
    sql +="WHERE product_code='"+product_code+"' AND printer_type='"+printer+"';";

    query = new QSqlQuery(sql, this->getDB());

    if(query->size()){
        ret = true;
    }

    delete query;
    disConnect();
    return ret;
}


