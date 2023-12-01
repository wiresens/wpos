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

BarCoreDB::BarCoreDB(
    const QString& connection_name,
    const QString& configuration_path):
    BasicDatabase(connection_name,configuration_path)
{}

QString BarCoreDB::getName(const QString& product_code)
{

    if (!isConnected()) return QString();

    QString sql {"SELECT product FROM products WHERE product_code='"+product_code+"';"};
    QSqlQuery query(sql, getDB());

    //prepare the query execution
    if (!query.isActive() || !query.size()){
        qDebug() << "Fault due to inactive query " << product_code.toLatin1() ;
        return QString();
    }

    query.first();
    return  query.value(0).toString();
}


QString BarCoreDB::getName(const QStringList article_list){
    int count,i;
    QString aux;
    count =article_list.count();
    aux = "";
    for(i=0;i<count;i++){
        aux = article_list[i];
        if (i != (count-1) )
            aux+=" ";
    }
    return getName(aux);
}


QString BarCoreDB::getPrice(const QString& product_code){
    double price;
    QSqlQuery *q = 0;
    QString query="";
    QString ret="";

    if (!this->isConnected())
        return ret="0.0";

    query = "SELECT price FROM products WHERE product_code='"+product_code+"';";

    q= new QSqlQuery(query,this->getDB());
    //prepare the query execution
    if (!q->isActive()){
        qDebug() << "fallo por query no activa ";
        delete q;
        return ret="0.0";
    }

    if (!q->size()){
        price = 0.0;
        delete q;
        return ret="0.0";
    }
    else{
        q->first();
        ret = (q->value(0)).toString();
        price = (q->value(0)).toDouble();

    }

    ret = QString::number(price);
    return ret;
}

QString BarCoreDB::getTax(const QString& product_code){
    double tax;
    QSqlQuery *q = 0;
    QString query="";
    QString ret="";

    if (!this->isConnected())
        return ret="0.0";

    query = "SELECT t.rate FROM products P, taxes T ";
    query +="WHERE (P.product_code='"+product_code+"') AND (T.tax=P.tax);";

    q= new QSqlQuery(query,this->getDB());
    //prepare the query execution
    if (!q->isActive()){
        qDebug() << "fallo por query no activa ";
        delete q;
        return ret="0.0";
    }

    if (!q->size()){
        tax = 0.0;
        delete q;
        return ret="0.0";
    }
    else{
        q->first();
        ret = (q->value(0)).toString();
        tax = (q->value(0)).toDouble();

    }

    ret = QString::number(tax);
    return ret;
}

QString  BarCoreDB::getTaxName(const QString& product_code){
    QSqlQuery *q=0;
    QString query;
    QString ret = "";



    if (!this->isConnected())
        return ret;

    query = "SELECT tax FROM products WHERE product_code='"+product_code+"';";
    q = new QSqlQuery(query,this->getDB());

    //prepare the query execution
    if (!q->isActive()){
        delete q;
        return QString("");
    }

    if (!q->size()){
        return ret;
        delete q;
    }
    else{
        q->first();
        ret = (q->value(0)).toString();
    }

    delete q;
    return ret;
}

QString BarCoreDB::getOptionModifier(const QString& product_code,
                                     const QString& option_type,const QString& option_value){
    QSqlQuery *q=0;
    QString query;
    QString ret = "";
    double d;
    bool ok;

    if (!this->isConnected())
        return ret;

    query =   "SELECT value FROM init_prod_options WHERE ";
    query += "(product_code='"+product_code+"') AND ";
    query += "(option_type='"+option_type+"') AND ";
    query += "(prod_option='"+option_value+"') ;";
    q = new QSqlQuery(query,this->getDB());

    //prepare the query execution
    if (!q->isActive()){
        delete q;
        return QString("");
    }

    if (!q->size()){
        return ret;
        delete q;
    }
    else{
        q->first();
        ret = (q->value(0)).toString();
    }

    d = ret.toDouble(&ok);
    if (!ok)
        ret = "";

    delete q;
    return ret;
}

bool BarCoreDB::checkOption(const QString& product_code,const QString& option_type,const QString& option_value){     
    QSqlQuery *q=0;
    QString query;
    bool ret = false;



    if (!this->isConnected())
        return ret;

    query =   "SELECT count(*) FROM init_prod_options ";
    query += "WHERE product_code='"+product_code+"' AND ";
    query += "option_type='"+option_type+"' AND prod_option='"+option_value+"';";
    q = new QSqlQuery(query,this->getDB());

    //prepare the query execution
    if (!q->isActive()){
        delete q;
        return ret;
    }

    if (!q->size()){
        return ret;
        delete q;
    }
    else{
        q->first();
        ret = (q->value(0)).toInt();
    }

    delete q;
    return ret;
}

OfferData* BarCoreDB::getOffer(const QString& product_code,const QString& offer_type,const QString& offer_name){
    OfferData* ret = 0;
    QSqlQuery *q=0;
    QString query;

    if (!this->isConnected())
        return ret;

    query =   "SELECT o.cpp_operator, i.value ";
    query += "FROM offer_types o, init_prod_offers i ";
    query += "WHERE (o.offer_type=i.offer_type) AND ";
    query += "(i.product_code='"+product_code+"') ";
    query += "AND (i.prod_offer='"+offer_name+"') ;";
    q = new QSqlQuery(query,this->getDB());

    //prepare the query execution
    if (!q->isActive()){
        delete q;
        return ret;
    }

    if (!q->size()){
        return ret;
        delete q;
    }
    else{
        q->first();
        ret = new OfferData;
        ret->offer_type = offer_type;
        ret->offer_name = offer_name;
        ret->offer_mode = (q->value(0)).toString();
        ret->offer_discount = (q->value(1)).toString();
    }

    delete q;
    return ret;


}

bool BarCoreDB::getProductAtPrinter(const QString& product_code, const QString& printer){
    QSqlQuery *q=0;
    QString query;
    bool ret = false;

    if (!this->isConnected())
        return ret;

    query =  "SELECT product_code FROM product_printer_types ";
    query += " WHERE (product_code='"+product_code+"') AND ";
    query += "(printer_type='"+printer+"') ;";
    q = new QSqlQuery(query,this->getDB());
    if (!q->isActive()){
        delete q;
        return false;
    }

    if (q->size()){
        ret = true;
    }

    delete q;
    return ret;
}
