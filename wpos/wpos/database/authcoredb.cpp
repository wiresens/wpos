/***************************************************************************
                          authcoredb.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "authcoredb.h"
#include <QVariant>

#include <iostream>
namespace std{}
using namespace std;

AuthCoreDB::AuthCoreDB(
    const QString& connection,
    const QString& hostname,
    const QString& database,
    const QString& username,
    const QString& passwd):
    BasicDatabase(
        connection,
        hostname,
        database,
        username,
        passwd)
{}

AuthCoreDB::AuthCoreDB(
    const QString& connection,
    XmlConfig *xml):
    BasicDatabase(connection,xml)
{}

AuthCoreDB::AuthCoreDB(
    const QString& connection,
    const QString& configFile):
    BasicDatabase(connection,configFile)
{}

UserData AuthCoreDB::userById(const QString& id) const{
    UserData user;
    if (isConnected()){

        QString sql = "SELECT employee_id, name, last_name, address, nss, phone, ";
        sql += "cellular, email, company_id, picture ";
        sql += "FROM staff ";
        sql += "WHERE employee_id='"+id+"' AND active='t';";
        QSqlQuery query(sql, getDB());

        //prepare the query execution
        if ( query.isActive() && query.size()){

            //only get the first record with this name
            query.first();
            user.id = query.value(0).toString();
            user.name = query.value(1).toString();
            user.last_name = query.value(2).toString();
            user.address = query.value(3).toString();
            user.nss = query.value(4).toString();
            user.phone = query.value(5).toString();
            user.cellular = query.value(6).toString();
            user.email = query.value(7).toString();
            user.company_id = query.value(8).toString();
            user.picture = query.value(9).toString();
            user.isRootUser = isRootUser(user.id);
        }
    }
    return user;
}

UserData AuthCoreDB::userByName(const QString& name) const{
    UserData user;

    if (isConnected()){
        QString sql = "SELECT employee_id, name, last_name, address, nss, phone, ";
        sql += "cellular, email, company_id, picture ";
        sql += "FROM staff ";
        sql += "WHERE name='"+name+"' AND active='t';";

        QSqlQuery query(sql, getDB());

        //prepare the query execution
        if ( query.isActive() && query.size()){

            //only get the first record with this name
            query.first();
            user.id = query.value(0).toString();
            user.name = query.value(1).toString();
            user.last_name = query.value(2).toString();
            user.address = query.value(3).toString();
            user.nss = query.value(4).toString();
            user.phone = query.value(5).toString();
            user.cellular = query.value(6).toString();
            user.email = query.value(7).toString();
            user.company_id = query.value(8).toString();
            user.picture = query.value(9).toString();
            user.isRootUser = isRootUser(user.id);
        }
    }
    return user;
}

UserData AuthCoreDB::userByLastName(const QString& lastName) const{
    UserData user;
    if ( isConnected()){
        QString sql = "SELECT employee_id, name, last_name, address, nss, phone, ";
        sql += "cellular, email, company_id, picture ";
        sql += "FROM staff ";
        sql += "WHERE last_name='"+lastName+"' AND active='t';";
        QSqlQuery query(sql, getDB());

        //prepare the query execution
        if ( query.isActive() && query.size()){
            //only get the first record with this name
            query.first();

            user.id = query.value(0).toString();
            user.name = query.value(1).toString();
            user.last_name = query.value(2).toString();
            user.address = query.value(3).toString();
            user.nss = query.value(4).toString();
            user.phone = query.value(5).toString();
            user.cellular = query.value(6).toString();
            user.email = query.value(7).toString();
            user.company_id = query.value(8).toString();
            user.picture = query.value(9).toString();
            user.isRootUser = isRootUser(user.id);
        }
    }
    return user;
}

bool AuthCoreDB::isRootUser(const QString& id) const{

    QString sql =  "SELECT permission ";
    sql += "FROM acl ";
    sql += "WHERE employee_id='"+id+"' ;";

    if(!isConnected())  return false;
    QSqlQuery query (sql, getDB());
    if (!query.isActive() || !query.size())  return false;
    while(query.next()){
        auto res = query.value(0).toString();
        if (res == "administracion"){
            return true;
        }
    }
    return false;
}

QVector<UserData> AuthCoreDB::userList() const{
    QVector<UserData> users;
    QString sql =  "SELECT employee_id, name, last_name, address, nss, phone, ";
    sql += "cellular, email, company_id, picture ";
    sql += "FROM staff WHERE active='t' ";
    sql += "ORDER BY employee_id;";

    if( isConnected()){
        QSqlQuery query (sql, getDB());
        if ( query.isActive() && query.size())
        while( query.next() ){
            UserData user;
            user.id = query.value(0).toString();
            user.name = query.value(1).toString();
            user.last_name = query.value(2).toString();
            user.address = query.value(3).toString();
            user.nss = query.value(4).toString();
            user.phone = query.value(5).toString();
            user.cellular = query.value(6).toString();
            user.email = query.value(7).toString();
            user.company_id = query.value(8).toString();
            user.picture = query.value(9).toString();
            user.isRootUser = isRootUser(user.id);
            users.append(user);
        }
    }
    return users;
}
