/***************************************************************************
                          dbusreceiptprimitive.h  -  description
                          ---------------------------------
    begin                : Thu Jan 8 2004
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

#ifndef DBUSRECEIPTPRIMITIVE_H
#define DBUSRECEIPTPRIMITIVE_H

#include "receiptprimitiveinterface.h"
#include "receiptdb.h"

#include <QObject>

/**
  *@author Carlos Manzanedo
  */

class QFileSystemWatcher;

class DBusReceiptPrimitive : public QObject, virtual public ReceiptPrimitiveInterface  {

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.dbusreceipt.DBusReceiptPrimitive")

public:

    static const QString DBusObject;

    explicit DBusReceiptPrimitive(QObject *parent=nullptr,const QString& name = QString());
    ~DBusReceiptPrimitive();

public slots:
    /**
        *       this method create a receipt in the database... . In this case the method create a
        *       receipt based on the employee_id and the start_time of the receipt...
        *       @param QString containing the xml with the receipt description.
        *       @return boolean with true if the receipt has been created at the database or false if:
        *       the receipt exist or there is no way to access to the database or the query fails.
        */
    bool createReceipt(QString xml_receipt);

    /**
        *       this method save and update a receipt at the database if it's not locked... So if it's locked you must
        *       unlock it before saving. It update the receipt given by employee_id and start_time of the receipt.
        *       @param QString containing the xml with the receipt description.
        *       @return true if the receipt has been updated at the database.
        */
    bool saveReceipt(QString xml_receipt);

    /**
        *       this method delete a receipt from the database by the employee_id and the start_time of the receipt
        *       @param QString with the employee_id wich start the receipt
        *       @param QString with the Start time of the receipt.... it must be like ('2004-01-01 13:00:00')
        *       @return true if the receipt has been deleted from the database, false if it's locked, it doesn't exist or
        *               it can't be deleted from the database (the query fails).
        */
    bool deleteReceiptByStartDate(QString employee_id, QString start_time);

    /**
        *       this method will return a receipt given by employee_id and start_time...
        *       @param QString with the employee_id wich start the receipt
        *       @param QString with the Start time of the receipt.... it must be like ('2004-01-01 13:00:00')
        *       @return an empty QString if the receipt does not exist, or a QString with an xml that contains
        *               the receipt description.
        */
    QString getReceiptByStartDate(QString employee_id, QString start_time);

    /**
        *       this method lock a receipt at the database by the employee_id and the start_time of the
        *       @param QString with the employee_id wich start the receipt
        *       @param QString with the Start time of the receipt.... it must be like ('2004-01-01 13:00:00')
        *       @return boolean with true if all's ok, or false if the receipt exists and it's locked or it does not exists
        */
    bool lockReceiptByStartDate(QString employee_id, QString start_time);

    /**
        *       this method unlock a receipt at the database by the employee_id and the start_time of the
        *       @param QString with the employee_id wich start the receipt
        *       @param QString with the Start time of the receipt.... it must be like ('2004-01-01 13:00:00')
        *       @return boolean with true if all's ok, or false if the receipt exists and it's unlocked or it does not exists
        */
    bool unlockReceiptByStartDate(QString employee_id, QString start_time);

    /**
        *       this two methods will return a serialized xml string with all the receipts
        *       and the state of those receipts at the local database. Second method does only
        *       return receipt of given employee_id.
        *       @param QString employee_id
        *       @return QString with a serialized xml.
        */
    QString getReceiptResume();
    QString getReceiptResume(QString employee_id);

    /**
        *       this method check if a receipt exists or not by the employee_id and the start_time of the receipt
        *       @param QString with the employee_id wich start the receipt
        *       @param QString with the Start time of the receipt.... it must be like ('2004-01-01 13:00:00')
        *       @return boolean with true if the receipt exists, false if not or have been problems while checking
        *               at the database.
        */
    bool existsReceiptByStartDate(QString employee_id, QString start_date);

    /**
        *       this method check the state of a receipt (locked/unlocked) at the database
        *       by the employee_id and the start_time of the receipt
        *       @param QString with the employee_id wich start the receipt
        *       @param QString with the Start time of the receipt.... it must be like ('2004-01-01 13:00:00')
        *       @return true if the receipt is locked, false if it's unlocked or it doesn't exist
        */
    bool getReceiptStateByStartDate(QString employee_id, QString start_time);

signals:
    void receiptChangedByStartDate(const QString&, const QString&);

protected slots:
    void fileDirtySlot(const QString& file);

protected:
    ReceiptDB *receipt_db;
    QFileSystemWatcher* file_watcher;
};

#endif
