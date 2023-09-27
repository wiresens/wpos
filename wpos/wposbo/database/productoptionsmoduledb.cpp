/***************************************************************************
                          productoptionsmoduledb.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#include "productoptionsmoduledb.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <iostream>
namespace std{}
using namespace std;

ProductOptionsModuleDB::ProductOptionsModuleDB(QString _name_connection, QString _path_connection, QString _database, QString _username, QString _passwd):
    BasicDatabase(_name_connection,_path_connection,_database,_username,_passwd){
}

ProductOptionsModuleDB::ProductOptionsModuleDB(const QString& _connection_name,XmlConfig *xml):
    BasicDatabase(_connection_name,xml){
}

ProductOptionsModuleDB::ProductOptionsModuleDB(const QString& _connection_name,const QString& configuration_path):
    BasicDatabase(_connection_name, configuration_path){
}


ProductOptionsModuleDB::~ProductOptionsModuleDB(){
}

bool ProductOptionsModuleDB::existOption(const QString& option_type, const QString& option_name){
    QSqlQuery *query = 0;
    QString sql;

    sql    = "SELECT prod_option ";
    sql += "FROM products_options_list ";
    sql += "WHERE (option_type = '" + option_type + "') AND ";
    sql +=               "(prod_option = '" + option_name + "');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(query->next()){
        delete query;
        disConnect();
        return true;
    }
    else{
        delete query;
        disConnect();
        return false;
    }
}

bool ProductOptionsModuleDB::existOptionType(const QString& option_type){
    QSqlQuery *query = 0;
    QString sql;

    sql    = "SELECT option_type ";
    sql += "FROM option_types ";
    sql += "WHERE option_type = '" + option_type + "';";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(query->next()){
        delete query;
        disConnect();
        return true;
    }
    else{
        delete query;
        disConnect();
        return false;
    }
}


bool ProductOptionsModuleDB::deleteOption(const QString& option_type, const QString& option_name){
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    sql    = "DELETE FROM products_options_list ";
    sql += "WHERE (option_type = '" + option_type + "') AND ";
    sql +=               "(prod_option = '" + option_name + "');";

    connect();
    if(!isConnected()){
        cerr << "Fallo de la conexion en la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->getDB());

    error = query->lastError();
    delete query;
    disConnect();

    switch(error.type()){
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        return false;
    default:
        return false;
        break;
    }
}

bool ProductOptionsModuleDB::deleteOptionType(const QString& option_type){
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(option_type.isEmpty()){
        cerr << "Tipo de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    sql    = "DELETE FROM option_types ";
    sql += "WHERE (option_type = '" + option_type + "');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->getDB());

    error = query->lastError();
    delete query;
    disConnect();

    switch (error.type()){
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        return false;
    default:
        return false;
        break;
    }
}

QString ProductOptionsModuleDB::getDescriptionOption(const QString& option_type, const QString& option_name){
    QSqlQuery *query = 0;
    QString sql, description;

    description = "";
    sql    = "SELECT description ";
    sql += "FROM products_options_list ";
    sql += "WHERE option_type = '" + option_type + "' AND ";
    sql +=               "option_name = '" + option_name + "';";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return description;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return description;
    }

    if(query->next())
        description = query->value(0).toString();

    delete query;
    disConnect();
    return description;
}

QString ProductOptionsModuleDB::getDescriptionOptionType(const QString& option_type){
    QSqlQuery *query = 0;
    QString sql, description;

    description = "";
    sql    = "SELECT description ";
    sql += "FROM option_types ";
    sql += "WHERE option_type = '" + option_type + "'";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return description;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return description;
    }

    if(query->next())
        description = query->value(0).toString();

    delete query;
    disConnect();
    return description;
}

ProductOptionData* ProductOptionsModuleDB::getOption(const QString& option_type, const QString& option_name){
    QSqlQuery *query = 0;
    QString sql;
    ProductOptionData *option = 0;

    sql    = "SELECT option_type, prod_option, description ";
    sql += "FROM products_options_list ";
    sql += "WHERE (option_type = '" + option_type + "') AND ";
    sql +=               "(prod_option = '" + option_name + "');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return option;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return option;
    }

    if(query->next()){
        option = new ProductOptionData;
        option->option_type = query->value(0).toString();
        option->option_name = query->value(1).toString();
        option->description_option = query->value(2).toString();
    }

    delete query;
    disConnect();
    return option;
}

QStringList* ProductOptionsModuleDB::getOptions(const QString& option_type){
    QSqlQuery *query = 0;
    QString sql;
    QStringList *options = 0;

    if(option_type.isEmpty()){
        cerr << "Tipo de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return options;
    }

    sql    = "SELECT prod_option ";
    sql += "FROM products_options_list ";
    sql += "WHERE (option_type='" + option_type + "');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return options;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return options;
    }

    options = new QStringList();

    while(query->next())
        options->append(query->value(0).toString());

    delete query;
    disConnect();
    return options;
}

QPtrList<ProductOptionData>* ProductOptionsModuleDB::getAllOptions(){
    QSqlQuery *query = 0;
    QString sql;
    QPtrList<ProductOptionData> *ret = 0;
    ProductOptionData *option = 0;

    sql  = "SELECT option_type,prod_option, logo, description ";
    sql += "FROM products_options_list ";
    sql += "ORDER BY option_type; ";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return ret;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return ret;
    }

    ret = new QPtrList<ProductOptionData>;
    while(query->next()){
        option = new ProductOptionData;
        option->option_type = (query->value(0).toString());
        option->option_name = (query->value(1).toString());
        ret->append(option);
    }

    delete query;
    disConnect();
    return ret;
}


ProductOptionData* ProductOptionsModuleDB::getOptionType(const QString& option_type){
    QSqlQuery *query = 0;
    QString sql;
    ProductOptionData *option = 0;

    if(option_type.isEmpty()){
        cerr << "Tipo de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return option;
    }

    sql    = "SELECT option_type, description ";
    sql += "FROM option_types ";
    sql += "WHERE option_type = '" + option_type + "';";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return option;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return option;
    }

    if(query->next()){
        option = new ProductOptionData;
        option->option_type = query->value(0).toString();
        option->description_type = query->value(1).toString();
    }

    delete query;
    disConnect();
    return option;
}

QStringList* ProductOptionsModuleDB::getOptionTypes(){
    QSqlQuery *query = 0;
    QString sql;
    QStringList *option_types = 0;

    sql    = "SELECT option_type ";
    sql += "FROM option_types;";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return option_types;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return option_types;
    }

    option_types = new QStringList();
    while(query->next())
        option_types->append(query->value(0).toString());

    delete query;
    disConnect();
    return option_types;
}

bool ProductOptionsModuleDB::insertOption(const QString& option_type, const QString& option_name,
                                          const QString& option_description){
    QSqlError error;
    QString sql, aux;
    QSqlQuery *query = 0;

    if(option_type.isEmpty()){
        cerr << "Tipo de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(option_name.isEmpty()){
        cerr << "Nombre de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    sql    = "INSERT INTO products_options_list ";
    sql +=       "(option_type, prod_option, description) ";
    sql += "VALUES ('" + option_type + "', '";
    sql +=                         option_name + "', '";
    sql +=                         option_description + "');";

    if(existOption(option_type, option_name))
        return updateOption(option_type, option_name, option_description);

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion en la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

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

bool ProductOptionsModuleDB::insertOptionType(const QString& option_type,
                                              const QString& description_type){
    QSqlError error;
    QString sql, aux;
    QSqlQuery *query = 0;

    sql    = "INSERT INTO option_types (option_type, description) ";
    sql += "VALUES ('" + option_type + "', '";
    sql +=                         description_type + "');";

    if(existOptionType(option_type))
        return updateOptionType(option_type, description_type);

    connect();
    if(!isConnected()){
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

bool ProductOptionsModuleDB::updateOption(const QString& option_type,
                                          const QString& option_name,
                                          const QString& description_option){
    QSqlError error;
    QString sql, aux;
    QSqlQuery *query = 0;

    if(option_type.isEmpty()){
        cerr << "Tipo de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(option_name.isEmpty()){
        cerr << "Nombre de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    sql    = "UPDATE products_options_list ";
    sql += "SET description = '" + description_option + "' ";
    sql += "WHERE option_type = '" + option_type + "' AND ";
    sql +=               "prod_option = '" + option_name + "';";

    if(!existOption(option_type, option_name))
        return insertOption(option_type, option_name, description_option);

    connect();
    if(!isConnected()){
        cerr << "Fallo de la conexion en la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

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

bool ProductOptionsModuleDB::updateOptionType(const QString& option_type,
                                              const QString& description_type){
    QSqlError error;
    QString sql, aux;
    QSqlQuery *query = 0;

    if(option_type.isEmpty()){
        cerr << "Tipo de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    sql    = "UPDATE option_types ";
    sql += "SET description = '" + description_type + "' ";
    sql += "WHERE option_type = '" + option_type + "';";

    if(!existOptionType(option_type))
        return insertOptionType(option_type, description_type);

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

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

bool ProductOptionsModuleDB::existProductAndOption(const QString& product_code,
                                                   const QString& option_type,
                                                   const QString& option_name){
    bool exist = false;
    QSqlQuery *query = 0;
    QString sql;

    if(product_code.isEmpty()){
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return exist;
    }

    if(option_type.isEmpty()){
        cerr << "Tipo de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return exist;
    }

    if(option_name.isEmpty()){
        cerr << "Nombre de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return exist;
    }

    sql    = "SELECT product_code ";
    sql += "FROM init_prod_options ";
    sql += "WHERE (product_code='" + product_code +"') AND ";
    sql +=               "(option_type='" + option_type + "') AND ";
    sql +=               "(prod_option='" + option_name + "');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return exist;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return exist;
    }

    if(query->next()){
        exist = true;
    }

    delete query;
    disConnect();
    return exist;
}

QPtrList<ProductOptionData>* ProductOptionsModuleDB::getProductOptions(const QString& product_code){
    QSqlQuery *query = 0;
    QString sql, aux;
    QPtrList<ProductOptionData> *options = 0;
    ProductOptionData *option = 0;

    if(product_code.isEmpty()){
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return options;
    }

    sql    = "SELECT t.option_type, t.description, o.prod_option, o.description, i.value, i.is_default ";
    sql += "FROM option_types t, products_options_list o, init_prod_options i ";
    sql += "WHERE i.product_code = '" + product_code + "' AND ";
    sql +=               "i.option_type = t.option_type AND ";
    sql +=               "i.prod_option = o.prod_option;";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return options;
    }

    query = new QSqlQuery(sql, this->getDB());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return options;
    }

    options = new QPtrList<ProductOptionData>;
    while(query->next()){
        option = new ProductOptionData;
        option->option_type = query->value(0).toString();
        option->description_type = query->value(1).toString();
        option->option_name = query->value(2).toString();
        option->description_option = query->value(3).toString();
        option->value = query->value(4).toDouble();

        aux = query->value(5).toString();
        if((aux == "true")||(aux == "t"))
            option->is_default = true;
        else
            option->is_default = false;

        options->append(option);
    }

    delete query;
    disConnect();

    return options;
}

bool ProductOptionsModuleDB::insertOptionToProduct(const QString& product_code,
                                                   const QString& option_type,
                                                   const QString& option_name,
                                                   double value, const QString& is_default){
    QSqlQuery *query = 0;
    QString sql;
    QSqlError error;

    if(option_type.isEmpty()){
        cerr << "option_type vacio" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }
    if(!existOptionType(option_type)){
        insertOptionType(option_type, "");
    }

    if(option_name.isEmpty()){
        cerr << "option_name vacio" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }
    if(!existOption(option_type, option_name)){
        insertOption(option_type, option_name, "");
    }

    if(product_code.isEmpty()){
        cerr << "product_code vacio" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(existProductAndOption(product_code, option_type, option_name))
        return updateOptionToProduct(product_code, option_type, option_name, value, is_default);

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion en la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    sql    = "INSERT INTO init_prod_options ";
    sql +=      "(product_code, option_type, ";
    sql +=       "prod_option, value, ";
    sql +=       "is_default) ";
    sql += "VALUES ('" + product_code + "', '" + option_type + "', ";
    sql +=                "'" + option_name + "', ";
    sql +=                "'" + QString::number(value) + "', ";

    if((is_default == "f")||(is_default == "false")||(is_default.isEmpty()))
        sql += "'f');";

    if((is_default == "t")||(is_default == "true"))
        sql += "'t');";

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
        cerr << "         Sentencia:  " << sql.toStdString() << endl;
    default:
        return false;
    }
}

bool ProductOptionsModuleDB::updateOptionToProduct(const QString& product_code,
                                                   const QString& option_type,
                                                   const QString& option_name,
                                                   double value, const QString& is_default){
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(product_code.isEmpty()){
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__  << endl;
        return false;
    }

    if(option_type.isEmpty()){
        cerr << "Tipo de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if(option_name.isEmpty()){
        cerr << "Nombre de opcion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    sql    = "UPDATE init_prod_options ";
    sql += "SET value = '" + QString::number(value) + "', ";

    if((is_default == "f")||(is_default == "false")||(is_default.isEmpty()))
        sql += "is_default = 'f' ";

    if((is_default == "t")||(is_default == "true"))
        sql += "is_default = 't' ";

    sql += "WHERE product_code = '" + product_code + "' AND ";
    sql +=               "option_type = '" + option_type + "' AND ";
    sql +=               "prod_option = '" + option_name + "';";

    if(!existProductAndOption(product_code, option_type, option_name))
        return insertOptionToProduct(product_code, option_type, option_name, value, is_default);

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

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
        cerr << "Error en" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        return false;
        break;
    }
}


bool ProductOptionsModuleDB::setAllProductOptionValue(const QString& option_type,const QString& option_name, const QString& value){
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(option_type.isEmpty())
        return false;

    if(!existOptionType(option_type))
        return false;

    if(option_name.isEmpty())
        return false;

    if(!existOption(option_type, option_name))
        return false;

    sql  = "SELECT set_init_prod_options ('"+option_type+"', '"+option_name+"', '"+value+"');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion" << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->getDB());
    error = query->lastError();
    delete query;
    disConnect();

    if(error.type() != QSqlError::NoError){
        cerr << "fallo al efectuar la query " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        cerr << sql.toStdString() << endl;
        return false;
    }

    return true;
}

bool ProductOptionsModuleDB::updateAllProductOptionValue(const QString& option_type,const QString& option_name,const QString& value){
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(option_type.isEmpty())
        return false;

    if(!existOptionType(option_type))
        return false;

    if(option_name.isEmpty())
        return false;

    if(!existOption(option_type, option_name))
        return false;

    sql  = "SELECT update_init_prod_options ('"+option_type+"', '"+option_name+"', '"+value+"');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion" << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->getDB());
    error = query->lastError();
    delete query;
    disConnect();

    if(error.type() != QSqlError::NoError){
        cerr << "fallo al efectuar la query " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        cerr << sql.toStdString() << endl;
        return false;
    }

    return true;
}

bool ProductOptionsModuleDB::deleteOptionToProduct(const QString& product_code,const QString& option_type,const QString& option_name){
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(product_code.isEmpty())
        return false;

    if(option_type.isEmpty())
        return false;

    if(option_name.isEmpty())
        return false;


    sql   = "DELETE  from init_prod_options ";
    sql+= "WHERE  product_code='"+product_code+"' AND option_type='"+option_type+"' AND prod_option='"+option_name+"';";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion" << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->getDB());
    error = query->lastError();
    delete query;
    disConnect();

    if(error.type() != QSqlError::NoError){
        cerr << "fallo al efectuar la query " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        cerr << sql.toStdString() << endl;
        return false;
    }

    return true;
}








