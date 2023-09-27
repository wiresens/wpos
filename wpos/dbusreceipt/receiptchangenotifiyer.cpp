/***************************************************************************
                          linuxbarcomunicator.cpp  -  description
                             -------------------
    begin                : Tue Jan 13 2004
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

#include "receiptchangenotifiyer.h"

#include <QDataStream>
#include <QString>
#include <QDBusConnection>
#include <QDBusMessage>
#include <iostream>

using std::cerr;
using std::endl;

ReceiptChangeNotifiyer::ReceiptChangeNotifiyer(QObject *parent,const QString& name):QObject(parent){
    setObjectName(name);
}

void ReceiptChangeNotifiyer::receiptChangedSlotByDate(
        const QString& employee_id,
        const QString& start_time)
{
    auto dbus = QDBusConnection::sessionBus();
    if (!dbus.isConnected())
    {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                        "To start it, run:\n"
                        "\teval `dbus-launch --auto-syntax`\n");
        return;
    }

    auto message = QDBusMessage::createMethodCall(
                       "com.wiresens.wpos.wpos",
                       "/wpos/wpos/ReceiptRecover",
                       "com.wiresens.wpos.wpos.ReceiptRecover",
                       "receiptChanged"
                     );

    QList<QVariant> args;
    args.append(employee_id);
    args.append(start_time);
    message.setArguments(args);
    if(!dbus.send(message))
        cerr << "Problems while trying to call wpos" << endl;
}
