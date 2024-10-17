/***************************************************************************
                          xmlrpcprinter.h  -  description
                             -------------------
    begin                : Mon may 10 2004
    copyright            : (C) 2004 by Carlos Manzanedo
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

#ifndef XMLRPCPRINTER_H
#define XMLRPCPRINTER_H

#include <xrclient.h>
#include <wposcore/hlist.h>

#include <QObject>
#include <QString>
#include <QVariant>

class XmlRpcPrinter : public QObject{
    Q_OBJECT
public: 
    XmlRpcPrinter(const QString& server_name,
                  const QString& module_name,
                  int port = -1,
                  QObject *parent = nullptr,
                  const QString& name = QString{});

    virtual ~XmlRpcPrinter();
    virtual bool isRemote();

    void printerTicket(QString, int num);
    void printerInvoice(QString, int num);
    void printerX(QString);
    void printerZ(QString);
    void printerTicketTotal(QString);

    QString getServer();
    QString getModule();
    int getPort();
    void setAuthToken(const QString& token);

public slots:
    virtual void setServer(QString server_name, QString module_name, int port) final;
    virtual void processHttpResponse(int req ,const QVariant& variant);
    virtual void handleFault(int req,int fault_num ,const QString& string);

private:
    XRClient *client{};
    QString server_name;
    QString module_name;
    QString auth_token;
    int port;
};

#endif
