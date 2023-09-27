/***************************************************************************
                          barcore.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#include "authcore.h"
#include "database/authcoredb.h"

#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>
#include <xmlconfig.h>

#include <QFile>
#include <QString>

#include <iostream>
using namespace std;



AuthCore::AuthCore(QObject *parent, QString name):
    QObject(parent),
    db{new AuthCoreDB("AuthCoreDBConnection", Files::configFilePath("database"))}
{
    setObjectName(name);
    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::USER_CHANGED, this);
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::CHANGE_USER, this);

}

AuthCore::~AuthCore(){
    if (db->isConnected()) db->disConnect();
    delete db;
}

QString AuthCore::getUserId(){
    if (!user_data_ptr)
        return "";
    return user_data_ptr->id;
}

QString AuthCore::getUserName(){
    if (!user_data_ptr)
        return "";
    return user_data_ptr->name;
}

QString AuthCore::getUserLastName(){
    if (!user_data_ptr)
        return "";
    return user_data_ptr->last_name;
}

QString AuthCore::getFullName(){
    QString ret;
    if (user_data_ptr)
        ret = user_data_ptr->name+" "+user_data_ptr->last_name;
    return ret;
}

bool AuthCore::getAdmin(){
    if (!user_data_ptr)
        return false;
    return user_data_ptr->administration;
}

bool AuthCore::setUserName(QString name){
    bool ret=false;
    ret = (!clearUser());
    if (db->connect()){
        user_data_ptr = db->getUserName(name);
        db->disConnect();
        ret = true;
        this->emitUserChanged();
        //          cout << "DATOS DE NUEVO USUARIO " << data->id <<"  "<< data->name <<"  "<< data->last_name << endl;
    }
    return ret;
}

bool AuthCore::setUserId(QString id){
    bool ret=false;

    ret = (!clearUser());
    if (db->connect()){
        user_data_ptr = db->getUserId(id);
        db->disConnect();
        ret = true;
        this->emitUserChanged();
        //          cout << "DATOS DE NUEVO USUARIO " << data->id <<"  "<< data->name <<"  "<< data->last_name << endl;
    }
    return ret;
}

bool AuthCore::setUserLastName(QString last_name){
    bool ret=false;
    ret = (!clearUser());
    if (db->connect()){
        user_data_ptr = db->getUserLastName(last_name);
        db->disConnect();
        ret = true;
        this->emitUserChanged();
        //          cout << "DATOS DE NUEVO USUARIO " << data->id <<"  "<< data->name <<"  "<< data->last_name << endl;
    }
    return ret;
}

bool AuthCore::clearUser(){
    if (user_data_ptr){
        delete user_data_ptr;
        user_data_ptr = 0;
        return true;
    }
    return false;
}

void AuthCore::genericDataSignalSlot(const QString& signal_name, XmlConfig *_xml){
    if (signal_name == GDATASIGNAL::CHANGE_USER){
        this->processXml(_xml);
    }
}

void AuthCore::processXml(XmlConfig *_xml){
    QString last_id="";
    QString new_id;

    if (user_data_ptr)
        last_id = user_data_ptr->id;

    _xml->pushDomain();
    _xml->delDomain();
    new_id = _xml->readString("id");

    if (new_id.isEmpty()){
        new_id = _xml->readString("name");
        _xml->popDomain();
        if (new_id.isEmpty())
            return;
        if ((!setUserName(new_id))&& (!last_id.isEmpty()))
            this->setUserId(last_id);
        return;
    }
    _xml->popDomain();
    //     cout << "DATOS DE NUEVO USUARIO " << data->id <<"  "<< data->name <<"  "<< data->last_name << endl;
    if ((!setUserId(new_id)) && (!last_id.isEmpty()))
        this->setUserId(last_id);
}

void AuthCore::emitUserChanged(){
    XmlConfig xml;
    xml.delDomain();
    xml.createElement("id",user_data_ptr->id);
    xml.createElement("name",user_data_ptr->name);
    xml.createElement("last_name",user_data_ptr->last_name);
    emit genericDataSignal(GDATASIGNAL::USER_CHANGED, &xml);
}
