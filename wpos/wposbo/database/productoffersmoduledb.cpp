/***************************************************************************
                          offer.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "productoffersmoduledb.h"

#include <QSqlError>
#include <QVariant>

#include <iostream>
using namespace std;

ProductOffersModuleDB::ProductOffersModuleDB(
        QString _name_connection,
        QString _path_connection,
        QString _database,
        QString _username,
        QString _passwd):
    BasicDatabase(_name_connection,
                  _path_connection,
                  _database,
                  _username,
                  _passwd){}

ProductOffersModuleDB::ProductOffersModuleDB(
        const QString& _connection_name,
        XmlConfig *xml):
    BasicDatabase(_connection_name,xml){}

ProductOffersModuleDB::ProductOffersModuleDB(
        const QString& _connection_name,
        const QString& configuration_path):
    BasicDatabase(_connection_name,configuration_path){}

ProductOffersModuleDB::~ProductOffersModuleDB(){}

bool ProductOffersModuleDB::existOffer(
        const QString& offer_type,
        const QString& offer_name)
{
    QString sql;
    QSqlQuery *query = 0;
    bool exist = false;

    if(offer_type.isEmpty())
        return false;
    if(!existOfferType(offer_type))
        return false;

    if(offer_name.isEmpty())
        return false;

    sql    = "SELECT prod_offer ";
    sql += "FROM offers_list ";
    sql += "WHERE offer_type = '" + offer_type + "' AND ";
    sql +=               "prod_offer = '" + offer_name + "';";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    if(!query->isActive()){
        delete query;
        disConnect();
        return false;
    }

    if(!query->size()){
        delete query;
        disConnect();
        return false;
    }

    if(query->next())
        exist = true;

    delete query;
    disConnect();
    return exist;
}

bool ProductOffersModuleDB::existOfferType(const QString& offer_type)
{
    QString sql;
    QSqlQuery *query = 0;
    bool exist = false;

    if(offer_type.isEmpty())
        return false;

    sql    = "SELECT offer_type ";
    sql += "FROM offer_types ";
    sql += "WHERE offer_type = '" + offer_type + "';";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());
    if(!query->isActive()){
        delete query;
        disConnect();
        return false;
    }

    if(!query->size()){
        delete query;
        disConnect();
        return false;
    }

    if(query->next())
        exist = true;

    delete query;
    disConnect();
    return exist;
}

bool ProductOffersModuleDB::deleteOffer(
        const QString& offer_type,
        const QString& offer_name)
{
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(offer_type.isEmpty())
        return false;
    if(!existOfferType(offer_type))
        return false;

    if(offer_name.isEmpty())
        return false;
    if(!existOffer(offer_type, offer_name))
        return false;

    sql =    "DELETE FROM offers_list ";
    sql += "WHERE (offer_type = '" + offer_type + "') AND ";
    sql +=               "(prod_offer = '" + offer_name + "');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    error = query->lastError();
    delete query;
    disConnect();

    if(error.type() == QSqlError::NoError)
        return true;
    else
        return false;
}

bool ProductOffersModuleDB::deleteOfferType(const QString& offer_type)
{
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(offer_type.isEmpty()){
        cerr << "Tipo de oferta vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    sql =    "DELETE FROM offer_types ";
    sql += "WHERE (offer_type = '" + offer_type + "');";

    connect();
    if(!isConnected()){
        cerr << "Fallo de la conexion en la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    error = query->lastError();
    delete query;
    disConnect();

    if(error.type() != QSqlError::NoError){
        cerr << "Fallo la query " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << sql.toStdString() << endl;
        return false;
    }
    return true;
}

QString ProductOffersModuleDB::getDescriptionOffer(
        const QString& offer_type,
        const QString& offer_name)
{
    QSqlQuery *query = 0;
    QString sql, description;

    description = "";
    sql    = "SELECT description ";
    sql += "FROM offers_list ";
    sql += "WHERE offer_type = '" + offer_type + "' AND ";
    sql +=               "prod_offer = '" + offer_name + "';";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return description;
    }

    query = new QSqlQuery(sql, this->dbHandle());

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

QString ProductOffersModuleDB::getDescriptionOfferType(
        const QString& offer_type)
{
    QSqlQuery *query = 0;
    QString sql, description;

    description = "";
    sql    = "SELECT description ";
    sql += "FROM offer_types ";
    sql += "WHERE offer_type = '" + offer_type + "'";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return description;
    }

    query = new QSqlQuery(sql, this->dbHandle());

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

QString ProductOffersModuleDB::getCppOperator(const QString& offer_type)
{
    QSqlQuery *query = 0;
    QString sql, cpp_operator;

    cpp_operator = "";
    sql    = "SELECT cpp_operator ";
    sql += "FROM offer_types ";
    sql += "WHERE offer_type = '" + offer_type + "'";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return cpp_operator;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return cpp_operator;
    }

    if(query->next())
        cpp_operator = query->value(0).toString();

    delete query;
    disConnect();
    return cpp_operator;
}

QString ProductOffersModuleDB::getLogoOffer(
        const QString& offer_type,
        const QString& offer_name)
{
    QSqlQuery *query = 0;
    QString sql, logo;

    logo = "";

    if(offer_type.isEmpty())
        return logo;
    if(!existOfferType(offer_type))
        return logo;

    if(offer_name.isEmpty())
        return logo;
    if(!existOffer(offer_type, offer_name))
        return logo;

    sql    = "SELECT logo ";
    sql += "FROM offers_list ";
    sql += "WHERE (offer_type = '" + offer_type + "') AND ";
    sql += "(prod_offer = '" + offer_name + "');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return logo;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql : " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return logo;
    }

    if(query->next())
        logo = query->value(0).toString();

    delete query;
    disConnect();

    return logo;
}

ProductOfferData* ProductOffersModuleDB::getOffer(
        const QString& offer_type,
        const QString& offer_name)
{
    QSqlQuery *query = 0;
    QString sql;
    ProductOfferData *offer = 0;

    if(offer_type.isEmpty())
        return offer;
    if(!existOfferType(offer_type))
        return offer;

    if(offer_name.isEmpty())
        return offer;
    if(!existOffer(offer_type, offer_name))
        return offer;

    sql    = "SELECT offer_type, prod_offer, description, logo ";
    sql += "FROM offers_list ";
    sql += "WHERE (offer_type = '" + offer_type + "') AND ";
    sql +=  "(prod_offer = '" + offer_name + "');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return offer;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql : " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return offer;
    }

    if(query->next()){
        offer = new ProductOfferData;
        offer->offer_type = query->value(0).toString();
        offer->offer_name = query->value(1).toString();
        offer->description_offer = query->value(2).toString();
        offer->logo = query->value(3).toString();
    }

    delete query;
    disConnect();

    return offer;
}

QList<ProductOfferData*>* ProductOffersModuleDB::getAllOffers()
{
    QSqlQuery *query = 0;
    QString sql;
    QList<ProductOfferData*> *offers = 0;
    ProductOfferData *offer = 0;


    sql  = "SELECT offer_type, prod_offer, logo, description ";
    sql += "FROM offers_list ;";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return offers;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return offers;
    }

    offers = new QList<ProductOfferData*>;
    while(query->next()){
        offer = new ProductOfferData;
        offer->offer_type = query->value(0).toString();
        offer->offer_name = query->value(1).toString();
        offer->logo = query->value(2).toString();
        offer->description_offer = query->value(3).toString();
        offers->append(offer);
    }
    delete query;
    disConnect();
    return offers;
}


QList<ProductOfferData*>* ProductOffersModuleDB::getOffers(
        const QString& offer_type)
{
    QSqlQuery *query = 0;
    QString sql;
    QList<ProductOfferData*> *offers = 0;
    ProductOfferData *offer = 0;

    if(offer_type.isEmpty())
        return offers;
    if(!existOfferType(offer_type))
        return offers;

    sql    = "SELECT t.offer_type, t.description, o.prod_offer, o.description, t.cpp_operator, o.logo ";
    sql += "FROM offers_list o, offer_types t ";
    sql += "WHERE (t.offer_type = '" + offer_type + "') AND ";
    sql +=               "(t.offer_type = o.offer_type);";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return offers;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return offers;
    }

    offers = new QList<ProductOfferData*>;
    while(query->next()){
        offer = new ProductOfferData;

        offer->offer_type = query->value(0).toString();
        offer->description_type = query->value(1).toString();
        offer->offer_name = query->value(2).toString();
        offer->description_offer = query->value(3).toString();
        offer->cpp_operator = query->value(4).toString();
        offer->logo = query->value(5).toString();

        offers->append(offer);
    }

    delete query;
    disConnect();

    return offers;
}

ProductOfferData* ProductOffersModuleDB::getOfferType(
        const QString& offer_type)
{
    QSqlQuery *query = 0;
    QString sql;
    ProductOfferData *offer = 0;

    if(offer_type.isEmpty())
        return offer;

    sql    = "SELECT offer_type, cpp_operator, description ";
    sql += "FROM offer_types ";
    sql += "WHERE offer_type = '" + offer_type + "';";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return offer;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return offer;
    }

    if(!query->size()){
        delete query;
        disConnect();
        return offer;
    }

    if(query->next()){
        offer = new ProductOfferData;
        offer->offer_type = query->value(0).toString();
        offer->description_type = query->value(2).toString();
        offer->cpp_operator = query->value(1).toString();
    }

    delete query;
    disConnect();

    return offer;
}

QStringList* ProductOffersModuleDB::getOfferTypes()
{
    QSqlQuery *query = 0;
    QString sql;
    QStringList *offer_types = 0;

    sql    = "SELECT offer_type ";
    sql += "FROM offer_types ";
    sql += "ORDER BY offer_type;";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return offer_types;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return offer_types;
    }

    offer_types = new QStringList();
    while(query->next()){
        offer_types->append(query->value(0).toString());
    }

    delete query;
    disConnect();
    return offer_types;
}

bool ProductOffersModuleDB::insertOffer(
        const QString& offer_type,
        const QString& offer_name,
        const QString& description,
        const QString& logo)
{
    QSqlError error;
    QString sql;
    QSqlQuery *query = 0;

    if(offer_type.isEmpty())
        return false;
    if(!existOfferType(offer_type))
        return false;

    if(offer_name.isEmpty())
        return false;

    sql = "INSERT INTO offers_list ";
    sql += "(offer_type, prod_offer, description, logo) ";
    sql += "VALUES ('" + offer_type + "', '";
    sql += offer_name + "', '";
    sql += description + "', '";
    sql += logo + "');";

    if(!existOfferType(offer_type))
        return false;

    if(existOffer(offer_type, offer_name))
        return updateOffer(offer_type, offer_name, description,logo);

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    error = query->lastError();
    delete query;
    disConnect();

    if(error.type() == QSqlError::NoError)
        return true;
    else
        return false;
}

bool ProductOffersModuleDB::insertOfferType(
        const QString& offer_type,
        const QString& description,
        const QString& cpp_operator)
{
    QSqlError error;
    QString sql;
    QSqlQuery *query = 0;

    if(offer_type.isEmpty())
        return false;

    sql    = "INSERT INTO offer_types (offer_type, cpp_operator, description) ";
    sql += "VALUES ('" + offer_type + "', '";
    sql +=                         cpp_operator + "', '";
    sql +=                         description + "');";

    if(existOfferType(offer_type))
        return updateOfferType(offer_type, description, cpp_operator);

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    error = query->lastError();
    delete query;
    disConnect();

    if(error.type() == QSqlError::NoError)
        return true;
    else
        return false;
}

int ProductOffersModuleDB::getNumOfferTypes()
{
    QSqlQuery *query = 0;
    QString sql;
    int count;

    count = 0;
    sql    = "SELECT COUNT(offer_type) ";
    sql += "FROM offer_types;";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return count;
    }

    query = new QSqlQuery(sql, this->dbHandle());
    if(query->next())
        count = query->value(0).toInt();

    delete query;
    disConnect();
    return count;
}

bool ProductOffersModuleDB::updateOffer(
        const QString& offer_type,
        const QString& offer_name,
        const QString& description_offer,
        const QString& logo)
{
    QSqlError error;
    QString sql;
    QSqlQuery *query = 0;

    if(offer_type.isEmpty())
        return false;
    if(!existOfferType(offer_type))
        return false;

    if(offer_name.isEmpty())
        return false;

    sql    = "UPDATE offers_list ";
    sql += "SET description = '" + description_offer + "', logo ='"+logo+"' ";
    sql += "WHERE offer_type = '" + offer_type + "' AND ";
    sql +=               "prod_offer = '" + offer_name + "';";

    if(!existOffer(offer_type, offer_name))
        return insertOffer(offer_type, offer_name, description_offer,logo);

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    error = query->lastError();
    delete query;
    disConnect();

    if(error.type() == QSqlError::NoError)
        return true;
    else
        return false;
}

bool ProductOffersModuleDB::updateOfferType(
        const QString& offer_type,
        const QString& description_type,
        const QString& cpp_operator)
{
    QSqlError error;
    QString sql;
    QSqlQuery *query = 0;

    if(offer_type.isEmpty())
        return false;

    sql    = "UPDATE offer_types ";
    sql += "SET description = '" + description_type + "', ";
    sql +=         "cpp_operator = '" + cpp_operator + "' ";
    sql += "WHERE offer_type = '" + offer_type + "';";

    if(!existOfferType(offer_type))
        return insertOfferType(offer_type, description_type, cpp_operator);

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    error = query->lastError();
    delete query;
    disConnect();

    if(error.type() == QSqlError::NoError)
        return true;
    else
        return false;
}

QList<ProductOfferData*>* ProductOffersModuleDB::getProductOffers(
        const QString& product_code)
{
    QSqlQuery *query = 0;
    QString sql;
    QList<ProductOfferData*> *offers = 0;
    ProductOfferData *offer = 0;

    if(product_code.isEmpty()){
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return offers;
    }

    sql    = "SELECT o.offer_type, t.description, t.cpp_operator, ";
    sql +=               "o.prod_offer, o.description, i.value, i.is_default, o.logo ";
    sql += "FROM init_prod_offers i, offers_list o, offer_types t ";
    sql += "WHERE i.product_code = '" + product_code + "' AND ";
    sql +=              "o.offer_type = i.offer_type AND ";
    sql +=              "o.prod_offer = i.prod_offer AND ";
    sql +=              "o.offer_type = t.offer_type;";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return offers;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    if(!query->isActive()){
        delete query;
        disConnect();
        return offers;
    }

    offers = new QList<ProductOfferData*>;
    while(query->next()){
        offer = new ProductOfferData;

        offer->offer_type = query->value(0).toString();
        offer->description_type = query->value(1).toString();
        offer->cpp_operator = query->value(2).toString();
        offer->offer_name = query->value(3).toString();
        offer->description_offer = query->value(4).toString();
        offer->value = (query->value(5).toString()).toDouble();
        offer->is_default = query->value(6).toBool();
        offer->logo = query->value(7).toString();

        offers->append(offer);
    }

    delete query;
    disConnect();

    return offers;
}

bool ProductOffersModuleDB::existProductAndOffer(
        const QString& product_code,
        const QString& offer_type,
        const QString& offer_name)
{
    bool exist = false;
    QSqlQuery *query = 0;
    QString sql;

    if(product_code.isEmpty())
        return false;

    if(offer_type.isEmpty())
        return false;
    if(!existOfferType(offer_type))
        return false;

    if(offer_name.isEmpty())
        return false;
    if(!existOffer(offer_type, offer_name))
        return false;

    sql    = "SELECT product_code ";
    sql += "FROM init_prod_offers ";
    sql += "WHERE (product_code='" + product_code + "') AND ";
    sql +=               "(offer_type='" + offer_type + "') AND ";
    sql +=               "(prod_offer='" + offer_name + "');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    if(!query->isActive()){
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        delete query;
        disConnect();
        return false;
    }

    if(query->next())
        exist = true;

    delete query;
    disConnect();
    return exist;
}

bool ProductOffersModuleDB::insertOfferToProduct(
        const QString& product_code,
        const QString& offer_type,
        const QString& offer_name,
        double value,
        const QString& is_default)
{
    QSqlQuery *query = 0;
    QSqlError error;
    QString sql;

    if(offer_type.isEmpty())
        return false;
    if(!existOfferType(offer_type))
        return false;

    if(offer_name.isEmpty())
        return false;
    if(!existOffer(offer_type, offer_name))
        return false;

    if(product_code.isEmpty())
        return false;

    if(existProductAndOffer(product_code, offer_type, offer_name))
        return updateOfferToProduct(product_code, offer_type, offer_name, value, is_default);

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        disConnect();
        return false;
    }

    sql    = "INSERT INTO init_prod_offers ";
    sql +=       "(product_code, offer_type, ";
    sql +=        "prod_offer, value, is_default) ";
    sql += "VALUES ('" + product_code + "', '";
    sql +=                         offer_type + "', ";
    sql +=                 "'" + offer_name + "', ";
    sql +=                 "'" + QString::number(value) + "', ";
    sql +=                 "'" + is_default + "');";

    query = new QSqlQuery(sql, this->dbHandle());

    error = query->lastError();
    delete query;
    disConnect();

    if(error.type() == QSqlError::NoError)
        return true;
    else
        return false;
}

bool ProductOffersModuleDB::deleteOfferToProduct(
        const QString& product_code,
        const QString& offer_type,
        const QString& offer_name)
{
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(product_code.isEmpty())
        return false;

    if(offer_type.isEmpty())
        return false;

    if(offer_name.isEmpty())
        return false;


    sql   = "DELETE  from init_prod_offers ";
    sql+= "WHERE  product_code='"+product_code+"' AND offer_type='"+offer_type+"' AND prod_offer='"+offer_name+"';";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion" << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());
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

bool ProductOffersModuleDB::updateOfferToProduct(
        const QString& product_code,
        const QString& offer_type,
        const QString& offer_name,
        double value,
        const QString& is_default)
{
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(offer_type.isEmpty())
        return false;
    if(!existOfferType(offer_type))
        return false;

    if(offer_name.isEmpty())
        return false;
    if(!existOffer(offer_type, offer_name))
        return false;

    if(product_code.isEmpty())
        return false;

    if(!existProductAndOffer(product_code, offer_type, offer_name))
        return insertOfferToProduct(product_code, offer_type, offer_name, value, is_default);

    sql    = "UPDATE init_prod_offers ";
    sql += "SET value = '" + QString::number(value) + "', ";
    sql +=        "is_default = '" + is_default + "' ";
    sql += "WHERE product_code = '" + product_code + "' AND ";
    sql +=              "offer_type = '" + offer_type + "' AND ";
    sql +=              "prod_offer = '" + offer_name + "';";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion" << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());

    error = query->lastError();
    delete query;
    disConnect();

    if(error.type() == QSqlError::NoError)
        return true;
    else
        return false;
}


bool ProductOffersModuleDB::setAllProductOfferValue(
        const QString& offer_type,
        const QString& offer_name,
        const QString& value)
{
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(offer_type.isEmpty())
        return false;

    if(!existOfferType(offer_type))
        return false;

    if(offer_name.isEmpty())
        return false;

    if(!existOffer(offer_type, offer_name))
        return false;

    sql  = "SELECT set_init_prod_offers ('"+offer_type+"', '"+offer_name+"', '"+value+"');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion" << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());
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

bool ProductOffersModuleDB::updateAllProductOfferValue(
        const QString& offer_type,
        const QString& offer_name,
        const QString& value)
{
    QSqlError error;
    QSqlQuery *query = 0;
    QString sql;

    if(offer_type.isEmpty())
        return false;

    if(!existOfferType(offer_type))
        return false;

    if(offer_name.isEmpty())
        return false;

    if(!existOffer(offer_type, offer_name))
        return false;

    sql  = "SELECT update_init_prod_offers ('"+offer_type+"', '"+offer_name+"', '"+value+"');";

    connect();
    if(!isConnected()){
        cerr << "Fallo en la conexion de la funcion" << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        disConnect();
        return false;
    }

    query = new QSqlQuery(sql, this->dbHandle());
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
