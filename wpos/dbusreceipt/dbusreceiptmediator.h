/***************************************************************************
                          dbusreceiptmediator.h  -  description
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

#ifndef DBUSRECEIPTMEDIATOR_H
#define DBUSRECEIPTMEDIATOR_H

#include "receiptmediatorinterface.h"

/**
  *@author Carlos Manzanedo
  */
#include <QObject>

#include "dbusreceiptprimitive.h"
#include "receiptclientmediator.h"

class DBusReceiptMediator :
        public QObject,
        virtual public ReceiptMediatorInterface
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.dbusreceipt.DBusReceiptMediator")

public:

    static const QString DBusObject;

    explicit DBusReceiptMediator(DBusReceiptPrimitive *receiptManager, QObject *parent=nullptr);
    ~DBusReceiptMediator();

public slots:
    bool createReceipt(QString xml_string);
    bool saveReceipt(QString xml_string);

    bool deleteReceiptByStartDate(QString employee_id, QString start_time);

    QString getReceiptByStartDate(QString employee_id, QString start_time);
    QString getReceiptResume();
    QString getReceiptResume(QString employee_id);

    bool lockRemoteByStartDate(QString employee_id, QString start_time);
    bool unlockRemoteByStartDate(QString employee_id, QString start_time);

protected:
    DBusReceiptPrimitive *primitive;
    ReceiptClientMediator *mediator;
};

#endif
