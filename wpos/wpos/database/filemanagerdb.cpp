/***************************************************************************
                          filemanagerdb.cpp  -  description
                             -------------------
    begin                :jun 2 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "filemanagerdb.h"
#include <wposcore/config.h>

#include <xmlconfig.h>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>


QString PATH = Files::ConfigDir + "/";

#include <iostream>
namespace std{}
using namespace std;

FileManagerDB::FileManagerDB(
    const QString& _connection_name,
    const QString& _hostname,
    const QString& _database,
    const QString& _username,
    const QString& _passwd):
    BasicDatabase(_connection_name,_hostname,_database,_username,_passwd){}

FileManagerDB::FileManagerDB(const QString& _connection_name,
                             XmlConfig *xml):
    BasicDatabase(_connection_name,xml){}

FileManagerDB::FileManagerDB(const QString& _connection_name,const QString& configuration_path):
    BasicDatabase(_connection_name,configuration_path){}

FileManagerDB::~FileManagerDB(){}

bool FileManagerDB::exists(const QString& file){

    if (!isConnected()) return false;

    QString query {"SELECT file_name FROM xml_files WHERE file_name='"+file+"' ; "};
    QSqlQuery query_obj {QSqlQuery(query, getDB())};

    //prepare the query execution
    return  (query_obj.isActive() && query_obj.size());
}

bool FileManagerDB::loadXmlFile(const QString& file){
    bool ret = false;
    if (!isConnected()) return ret;

    QString query {"SELECT file_contents FROM xml_files WHERE file_name='"+file+"' ; "};
    QSqlQuery query_obj {QSqlQuery(query, getDB())};

    //prepare the query execution
    if (!query_obj.isActive() || !query_obj.size())
        return ret;

    query_obj.first();
    auto xml_string = query_obj.value(0).toString();
    xml_string.replace("*","\'");

    std::unique_ptr<XmlConfig> xml {new XmlConfig()};
    xml->readXmlFromString(xml_string);

    if (xml->wellFormed()){
        xml->save(PATH+file);
        cout << "Retrieved file from the database : "
             << QString(PATH+file).toStdString() << endl;
    }
    return true;
}

bool FileManagerDB::saveXmlFile(const QString& file){

    QFile qfile {QFile(PATH+file)};
    if ( !isConnected() || !qfile.exists()) return false;

    std::unique_ptr<XmlConfig> xml {new XmlConfig(PATH + file)};

    QString xml_string = xml->toString();
    xml_string.replace("\'","*");
    xml_string.replace("\n"," ");

    QString query {"INSERT INTO xml_files (file_path, file_name, file_contents)"};
    query += "VALUES (";
    query += "'"+PATH+"',";
    query += "'"+file+"',";
    query += "'"+xml_string+"');";

    QSqlQuery query_obj {QSqlQuery( query, getDB() ) };

    QSqlError error{ query_obj.lastError() };
    if ( error.type()!= QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}

bool FileManagerDB::delXmlFile(const QString& file){

    if ( !isConnected() ) return false;

    QString query {"DELETE FROM xml_files WHERE file_name='"+file+"';"};
    QSqlQuery query_obj { QSqlQuery( query, getDB() ) };
    QSqlError error{ query_obj.lastError() };

    if ( error.type()!= QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}
