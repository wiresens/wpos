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

#include <libbslxml/xmlconfig.h>
#include <wposcore/config.h>

#include <string>
using std::string;

#include <QString>
#include <QFileSystemWatcher>
#include <QDebug>

const int WPOSD_RPC_PORT        = 18300;
const QString& DEFAULT_TOKEN    = QString("0123456789ABCDEF");
const QString& DTD_CFG_FILE     = cfg::dtdFileByKey(cfg::DTDKey::Receipt);
const QString& XML_CFG_FILE     = cfg::xmlFileByKey(cfg::XMLKey::Receipt);

ReceiptClientMediator::ReceiptClientMediator(
    QObject *parent,
    QString name)
    :QObject(parent),
    m_xml_cfg_file {QFile(XML_CFG_FILE).fileName()},
    m_dtd_cfg_file {QFile(DTD_CFG_FILE).fileName()}
{
    setObjectName(std::move(name));

    file_watcher = new QFileSystemWatcher(this);
    file_watcher->addPath(m_xml_cfg_file);

    connect(file_watcher, &QFileSystemWatcher::fileChanged,
            this, &ReceiptClientMediator::updateClients);

    createReceiptClients(m_xml_cfg_file);
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
bool ReceiptClientMediator::createReceiptClients(const QString& xml_file){

    XmlConfig xml (xml_file, m_dtd_cfg_file);
    xml.delDomain();
    auth_token = DEFAULT_TOKEN;
    if ( xml.howManyTags("authtoken") ){
        auto aux_token = xml.readString("authtoken");
        if (aux_token.length() == 16)
            auth_token = aux_token;
    }

    xml.setDomain("servers");
    for (auto i = 0; i < xml.howManyTags("server"); i++){
        auto server_name = xml.readString("server["+QString::number(i)+"].ip");
        if ( !server_name.isEmpty() ){

            if ( server_name == "localhost" ||
                server_name.startsWith("127.0.0")
            )
                continue;

            auto xr_client = new ReceiptClient(
                server_name, qApp->applicationName(),
                WPOSD_RPC_PORT, this, server_name
            );

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

void ReceiptClientMediator::updateClients(const QString& file){
    if (file == m_xml_cfg_file){
        qDebug() << "ReceiptClientMediator::updateClients() :"
                << QDateTime::currentDateTime().toString()
                << ": File "+ file +" has changed. Rereading configuration ...";

        for (auto* xr_client : rpc_clients)
            xr_client->deleteLater();
        rpc_clients.clear();

        if( createReceiptClients(m_xml_cfg_file))
            qDebug() << "File "+ file +" configuration reloaded succesfully.";
    }
    else if ( file == m_dtd_cfg_file)
        qDebug() << "ReceiptClientMediator::updateClients() :"
                 << QDateTime::currentDateTime().toString()
                 <<": dbusreceipt's DTD has been modified !!!";
}
