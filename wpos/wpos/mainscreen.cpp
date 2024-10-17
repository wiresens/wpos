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

#include <libbslxml/xmlconfig.h>
#include <wposcore/genericsignalmanager.h>
#include <wposgui/windows/menupage.h>
#include <libbslxml/xmlconfig.h>

#include "greeter.h"
#include "salesscreen.h"
#include "readconfigscreen.h"

using namespace std;

const QString MainScreen::DBusService = QString{"com.wiresens.wpos.wpos"};

const QString MainScreen::LOGIN_SCREEN{"LOGIN_SCREEN"};
const QString MainScreen::SALES_SCREEN {"SALES_SCREEN"};
const QString MainScreen::CONFIG_SCREEN {"READ_CONFIG_SCREEN"};

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
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, this);

    // Create and add to the stack the Login Widget
    auto page = new MenuPage(this, LOGIN_SCREEN);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    m_login_screen = new Greeter(page, LOGIN_SCREEN);
    addPage(page, LOGIN_SCREEN);

    // Create and add to the stack the Sales Widget
    page = new MenuPage(this, SALES_SCREEN);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    m_sales_screen = new SalesScreen(page, splash, SALES_SCREEN);
    addPage(page, SALES_SCREEN);

    // Create and add to the stack the ReadConfig Widget
    page = new MenuPage(this, CONFIG_SCREEN);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    m_config_screen = new ReadConfigScreen(page, CONFIG_SCREEN);
    addPage(page, CONFIG_SCREEN);

    setCurrentPage(LOGIN_SCREEN);
}

void MainScreen::genericDataSignalSlot(
    const QString& signal,
    XmlConfig *xml)
{
    if (signal == GDATASIGNAL::MAINSTACK_SET_PAGE){
        xml->pushDomain();
        xml->delDomain();
        auto page_name = xml->readString("name");
        setCurrentPage(page_name);
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
