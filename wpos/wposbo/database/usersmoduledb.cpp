/***************************************************************************
                          usersmoduledb.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#include "usersmoduledb.h"

#include <QString>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>

#include <iostream>
using namespace std;

bool UserData::isEmpty() const{
    return employee_id.isEmpty()
            && name.isEmpty()
            && last_name.isEmpty()
            && address.isEmpty()
            && nss.isEmpty()
            && phone.isEmpty()
            && cellular.isEmpty()
            && email.isEmpty()
            && company_id.isEmpty()
            && picture.isEmpty()
            && !gerente
            && !active ;
}

void UserData::clear(){
    using std::swap;
    UserData tmp;
    swap(*this, tmp);
}

UsersModuleDB::UsersModuleDB(const QString& _connection_name,const QString& _hostname,
                             const QString& _database, const QString& _username,const QString& _passwd):
    BasicDatabase(_connection_name,_hostname,_database,_username,_passwd){
}

UsersModuleDB::UsersModuleDB(const QString& _connection_name,XmlConfig *xml):
    BasicDatabase(_connection_name,xml){
}

UsersModuleDB::UsersModuleDB(const QString& _connection_name,const QString& configuration_path):
    BasicDatabase(_connection_name,configuration_path){
}

UsersModuleDB::~UsersModuleDB(){
}

bool UsersModuleDB::addUser(const UserData *user_data){
    QString query;
    QSqlError error;
    QSqlQuery *q = 0;


    query   =  "INSERT INTO staff ";
    query +=  "(employee_id, name, last_name, address, nss, phone, cellular, email, company_id, picture, active ) ";
    query +=  "VALUES ('" + user_data->employee_id+ "', ";
    query +=  "'" + user_data->name + "', '" + user_data->last_name + "', ";
    query +=  "'" + user_data->address + "', '" + user_data->nss + "', ";
    query +=  "'" + user_data->phone + "', '" + user_data->cellular + "', ";
    query +=  "'" + user_data->email + "', '" + user_data->company_id + "', ";
    query +=  "'" + user_data->picture +"', ";
    query +=  "'t') ;";

    //        cerr << query << endl;

    connect();
    if(!connect()){
        cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << endl;
        return false;
    }
    q = new QSqlQuery(query, this->getDB());
    error = q->lastError();
    if (error.type()!=QSqlError::NoError){
        switch (error.type()){
        case  QSqlError::NoError:
            break;
        case  QSqlError::ConnectionError:
            break;
        case  QSqlError::StatementError:
            cout << "QUERY ERROR: "<< query.toStdString() << endl;
            break;
        case  QSqlError::TransactionError:
            break;
        case  QSqlError::UnknownError:
            break;
        }
        //some errors ocurred
        cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
    }

    delete q;
    setUserAdminPermission(user_data->employee_id,user_data->gerente);
    disConnect();
    return ((bool) !error.type());
}

bool UsersModuleDB::updateUser(const UserData *user_data){
    QString query;
    QSqlError error;
    QSqlQuery *q = 0;

    query =   "UPDATE staff SET name = '" + user_data->name+ "', ";
    query += "last_name = '" + user_data->last_name + "', ";
    query += "address = '" + user_data->address + "', ";
    query += "nss = '" + user_data->nss + "', ";
    query += "phone = '" + user_data->phone + "', ";
    query += "cellular = '" + user_data->cellular + "', ";
    query += "email = '" + user_data->email + "', ";
    query += "picture = '" + user_data->picture + "', ";
    query += "company_id = '" + user_data->company_id + "' ";
    query += "WHERE employee_id ='" + user_data->employee_id +"';";

    connect();
    if(!connect()){
        cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << endl;
        return false;
    }
    q = new QSqlQuery(query, this->getDB());
    error = q->lastError();
    if (error.type()!=QSqlError::NoError){
        switch (error.type()){
        case  QSqlError::NoError:
            break;
        case  QSqlError::ConnectionError:
            break;
        case  QSqlError::StatementError:
            cout << "QUERY ERROR: "<< query.toStdString() << endl;
            break;
        case  QSqlError::TransactionError:
            break;
        case  QSqlError::UnknownError:
            break;
        }
        cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
        delete q;
        disConnect();
        return false;
    }
    delete q;
    setUserAdminPermission(user_data->employee_id,user_data->gerente);
    disConnect();
    return true;
}

bool UsersModuleDB::delUser(const QString& id){
    return setActiveFlag(id,false);
}

bool UsersModuleDB::realDelUser(const QString& id){
    QString query;
    QSqlQuery *q = 0;
    QSqlError error;
    //the system user can't be deleted
    if (id == "1")
        return false;

    query = "DELETE FROM staff WHERE employee_id='"+ id +"';";
    connect();
    if(!connect()){
        cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << endl;
        return false;
    }

    if (getUserAdminPermission(id))
        this->setUserAdminPermission(id,false);

    q = new QSqlQuery(query, this->getDB());
    error = q->lastError();
    if (error.type()!=QSqlError::NoError){
        switch (error.type()){
        case  QSqlError::NoError:
            break;
        case  QSqlError::ConnectionError:
            break;
        case  QSqlError::StatementError:
            cout << "QUERY ERROR: "<< query.toStdString() << endl;
            break;
        case  QSqlError::TransactionError:
            break;
        case  QSqlError::UnknownError:
            break;
        }
        cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
        delete q;
        disConnect();
        return false;
    }
    delete q;
    disConnect();
    return true;
}

QVector<UserData> UsersModuleDB::getUserList(bool all){
    if(!connect()){
        cerr << "Database Connection Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << endl;
        QVector<UserData>();
    }
    //    QString sql = "SELECT employee_id, name, last_name, address, nss, phone, cellular, email, company_id, picture ";
    //    if (all) sql += "FROM staff ORDER BY employee_id;";
    //    else sql += "FROM staff WHERE active='t' ORDER BY employee_id;";
    QString sql = "SELECT s.employee_id, name, last_name, address, nss, phone, cellular, email, company_id, picture, permission FROM staff s, acl a WHERE s.employee_id = a.employee_id ORDER BY s.employee_id;";
    QSqlQuery query(sql, getDB());
    if ( !query.isActive() || !query.size()){
        disConnect();
        return QVector<UserData>();
    }

    QVector<UserData> result ;
    while(query.next()){
        UserData data;
        data.employee_id = query.value(0).toString();
        data.name = query.value(1).toString();
        data.last_name = query.value(2).toString();
        data.address = query.value(3).toString();
        data.nss = query.value(4).toString();
        data.phone = query.value(5).toString();
        data.cellular = query.value(6).toString();
        data.email = query.value(7).toString();
        data.company_id = query.value(8).toString();
        data.picture = query.value(9).toString();
        data.gerente = !query.value(10).toString().isEmpty();
        result.push_back(data);
    }

    disConnect();
    return result;
}

UserData UsersModuleDB::getUserInfo(QString employee_id){

    QString sql =   "SELECT employee_id, name, last_name, address, nss, phone, cellular, email, company_id, picture ";
    sql += "FROM staff WHERE employee_id='" + employee_id + "' ;";

    if(!connect()){
        cerr << "Database Connection Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << endl;
        return UserData();
    }

    QSqlQuery query(sql, getDB());
    if ( !query.isActive() || !query.size()){
        disConnect();
        return UserData();
    }

    query.first();
    UserData data;
    data.employee_id = query.value(0).toString();
    data.name = query.value(1).toString();
    data.last_name = query.value(2).toString();
    data.address = query.value(3).toString();
    data.nss = query.value(4).toString();
    data.phone = query.value(5).toString();
    data.cellular = query.value(6).toString();
    data.email = query.value(7).toString();
    data.company_id = query.value(8).toString();
    data.picture = query.value(9).toString();
    data.gerente = this->getUserAdminPermission(data.employee_id);

    disConnect();
    return data;
}

bool UsersModuleDB::setUserAdminPermission(const QString& employee_id, bool permission ){
    QString query;
    QSqlError error;
    QSqlQuery *q = 0;
    bool aux_boolean;

    aux_boolean = this->getUserAdminPermission(employee_id);
    if (aux_boolean == permission)
        return true;

    if (permission){
        query   =  "INSERT INTO acl ";
        query +=  "(permission, employee_id) ";
        query +=  "VALUES ('administracion','"+employee_id+"');";

        if(!connect()){
            cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << endl;
            return false;
        }
        q = new QSqlQuery(query, this->getDB());
        error = q->lastError();
        if (error.type()!=QSqlError::NoError){
            switch (error.type()){
            case  QSqlError::NoError:
                break;
            case  QSqlError::ConnectionError:
                break;
            case  QSqlError::StatementError:
                cout << "QUERY ERROR: "<< query.toStdString() << endl;
                break;
            case  QSqlError::TransactionError:
                break;
            case  QSqlError::UnknownError:
                break;
            }
            //some errors ocurred
            cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
        }
        delete q;
        return ((bool) !error.type());
    }
    else{
        query ="DELETE FROM acl ";
        query +=  "WHERE employee_id='"+employee_id+"');";

        if(!connect()){
            cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << endl;
            return false;
        }
        q = new QSqlQuery(query, this->getDB());
        error = q->lastError();
        if (error.type()!=QSqlError::NoError){
            switch (error.type()){
            case  QSqlError::NoError:
                break;
            case  QSqlError::ConnectionError:
                break;
            case  QSqlError::StatementError:
                cout << "QUERY ERROR: "<< query.toStdString() << endl;
                break;
            case  QSqlError::TransactionError:
                break;
            case  QSqlError::UnknownError:
                break;
            }
            //some errors ocurred
            cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
        }
        delete q;
        return ((bool) !error.type());
    }
}

bool UsersModuleDB::getUserAdminPermission(const QString& employee_id){
    QString query;
    QSqlQuery *q = 0;
    bool ret = false;

    query =  "SELECT permission from acl ";
    query += "WHERE employee_id='"+employee_id+"' and permission='administracion';";

    if(!connect()){
        cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << endl;
        return ret;
    }

    q= new QSqlQuery(query, this->getDB());
    if (!q->isActive()){
        delete q;
        return ret;
    }
    if (!q->size()){
        delete q;
        return ret;
    }

    ret = true;
    delete q;
    return ret;
}

bool UsersModuleDB::getActiveFlag(const QString& employee_id){
    QString query;
    QSqlQuery *q = 0;
    bool ret = false;

    connect();
    if(!connect()){
        cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << endl;
        return ret;
    }

    query =  "SELECT active from staff ";
    query += "WHERE employee_id='"+employee_id+";";


    q= new QSqlQuery(query, this->getDB());
    if (!q->isActive()){
        delete q;
        disConnect();
        return ret;
    }
    if (!q->size()){
        delete q;
        disConnect();
        return ret;
    }
    q->first();
    ret = (q->value(0)).toBool();
    delete q;
    disConnect();
    return ret;
}


bool UsersModuleDB::setActiveFlag(const QString& employee_id,bool flag){
    QString query;
    QSqlQuery *q = 0;
    QSqlError error;
    //the system user can't be deleted
    if (employee_id == "1")
        return false;

    if (flag)
        query = "UPDATE staff SET active='t' WHERE employee_id='"+ employee_id +"';";
    else
        query = "UPDATE staff SET active='f' WHERE employee_id='"+ employee_id +"';";

    connect();
    if(!connect()){
        cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << endl;
        return false;
    }

    q = new QSqlQuery(query, this->getDB());
    error = q->lastError();
    if (error.type()!=QSqlError::NoError){
        switch (error.type()){
        case  QSqlError::NoError:
            break;
        case  QSqlError::ConnectionError:
            break;
        case  QSqlError::StatementError:
            cout << "QUERY ERROR: "<< query.toStdString() << endl;
            break;
        case  QSqlError::TransactionError:
            break;
        case  QSqlError::UnknownError:
            break;
        }
        cerr << "Error at UsersModuleDB" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
        delete q;
        disConnect();
        return false;
    }
    delete q;
    disConnect();
    return true;
}





