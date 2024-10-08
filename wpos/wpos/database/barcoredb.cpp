/***************************************************************************
                          barcoredb.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "barcoredb.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QDebug>

BarCoreDB::BarCoreDB(
    const QString& connection_name,
    const QString& hostname,
    const QString& database,
    const QString& username,
    const QString& passwd):
    BasicDatabase(
        connection_name,
        hostname,
        database,
        username,passwd
        )
{}

BarCoreDB::BarCoreDB(const QString& connection,
                     const QString& configFile):
    BasicDatabase(connection,configFile)
{}

QString BarCoreDB::getName(const QString& product_code){
    if (!isConnected()) return QString();

    QString sql {"SELECT product FROM products WHERE product_code='"+product_code+"';"};
    QSqlQuery query(sql, dbHandle());

    //prepare the query execution
    if (!query.isActive() || !query.size()){
        qDebug() << "Fault due to inactive query " << product_code.toLatin1() ;
        return QString();
    }

    query.first();
    return  query.value(0).toString();
}

QString BarCoreDB::getName(const QStringList& article_list){
    QString product_code;
    for(const QString& name : article_list)
        product_code += name + " ";
    return getName(product_code.trimmed());
}

QString BarCoreDB::getPrice(const QString& product_code){
    if (!isConnected())  return QString("0.0");

    QString sql = "SELECT price FROM products WHERE product_code='"+product_code+"';";

    QSqlQuery query(sql, dbHandle());
    //prepare the query execution
    if (!query.isActive() || !query.size()){
        return QString("0.0");
    }

    query.first();
    return QString::number(query.value(0).toDouble());
}

QString BarCoreDB::getTax(const QString& product_code){    
    if (!isConnected()) return QString("0.0");

    QString sql = "SELECT t.rate FROM products p, taxes t ";
    sql += "WHERE (p.product_code='"+product_code+"') AND (t.tax=p.tax);";

    QSqlQuery query(sql, dbHandle());
    //prepare the query execution
    if (!query.isActive() || !query.size())
        return QString("0.0");

    query.first();
    return QString::number(query.value(0).toDouble());
}

QString  BarCoreDB::getTaxName(const QString& product_code){    
    if (!isConnected())  return QString();

    QString sql = "SELECT tax FROM products WHERE product_code='"+product_code+"';";
    QSqlQuery query(sql, dbHandle());

    //prepare the query execution
    if (!query.isActive() || !query.size())
        return QString();

    query.first();
    return query.value(0).toString();
}

QString BarCoreDB::getOptionModifier(
    const QString& product_code,
    const QString& option_type,
    const QString& option_value)
{
    if (!isConnected()) return QString("");

    QString sql =   "SELECT value FROM init_prod_options WHERE ";
    sql += "(product_code='"+product_code+"') AND ";
    sql += "(option_type='"+option_type+"') AND ";
    sql += "(prod_option='"+option_value+"') ;";

    QSqlQuery query(sql, dbHandle());

    //prepare the query execution
    if (!query.isActive()  || !query.size())  return QString("");


    query.first();
    QString result = query.value(0).toString();

    bool ok{false};
    result.toDouble(&ok);
    if (!ok) return QString("");

    return result;
}

bool BarCoreDB::checkOption(
    const QString& product_code,
    const QString& option_type,
    const QString& option_value)
{
    if (!isConnected())  return false;

    QString sql =   "SELECT count(*) FROM init_prod_options ";
    sql += "WHERE product_code='"+product_code+"' AND ";
    sql += "option_type='"+option_type+"' AND prod_option='"+option_value+"';";
    QSqlQuery query(sql, dbHandle());

    //prepare the query execution
    if (!query.isActive() || !query.size()) return false;

    query.first();
    return query.value(0).toInt();
}

//OfferData* BarCoreDB::getOffer(const QString& product_code,const QString& offer_type,const QString& offer_name){
//    if (!isConnected()) return nullptr;

//    QString sql =   "SELECT o.cpp_operator, i.value ";
//    sql += "FROM offer_types o, init_prod_offers i ";
//    sql += "WHERE (o.offer_type=i.offer_type) AND ";
//    sql += "(i.product_code='"+product_code+"') ";
//    sql += "AND (i.prod_offer='"+offer_name+"') ;";
//    QSqlQuery query(sql, getDB());

//    //prepare the query execution
//    if (!query.isActive() || !query.size())  return nullptr;

//    query.first();
//    OfferData* offer = new OfferData;
//    offer->offer_type = offer_type;
//    offer->offer_name = offer_name;
//    offer->offer_mode = query.value(0).toString();
//    offer->offer_discount = query.value(1).toString();

//    return offer;
//}

OfferData BarCoreDB::getOffer(
    const QString& product_code,
    const QString& offer_type,
    const QString& offer_name) const
{
    OfferData offer;
    if (isConnected()){
        QString sql =   "SELECT o.cpp_operator, i.value ";
        sql += "FROM offer_types o, init_prod_offers i ";
        sql += "WHERE (o.offer_type=i.offer_type) AND ";
        sql += "(i.product_code='"+product_code+"') ";
        sql += "AND (i.prod_offer='"+offer_name+"') ;";
        QSqlQuery query(sql, dbHandle());

        //prepare the query execution
        if ( query.isActive() && query.size()){
            query.first();
            offer.offer_type = offer_type;
            offer.offer_name = offer_name;
            offer.offer_mode = query.value(0).toString();
            offer.offer_discount = query.value(1).toString();
        }
    }
    return offer;
}

bool BarCoreDB::getProductAtPrinter(
    const QString& product_code,
    const QString& printer)
{
    if (!isConnected())  return false;
    QString sql =  "SELECT product_code FROM product_printer_types ";
    sql += " WHERE (product_code='"+product_code+"') AND ";
    sql += "(printer_type='"+printer+"') ;";

    QSqlQuery query (sql, dbHandle());
    return  ( query.isActive() && query.size() );
}
