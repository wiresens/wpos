// file      :  basic_application.cpp
// birth     :  2/19/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "basic_application.h"
#include <wposgui/windows/basic_main_window.h>
#include <QStyleFactory>

BasicApplication::BasicApplication(int &argc,
    char **argv):
    QApplication(argc,argv)
{
    // QApplication::setStyle(QStyleFactory::create("Fusion"));
}

BasicMainWindow& BasicApplication::createMainWindow(){
    loadSettings();
    BasicMainWindow& window = create();
    window.polulate();
    loadModules();
    loadPlugins();
    installLicenseManager();
    setupApplication(window);
    return window;
}

void BasicApplication::loadSettings(){}
void BasicApplication::setupApplication(BasicMainWindow& window){
    window.setupWindow();
}
