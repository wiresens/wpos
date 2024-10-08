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
#include <libbslxml/xmlconfig.h>

#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

FileManagerDB::FileManagerDB(
    const QString& connection,
    const QString& hostname,
    const QString& database,
    const QString& username,
    const QString& passwd)
    :    BasicDatabase(
          connection,
          hostname,
          database,
          username,
          passwd)
{}

FileManagerDB::FileManagerDB(
    const QString& connection,
    XmlConfig *xml
):  BasicDatabase(connection,xml){}

FileManagerDB::FileManagerDB(
    const QString& connection,
    const QString& configFile)
    : BasicDatabase(
          connection,
          configFile)
{}

bool FileManagerDB::exists(
    const QString& file)
{
    if (!isConnected()) return false;

    QString sql {"SELECT file_name FROM xml_files WHERE file_name='"+file+"' ; "};
    QSqlQuery query {QSqlQuery(sql, dbHandle())};

    //prepare the query execution
    return  (query.isActive() && query.size());
}

ResultSet FileManagerDB::loadXmlFiles(
    const QStringList &files)
{
    if (!isConnected()) ResultSet{}; // We shall throw an exception here

    QString sql {"SELECT  file_path, file_name, file_contents FROM xml_files ORDER BY file_name ASC ;"};
    QSqlQuery query(sql, dbHandle());
    // query.setForwardOnly(true);

    ResultSet res;    
    while (query.next()) {
        auto file_name = query.value("file_name").toString();
        if( ! files.contains( file_name ) ) continue;

        auto file_path = query.value("file_path").toString();
        auto file_full_name = query.value("file_path").toString().append(file_name);
        auto xml_contents = query.value("file_contents").toString();
        xml_contents.replace("*","\'");

        FileRecord fr;
        fr.m_file_path      = file_path;
        fr.m_file_name      = file_name;
        fr.m_file_contents  = xml_contents;
        res.push_back(fr);

        qDebug() << "Fetched file " << file_name << " Located at "
                 << QFile(file_full_name).fileName();
    }
    return res;
}

bool FileManagerDB::saveXmlFile(
    const QString& file_name)
{
    QString file_path{ cfg::xmlFileByName(file_name) };
    if ( !isConnected() || !QFile::exists(file_path))
        return false;

    XmlConfig xml(file_path);

    QString xml_string = xml.toString();
    xml_string.replace("\'","*");
    xml_string.replace("\n"," ");

    QString sql {"INSERT INTO xml_files (file_path, file_name, file_contents)"};
    sql += "VALUES (";
    sql += "'"+ cfg::CFG_XML_DIR + "',";
    sql += "'" + file_name + "',";
    sql += "'" + xml_string + "');";

    QSqlQuery query {QSqlQuery( sql, dbHandle() ) };

    QSqlError error{ query.lastError() };
    if ( error.type()!= QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}

bool FileManagerDB::delXmlFile(
    const QString& file_name)
{
    if ( !isConnected() ) return false;

    QString sql {"DELETE FROM xml_files WHERE file_name='"+file_name+"';"};
    QSqlQuery query { QSqlQuery( sql, dbHandle() ) };
    QSqlError error{ query.lastError() };

    if ( error.type()!= QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}
