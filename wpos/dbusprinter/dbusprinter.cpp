/***************************************************************************
                          dcopprinter.cpp  -  description
                             -------------------
    begin                : lun may 10 17:37:01 CEST 2004
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

#include "dbusprinter.h"
#include "dbusprinter_adaptor.h"
#include "printermanager.h"
#include "xmlrpcprinter.h"
#include <xmlconfig.h>

#include <QString>
#include <QFile>
#include <QFileSystemWatcher>
#include <QtDBus/QDBusConnection>
#include <QCoreApplication>

#include <iostream>
using namespace std;

#define DEFAULT_TYPE "direct"
#define DEFAULT_DEVICE "/dev/lp0"

const int BSLKXMLRPC_PORT = 18300;
const QString& DEFAULT_TOKEN = QString("0123456789ABCDEF");
const QString& CONFIG_FILE_DTD = QString("/etc/ntpv/dtds/dcopprinter_config.dtd");
const QString& CONFIG_FILE = QString("/etc/ntpv/dcopprinter_config.xml");

extern QString PRINTER_DEVICE;
extern QString PRINTER_SYSTEM;
extern QString PRINTER_NAME;

const QString DBusPrinter::DBusService = QString{"com.wiresens.wpos.dbusprinter"};
const QString DBusPrinter::DBusObject  = QString{"/wpos/dbusprinter/DBusReceiptPrinter"};

DBusPrinter::DBusPrinter(QObject *parent, const QString& name):
    QObject(parent)
{
    setObjectName(name);
    new DBusReceiptPrinterAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(DBusPrinter::DBusObject, this);

    if (conf_file_name.isEmpty()) conf_file_name = CONFIG_FILE;
    readConfig(conf_file_name);

    //it should be used by other modules so please be sure that if not implemented here it
    //should be created before all the modules are created.
    file_watcher = new QFileSystemWatcher(this);

    file_watcher->addPath(conf_file_name);
    file_watcher->addPath(CONFIG_FILE_DTD);
    connect(file_watcher, SIGNAL(fileChanged(const QString&)),
            this, SLOT(fileDirtySlot(const QString&)));
}

DBusPrinter::~DBusPrinter(){
    //the file_watcher must be deleted by the kapplication
}

/**
*       this method is used to read the ip that are serving the dbusreceipt module
*       with an xml-rpc server. You can export dbusreceipt modules by using the
*       kxmlrpcd daemon or the bslkxmlrpc daemon if you don't want to autenticate.
*/

void DBusPrinter::readConfig(const QString& path){

    XmlConfig xml(path);
    if (!xml.isValid() || !xml.validateXmlWithDTD(CONFIG_FILE_DTD, true))
        return;

    xml.delDomain();
    auth_token = DEFAULT_TOKEN;
    if ( xml.howManyTags("authtoken") ){
        QString aux_token {xml.readString("authtoken")};
        if (aux_token.length() == 16)
            auth_token = aux_token;
    }

    xml.setDomain("servers");
    auto count = xml.howManyTags("server");
    for ( auto i = 0; i < count; i++ ){
        QString aux  { xml.readString("server["+QString::number(i)+"].ip") };
        if (!aux.isEmpty()){
            auto xr_client = new XmlRpcPrinter(
                 aux,
                 qApp->applicationName(),
                 BSLKXMLRPC_PORT,
                 this,
                 aux.toStdString().c_str()
             );

            xr_client->setAuthToken(auth_token);
            rpc_printers.append(xr_client, aux);
        }
    }

    xml.delDomain();
    main_type = xml.readString("main.type");
    main_device = xml.readString("main.device");
    kitchen_type = xml.readString("kitchen.type");
    kitchen_device = xml.readString("kitchen.device");

    if ( main_type.isEmpty() )   main_type = DEFAULT_TYPE;
    if ( main_device.isEmpty() ) main_device = DEFAULT_DEVICE;
}

void DBusPrinter::printTicket(QString xml_string, int num){

    qDebug() << "DCOPPrinter::printTicket Entering DBus Service Request : ";

    for (auto xr_client : rpc_printers ){
        if  ( xr_client->isRemote() ){
            qDebug() << "DCOPPrinter::printTicket : Sending Request for Remote Printing at : " << xr_client->getServer();
            xr_client->printerTicket( xml_string, num);
        }else {
            qDebug() << "DCOPPrinter::printTicket Local printing :";
            realPrintTicket( xml_string, num);
        }
    }

    qDebug() << "DCOPPrinter::printTicket Leaving DBus Service Request : ";
}

void DBusPrinter::printInvoice(QString xml_string, int num){

    for (auto* xr_client : rpc_printers){
        if ( xr_client->isRemote() ) xr_client->printerInvoice( xml_string, num);
        else  realPrintInvoice( xml_string, num);
    }
}

void DBusPrinter::printX(QString xml_string){

    for (auto* xr_client : rpc_printers){
        if (xr_client->isRemote()) xr_client->printerX(xml_string);
        else realPrintX(xml_string);
    }
}

void DBusPrinter::printZ(QString xml_string){

    for (auto* xr_client : rpc_printers){
        if (xr_client->isRemote()) xr_client->printerZ(xml_string);
        else realPrintZ(xml_string);
    }
}

void DBusPrinter::printTicketTotal(QString xml_string){

    for (auto* xr_client : rpc_printers){
        if (xr_client->isRemote()) xr_client->printerTicketTotal(xml_string);
        else realPrintTicketTotal(xml_string);
    }
}

void DBusPrinter::printKitchenOrder(QString xml_string){
    realPrintKitchenOrder(xml_string);
}

bool DBusPrinter::realPrintTicket(QString xml_string, int num){

    qDebug() << "   Begin DCOPPrinter::realPrintTicket DBus Service Request";

    XmlConfig xml;
    xml.readXmlFromString(xml_string);
    if (!xml.isValid())  return false;

    PrinterManager printer (main_type, main_device);
    printer.printTicket(&xml, num);
    qDebug() << "   End DCOPPrinter::realPrintTicket DBus Service Request";
    return true;
}

bool DBusPrinter::realPrintInvoice(QString xml_string, int num){

    XmlConfig xml;
    xml.readXmlFromString(xml_string);
    if ( !xml.isValid() )  return false;

    PrinterManager printer(main_type, main_device);
    printer.printInvoice(&xml, num);
    return true;
}

bool DBusPrinter::realPrintX(QString xml_string){

    XmlConfig xml ;
    xml.readXmlFromString(xml_string);
    if ( !xml.isValid() )  return false;

    PrinterManager printer(main_type, main_device);
    printer.printX(&xml);
    return true;
}

bool DBusPrinter::realPrintZ(QString xml_string){

    XmlConfig xml ;
    xml.readXmlFromString(xml_string);
    if ( !xml.isValid() )  return false;

    PrinterManager printer(main_type, main_device);
    printer.printZ(&xml);
    return true;
}

bool DBusPrinter::realPrintTicketTotal(QString xml_string){

    XmlConfig xml ;
    xml.readXmlFromString(xml_string);
    if ( !xml.isValid() )  return false;

    PrinterManager printer(main_type, main_device);
    printer.printTicketTotal(&xml);
    return true;
}

bool DBusPrinter::realPrintKitchenOrder(QString xml_string){

    if ( kitchen_type.isEmpty() || kitchen_device.isEmpty() )  return true;

    XmlConfig xml ;
    xml.readXmlFromString(xml_string);
    if ( !xml.isValid() )  return false;

    PrinterManager printer(kitchen_type, kitchen_device);
    printer.printKitchenOrder(&xml);
    return true;
}

void DBusPrinter::fileDirtySlot(const QString& file){
    if (file == conf_file_name){
        rpc_printers.clear();
        readConfig(conf_file_name);

        qDebug() << "DCOPPrinter::fileDirtySlot() :"
                 << QDateTime::currentDateTime().toString()
                 << ": File "+ file +" has changed. Rereading configuration ...";
    }

    if ( file == CONFIG_FILE_DTD)
        qDebug() << "DCOPPrinter::fileDirtySlot() :"
                 << QDateTime::currentDateTime().toString()
                  << " DTD "+ file +" has been modified !!!";
}
