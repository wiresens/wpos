/***************************************************************************
                          dcopreceiptcomm.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef DBUS_RECEIPTAGENT_H
#define DBUS_RECEIPTAGENT_H

#include <QObject>

class QString;
class XmlConfig;

class DBusReceiptQuerier : public QObject  {
    Q_OBJECT

public:
    explicit DBusReceiptQuerier(QObject *parent=nullptr);

    //Using ReceiptPrimitivesInterface : ReadOnly Access
    bool receiptExist(const QString &  employee_id, const QString & start_time);
    bool receiptState(const QString& employee_id, const QString& start_time);
    XmlConfig* getReceipt(const QString& employee_id, const QString &start_time);
    XmlConfig* getReceiptResume();
    XmlConfig* getReceiptResume(QString employee_id);

    //Using ReceiptMediatorInterface : ReadWrite Access
    bool createReceipt(XmlConfig *xml);
    bool saveReceipt(XmlConfig *xml);
    bool deleteReceipt(const QString employee_id, const QString start_time);
    bool lockRemoteReceipts(const QString &employee_id, const QString& start_time);
    bool unlockRemoteReceipts(const QString &employee_id, const QString& start_time);

private:
    void log(const QString& svc, const QString& obj, const QString& method, const QString& msg = QString() );
};

#endif
