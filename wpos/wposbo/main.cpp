/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Sun Sep  7 15:21:43 CEST 2003
    copyright            : (C) 2003 by BSTeam
    email                : bsteam@bluesock.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "mainscreen.h"
#include <wposcore/config.h>

#include <QSplashScreen>
#include <QApplication>
#include <QString>
#include <QFile>
#include <QDir>
#include <QFileInfo>

static const QString& APP_VERSION {"2.0-rc1"};
static const QString& SPLASH_PIXMAP {"controls:bo_splashscreen.png"};

static const QString CFG_XML_DIR    {"etc/wpos/wpos/"};
static const QString CFG_DTD_DIR    {"etc/wpos/wpos/dtds/"};
static const QString PIXMAP_DIR     {"share/wpos/"};
QSplashScreen *splash{};
int main(int argc, char *argv[])
{
    QApplication app{argc, argv};
    auto appPath = app.applicationDirPath();
    QDir::setSearchPaths( "xmldocs",  QStringList( {appPath, appPath + "/etc/wpos/wpos/", appPath + "/etc/wpos/wposbo/"} ) );
    QDir::setSearchPaths( "dtddocs",  QStringList( {appPath, appPath + "/etc/wpos/wposbo/dtds/", appPath + "/etc/wpos/wpos/dtds/"} ) );
    QDir::setSearchPaths( "pixmaps",  QStringList( appPath ) );

    QDir tmp_dir;
    auto path = QDir::tempPath() + "/wpos/";
    tmp_dir.mkpath(path);
    QDir::setSearchPaths( "tmps",  QStringList( path ));

    QDir::setSearchPaths("controls",   QStringList( appPath + "/" + cfg::CONTROLS_DIR ) );
    QDir::setSearchPaths("controls32", QStringList( appPath + "/" + cfg::CONTROLS_32_DIR ));
    QDir::setSearchPaths("controls48", QStringList( appPath + "/" + cfg::CONTROLS_48_DIR ));
    QDir::setSearchPaths("controls64", QStringList( appPath + "/" + cfg::CONTROLS_64_DIR ));
    QDir::setSearchPaths("hands", QStringList( appPath + "/" + cfg::HANDS_DIR));
    QDir::setSearchPaths("products", QStringList( appPath + "/" + cfg::PRODUCT_DIR));

    app.setApplicationName(QFileInfo(QFile(argv[0]).fileName()).baseName());
    app.setApplicationVersion(APP_VERSION);
//    DCOPClient *client = a.dcopClient();

//    client->attach();
//    if (!client->isAttached()){
//           cerr << "Can't attach to dcopserver: is dcopserver running" << endl;
//           exit (1);
//    }

    // Check if exists the file to be show at splash screen
    // If true create a splash screen

    bool has_splash = false;
    if (QFile::exists(SPLASH_PIXMAP)){
           splash = new QSplashScreen(QPixmap(SPLASH_PIXMAP));
           splash->show();
           has_splash = true;
    }

//    QString app_name = client->registerAs(QString(program_name),false);
//    if (app_name.isNull()){
//           cerr << "Can't attach to dcopserver: check dcopserver is running" << endl;
//           exit (1);
//    }
//    cerr << "Application registered at DcopServer with name : " << app_name << endl;

    MainScreen wposbo;
    wposbo.adjustSize();
    wposbo.show();

    // If we have a splash screen, now it's time to switch off
    if (has_splash){
        splash->finish(&wposbo);
        delete(splash);
    }
    return app.exec();
}
