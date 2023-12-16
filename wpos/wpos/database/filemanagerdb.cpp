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
    const QString& connection,
    const QString& hostname,
    const QString& database,
    const QString& username,
    const QString& passwd):
    BasicDatabase(connection,hostname,database,username,passwd){}

FileManagerDB::FileManagerDB(const QString& connection,
                             XmlConfig *xml):
    BasicDatabase(connection,xml){}

FileManagerDB::FileManagerDB(const QString& connection, const QString& configFile):
    BasicDatabase(connection,configFile){}

bool FileManagerDB::exists(const QString& file){

    if (!isConnected()) return false;

    QString sql {"SELECT file_name FROM xml_files WHERE file_name='"+file+"' ; "};
    QSqlQuery query {QSqlQuery(sql, getDB())};

    //prepare the query execution
    return  (query.isActive() && query.size());
}

bool FileManagerDB::loadXmlFile(const QString& file){
    bool ret = false;
    if (!isConnected()) return ret;

    QString sql {"SELECT file_contents FROM xml_files WHERE file_name='"+file+"' ; "};
    QSqlQuery query {QSqlQuery(sql, getDB())};

    //prepare the query execution
    if (!query.isActive() || !query.size())
        return ret;

    query.first();
    auto xml_string = query.value(0).toString();
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

    XmlConfig xml(PATH + file);

    QString xml_string = xml.toString();
    xml_string.replace("\'","*");
    xml_string.replace("\n"," ");

    QString sql {"INSERT INTO xml_files (file_path, file_name, file_contents)"};
    sql += "VALUES (";
    sql += "'"+PATH+"',";
    sql += "'"+file+"',";
    sql += "'"+xml_string+"');";

    QSqlQuery query {QSqlQuery( sql, getDB() ) };

    QSqlError error{ query.lastError() };
    if ( error.type()!= QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}

bool FileManagerDB::delXmlFile(const QString& file){

    if ( !isConnected() ) return false;

    QString sql {"DELETE FROM xml_files WHERE file_name='"+file+"';"};
    QSqlQuery query { QSqlQuery( sql, getDB() ) };
    QSqlError error{ query.lastError() };

    if ( error.type()!= QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}
