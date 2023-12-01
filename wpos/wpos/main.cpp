/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : mar jun 17 11:39:02 CEST 2003
    copyright            : (C) 2003 by Carlos Manzanedo
    email                : support@bluesock.net
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
#include "mainscreen.h"

#include <xmlconfig.h>
#include <wposcore/config.h>

#include <QApplication>
#include <QtDBus/QtDBus>
#include <QScreen>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSplashScreen>
#include <QPixmap>
#include <QTranslator>
#include <QFont>
#include <QTextCodec>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include <iostream>
using namespace std;

static const QString config_dir = "/etc/ntpv/";

static const QString version{"2.0"};

extern FileManager *file_manager;

extern QString PATH;
extern QString CASHBOX_DEVICE;
extern QString CASHBOX_TYPE;

bool saveReportmanDeff(
    const QString& host,
    const QString& db_name,
    const QString& user,
    const QString& passwd);

int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    QDir::setSearchPaths("controls", QStringList(Files::ControlsDir));
    QDir::setSearchPaths("payments", QStringList(Files::PaymentsDir));
    QDir::setSearchPaths("products", QStringList(Files::ProductsDir));

    app.setApplicationName(QFileInfo(QFile(argv[0]).fileName()).baseName());
    app.setApplicationVersion(version);

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    QCommandLineOption hostOp({ {"o","host"}, QObject::tr("Machine to which you want to connect the database."),"<host>"});
    QCommandLineOption databaseOp({ {"d","database"}, QObject::tr("Database name."),"<database>"});
    QCommandLineOption userOp({ {"u","user"}, QObject::tr("User for the connection to the database."),"<user>"});
    QCommandLineOption passwdOp({ {"p","passwd"}, QObject::tr("User passwd."),"<password>"});
    QCommandLineOption cashboxOp({ {"c","cashbox"}, QObject::tr("Device to which the drawer is connected."),"<device>"});
    QCommandLineOption cashtypeOp({ {"t","cashtype"}, QObject::tr("Drawer types <serial,cash_drawer,p_samsung_350>:Cash drawer is the drawer\n\that is connected to a printer port or similar, the serial type are \n\the drawers that are connected to serial ports. By default the type is serial."),"<type>","serial"});

    parser.addOption(hostOp);
    parser.addOption(databaseOp);
    parser.addOption(userOp);
    parser.addOption(passwdOp);
    parser.addOption(cashboxOp);
    parser.addOption(cashtypeOp);
    parser.process(app);

    QString cashbox;
    if ( parser.isSet(cashboxOp) ){
        cashbox = parser.value(cashboxOp);
        if (!cashbox.isEmpty() && QFile::exists(cashbox) )
            CASHBOX_DEVICE = cashbox;
    }

    QString cashtype;
    if (parser.isSet(cashtypeOp)){
        QString aux;
        aux = parser.value(cashtypeOp);
        if (aux == "cash_drawer")
            CASHBOX_TYPE = aux;
        else if (aux == "p_samsung_350")
            CASHBOX_TYPE = aux;
        else if (aux == "serial")
            CASHBOX_TYPE = aux;

        cashtype = CASHBOX_TYPE;
    }

    QString database, host, user, passwd;
    if (parser.isSet(hostOp))
        host = parser.value(hostOp);

    if (parser.isSet(databaseOp))
        database = parser.value(databaseOp);

    if (parser.isSet(userOp))
        user = parser.value(userOp);

    if (parser.isSet(passwdOp))
        passwd = parser.value(passwdOp);

    {
        XmlConfig xml (Files::configFilePath("database"));
        if (!xml.wellFormed()){
            qDebug() << "Incorrect database configuration file : "+Files::configFilePath("database");
            exit (1);
        }

        if (!xml.setDomain("database")){
            cout << "The xml that defines the database is not correct" << endl;
            exit (1);
        }

        if ( !host.isEmpty() )      xml.doWrite("hostname", host);
        if ( !database.isEmpty() )  xml.doWrite("dbname",database);
        if ( !user.isEmpty() )      xml.doWrite("user",user);
        if ( !passwd.isEmpty() )    xml.doWrite("passwd",passwd);

        host =      xml.readString("hostname");
        database =  xml.readString("dbname");
        user =      xml.readString("user");
        passwd =    xml.readString("passwd");
        xml.save();
    }

    //device section
    XmlConfig xml (Files::configFilePath("devices"));
    if ( !xml.wellFormed() ){
        cout << "The xml that defines the devices is not correct" << endl;
        exit (2);
    }

    xml.delDomain();

    bool write_devices {false};
    if (cashtype.isEmpty()){
        cashtype = xml.readString("cashbox.type");
        if (cashtype.isEmpty()){
            xml.doWrite("cashbox.type","cash_drawer");
            CASHBOX_TYPE = "cash_drawer";
            write_devices = true;
        }
        else
            CASHBOX_TYPE = cashtype;
    }
    else{
        xml.doWrite("cashbox.type", cashtype);
        write_devices = true;
    }

    if (cashbox.isEmpty()){
        cashbox = xml.readString("cashbox.dev");
        if (cashbox.isEmpty()){
            xml.doWrite("cashbox.dev","/dev/lp0");
            CASHBOX_DEVICE = "/dev/lp0";
            write_devices = true;
        }
        else
            CASHBOX_DEVICE = cashbox;
    }
    else{
        xml.doWrite("cashbox.dev",cashbox);
        write_devices = true;
    }

    if (write_devices){
        cout << "saving device configuration" << endl;
        xml.save();
    }

    saveReportmanDeff(host, database, user, passwd);

    file_manager = new FileManager(nullptr, "file_manager");
    file_manager->addFile("advanced_order_description.xml");
    file_manager->addFile("order_description.xml");
    file_manager->addFile("bar.xml");
    file_manager->addFile("button_state.xml");
    file_manager->addFile("company_ticket_data.xml");
    file_manager->addFile("dcopprinter_config.xml");
    file_manager->addFile("dcopreceipt_config.xml");
    file_manager->addFile("invitations_description.xml");
    file_manager->addFile("ntpv_buttons.xml");
    file_manager->addFile("printerhtml.xml");
    file_manager->addFile("printertickettotal.xml");
    file_manager->addFile("printerz.xml");
    file_manager->addFile("invoice.xml");
    file_manager->addFile("lounges.xml");
    file_manager->addFile("devices_config.xml");
    file_manager->addFile("kitchen_printerhtml.xml");

    file_manager->loadFiles();

    // Check if the file to be show at splash screen exists
    // If true create a splash screen
    bool has_splash_screen {false};

    QSplashScreen *splash_screen{};   
    const QString SPLASH_SCREEN_PIXMAP{ "controls:splashscreen.png"};
    if ( QFile(SPLASH_SCREEN_PIXMAP).exists() ){
        splash_screen = new QSplashScreen(QPixmap(SPLASH_SCREEN_PIXMAP));
        splash_screen->show();
        has_splash_screen = true;
    }

    app.setFont(QFont("helvetica", 12));
    QTranslator translator;

    // set the location where your .qm files are in load() below as the last parameter instead of "."
    // for development, use "/" to use the english original as
    // .qm files are stored in the base project directory.

    //    translator.load( QString("ntpv.") + QTextCodec::, "." );
    app.installTranslator( &translator );

    //We try to register the service to the dbus-daemon session bus
    auto dbus = QDBusConnection::sessionBus();
    if(! dbus.registerService(MainScreen::DBusService)){
        qDebug() << "Unable to register D-Bus Service"
                 << "Some functionalities will be absent"
                 << dbus.lastError().message();
        exit(EXIT_FAILURE);
    }

    MainScreen wpos (*splash_screen, "MainScreen");
    wpos.setWindowTitle("wPOS");
    wpos.setAttribute(Qt::WA_QuitOnClose, true);
    wpos.show();

    // If we have a splash screen, now it's time to switch off
    if (has_splash_screen){
        splash_screen->finish(&wpos);
        delete(splash_screen);
    }

    return app.exec();
}

bool saveReportmanDeff(const QString& host,
                       const QString& db_name,
                       const QString& user,
                       const QString& passwd)
{

    QDir dir = QDir::home();
    if (!dir.cd(".borland") && !dir.mkdir(".borland") ){
        cout << "There is no borland directory in"
             << dir.path().toStdString()
             << " and could not be created"<< endl;
        return false;
    }

    QFile file (dir.path() + "/dbxconnections");
    if (file.exists() && !file.remove()){
        cout << "Cannot delete file dbxconnections from rerportman" << endl;
        return false;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        cout << "Unable to create rportman dbxconnections file" << endl;
        return false;
    }

    QTextStream stream(&file);
    stream << "[caja]\n";
    stream << "DriverName=ZeosLib\n";
    stream << "Database Protocol=postgresql\n";
    stream << "HostName="<< host.toLatin1() <<"\n";
    stream << "Database="<< db_name.toLatin1() <<"\n";
    stream << "User_Name="<< user.toLatin1() <<"\n";
    stream << "Password="<< passwd.toLatin1() <<"\n";
    stream << "Port=5432\n";
    stream << "Zeos Translsolation=None\n";
    stream << "Property1=\n";
    stream << "Property2=\n";
    stream << "Property3=\n";
    stream << "Property4=\n";
    stream << "Property5=\n";
    stream << "Property6=\n";
    stream << "Property7=\n";
    stream << "Property8=\n";
    stream << "Property9=\n";

    file.close();
    return true;
}
