/***************************************************************************
                          authcoredb.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef AUTHCOREDB_H
#define AUTHCOREDB_H

#include <basicdatabase.h>
#include <wposcore/hlist.h>

struct UserData {
    QString id;
    QString name;
    QString last_name;
    QString address;
    QString nss;
    QString phone;
    QString cellular;
    QString email;
    QString company_id;
    QString picture;
    bool administration;
};

struct AuthCoreDB : public BasicDatabase  {
    AuthCoreDB(const QString& _connection_name,
               const QString& _hostname,
               const QString& _database,
               const QString& _username,
               const QString& _passwd);

    AuthCoreDB(const QString& _connection_name, XmlConfig *xml);

    AuthCoreDB(const QString& _connection_name,
               const QString& configuration_path);

    ~AuthCoreDB();

    UserData* getUserName(const QString& _name);
    UserData* getUserId(const QString& _id);
    UserData* getUserLastName(const QString& _last_name);
    bool getAdministracion(const QString& _id);
    HList<UserData>* getUsers();
};

#endif
