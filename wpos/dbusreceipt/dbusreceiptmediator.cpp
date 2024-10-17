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
#include <libbslxml/xmlconfig.h>

#include <QString>

const QString DBusReceiptMediator::DBusObject  = QString{"/wpos/dbusreceipt/DBusReceiptMediator"};

DBusReceiptMediator::DBusReceiptMediator(DBusReceiptPrimitive *receiptManager,
        QObject *parent):

    QObject(parent),
    m_receipt_primitive{ receiptManager }
{
    new DBusReceiptMediatorAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(DBusReceiptMediator::DBusObject, this);
    m_receipt_client_mediator = new ReceiptClientMediator(this, "remote");
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
    if (m_receipt_primitive->getReceiptStateByStartDate(employee_id, start_time))
        return false;

    m_receipt_primitive->createReceipt(xml_string);
    m_receipt_client_mediator->createReceipt(xml_string);
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
    if (m_receipt_primitive->getReceiptStateByStartDate(employee_id, start_time))
        return false;

    m_receipt_primitive->saveReceipt(xml_string);
    m_receipt_client_mediator->unlockReceiptByStartDate(employee_id, start_time);
    m_receipt_client_mediator->saveReceipt(xml_string);
    return true;
}

bool DBusReceiptMediator::deleteReceiptByStartDate(QString employee_id, QString start_time){
    if ( employee_id.isEmpty() ||
         start_time.isEmpty() ||
         m_receipt_primitive->getReceiptStateByStartDate(employee_id, start_time))

        return false;

    m_receipt_primitive->deleteReceiptByStartDate(employee_id, start_time);
    m_receipt_client_mediator->unlockReceiptByStartDate(employee_id, start_time);
    m_receipt_client_mediator->deleteReceiptByStartDate(employee_id, start_time);

    return true;
}

QString DBusReceiptMediator::getReceiptByStartDate(QString employee_id, QString start_time){
    if ( employee_id.isEmpty() ||
         start_time.isEmpty() ||
         m_receipt_primitive->getReceiptStateByStartDate(employee_id, start_time))
        return QString{};

    return m_receipt_primitive->getReceiptByStartDate(employee_id, start_time);
}

bool DBusReceiptMediator::lockRemoteByStartDate(QString employee_id, QString start_time){
    m_receipt_client_mediator->lockReceiptByStartDate(employee_id, start_time);
    return true;
}

bool DBusReceiptMediator::unlockRemoteByStartDate(QString employee_id, QString start_time){
    m_receipt_client_mediator->unlockReceiptByStartDate(employee_id, start_time);
    return true;
}

QString DBusReceiptMediator::getReceiptResume(){
    return m_receipt_primitive->getReceiptResume();
}

QString DBusReceiptMediator::getReceiptResume(QString employee_id){
    return m_receipt_primitive->getReceiptResume(employee_id);
}
