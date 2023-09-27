/***************************************************************************
                          receiptdb.cpp  -  description
                             -------------------
    begin                : Thu Jan 8 2004
    copyright            : (C) 2004 by Napsis S.L.
    email                : carlos@napsis.com

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "receiptdb.h"

#include <QtSql/QSql>
#include <QtSql/QSqlError>
#include <QString>
#include <QStringList>
#include <QDebug>

#include <iostream>
namespace std{}
using namespace std;

ReceiptDB::ReceiptDB(
        const QString& _connection_name,
        const QString& _hostname,
        const QString& _database,
        const QString& _username,
        const QString& _passwd)
    :BasicDatabase(_connection_name,
                   _hostname,
                   _database,
                   _username,
                   _passwd){}

ReceiptDB::ReceiptDB(
        const QString& _connection_name,
        XmlConfig *xml)
    :BasicDatabase(_connection_name, xml){}

ReceiptDB::ReceiptDB(
        const QString& _connection_name,
        const QString& configuration_path)
    :BasicDatabase(_connection_name, configuration_path){}

ReceiptDB::~ReceiptDB() = default;

bool ReceiptDB::create(XmlConfig &xml, bool lock){

    //get all data from XML
    auto employee_id = xml.readString("employee.dni");
    auto start_time = xml.readString("timestamps.start");
    auto end_time = xml.readString("timestamps.last");
    auto description = xml.readString("description");

    //problems with the XML
    auto xml_string = xml.toString().replace('\n'," ").replace('\'','*');
    QString lock_string;
    auto order_state = QString("bloqueada");
    if (lock) lock_string = "true";
    else lock_string = "false";

    if (employee_id.isEmpty()
            || start_time.isEmpty()
            || end_time.isEmpty()
            || xml_string.isEmpty())
        return false;

    if (!connect()){
        qDebug() << "Error: Connection Error" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ ;
        return false;
    }

    QString sql{ "INSERT INTO orders (employee_id, start_time, last_time, description, order_state, blocked, xml) "};
    sql += "VALUES (";
    sql += "'"+employee_id+"',";
    sql += "'"+start_time+"',";
    sql += "'"+end_time+"',";
    sql += "'"+description+"',";
    sql += "'"+order_state+"',";
    sql += "'"+lock_string+"',";
    sql += "'"+xml_string+"'";
    sql+= ");";

    QSqlQuery query (sql, getDB());

    bool ret = false;
    switch ( query.lastError().type() ){
    case QSqlError::NoError:
        ret = true;
        break;
    default:
        qDebug() << query.lastError().databaseText();
        ret = false;
        break;
    }
    disConnect();
    return ret;
}

bool ReceiptDB::existReceiptByDate(QString employee_id, QString start_date){

    qDebug() << "Begin ReceiptDB::existReceiptByDate( " << employee_id <<", " << start_date << ")";
    start_date.replace("T", " ");

    if (!connect()){
        qDebug() << "Database Connection Failiure:"
                 <<  __PRETTY_FUNCTION__ << ";"
                  << __LINE__ ;
        return false;
    }

    QString sql = "SELECT xml FROM orders ";
    sql += "WHERE employee_id ='"+employee_id+"' AND start_time = '"+start_date+"' ;";

    QSqlQuery query(sql, getDB());
    if (!query.isActive()){
        disConnect();
        return false;
    }

    if ( query.lastError().type() != QSqlError::NoError)
        qDebug() << query.lastError().databaseText();

    auto exists = ( query.size() >= 1 );
    disConnect();

    qDebug() << "End ReceiptDB::existReceiptByDate";
    return exists;
}

bool ReceiptDB::lockReceiptByDate(QString employee_id, QString start_date){

    bool ret = false;
    if (!connect()){
        qDebug() << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ ;
        return ret;
    }

    QString sql  = "UPDATE orders SET blocked = 'true' ";
    sql += "WHERE employee_id = '"+employee_id+"' ";
    sql += "AND start_time = '"+start_date+"' ";
    //        query += "AND blocked = 'false' ";
    sql += ";";

    QSqlQuery query (sql, getDB());
    switch ( query.lastError().type() ){
    case QSqlError::NoError :
        ret = true;
        break;
    default:
        qDebug() << query.lastError().databaseText();
        ret = false;
        break;
    }
    disConnect();
    return ret;
}

bool ReceiptDB::unlockReceiptByDate(QString employee_id, QString start_date){

    bool ret = false;
    if (!connect()){
        qDebug() << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ ;
        return ret;
    }

    QString sql   = "UPDATE orders SET blocked = 'false' ";
    sql += "WHERE employee_id = '"+employee_id+"' ";
    sql += "AND start_time = '"+start_date+"' ";
    //        query += "AND blocked = 'true' ";
    sql += ";";

    QSqlQuery query (sql, getDB() );

    switch ( query.lastError().type() ){
    case QSqlError::NoError :
        ret = true;
        break;

    default:
        qDebug() << query.lastError().databaseText();
        ret = false;
        break;
    }
    disConnect();
    return ret;
}

bool ReceiptDB::getReceiptStateByDate(QString employee_id, QString start_date){

    QString aux;
    bool ret = false;

    if (!connect()){
        qDebug() << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ ;
        return ret;
    }

    QString sql  =  "SELECT blocked ";
    sql += "FROM orders ";
    sql += "WHERE employee_id ='"+employee_id+"' AND start_time = '"+start_date+"' ;";

    QSqlQuery query(sql,getDB());
    if (!query.isActive()){
        disConnect();
        return ret;
    }
    if ( query.lastError().type() != QSqlError::NoError)
        qDebug() << query.lastError().databaseText();

    if (!query.size()){
        disConnect();
        return ret;
    }

    query.first();
    aux = query.value(0).toString();

    if (aux == "true") ret = true;
    disConnect();
    return ret;
}

bool ReceiptDB::deleteReceiptByDate(QString employee_id, QString start_date){

    bool ret = false;

    if (!connect())  return ret;

    QString sql   = "DELETE FROM orders ";
    sql += "WHERE employee_id = '"+employee_id+"' ";
    sql += "AND start_time = '"+start_date+"' ";
    sql += ";";

    QSqlQuery query (sql, getDB());

    switch ( query.lastError().type() ){
    case QSqlError::NoError:
        ret = true;
        break;

    default:
        qDebug() << query.lastError().databaseText();
        ret = false;
        break;
    }
    disConnect();
    return ret;
}

bool ReceiptDB::saveReceipt(XmlConfig& xml){

    //get all data from XML
    auto employee_id = xml.readString("employee.dni");
    auto start_time = xml.readString("timestamps.start");
    auto end_time = xml.readString("timestamps.last");
    auto description = xml.readString("description");
    auto xml_string = xml.toString();
    xml_string.replace('\n'," ").replace('\'','*');

    if (employee_id.isEmpty()
            || start_time.isEmpty()
            || end_time.isEmpty()
            || xml_string.isEmpty() )
        return false;

    if (!connect()){
        qDebug() << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ ;
        return false;
    }

    QString sql =   "UPDATE orders SET xml = '"+xml_string+"', ";
    sql += "last_time = '"+end_time+"',";
    sql += "description = '"+description+"' ";
    sql += "WHERE employee_id = '"+employee_id+"' ";
    sql += "AND start_time = '"+start_time+"' ";
    sql += ";";

    QSqlQuery query (sql, getDB());
    bool ret = false;
    switch ( query.lastError().type() ){

    case QSqlError::NoError :
        ret = true;
        break;

    default:
        qDebug() << query.lastError().databaseText();
        break;
    }

    disConnect();
    return ret;
}

QString ReceiptDB::getReceiptByDate(QString employee_id, QString start_date){

    if (!connect()){
        qDebug() << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ ;
        return QString();
    }

    QString sql  =  "SELECT xml FROM orders ";
    sql += "WHERE employee_id ='"+employee_id+"' AND start_time = '"+start_date+"' ;";

    QSqlQuery query (sql, getDB());
    if (!query.isActive()){
        disConnect();
        return QString();
    }

    if ( query.lastError().type() != QSqlError::NoError){
        qDebug() << query.lastError().databaseText();
        disConnect();
        return QString();
    }

    if (!query.size()){
        disConnect();
        return QString();
    }

    query.first();
    auto result = query.value(0).toString().replace('*', "\'");
    disConnect();
    return result;
}

QString ReceiptDB::getReceiptResume(){

    if (!connect()){
        qDebug() << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ ;
        return QString();
    }

    QString sql = "SELECT employee_id, start_time, blocked, blocker, name, description ";
    sql += "FROM orders JOIN staff USING (employee_id);";

    QSqlQuery query(sql , getDB());
    if (!query.isActive()){
        disConnect();
        return QString();
    }

    if ( query.lastError().type() != QSqlError::NoError){
        qDebug() << query.lastError().databaseText();
        return QString();
    }

    if (!query.size()){
        disConnect();
        return QString();
    }

    XmlConfig xml;
    xml.createElementSetDomain("receipts");

    while(query.next()){
        xml.createElementSetDomain("receipt");
        xml.createElement("employee_id",(query.value(0)).toString());
        auto tmp = (query.value(1)).toString().replace("T"," ");
        xml.createElement("start_time", tmp);

        if ((query.value(2)).toBool())
            xml.createElement("blocked","true");
        else
            xml.createElement("blocked","false");

        xml.createElement("blocker",(query.value(3)).toString());
        xml.createElement("employee_name",(query.value(4)).toString());
        xml.createElement("description",(query.value(5)).toString());
        xml.releaseDomain("receipt");
    }

    disConnect();
    xml.delDomain();
    return xml.toString();
}

QString ReceiptDB::getReceiptResume(QString employee_id){

    if (!connect()){
        qDebug() << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ ;
        return QString();
    }

    QString sql =  "SELECT employee_id, start_time, blocked, blocker, name, description ";
    sql += "FROM orders JOIN staff USING (employee_id) WHERE employee_id = '"+employee_id+"';";

    QSqlQuery query( sql, getDB());
    if (!query.isActive()){
        disConnect();
        return QString();
    }

    if ( query.lastError().type() != QSqlError::NoError){
        qDebug() << query.lastError().databaseText();
        disConnect();
        return QString();
    }

    if (!query.size()){
        disConnect();
        return QString();
    }

    XmlConfig xml;
    xml.createElementSetDomain("receipts");
    while(query.next()){
        xml.createElementSetDomain("receipt");
        xml.createElement("employee_id",(query.value(0)).toString());
        auto tmp = (query.value(1)).toString().replace("T"," ");

        xml.createElement("start_time", tmp);
        if ((query.value(2)).toBool())
            xml.createElement("blocked", "true");
        else
            xml.createElement("blocked","false");

        xml.createElement("blocker",(query.value(3)).toString());
        xml.createElement("employee_name",(query.value(4)).toString());
        xml.createElement("description",(query.value(5)).toString());
        xml.releaseDomain("receipt");
    }

    disConnect();
    xml.delDomain();
    return xml.toString();
}
