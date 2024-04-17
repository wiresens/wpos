/***************************************************************************
                          xmlrpcprinter.cpp  -  description
                             -------------------
    begin                : lun may 10 2004
    copyright            : (C) 2004 by Carlos Manzanedo
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

#include "xmlrpcprinter.h"
#include <qurl.h>

#include <iostream>
using namespace std;

static const uint DEFAULT_PORT {18300};

XmlRpcPrinter::XmlRpcPrinter(
        const QString& _server_name,
        const QString& _module_name,
        int _port,
        QObject *parent,
        const QString& name):

    QObject(parent),
    port{_port}
{
    setObjectName(name);
    QString url, client_name;
    server_name = _server_name;
    module_name = _module_name;
    if (port < 0)   port =  DEFAULT_PORT;

    url = "http://" +server_name+":"+QString::number(port)+"/"+ module_name;
    client_name = "client" + server_name + "-" + module_name;
    client = new XRClient( QUrl(url), this);
    client->setObjectName(client_name);
    client->acceptCompressed(false);

    connect(client, &XRClient::methodResponse, this, &XmlRpcPrinter::processHttpResponse);
    connect(client, &XRClient::fault, this, &XmlRpcPrinter::handleFault);
}

XmlRpcPrinter::~XmlRpcPrinter(){
    client->deleteLater();
}

bool XmlRpcPrinter::isRemote(){
    return !( server_name.contains("localhost") || server_name.startsWith( "127.0.0") );
}

QString XmlRpcPrinter::getServer(){
    return server_name;
}

QString XmlRpcPrinter::getModule(){
    return module_name;
}

void XmlRpcPrinter::setAuthToken(const QString& token){
    auth_token = token;
}

void XmlRpcPrinter::setServer(
        QString _server_name,
        QString _module_name,
        int _port)
{
    QString url;

    if (_port <0)
        port = DEFAULT_PORT;
    else
        port = _port;

    server_name = _server_name;
    module_name = _module_name;
    url = "http://" +server_name+":"+QString::number(port)+"/" + module_name;
    client->setUrl(QUrl(url));
}

void XmlRpcPrinter::processHttpResponse(int req ,const QVariant& variant){

    qDebug() << "XmlRpcPrinter::processHttpResponse : " << XRClient::USER_AGENT;
    if (variant.type() == QVariant::String)
        qDebug() << "Received From " << server_name
             << "request_id : " << req << " "
             << variant.toString();

    else if (variant.type() == QVariant::Bool){
        if (variant.toBool())
            qDebug() << "Received From " << server_name
                     << "request_id : "<< req << " boolean -> TRUE ";
        else
            qDebug() << "Received From "<< server_name
                     << "request_id : "<< req << " boolean -> FALSE ";
    }
}

void XmlRpcPrinter::handleFault(int req, int fault_num, const QString& string){
    qDebug() << XRClient::USER_AGENT<<" : Remote Server Reply for Request N°" << req
             << "-- XML RPC Error" << fault_num << ":" << string;
}

void XmlRpcPrinter::printerTicket(QString xml_string, int num){

    QList<QVariant> params;

    if ( ! auth_token.isEmpty() )
        params.push_back(auth_token);

    params.push_back(xml_string);
    params.push_back(num);

    qDebug() << "XmlRpcPrinter::printTicket : Using User Agent" << XRClient::USER_AGENT
             << " for Remote Printing" ;
    client->call("printer.realPrintTicket", params);

}

void XmlRpcPrinter::printerInvoice(QString xml_string,int num){
    QList<QVariant> params;

    if (!auth_token.isEmpty())
        params.push_back(auth_token);
    params.push_back(xml_string);
    params.push_back(num);

    client->call("printer.realPrintInvoice",params);
}

void XmlRpcPrinter::printerX(QString xml_string){

    QList<QVariant> params;

    if (!auth_token.isEmpty())
        params.push_back(auth_token);
    params.push_back(xml_string);

    client->call("printer.realPrintX",params);
}

void XmlRpcPrinter::printerZ(QString xml_string){

    QList<QVariant> params;

    if (!auth_token.isEmpty())
        params.push_back(auth_token);
    params.push_back(xml_string);

    client->call("printer.realPrintZ",params);
}

void XmlRpcPrinter::printerTicketTotal(QString xml_string){

    QList<QVariant> params;

    if (!auth_token.isEmpty())
        params.push_back(auth_token);
    params.push_back(xml_string);

    client->call("printer.realPrintTicketTotal", params);
}
