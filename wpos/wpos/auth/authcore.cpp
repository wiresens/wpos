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

AuthCore::AuthCore(QObject *parent, QString name):
    QObject(parent),
    db{ new AuthCoreDB("AuthCoreConnection", cfg::xmlFileByKey(cfg::XMLKey::Database))}
{
    setObjectName(name);
    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::USER_CHANGED, this);
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::CHANGE_USER, this);
    db->connect();
}

AuthCore::~AuthCore(){
    db->disConnect();
    delete db;
}

const QString& AuthCore::userId() const{
    return user.id;
}

const QString& AuthCore::userName() const{
    return user.name;
}

const QString& AuthCore::userLastName() const{
    return user.last_name;
}

QString AuthCore::userFullName() const{
    return  user.name+" "+user.last_name;
}

bool AuthCore::isRootUser() const{
    return user.isRootUser;
}

bool AuthCore::loadUserById(QString id){
    bool loaded {false};

    if (db->isConnected()){
        user = db->userById(id);
        loaded = true;
        notifyUserChanged();
    }
    return loaded;
}

bool AuthCore::loadUserByName(QString name){
    bool loaded {false};
    if (db->isConnected()){
        user = db->userByName(name);
        loaded = true;
        notifyUserChanged();
    }
    return loaded;
}

bool AuthCore::loadUserByLastName(QString last_name){
    bool loaded{false};
    if (db->isConnected()){
        user = db->userByLastName(last_name);
        loaded = true;
        notifyUserChanged();
    }
    return loaded;
}

void AuthCore::genericDataSignalSlot(const QString& signal, XmlConfig *xml){
    if (signal == GDATASIGNAL::CHANGE_USER)
        processXml(*xml);

}

void AuthCore::notifyUserChanged(){
    XmlConfig xml;
    xml.delDomain();
    xml.createElement("id", user.id);
    xml.createElement("name",user.name);
    xml.createElement("last_name",user.last_name);
    emit genericDataSignal(GDATASIGNAL::USER_CHANGED, &xml);
}

void AuthCore::processXml(XmlConfig &xml){
    QString lastId = user.id;

    xml.pushDomain();
    xml.delDomain();
    QString new_id = xml.readString("id");

    if (new_id.isEmpty()){
        new_id = xml.readString("name");
        xml.popDomain();

        if (new_id.isEmpty()) return;
        if ( !loadUserByName(new_id) && !lastId.isEmpty() )
            loadUserById(lastId);
        return;
    }
    xml.popDomain();
    if (!loadUserById(new_id) && !lastId.isEmpty()) loadUserById(lastId);
}
