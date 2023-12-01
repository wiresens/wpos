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
    bool isRootUser{false};
};

struct AuthCoreDB :
    public BasicDatabase
{

    AuthCoreDB() = default;

    AuthCoreDB(
        const QString& connection,
        const QString& hostname,
        const QString& database,
        const QString& username,
        const QString& passwd);

    AuthCoreDB(
        const QString& connection,
        XmlConfig *xml);

    AuthCoreDB(
        const QString& connection,
        const QString& configFile);

//    UserData* userById(const QString& id)  const;
    UserData userById(const QString& id)  const;

    //Potential bug : Name and Last are not unique identifiers
    UserData userByName(const QString& name) const; // First record with  name
    UserData userByLastName(const QString& lastName)  const; // First record with  lastName

    bool isRootUser(const QString& id)  const;
    QVector<UserData> userList()  const;
};

#endif
