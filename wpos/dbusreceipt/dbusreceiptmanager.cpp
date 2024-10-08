/***************************************************************************
                          dbusreceipt.cpp  -  description
                             -------------------
    begin                : Thu Jan  8 16:58:58 CET 2004
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

#include "dbusreceiptmanager.h"
#include <QString>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>

const QString DBusReceiptManager::DBusService = QString{"com.wiresens.wpos.dbusreceipt"};

DBusReceiptManager::DBusReceiptManager(QObject *parent, const QString& name) :
    QObject(parent)
{
    setObjectName(name);
    //We try to register the service to the dbus-daemon session buss
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if(! dbus.registerService(DBusReceiptManager::DBusService)){
        qDebug() << dbus.lastError().message();
        exit(EXIT_FAILURE);
    }

    // Initialize the interfaces...
    m_receipt_primitive   = new DBusReceiptPrimitive(this, "DBusReceiptPrimitive");
    m_receipt_mediator    = new DBusReceiptMediator(m_receipt_primitive, this);
    m_receipt_mediator->setObjectName("DBusReceiptMediator");
    m_receipt_change_notifiyer   = new ReceiptChangeNotifiyer(this, "ReceiptChangeNotifiye");

    connect(m_receipt_primitive, &DBusReceiptPrimitive::receiptChangedByStartDate,
            m_receipt_change_notifiyer, &ReceiptChangeNotifiyer::receiptChangedSlotByDate);
}
