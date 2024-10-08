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

#include "main.h"
#include "dbusprinter.h"
#include <libbslxml/xmlconfig.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtDBus/QtDBus>

#ifndef _WINDOWS
    extern "C"{
        #include <cups/cups.h>
    }
#endif

#include <iostream>

extern QString PRINTER_DEVICE;
extern QString PRINTER_SYSTEM;
extern QString PRINTER_NAME;

int main(int argc, char *argv[])
{
    static const QString version{"2.0.0"};

    QCoreApplication app(argc, argv);
    auto appPath = app.applicationDirPath();
    auto appName = QFileInfo(QFile(argv[0]).fileName()).baseName();

    QDir::setSearchPaths( "xmldocs",  QStringList( appPath ) );
    QDir::setSearchPaths( "dtddocs",  QStringList( appPath ) );
    app.setApplicationName(appName);

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

    XmlConfig xml (DBusPrinter::XML_CFG_FILE);
    xml.delDomain();

    PRINTER_SYSTEM = "direct";
    if (parser.isSet(systemOp)){
        QString system {parser.value(systemOp)};
        if (system == "direct" || system == "ipp")
            PRINTER_SYSTEM = system;
        xml.doWrite("main.type", PRINTER_SYSTEM);
    }

    PRINTER_DEVICE = "/dev/lp0";
    if (parser.isSet(deviceOp)){
        QString device_node = parser.value(deviceOp);
        QFile file(device_node);
        if (file.exists())
            PRINTER_DEVICE = device_node;
        xml.doWrite("main.type", PRINTER_DEVICE);
    }

    PRINTER_NAME = "";
    if ( parser.isSet(addressOp) ){
        QString address {parser.value(addressOp)};
        if (! address.isEmpty())
            PRINTER_NAME = address;
        xml.doWrite("main.device", PRINTER_NAME);
    }

    if ( PRINTER_SYSTEM == "ipp" && PRINTER_NAME.isEmpty() ){
        QString aux = xml.readString("main.device");
        if ( aux.isEmpty() ){
#ifndef _WINDOWS
            PRINTER_NAME = QString(cupsGetDefault());
#endif
            xml.doWrite("main.device", PRINTER_NAME);
            std::cout << "Default printer will be used (DEFAULT PRINTER = "
                 << PRINTER_NAME.toStdString() << ")\n";
        }
    }
    xml.save();

    //We try to register the service to the dbus-daemon session buss
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if(! dbus.registerService(DBusPrinter::DBusService)){
        qDebug() << dbus.lastError().message();
        exit(EXIT_FAILURE);
    }

    new DBusPrinter(&app, app.applicationName());
    return app.exec();
}
