/***************************************************************************
                          xmlrpcrecipt.cpp  -  description
                             -------------------
    begin                : Fri Jan 9 2004
    copyright            : (C) 2004 by Napsis S.L.
    email                : carlos@napsis.com
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
#include <QUrl>
#include <QDebug>

#define DEFAULT_PORT 18300

ReceiptClient::ReceiptClient(
        const QString& _server_name,
        const QString& _module_name,
        uint _port,
        QObject *parent,
        const QString& name):

    QObject(parent),
    server_name{_server_name},
    module_name {_module_name},
    port(_port)
{
    setObjectName(name);

    QString url {"http://" + server_name +":"+ QString::number(port) +"/"+ module_name};
    QString client_name  {"client" + server_name +"-"+ module_name};
    client = new XRClient(QUrl(url), this);
    client->setObjectName(client_name);
    client->acceptCompressed(false);

    connect(client, &XRClient::methodResponse, this, &ReceiptClient::responseSlot);
    connect(client, &XRClient::fault, this, &ReceiptClient::faultSlot);
}

ReceiptClient::~ReceiptClient(){
    client->deleteLater();
}

QString ReceiptClient::getServer(){
    return server_name;
}

QString ReceiptClient::getModule(){
    return module_name;
}

void ReceiptClient::setAuthToken(const QString& token){
    auth_token = token;
}

void ReceiptClient::setServer(
        QString _server_name,
        QString _module_name,
        int _port)
{

    if ( _port < 0 )  port = DEFAULT_PORT;
    else  port = _port;

    server_name = _server_name;
    module_name = _module_name;
    QString url  { "http://" +server_name+":"+QString::number(port)+"/" + module_name };
    client->setUrl(QUrl(url));
}

void ReceiptClient::responseSlot(int req , const QVariant& variant){

    if ( variant.type() == QVariant::String)
        qDebug() << "Received from " << server_name
                 << "req_id : "<< req << " " << variant.toString();
    else if ( variant.type() == QVariant::Bool){
        if ( variant.toBool() )
            qDebug() << "Received from "<< server_name
                     << "req_id : "<< req << " boolean -> TRUE ";
        else
            qDebug() << "Received from "<< server_name
                     << "req_id : "<< req << " boolean -> FALSE ";
    }
}

void ReceiptClient::faultSlot(int req, int fault_num , const QString& string){
    //The connection refused fault_num

    if (fault_num == -32300 ) return;

    qDebug() << "Fault Number " << fault_num << " For Request " << req;
    qDebug() << " String:  " << string.toLatin1();
}

void ReceiptClient::createReceipt(QString _xml_receipt){

    QList<QVariant> params;
    if (!auth_token.isEmpty())
        params.push_back(auth_token);

    params.push_back(_xml_receipt);
    client->call("DCOPReceiptPrimitives.createReceipt", params);
}

void ReceiptClient::lockReceiptByStartDate(QString employee_id, QString start_time){

    QList<QVariant> params;
    if (!auth_token.isEmpty())
        params.push_back(auth_token);

    params.push_back(employee_id);
    params.push_back(start_time);
    client->call("DCOPReceiptPrimitives.lockReceiptByStartDate", params);
}

void ReceiptClient::unlockReceiptByStartDate(QString employee_id, QString start_time){

    QList<QVariant> params;

    if (!auth_token.isEmpty())
        params.push_back(auth_token);

    params.push_back(employee_id);
    params.push_back(start_time);
    client->call("DCOPReceiptPrimitives.unlockReceiptByStartDate", params);
}

void ReceiptClient::deleteReceiptByStartDate(QString employee_id, QString start_time){

    QList<QVariant> params;
    if (!auth_token.isEmpty())
        params.push_back(auth_token);

    params.push_back(employee_id);
    params.push_back(start_time);
    client->call("DCOPReceiptPrimitives.deleteReceiptByStartDate", params);
}

void ReceiptClient::saveReceipt(QString xml_receipt){

    QList<QVariant> params;
    if ( !auth_token.isEmpty() ) params.push_back(auth_token);
    params.push_back(xml_receipt);
    client->call("DCOPReceiptPrimitives.saveReceipt", params);
}

int ReceiptClient::getPort(){
    return port;
}
