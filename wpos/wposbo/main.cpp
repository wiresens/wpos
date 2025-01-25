// file      :  main.cpp
// birth     :  9/7/2003
// copyright :  Copyright (c) 2003 by BSTeam
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  BSTeam, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "mainscreen.h"
#include <wposcore/config.h>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSplashScreen>
#include <QString>

static const QString& APP_VERSION { "2.0-rc1" };
static const QString& SPLASH_PIXMAP { "controls:bo_splashscreen.png" };

static const QString CFG_XML_DIR { "etc/wpos/wpos/" };
static const QString CFG_DTD_DIR { "etc/wpos/wpos/dtds/" };
static const QString PIXMAP_DIR { "share/wpos/" };
QSplashScreen* splash {};
int main(int argc, char* argv[])
{
    QApplication app { argc, argv };
    auto appPath = app.applicationDirPath();
    QDir::setSearchPaths("xmldocs", QStringList({ appPath, appPath + "/etc/wpos/wposbo/", appPath + "/etc/wpos/wpos/" }));
    QDir::setSearchPaths("dtddocs", QStringList({ appPath, appPath + "/etc/wpos/wposbo/dtds/", appPath + "/etc/wpos/wpos/dtds/" }));
    QDir::setSearchPaths("pixmaps", QStringList(appPath));

    QDir tmp_dir;
    auto path = QDir::tempPath() + "/wposbo/";  // Dont remove the last /
    tmp_dir.mkpath(path);
    QDir::setSearchPaths("tmps", QStringList(path));

    QDir::setSearchPaths("controls", QStringList(appPath + "/" + cfg::CONTROLS_DIR));
    QDir::setSearchPaths("controls32", QStringList(appPath + "/" + cfg::CONTROLS_32_DIR));
    QDir::setSearchPaths("controls48", QStringList(appPath + "/" + cfg::CONTROLS_48_DIR));
    QDir::setSearchPaths("controls64", QStringList(appPath + "/" + cfg::CONTROLS_64_DIR));
    QDir::setSearchPaths("hands", QStringList(appPath + "/" + cfg::HANDS_DIR));
    QDir::setSearchPaths("products", QStringList(appPath + "/" + cfg::PRODUCT_DIR));

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
    if (QFile::exists(SPLASH_PIXMAP)) {
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
    if (has_splash) {
        splash->finish(&wposbo);
        delete (splash);
    }
    return app.exec();
}
