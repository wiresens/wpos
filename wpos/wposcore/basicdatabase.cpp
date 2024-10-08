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

const QString BasicDatabase::SQL_DRIVER {"QPSQL"};
size_t BasicDatabase::DEFAULT_SQL_PORT {5434};


void BasicDatabase::initDB()
{
    if(!QSqlDatabase::contains(m_con_name))
        m_qsl_db = QSqlDatabase::addDatabase(SQL_DRIVER, m_con_name);
    m_qsl_db.setHostName(m_host);
    m_qsl_db.setDatabaseName(m_db);
    m_qsl_db.setUserName(user);
    m_qsl_db.setPassword(m_pwd);
    m_qsl_db.setPort(m_port);
}

BasicDatabase::BasicDatabase(
    const QString& con_name,
    const QString& host,
    const QString& db,
    const QString& user,
    const QString& pwd,
    size_t port
)
    :m_con_name { con_name},
    m_host { host},
    m_db { db},
    user  {user},
    m_pwd { pwd},
    m_port  {port}
{}

//FIXME the passwd should be crypted at the xml
BasicDatabase::BasicDatabase(
    const QString& con_name,
    XmlConfig *xml
)
    :m_con_name {con_name}
{
    loadDBSettings(*xml);
}

BasicDatabase::BasicDatabase(
    const QString& con_name,
    const QString& config_path
)
    :m_con_name {con_name}
{

    if ( QFile::exists( config_path) ){
        XmlConfig xml(config_path) ;
        loadDBSettings(xml);
    }  
}

void BasicDatabase::loadDBSettings(
    XmlConfig &xml
)
{
    xml.pushDomain();
    xml.delDomain();
        m_host = xml.readString("database.hostname");
        m_db = xml.readString("database.dbname");
        user = xml.readString("database.user");
        m_pwd = xml.readString("database.passwd");

        bool portExist;
        m_port = xml.readString("database.port").toInt( &portExist );
        if ( !portExist ) m_port = DEFAULT_SQL_PORT;
    xml.delDomain();
    xml.popDomain();
}

const QSqlDatabase& BasicDatabase::dbHandle() const{
    return m_qsl_db;
}

bool BasicDatabase::connect(){
    initDB();
    return m_qsl_db.open();
}

void BasicDatabase::disConnect(){
    if (m_qsl_db.isOpen()){
        m_qsl_db.close();
    }
}

bool BasicDatabase::isConnected() const{
    return m_qsl_db.isOpen();
}

const QString& BasicDatabase::connectionName() const{
    return m_con_name;
}

bool BasicDatabase::startTransaction(){
    return m_qsl_db.transaction();
}

bool BasicDatabase::commit(){
    return m_qsl_db.commit();
}

bool BasicDatabase::rollback(){
    return m_qsl_db.rollback();
}

QString BasicDatabase::lastError() const{
    return m_qsl_db.lastError().text();
}
