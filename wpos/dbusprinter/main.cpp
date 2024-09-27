/***************************************************************************
                          main.cpp  -  description
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
#define VERSION "1.2rc2"
#define DCOPPRINTER_XML_CONFIG "/etc/ntpv/dcopprinter_config.xml"

#include "main.h"
#include "dbusprinter.h"
#include <xmlconfig.h>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QtDBus/QtDBus>
#include <QDebug>
#include <QString>
#include <QFile>

#ifndef _WINDOWS
    extern "C"{
        #include <cups/cups.h>
    }
#endif

#include <iostream>
using namespace std;

extern QString PRINTER_DEVICE;
extern QString PRINTER_SYSTEM;
extern QString PRINTER_NAME;

static const QString version = VERSION;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(QFileInfo(QFile(argv[0]).fileName()).baseName());

    app.setApplicationVersion(version);

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    QCommandLineOption systemOp({ {"s","system"}, QObject::tr("With ipp you must specify printer, with direct, the device."),"ipp,direct"});
    QCommandLineOption deviceOp({ {"d","device"}, QObject::tr("Printer device (ESC/POS Supported printers)."),"dev"});
    QCommandLineOption addressOp({ {"p","printer"}, QObject::tr("Printer address (ESC/POS Supported printers."),"host@printer"});
    parser.addOption(systemOp);
    parser.addOption(deviceOp);
    parser.addOption(addressOp);
    parser.process(app);

    std::unique_ptr<XmlConfig> xml {new XmlConfig(DCOPPRINTER_XML_CONFIG)};
    xml->delDomain();

    PRINTER_SYSTEM = "direct";
    if (parser.isSet(systemOp)){
        QString system {parser.value(systemOp)};
        if (system == "direct" || system == "ipp")
            PRINTER_SYSTEM = system;
        xml->doWrite("main.type",PRINTER_SYSTEM);
    }

    PRINTER_DEVICE = "/dev/lp0";
    if (parser.isSet(deviceOp)){
        QString device_node = parser.value(deviceOp);
        QFile file(device_node);
        if (file.exists())
            PRINTER_DEVICE = device_node;
        xml->doWrite("main.type", PRINTER_DEVICE);
    }

    PRINTER_NAME = "";
    if ( parser.isSet(addressOp) ){
        QString address {parser.value(addressOp)};
        if (! address.isEmpty())
            PRINTER_NAME = address;
        xml->doWrite("main.device", PRINTER_NAME);
    }

    if ( PRINTER_SYSTEM == "ipp" && PRINTER_NAME.isEmpty() ){
        QString aux = xml->readString("main.device");
        if ( aux.isEmpty() ){
#ifndef _WINDOWS
            PRINTER_NAME = QString(cupsGetDefault());
#endif
            xml->doWrite("main.device", PRINTER_NAME);
            cout << "Default printer will be used (DEFAULT PRINTER = " << PRINTER_NAME.toStdString() << ")" << endl;
        }
    }

    xml->save();

    //We try to register the service to the dbus-daemon session buss
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if(! dbus.registerService(DBusPrinter::DBusService)){
        qDebug() << dbus.lastError().message();
        exit(EXIT_FAILURE);
    }

    new DBusPrinter(&app, app.applicationName());
    return app.exec();
}
