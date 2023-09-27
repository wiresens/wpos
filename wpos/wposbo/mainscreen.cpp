/***************************************************************************
                          linuxbarbackoffice.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include <wposcore/config.h>
#include <wposwidget/global.h>

#include "mainscreen.h"

#include "menusystem/menus/mainscreenwidget.h"

#include <libbslxml/xmlconfig.h>
#include <wposwidget/menustack.h>
#include <wposwidget/menupage.h>

#include <QLabel>
#include <QLayout>
#include <QApplication>
#include <QCursor>
#include <QFile>
#include <QStyleFactory>
#include <QStyle>
#include <QFont>

//static const int  DEFAULT_FONT_SIZE {11};
#include <iostream>
using namespace std;

MainScreen::MainScreen(QWidget *parent) :
    QWidget(parent)
{
    setupApplication();
    buildMenuStack();
    buildMainScreen();
}

void MainScreen::setupApplication(){
//    setGeometry(0,0,1024,768);
    auto configFile = Files::configFilePath("buttons");
    if (QFile::exists(configFile)){
        XmlConfig xml (configFile);
        if ( xml.isValid() && xml.setDomain("initscreen")){
            if (xml.setDomain("font")){
                auto family = xml.readString("family");
                auto size = xml.readString("size");
                xml.releaseDomain("font");
                if ( !family.isEmpty() ){
                    bool ok = false;
                    auto font_size = size.toInt(&ok);
                    QFont font(family, Sizes::FONT_SIZE_11);
                    if (ok) font = QFont(family,font_size);
                    QApplication::setFont(font);
                }
            }
            auto style = xml.readString("style");
            if ( !style.isEmpty() ){
                auto st = QStyleFactory::create(style);
                if (st)  QApplication::setStyle(st);
            }
        }
    }
}

void MainScreen::buildMenuStack(){
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    main_layout = new QVBoxLayout(this);
    menus_stack = new MenuStack(this);
    menus_stack->setSizePolicy(sizePolicy());
    menus_stack->setContentsMargins(0,0,0,0);
    main_layout->addWidget(menus_stack);

    // Sets default screen when application inits
    menus_stack->setCurrentPage(Menus::MAIN_MENU);
}

void MainScreen::buildMainScreen(){
    // Add it to a page of menus_stack
    auto *page = new MenuPage(menus_stack);
    page->setLayoutType(MenuPage::VBOX);

    // Create authentification widget
    main_screen = new MainScreenWidget(page,"MAIN_SCREEN_WIDGET");
    page->addWidget(main_screen,"MAIN_SCREEN_WIDGET");
    menus_stack->addPage(page, Menus::MAIN_MENU);
}
