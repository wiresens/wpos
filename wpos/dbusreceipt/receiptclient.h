/***************************************************************************
                          xmlrpcrecipt.h  -  description
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

#ifndef RECEIPTCLIENT_H
#define RECEIPTCLIENT_H


/**
  *@author Carlos Manzanedo
  */
#include <QObject>

#include <xrclient.h>
#include <wposcore/hlist.h>


#include <QString>
#include <QVariant>


class ReceiptClient : public QObject{
    Q_OBJECT

public:
    ReceiptClient(const QString& server_name,
                  const QString& module_name,
                  uint port,
                  QObject *parent = nullptr,
                  const QString& name = QString());

    ~ReceiptClient();

    void createReceipt(QString _xml_receipt);
     void saveReceipt(QString xml_receipt);

    void lockReceiptByStartDate(QString employee_id, QString start_time);
    void unlockReceiptByStartDate(QString employee_id, QString start_time);

    void deleteReceiptByStartDate(QString employee_id, QString start_time);

    QString getServer();
    QString getModule();
    int getPort();
    void setAuthToken(const QString& token);

public slots:
    void setServer(QString server_name, QString module_name, int port);
    void responseSlot(int req ,const QVariant& variant);
    void faultSlot(int req,int fault_num ,const QString& string);

protected:
    QString server_name;
    QString module_name;
    QString auth_token;

    XRClient *client{};
    unsigned int port;
};

#endif
