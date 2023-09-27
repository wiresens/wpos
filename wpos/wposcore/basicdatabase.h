/***************************************************************************
                          basicdatabase.h  -  description
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

#ifndef BASICDATABASE_H
#define BASICDATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QString>

/**
  *@author Carlos Manzanedo
  */

class XmlConfig;

//#define SQL_DRIVER "QPSQL"
//#define SQL_PORT 5432


class BasicDatabase {
public:

    static QString SQL_DRIVER;
    static int SQL_PORT;

    /**
    *  constructor, the connection name should be different for each connection, the rest of parametters are:
    *  @param  QString the name of the connection. Each connection must have different name.
    *  @param  QString with the hostname of the host that have the database.
    *  @param  QString with the name of the database.
    *  @param  QString with the name of the user to connect to the database.
    *  @param  QString with the password to make operations at the database.
    *  @param  int with the port of the database server.
    */

    BasicDatabase() = default ;

    BasicDatabase(const QString& _connection_name,
                  const QString& _hostname,
                  const QString& _database,
                  const QString& _username,
                  const QString& _passwd,
                  int port = SQL_PORT);
    /**
    *  constructor, the connection name should be different for each connection.
    *  The XmlConfig should describethe connection.
    *  @param  QString the name of the connection. Each connection must have different name.
    *  @param  XmlConfig with the description of the connection. The XML Should validate against the following DTD
    *  <!ELEMENT AUTOMATICALLY_GENERATED_XML (databse)>
    *  <!ELEMENT database (hostname,dbname,user,passwd,port?)>
    *  <!ELEMENT hostname (#PCDATA)>
    *  <!ELEMENT dbname (#PCDATA)>
    *  <!ELEMENT user (#PCDATA)>
    *  <!ELEMENT passwd (#PCDATA)>
    *  <!ELEMENT port (#PCDATA)>
    */
    BasicDatabase(const QString& _connection_name, XmlConfig *xml);

    /**
    *  constructor, the connection name should be different for each connection.
    *  The XmlConfig should describe the connection.
    *  @param  QString the name of the connection. Each connection must have different name.
    *  @param  A path to an Xml with the description of the connection.
    *  The Xml Should validate against the basicDatabase Connection DTD
    **/
    BasicDatabase(const QString& _connection_name,
                  const QString& configuration_path);

    //use this to create queries outside this class
    QSqlDatabase& getDB();

    bool connect();
    bool disConnect();
    bool isConnected() const;

    //transaction methods
    bool startTransaction();
    bool commit();
    bool rollback();

    const QString& connectionName() const;

protected:
    void loadDBSettings(XmlConfig* xml);
    void initDB();
    void delConnection();

    QString connection_name;
    QString hostname;
    QString dbname;
    QString user;
    QString passwd;
    int port{SQL_PORT};

    QSqlDatabase database;
};

#endif
