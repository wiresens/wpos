/***************************************************************************
                          bsllinuxbar.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#include "mainscreen.h"

#include <xmlconfig.h>
#include <wposcore/genericsignalmanager.h>
#include <wposwidget/menupage.h>
#include <xmlconfig.h>

#include "loginscreen.h"
#include "salesscreen.h"
#include "readconfigscreen.h"

#include <iostream>
using namespace std;

/************************************************************************************************************
 *
 *
 ************************************************************************************************************/

const QString MainScreen::DBusService = QString{"com.wiresens.wpos.wpos"};

const QString MainScreen::LOGIN_SCREEN{"LOGIN_SCREEN"};
const QString MainScreen::SALES_SCREEN {"SALES_SCREEN"};
const QString MainScreen::READ_CONFIG_SCREEN {"READ_CONFIG_SCREEN"};


MainScreen::MainScreen(QSplashScreen& splash_screen, const QString& name):
    MenuStack(nullptr)
{
    setObjectName(name);
    setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding) );
    setWindowIcon(QIcon("icons/wiresens.png"));
//    setWindowFlag(Qt::FramelessWindowHint);
    // Create and add to the stack the "Enter the Matrix" Widget
    /** We've modified BslEnterTheMatrix so that it's parent is a BslMenuPage. It's constructor
     * now automatically calls parent->addWidget(this, name) in it's constructor so that we do no longer call
     * page->addWidget(enter_the_matrix, "ENTER_THE_MATRIX_WIDGET"). We will bring the change later to
     * ScreensPresentation and ReadConfigWidget. We will change BslMenuPage constructor so that it also take the Layout
    **/

    MenuPage* page = new MenuPage(this, LOGIN_SCREEN);
    page->setLayoutType(MenuPage::VBOX);
    login_screen = new LoginScreen(page, LOGIN_SCREEN);
    addPage(page, LOGIN_SCREEN);
//    addPage(login_screen, LOGIN_SCREEN);

    // Create and add to the stack the "Application" Widget
    page = new MenuPage(this, SALES_SCREEN);
    page->setLayoutType(MenuPage::VBOX);
    sales_screen = new SalesScreen(page, splash_screen, SALES_SCREEN);
    page->addWidget(sales_screen, SALES_SCREEN);
    addPage(page, SALES_SCREEN);
//    addPage(sales_screen, SALES_SCREEN);

    // Create and add to the stack the "ReadConfig" Widget
    page = new MenuPage( this, READ_CONFIG_SCREEN);
    page->setLayoutType(MenuPage::VBOX);
    config_screen = new ReadConfigScreen(page, READ_CONFIG_SCREEN);
    page->addWidget(config_screen, READ_CONFIG_SCREEN);
    addPage(page, READ_CONFIG_SCREEN);
//    addPage(config_screen, READ_CONFIG_SCREEN);

    auto gsm = GenericSignalManager::instance();
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, this);
    setCurrentPage(LOGIN_SCREEN);
}

void MainScreen::genericDataSignalSlot(const QString& signal, XmlConfig *xml){
    if (signal == GDATASIGNAL::MAINSTACK_SETPAGE){
        xml->pushDomain();
        xml->delDomain();
        auto aux = xml->readString("name");
        setCurrentPage(aux);
        xml->popDomain();
    }
}

void MainScreen::genericSignalSlot(const QString& signal){
    if (signal == GSIGNAL::ENABLE_MAINSTACK ){
        setEnabled(true);
    }
    else if (signal == GSIGNAL::DISABLE_MAINSTACK ){
        setEnabled(false);
    }
}
