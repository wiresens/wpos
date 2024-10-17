/***************************************************************************
                          dbusreceipt.h  -  description
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

#ifndef DBUS_RECEIPT_H
#define DBUS_RECEIPT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <QObject>

#include "dbusreceiptprimitive.h"
#include "dbusreceiptmediator.h"
#include "receiptchangenotifiyer.h"

/** DBusReceiptManager is the base class of the project */
class DBusReceiptManager : public QObject {
    Q_OBJECT

public:
    static const QString DBusService;

    explicit DBusReceiptManager(
        QObject *parent = nullptr,
        const QString& name = QString{}
    );

private:
    DBusReceiptPrimitive    *m_receipt_primitive;
    DBusReceiptMediator     *m_receipt_mediator;
    ReceiptChangeNotifiyer  *m_receipt_change_notifiyer;
};

#endif
