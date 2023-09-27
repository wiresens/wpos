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


AuthCoreDB::AuthCoreDB(const QString& _connection_name,const QString& _hostname,
                       const QString& _database, const QString& _username,const QString& _passwd):
    BasicDatabase(_connection_name,_hostname,_database,_username,_passwd){
}

AuthCoreDB::AuthCoreDB(const QString& _connection_name,XmlConfig *xml):
    BasicDatabase(_connection_name,xml){
}

AuthCoreDB::AuthCoreDB(const QString& _connection_name,const QString& configuration_path):
    BasicDatabase(_connection_name,configuration_path){
}


AuthCoreDB::~AuthCoreDB(){
}


UserData* AuthCoreDB::getUserName(const QString& _name){
    QSqlQuery *q=0;
    QString query;
    UserData *ret = 0;

    if (!isConnected())
        return ret;

    query = "SELECT employee_id, name, last_name, address, nss, phone, ";
    query += "cellular, email, company_id, picture ";
    query += "FROM staff ";
    query += "WHERE name='"+_name+"' AND active='t';";
    q = new QSqlQuery(query, this->getDB());

    //prepare the query execution
    if (!q->isActive()){
        delete q;
        return ret;
    }
    if (!q->size()){
        delete q;
        return ret;
    }
    else{
        //only get the first record with this name
        q->first();
        ret = new UserData;
        ret->id = q->value(0).toString();
        ret->name = q->value(1).toString();
        ret->last_name = q->value(2).toString();
        ret->address = q->value(3).toString();
        ret->nss = q->value(4).toString();
        ret->phone = q->value(5).toString();
        ret->cellular = q->value(6).toString();
        ret->email = q->value(7).toString();
        ret->company_id = q->value(8).toString();
        ret->picture = q->value(9).toString();
        ret->administration = this->getAdministracion(ret->id);
    }

    delete q;
    return ret;
}



UserData* AuthCoreDB::getUserId(const QString& _id){
    QSqlQuery *q=0;
    QString query;
    UserData *ret = 0;

    if (!isConnected())
        return ret;

    query = "SELECT employee_id, name, last_name, address, nss, phone, ";
    query += "cellular, email, company_id, picture ";
    query += "FROM staff ";
    query += "WHERE employee_id='"+_id+"' AND active='t';";
    q = new QSqlQuery(query,this->getDB());

    //prepare the query execution
    if (!q->isActive()){
        delete q;
        return ret;
    }
    if (!q->size()){
        delete q;
        return ret;
    }
    else{
        //only get the first record with this name
        q->first();
        ret = new UserData;
        ret->id = q->value(0).toString();
        ret->name = q->value(1).toString();
        ret->last_name = q->value(2).toString();
        ret->address = q->value(3).toString();
        ret->nss = q->value(4).toString();
        ret->phone = q->value(5).toString();
        ret->cellular = q->value(6).toString();
        ret->email = q->value(7).toString();
        ret->company_id = q->value(8).toString();
        ret->picture = q->value(9).toString();
        ret->administration = this->getAdministracion(ret->id);
    }
    delete q;
    return ret;
}



UserData* AuthCoreDB::getUserLastName(const QString& _last_name){
    QSqlQuery *q=0;
    QString query;
    UserData *ret = 0;

    if (!isConnected())
        return ret;

    query = "SELECT employee_id, name, last_name, address, nss, phone, ";
    query += "cellular, email, company_id, picture ";
    query += "FROM staff ";
    query += "WHERE last_name='"+_last_name+"' AND active='t';";
    q = new QSqlQuery(query,this->getDB());

    //prepare the query execution
    if (!q->isActive()){
        delete q;
        return ret;
    }
    if (!q->size()){
        delete q;
        return ret;
    }
    else{
        //only get the first record with this name
        q->first();
        ret = new UserData;
        ret->id = q->value(0).toString();
        ret->name = q->value(1).toString();
        ret->last_name = q->value(2).toString();
        ret->address = q->value(3).toString();
        ret->nss = q->value(4).toString();
        ret->phone = q->value(5).toString();
        ret->cellular = q->value(6).toString();
        ret->email = q->value(7).toString();
        ret->company_id = q->value(8).toString();
        ret->picture = q->value(9).toString();
        ret->administration = this->getAdministracion(ret->id);
    }

    delete q;
    return ret;
}

bool AuthCoreDB::getAdministracion(const QString& _id){
    QString query;
    QSqlQuery *q = 0;
    QString aux;


    query =  "SELECT permission ";
    query += "FROM acl ";
    query += "WHERE employee_id='"+_id+"' ;";

    if(!isConnected()){
        cout << "problems at" << __PRETTY_FUNCTION__ <<":"<< __LINE__ << endl;
        return false;
    }

    q = new QSqlQuery(query, this->getDB());
    if (!q->isActive()){
        delete q;
        return false;
    }

    if (!q->size()){
        delete q;
        return false;
    }

    while(q->next()){
        aux = q->value(0).toString();
        if (aux == "administracion"){
            delete q;
            return true;
        }
    }
    delete q;
    return false;
}

HList<UserData>* AuthCoreDB::getUsers(){
    QString sql;

    UserData *user = 0;
    HList<UserData> *list = 0;

    sql =  "SELECT employee_id, name, last_name, address, nss, phone, ";
    sql += "cellular, email, company_id, picture ";
    sql += "FROM staff WHERE active='t' ";
    sql += "ORDER BY employee_id;";

    if(!isConnected()){
        cout << "problems at" << __PRETTY_FUNCTION__ <<":"<< __LINE__ << endl;
        return list;
    }

    QSqlQuery query (sql, this->getDB());
    if (!query.isActive()){
        return list;
    }

    if (!query.size()){
        return list;
    }

    list = new HList<UserData>;
    while( query.next() ){
        user = new UserData();
        user->id = query.value(0).toString();
        user->name = query.value(1).toString();
        user->last_name = query.value(2).toString();
        user->address = query.value(3).toString();
        user->nss = query.value(4).toString();
        user->phone = query.value(5).toString();
        user->cellular = query.value(6).toString();
        user->email = query.value(7).toString();
        user->company_id = query.value(8).toString();
        user->picture = query.value(9).toString();
        user->administration = this->getAdministracion(user->id);
        list->append(user,user->id);
    }
    return list;
}
