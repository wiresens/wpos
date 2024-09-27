/***************************************************************************
                          basicdatabase.cpp  -  description
                             -------------------
    begin                : jue jul 10 2003
    copyright            : (C) 2003 by Napsis S.L.
    email                : support@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "basicdatabase.h"
#include <xmlconfig.h>
#include <QSqlError>
#include <QFile>

QString BasicDatabase::SQL_DRIVER {"QPSQL"};
unsigned int BasicDatabase::DEFAULT_SQL_PORT {5432};

BasicDatabase::BasicDatabase(
    const QString& _connection_name,
    const QString& _hostname,
    const QString& _database,
    const QString& _username,
    const QString& _passwd,
    int _port)
    :connection_name { _connection_name},
    hostname { _hostname},
    dbname { _database},
    user  {_username},
    passwd { _passwd},
    port  {_port}
{}

//FIXME the passwd should be crypted at the xml
BasicDatabase::BasicDatabase(
    const QString& _connection_name,
    XmlConfig *xml):
    connection_name {_connection_name}
{
    loadDBSettings(*xml);
}

BasicDatabase::BasicDatabase(
    const QString& _connection_name,
    const QString& configuration_path):
    connection_name {_connection_name}
{
    if ( QFile::exists(configuration_path) ){
        XmlConfig xml(configuration_path) ;
        loadDBSettings(xml);
    }  
}

void BasicDatabase::loadDBSettings(XmlConfig &xml){

    xml.pushDomain();
    xml.delDomain();
    hostname = xml.readString("database.hostname");
    dbname = xml.readString("database.dbname");
    user = xml.readString("database.user");
    passwd = xml.readString("database.passwd");

    bool portExist;
    port = xml.readString("database.port").toInt( &portExist );
    if ( !portExist ) port = DEFAULT_SQL_PORT;
    xml.delDomain();
    xml.popDomain();
}

void BasicDatabase::initDB(){
    qsl_database = QSqlDatabase::addDatabase(SQL_DRIVER, connection_name);
    qsl_database.setHostName(hostname);
    qsl_database.setDatabaseName(dbname);
    qsl_database.setUserName(user);
    qsl_database.setPassword(passwd);
    qsl_database.setPort(port);
}

const QSqlDatabase& BasicDatabase::getDB() const{
    return qsl_database;
}

bool BasicDatabase::connect(){
    initDB();
    return qsl_database.open();
}

void BasicDatabase::disConnect(){
    if (qsl_database.isOpen()){
        qsl_database.close();
        //        QSqlDatabase::removeDatabase(connection_name);
    }
}

bool BasicDatabase::isConnected() const{
    return qsl_database.isOpen();
}

void BasicDatabase::delConnection(){

    if (qsl_database.isOpen()){
        qsl_database.close();
        //        QSqlDatabase::removeDatabase(connection_name);
    }
}

const QString& BasicDatabase::connectionName() const{
    return connection_name;
}

bool BasicDatabase::startTransaction(){
    return qsl_database.transaction();
}

bool BasicDatabase::commit(){
    return qsl_database.commit();
}

bool BasicDatabase::rollback(){
    return qsl_database.rollback();
}

QString BasicDatabase::lastError() const{
    return qsl_database.lastError().text();
}
