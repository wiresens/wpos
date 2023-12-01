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

#include <iostream>
using namespace std;

static const QString& VERSION {"2.0rc1"};
static const QString& SPLASH_PIXMAP {"/usr/share/ntpv_backoffice/apps/ntpv_backoffice_splash.png"};

QSplashScreen *splash{};
int main(int argc, char *argv[])
{
    QApplication app{argc, argv};
    QDir::setSearchPaths("controls32", QStringList(Files::Controls32Dir));
    QDir::setSearchPaths("controls48", QStringList(Files::Controls48Dir));
    QDir::setSearchPaths("controls64", QStringList(Files::Controls64Dir));
    QDir::setSearchPaths("hands", QStringList(Files::HandsDir));
    QDir::setSearchPaths("products", QStringList(Files::ProductsDir));

    app.setApplicationName(QFileInfo(QFile(argv[0]).fileName()).baseName());
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
