/***************************************************************************
                          remotereceipt.cpp  -  description
                             -------------------
    begin                : Fri Jan 9 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                :
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "receiptclient.h"
#include "receiptclientmediator.h"
#include <xmlconfig.h>

#include <string>
using std::string;

#include <QString>
#include <QFileSystemWatcher>
#include <QDebug>

const int BSLKXMLRPC_PORT = 18300;
const QString& DEFAULT_TOKEN = QString("0123456789ABCDEF");
const QString& CONFIG_FILE_DTD = QString("/etc/ntpv/dtds/dcopreceipt_config.dtd");
const QString& CONFIG_FILE = QString("/etc/ntpv/dcopreceipt_config.xml");

ReceiptClientMediator::ReceiptClientMediator(QObject *parent, QString name):
    QObject(parent)
{
    setObjectName(std::move(name));
    if (file_path.isEmpty()) file_path = CONFIG_FILE;

    file_watcher = new QFileSystemWatcher(this);
    file_watcher->addPath(file_path);
    file_watcher->addPath(CONFIG_FILE_DTD);

    connect(file_watcher, &QFileSystemWatcher::fileChanged,
            this, &ReceiptClientMediator::fileDirtySlot);

    readConfig(file_path);
}

ReceiptClientMediator::~ReceiptClientMediator(){
    //the file_watcher must be deleted by the kapplication
    file_watcher->deleteLater();
    for( auto client: rpc_clients)
        client->deleteLater();
}

/**
*       this method is used to read the ip that are serving the dbusreceipt module
*       with an xml-rpc server. You can export dbusreceipt modules by using the
*       kxmlrpcd daemon or the bslkxmlrpc daemon if you don't want to autenticate.
*/
bool ReceiptClientMediator::readConfig(const QString& path){

   XmlConfig xml (path);

    if ( !xml.wellFormed()
         || !xml.validateXmlWithDTD(CONFIG_FILE_DTD, true))
        return false;

    xml.delDomain();
    auth_token = DEFAULT_TOKEN;
    if ( xml.howManyTags("authtoken") ){
        auto aux_token = xml.readString("authtoken");
        if (aux_token.length() == 16)
            auth_token = aux_token;
    }

    xml.setDomain("servers");
    auto count = xml.howManyTags("server");

    for (auto i = 0; i < count; i++){
        auto server_name = xml.readString("server["+QString::number(i)+"].ip");
        if ( !server_name.isEmpty() ){
            if ( server_name == "localhost" || server_name.startsWith("127.0.0") )
                continue;
            auto xr_client = new ReceiptClient( server_name, qApp->applicationName(), BSLKXMLRPC_PORT, this, server_name);
            xr_client->setAuthToken( auth_token );
            rpc_clients.append(xr_client, server_name);
        }
    }
    return true;
}

void ReceiptClientMediator::createReceipt(QString _xml_receipt){

    for (auto* xr_client : rpc_clients)
        xr_client->createReceipt( _xml_receipt );
}

void ReceiptClientMediator::lockReceiptByStartDate(QString employee_id, QString start_time){

    for (auto* xr_client : rpc_clients)
        xr_client->lockReceiptByStartDate(employee_id, start_time);
    }

void ReceiptClientMediator::unlockReceiptByStartDate(QString employee_id, QString start_time){

    for (auto* xr_client : rpc_clients)
        xr_client->unlockReceiptByStartDate(employee_id, start_time);

}

void ReceiptClientMediator::deleteReceiptByStartDate(QString employee_id, QString start_time){

    for (auto* xr_client : rpc_clients)
        xr_client->deleteReceiptByStartDate(employee_id, start_time);
}

void ReceiptClientMediator::saveReceipt(QString _xml_receipt){

    for (auto* xr_client : rpc_clients)
        xr_client->saveReceipt(_xml_receipt);
}

void ReceiptClientMediator::fileDirtySlot(const QString& file){

    if (file == file_path){
        qDebug() << "ReceiptClientMediator::fileDirtySlot() :" << QDateTime::currentDateTime().toString() << ": File "+ file +" has changed. Rereading configuration ...";
        rpc_clients.clear();
        if( readConfig(file_path))
            qDebug() << "File "+ file +" configuration reloaded succesfully.";
    }

    if ( file == CONFIG_FILE_DTD)
        qDebug() << "ReceiptClientMediator::fileDirtySlot() :" << QDateTime::currentDateTime().toString() <<": dbusreceipt's DTD has been modified !!!";
}
