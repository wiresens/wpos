/***************************************************************************
                          remotereceipt.h  -  description
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

#ifndef RECEIPTCLIENTMEDIATOR_H
#define RECEIPTCLIENTMEDIATOR_H


/**
  *@author Carlos Manzanedo
  */

#include <QObject>
#include <wposcore/hlist.h>

class ReceiptClient;
class QFileSystemWatcher;

class ReceiptClientMediator : public QObject {
    Q_OBJECT

public:
    explicit ReceiptClientMediator(QObject *parent = nullptr, QString name = QString());
    ~ReceiptClientMediator();

    bool readConfig(const QString& path);

    /**
        *       all changes are done via xml-rpc at the remotes machines shorted in list.
        *       these machines should run the bslkxmlrpcd and the dbusreceipt module.
        ********************************************************************
        *       this method create a receipt in the database... . In this case the method create a
        *       receipt based on the employee_id and the start_time of the receipt...
        *       @param QString containing the xml with the receipt description.
        *       @return boolean with true if the receipt has been created at the database or false if:
        *               the receipt exist or there is no way to acces to the database or the query fails.
        */
    void createReceipt(QString _xml_receipt);

    /**
        *       all changes are done via xml-rpc at the remotes machines sorted in list.
        *       this machines should run the bslkxmlrpcd and the dbusreceipt module.
        ********************************************************************
        *       this method save and update a receipt at the database if it's not locked... So if it's locked you must
        *       unlock it before saving. It updates the receipt given by employee_id and start_time of the receipt.
        *       @param QString containing the xml with the receipt description.
        *       @return true if the receipt has been updated at the database.
        */
    void saveReceipt(QString _xml_receipt);

    /**
        *       all changes are done via xml-rpc at the remotes machines shorted in list.
        *       this machines should run the bslkxmlrpcd and the dbusreceipt module.
        ********************************************************************
        *       this method lock a receipt at the database by the employee_id and the start_time of the
        *       @param QString with the employee_id wich start the receipt
        *       @param QString with the Start time of the receipt.... it must be like ('2004-01-01 13:00:00')
        *       @return boolean with true if all's ok, or false if the receipt exists and it's locked or it does not exists
        */
    void lockReceiptByStartDate(QString employee_id, QString start_time);

    /**
        *       all changes are done via xml-rpc at the remotes machines shorted in list.
        *       this machines should run the bslkxmlrpcd and the dbusreceipt module.
        ********************************************************************
        *       this method unlock a receipt at the database by the employee_id and the start_time of the
        *       @param QString with the employee_id wich start the receipt
        *       @param QString with the Start time of the receipt.... it must be like ('2004-01-01 13:00:00')
        *       @return boolean with true if all's ok, or false if the receipt exists and it's unlocked or it does not exists
        */
    void unlockReceiptByStartDate(QString employee_id, QString start_time);

    /**
        *       all changes are done via xml-rpc at the remotes machines shorted in list.
        *       this machines should run the bslkxmlrpcd and the dbusreceipt module.
        ********************************************************************
        *       this method delete a receipt from the database by the employee_id and the start_time of the receipt
        *       @param QString with the employee_id wich start the receipt
        *       @param QString with the Start time of the receipt.... it must be like ('2004-01-01 13:00:00')
        *       @return true if the receipt has been deleted from the database, false if it's locked, it doesn't exist or
        *               it can't be deleted from the database (the query fails).
        */
    void deleteReceiptByStartDate(QString employee_id, QString start_time);

protected slots:
    void fileDirtySlot(const QString& file);
protected:
    HList<ReceiptClient> rpc_clients;
    QString file_path;
    QString auth_token;

    QFileSystemWatcher *file_watcher;
};

#endif
