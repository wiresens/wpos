/***************************************************************************
                          printermanager.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Pablo Fernandez
modified by Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "printermanager.h"
#include "dbusprinter_interface.h"

#include <xmlconfig.h>
#include <iostream>

#include <QString>
#include <QDataStream>
#include <QDebug>

using namespace std;
using IDBusReceiptPrinter = com::wiresens::wpos::dbusprinter::DBusReceiptPrinter;
static const QString dbusService = "com.wiresens.wpos.dbusprinter";
static const QString dbusObject = "/wpos/dbusprinter/DBusReceiptPrinter";

PrinterManager::PrinterManager(QObject *parent, const QString& name):
    QObject(parent)
{
    setObjectName(name);
}

PrinterManager::~PrinterManager(){}

bool PrinterManager::printTicket(XmlConfig* xml, int times){

    auto dbusConnection = QDBusConnection::sessionBus();
    IDBusReceiptPrinter *iface;
    iface = new IDBusReceiptPrinter(dbusService, dbusObject, dbusConnection, this);
    if (!iface->isValid()){
        qDebug() << dbusConnection.lastError().message();
        return false;
    }

    auto reply = iface->printTicket(xml->toString(), times);
    return ! reply.isError();
}

//bool PrinterManager::printInvoice(XmlConfig* xml, int times){
//    auto bus = QDBusConnection::sessionBus();
//    IPrinter *iface;
//    iface = new IPrinter(dbus_service, dbus_object, bus, this);
//    if (!iface->isValid()){
//        qDebug() << bus.lastError().message();
//        return false;
//    }

//    auto reply = iface->printInvoice(xml->toString(), times);
//    return reply.isValid();
//}

bool PrinterManager::printInvoice(XmlConfig* xml, int times){
    auto bus = QDBusConnection::sessionBus();
    IDBusReceiptPrinter iface(dbusService, dbusObject, bus);
    if (!iface.isValid()){
        qDebug() << bus.lastError().message();
        return false;
    }

    auto reply = iface.printInvoice(xml->toString(), times);
    return reply.isValid();
}


bool PrinterManager::printX(XmlConfig* xml){
    auto bus = QDBusConnection::sessionBus();
    IDBusReceiptPrinter *iface;
    iface = new IDBusReceiptPrinter(dbusService, dbusObject, bus, this);
    if (!iface->isValid()){
        qDebug() << bus.lastError().message();
        return false;
    }

    auto reply = iface->printX(xml->toString());
    return reply.isValid();
}

bool PrinterManager::printZ (XmlConfig* xml){
    auto bus = QDBusConnection::sessionBus();
    IDBusReceiptPrinter *iface;
    iface = new IDBusReceiptPrinter(dbusService, dbusObject, bus, this);
    if (!iface->isValid()){
        qDebug() << bus.lastError().message();
        return false;
    }

    auto reply = iface->printZ(xml->toString());
    return reply.isValid();
}

bool PrinterManager::printTicketTotal (XmlConfig* xml){
    auto bus = QDBusConnection::sessionBus();
    IDBusReceiptPrinter *iface;
    iface = new IDBusReceiptPrinter(dbusService, dbusObject, bus, this);
    if (!iface->isValid()){
        qDebug() << bus.lastError().message();
        return false;
    }

    auto reply = iface->printTicketTotal(xml->toString());
    return reply.isValid();
}

bool PrinterManager::printKitchenOrder(XmlConfig* xml){
    auto bus = QDBusConnection::sessionBus();
    IDBusReceiptPrinter *iface;
    iface = new IDBusReceiptPrinter(dbusService, dbusObject, bus, this);
    if (!iface->isValid()){
        qDebug() << bus.lastError().message();
        return false;
    }

    auto reply = iface->printKitchenOrder(xml->toString());
    return reply.isValid();
}
