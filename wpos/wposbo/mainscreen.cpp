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

#include "menusystem/menus/mainscreenmenu.h"

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

#include <iostream>
using namespace std;

MainScreen::MainScreen(QWidget *parent):
    QWidget(parent)
{
    setupApplication();
    buildMenuStack();
    buildScreenMenu();
}

void MainScreen::setupApplication(){
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    auto configFile = Files::configFilePath("buttons");
    if (QFile::exists(configFile)){
        XmlConfig xml (configFile);
        if ( xml.wellFormed() && xml.setDomain("initscreen")){
            if (xml.setDomain("font")){
                auto family = xml.readString("family");
                auto size = xml.readString("size");
                xml.releaseDomain("font");
                if ( !family.isEmpty() ){
                    bool ok = false;
                    auto font_size = size.toInt(&ok);
                    QFont font(family, Sizes::FONT_SIZE_11);
                    if (ok) font = QFont(family, font_size);
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
    mainLayout = new QVBoxLayout(this);
    menuStack = new MenuStack(this);
    menuStack->setSizePolicy(sizePolicy());
    menuStack->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(menuStack);
    menuStack->setCurrentPage(Menus::MAIN_MENU); //start page
}

void MainScreen::buildScreenMenu(){
    // Add it to a page of menus_stack
    auto menuPage = new MenuPage(menuStack, Menus::MAIN_MENU);
    menuPage->setLayoutType(MenuPage::VBOX);

    // Create authentification widget
    screenMenu = new MainScreenMenu(menuPage, Menus::MAIN_SCREEN_MENU);
    menuStack->addPage(menuPage, menuPage->objectName());
}
