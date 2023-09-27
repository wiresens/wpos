/***************************************************************************
                          dbusreceiptmediator.cpp  -  description
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

#include "dbusreceiptmediator.h"
#include "dbusreceiptmediator_adaptor.h"
#include <xmlconfig.h>

#include <QString>

const QString DBusReceiptMediator::DBusObject  = QString{"/wpos/dbusreceipt/DBusReceiptMediator"};

DBusReceiptMediator::DBusReceiptMediator(DBusReceiptPrimitive *receiptManager,
        QObject *parent):

    QObject(parent),
    primitive{ receiptManager }
{
    new DBusReceiptMediatorAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(DBusReceiptMediator::DBusObject, this);
    mediator = new ReceiptClientMediator(this, "remote");
}

DBusReceiptMediator::~DBusReceiptMediator(){}

bool DBusReceiptMediator::createReceipt(QString xml_string){
    XmlConfig xml;
    if (xml_string.isEmpty() || ! xml.readXmlFromString(xml_string) )
        return false;

    xml.delDomain();
    auto employee_id = xml.readString("employee.dni");
    auto start_time = xml.readString("timestamps.start");

    if ( employee_id.isEmpty() || start_time.isEmpty())
        return false;

    //check the receipt status.
    if (primitive->getReceiptStateByStartDate(employee_id, start_time))
        return false;

    primitive->createReceipt(xml_string);
    mediator->createReceipt(xml_string);
    return true;
}

bool DBusReceiptMediator::saveReceipt(QString xml_string){

    XmlConfig xml;
    if (xml_string.isEmpty() || ! xml.readXmlFromString(xml_string) )
        return false;

    xml.delDomain();
    auto employee_id = xml.readString("employee.dni");
    auto start_time = xml.readString("timestamps.start");

    if ( employee_id.isEmpty() || start_time.isEmpty())
        return false;

    //Check the receipt status.
    if (primitive->getReceiptStateByStartDate(employee_id, start_time))
        return false;

    primitive->saveReceipt(xml_string);
    mediator->unlockReceiptByStartDate(employee_id, start_time);
    mediator->saveReceipt(xml_string);
    return true;
}

bool DBusReceiptMediator::deleteReceiptByStartDate(QString employee_id, QString start_time){
    if ( employee_id.isEmpty() ||
         start_time.isEmpty() ||
         primitive->getReceiptStateByStartDate(employee_id, start_time))

        return false;

    primitive->deleteReceiptByStartDate(employee_id, start_time);
    mediator->unlockReceiptByStartDate(employee_id, start_time);
    mediator->deleteReceiptByStartDate(employee_id, start_time);

    return true;
}

QString DBusReceiptMediator::getReceiptByStartDate(QString employee_id, QString start_time){
    if ( employee_id.isEmpty() ||
         start_time.isEmpty() ||
         primitive->getReceiptStateByStartDate(employee_id, start_time))
        return QString();

    return primitive->getReceiptByStartDate(employee_id, start_time);
}

bool DBusReceiptMediator::lockRemoteByStartDate(QString employee_id, QString start_time){
    mediator->lockReceiptByStartDate(employee_id, start_time);
    return true;
}

bool DBusReceiptMediator::unlockRemoteByStartDate(QString employee_id, QString start_time){
    mediator->unlockReceiptByStartDate(employee_id, start_time);
    return true;
}

QString DBusReceiptMediator::getReceiptResume(){
    return primitive->getReceiptResume();
}

QString DBusReceiptMediator::getReceiptResume(QString employee_id){
    return primitive->getReceiptResume(employee_id);
}
