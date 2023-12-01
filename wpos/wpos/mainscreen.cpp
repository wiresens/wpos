/***************************************************************************
                          mainscreen.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2022
    copyright             : (C) 2022 by Wiresens INC.
    email                 : gilles.bene@wiresens.com

@author Gilles Bene Pougoue

%LICENCE%
 ***************************************************************************/
#include "mainscreen.h"

#include <xmlconfig.h>
#include <wposcore/genericsignalmanager.h>
#include <wposwidget/menupage.h>
#include <xmlconfig.h>

#include "greeter.h"
#include "salesscreen.h"
#include "readconfigscreen.h"

#include <iostream>
using namespace std;

const QString MainScreen::DBusService = QString{"com.wiresens.wpos.wpos"};

const QString MainScreen::LOGIN_SCREEN{"LOGIN_SCREEN"};
const QString MainScreen::SALES_SCREEN {"SALES_SCREEN"};
const QString MainScreen::READ_CONFIG_SCREEN {"READ_CONFIG_SCREEN"};

MainScreen::MainScreen(
    QSplashScreen& splash,
    const QString& name):
    MenuStack{nullptr, name}  //The MenuStack has no parent
{
    setObjectName(name);
    setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding) );
    setWindowIcon(QIcon("icons/wiresens.png"));
//    setWindowFlag(Qt::FramelessWindowHint);

    /** We've modified LoginScreen, SalesScreen and ReadConfigScreen so that it's parent is a MenuPage.
     *  It's constructor now automatically calls parent->addWidget(this, name) so that we do no longer call
     * page->addWidget(enter_the_matrix, "ENTER_THE_MATRIX_WIDGET") explicitely. We will bring the change later to
     * We will change MenuPage constructor so that it also take the Layout
    **/

    auto gsm = GenericSignalManager::instance();
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, this);

    // Create and add to the stack the Login Widget
    auto page = new MenuPage(this, LOGIN_SCREEN);
    page->setLayoutType(MenuPage::VBOX);
    loginScreen = new Greeter(page, LOGIN_SCREEN);
    addPage(page, LOGIN_SCREEN);

    // Create and add to the stack the Sales Widget
    page = new MenuPage(this, SALES_SCREEN);
    page->setLayoutType(MenuPage::VBOX);
    salesScreen = new SalesScreen(page, splash, SALES_SCREEN);
    addPage(page, SALES_SCREEN);

    // Create and add to the stack the ReadConfig Widget
    page = new MenuPage(this, READ_CONFIG_SCREEN);
    page->setLayoutType(MenuPage::VBOX);
    configScreen = new ReadConfigScreen(page, READ_CONFIG_SCREEN);
    addPage(page, READ_CONFIG_SCREEN);

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
