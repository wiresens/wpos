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

// #include "main.h"
#include "barcore/filemanager.h"
#include "auth/authcore.h"
#include "mainscreen.h"

#include <libbslxml/xmlconfig.h>
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

// Global variables( extern in used in other .cpp files ()
AuthCore    *global_auth_core;
FileManager *global_file_manager;
QString     global_cashbox_device;
QString     global_cashbox_type;

void save_reportman_cfg(
    const QString& host,
    const QString& db_name,
    const QString& user,
    const QString& passwd,
    const QString& port);

static const QString APP_VERSION    {"2.0-rc1"};
static const QString WINDOW_TITLE   {"wPOS"};
// static const QString CFG_XML_DIR    {"etc/wpos/wpos/"};
// static const QString CFG_DTD_DIR    {"etc/wpos/wpos/dtds/"};
// static const QString PIXMAP_DIR     {"share/wpos/"};

int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    auto appPath = app.applicationDirPath();
    QDir::setSearchPaths( "controls", QStringList( appPath + "/" + cfg::CONTROLS_DIR ) );
    QDir::setSearchPaths( "controls32", QStringList( appPath + "/" + cfg::CONTROLS_32_DIR ) );
    QDir::setSearchPaths( "products", QStringList( appPath + "/" + cfg::PRODUCT_DIR ) );
    QDir::setSearchPaths( "payments", QStringList( appPath + "/" + cfg::PAYMENT_DIR) );
    QDir::setSearchPaths( "avatars",  QStringList( appPath + "/" + cfg::AVATAR_DIR) );

    QDir::setSearchPaths( "xmldocs",  QStringList( appPath ) );
    QDir::setSearchPaths( "dtddocs",  QStringList( appPath ) );
    QDir::setSearchPaths( "pixmaps",  QStringList( appPath ) );

    QDir tmp_dir;
    auto path = QDir::tempPath() + "/wpos/";
    tmp_dir.mkpath(path);
    QDir::setSearchPaths( "tmps",  QStringList( path ) );

    app.setApplicationName(QFileInfo(QFile(argv[0]).fileName()).baseName());
    app.setApplicationVersion(APP_VERSION);
    app.setWindowIcon(QIcon("avatars:wpos.png"));

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();

    QCommandLineOption hostOp({ {"o","host"}, app.tr("Server hosting the database."),"<host>"});
    QCommandLineOption databaseOp({ {"d","database"}, app.tr("Database name."),"<database>"});
    QCommandLineOption userOp({ {"u","user"}, app.tr("Database User."),"<user>"});
    QCommandLineOption passwdOp({ {"p","passwd"}, app.tr("User password."),"<password>"});
    QCommandLineOption portOp({ {"P","port"}, app.tr("Database port."),"<port>"});
    QCommandLineOption cashboxOp({ {"c","cashbox"}, app.tr("Device to which the drawer is connected."),"<device>"});
    QCommandLineOption cashtypeOp({ {"t","cashtype"}, app.tr("Drawer type <serial,cash_drawer,p_samsung_350>:Cash drawer is the drawer\n\that is connected to a printer port or similar, the serial type are \n\the drawers that are connected to serial ports. By default the type is serial."),"<type>","serial"});

    parser.addOptions(
        {
            hostOp,
            databaseOp,
            userOp,
            passwdOp,
            portOp,
            cashboxOp,
            cashtypeOp
        }
    );

    parser.process(app);

    QString cashbox;
    if ( parser.isSet(cashboxOp) ){
        cashbox = parser.value(cashboxOp);
        if (!cashbox.isEmpty() && QFile::exists(cashbox) )
            global_cashbox_device = cashbox;
    }

    QString cashtype;
    if (parser.isSet(cashtypeOp)){
        QString aux;
        aux = parser.value(cashtypeOp);
        if (aux == "cash_drawer")
            global_cashbox_type = aux;
        else if (aux == "p_samsung_350")
            global_cashbox_type = aux;
        else if (aux == "serial")
            global_cashbox_type = aux;

        cashtype = global_cashbox_type;
    }

    QString database, host, user, passwd, port;
    if (parser.isSet(hostOp))
        host = parser.value(hostOp);

    if (parser.isSet(databaseOp))
        database = parser.value(databaseOp);

    if (parser.isSet(userOp))
        user = parser.value(userOp);

    if (parser.isSet(passwdOp))
        passwd = parser.value(passwdOp);

    if (parser.isSet(portOp))
        port = parser.value(portOp);

    {
        // File containing db connection string.
        auto db_con_file = cfg::xmlFileByKey(cfg::XMLKey::Database);
        XmlConfig xml ( db_con_file );
        if (!xml.wellFormed() || !xml.setDomain("database")){
            qDebug() << "Fatal : Incorrect Database connection file "
                     << db_con_file;
            exit (1);  // Exit code for incorrect db conf file
        }

        // Cmd line options have precedence over conf file
        if ( !host.isEmpty() )      xml.doWrite("hostname", host);
        if ( !database.isEmpty() )  xml.doWrite("dbname",database);
        if ( !user.isEmpty() )      xml.doWrite("user",user);
        if ( !passwd.isEmpty() )    xml.doWrite("passwd",passwd);
        if ( !port.isEmpty() )      xml.doWrite("port", port);

        host     =  xml.readString("hostname");
        database =  xml.readString("dbname");
        user     =  xml.readString("user");
        passwd   =  xml.readString("passwd");
        port     =  xml.readString("port");
        xml.save();
    }

    //device section
    {
        auto device_con_file = cfg::xmlFileByKey(cfg::XMLKey::Devices);
        XmlConfig xml ( device_con_file );
        if ( !xml.wellFormed() ){
            qDebug() << "Fatal : Incorrect Devices conf file ";
            exit (2); // Exit code for incorrect device conf file
        }

        xml.delDomain();

        bool write_devices {false};
        if (cashtype.isEmpty()){
            cashtype = xml.readString("cashbox.type");
            if (cashtype.isEmpty()){
                xml.doWrite("cashbox.type","cash_drawer");
                global_cashbox_type = "cash_drawer";
                write_devices = true;
            }
            else
                global_cashbox_type = cashtype;
        }
        else{
            xml.doWrite("cashbox.type", cashtype);
            write_devices = true;
        }

        if (cashbox.isEmpty()){
            cashbox = xml.readString("cashbox.dev");
            if (cashbox.isEmpty()){
                xml.doWrite("cashbox.dev","/dev/lp0");
                global_cashbox_device = "/dev/lp0";
                write_devices = true;
            }
            else
                global_cashbox_device = cashbox;
        }
        else{
            xml.doWrite("cashbox.dev",cashbox);
            write_devices = true;
        }

        if (write_devices){
            qDebug() << "saving device configuration";
            xml.save();
        }
    }

    save_reportman_cfg(host, database, user, passwd, port);
    QDir::setCurrent(path);

    global_file_manager = new FileManager(nullptr, "FileManager");
    QStringList cfg_files{
        "advanced_order_description.xml",
        "order_description.xml",
        "bar.xml",
        "button_state.xml",
        "company_ticket_data.xml",
        "dcopprinter_config.xml",
        "dcopreceipt_config.xml",
        "invitations_description.xml",
        "ntpv_buttons.xml",
        "printerhtml.xml",
        "printertickettotal.xml",
        "printerz.xml",
        "invoice.xml",
        "lounges.xml",
        "devices_config.xml",
        "kitchen_printerhtml.xml"
    };

    global_file_manager->registerFiles(cfg_files);
    global_file_manager->fetchConfigFiles(cfg_files);

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
        // exit(EXIT_FAILURE);
    }

    MainScreen wpos (*splash_screen, "MainScreen");
    wpos.setWindowTitle(WINDOW_TITLE);
    wpos.setAttribute(Qt::WA_QuitOnClose, true);
    wpos.show();

    // If we have a splash screen, now it's time to switch off
    if (has_splash_screen){
        splash_screen->finish(&wpos);
        delete(splash_screen);
    }

    return app.exec();
}

void
save_reportman_cfg(
    const QString& host,
    const QString& db_name,
    const QString& user,
    const QString& passwd,
    const QString& port)
{
    QDir dir = QDir::current();
    if ( !dir.mkdir(".borland") && !dir.cd(".borland") ){
        qDebug() << "Warning : There is no borland directory in "
                 << dir.path()
                 << " and could not be created";
        return;
    }

    QFile file (dir.path() + "/dbxconnections");
    if (file.exists() && !file.remove()){
        qDebug() << "Warning : Cannot delete file dbxconnections from reportman";
        return;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Warning : Unable to create reportman dbxconnections file";
        return;
    }

    QTextStream stream(&file);
    stream << "[database]\n";
    stream << "DriverName=ZeosLib\n";
    stream << "Database Protocol=postgresql\n";
    stream << "HostName="<< host <<"\n";
    stream << "Database="<< db_name <<"\n";
    stream << "User_Name="<< user <<"\n";
    stream << "Password="<< passwd <<"\n";
    stream << "Port="<< port <<"\n";
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
}
