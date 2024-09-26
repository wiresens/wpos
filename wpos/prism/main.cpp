// file      :  main.cpp
// birth     :  2/19/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "views/prism_main_window.h"
#include "views/prism_application.h"

#include <wposgui/application/basic_application.h>
#include "views/stock_entry_form.h"

#include <QSplashScreen>
#include <QTranslator>
#include <QFile>
#include <QFileInfo>

static const QString version{"0.1"};

int main(int argc, char *argv[]){

    PrismApplication app(argc, argv);
    app.setApplicationName(QFileInfo(QFile(argv[0]).fileName()).baseName());
    app.setApplicationVersion(version);

    BasicMainWindow& basicWindow = app.createMainWindow();

    bool has_splash_screen = false;
    QSplashScreen *splash{};
    const QString SPLASH_SCREEN_PIXMAP{ "controls:splashscreen.png"};
    if ( QFile(SPLASH_SCREEN_PIXMAP).exists() ){
        splash = new QSplashScreen(QPixmap(SPLASH_SCREEN_PIXMAP));
        splash->show();
        has_splash_screen = true;
    }

    QTranslator translator;
    // set the location where your .qm files are in load() below as the last parameter instead of "."
    // for development, use "/" to use the english original as
    // .qm files are stored in the base project directory.

    // translator.load( QString("ntpv.") + QTextCodec::, "." );
    app.installTranslator( &translator );
    basicWindow.showMaximized();

    // If we have a splash screen, now it's time to switch off
    if (has_splash_screen){
        splash->finish(&basicWindow);
        delete(splash);
    }

    return app.exec();
}
