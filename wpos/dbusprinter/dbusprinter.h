/***************************************************************************
                          dcopprinter.h  -  description
                             -------------------
    begin                : lun may 10 17:37:01 CEST 2004
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

#ifndef H_DBUS_PRINTER_H
#define H_DBUS_PRINTER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "dbusprinterinterface.h"
#include <QObject>
#include <wposcore/hlist.h>

class QStringList;
class QString;
class XmlRpcPrinter;
class QFileSystemWatcher;

class DBusPrinter : public QObject , virtual public DBusPrinterInterface
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.dbusprinter.DBusReceiptPrinter")

public:
    static const QString DBusService;
    static const QString DBusObject;

    explicit DBusPrinter(QObject* parent=nullptr, const QString& name = QString());
    ~DBusPrinter();

    void readConfig(const QString& path);
    bool realPrintKitchenOrder(QString);

public slots:
    virtual void printTicket(QString, int num) override;
    virtual void printInvoice(QString, int num) override;
    virtual void printX(QString) override;
    virtual void printZ(QString) override;
    virtual void printTicketTotal(QString) override;
    virtual void printKitchenOrder(QString) override;

    virtual bool realPrintTicket(QString,int) override;
    virtual bool realPrintInvoice(QString,int) override;
    virtual bool realPrintX(QString) override;
    virtual bool realPrintZ(QString) override;
    virtual bool realPrintTicketTotal(QString) override;

protected slots:
    void fileDirtySlot(const QString& file);

protected:
    HList<XmlRpcPrinter> rpc_printers{};
    QFileSystemWatcher* file_watcher{};
    QString conf_file_name;
    QString auth_token;

    QString main_device;
    QString main_type;
    QString kitchen_device;
    QString kitchen_type;
};

#endif
