/***************************************************************************
                          pantallapresentacion.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "mainscreen.h"

#include "barcore/filemanager.h"

#include <xmlconfig.h>
#include <wposwidget/menustack.h>
#include <wposwidget/menupage.h>
#include <wposwidget/numkeyboardbox.h>
#include <wposwidget/orderpickerview.h>
#include <wposwidget/orderview.h>
#include <wposwidget/ordercontentview.h>
#include <wposwidget/visualproduct.h>
#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>

#include "salesscreen.h"
#include "menusmanager.h"
#include "prodselect/productscreen.h"
#include "prodselect/productscreenstack.h"
#include "prodselect/product.h"
#include "customize/buttonsetwidget.h"
#include "customize/menubutton.h"
#include "customize/genericbutton.h"
#include "customize/genericbuttongroup.h"
#include "auth/authcore.h"
#include "barcore/barcore.h"
#include "barcore/cashdevice.h"
#include "barcore/eventlogcore.h"
#include "barcore/filealterationwatcher.h"
#include "ticket/ticketcore.h"
#include "menusystem/menus/adminwidget.h"
#include "menusystem/menus/numcodewidget.h"
#include "menusystem/menus/freepricewidget.h"
#include "menusystem/menus/changemoneywidget.h"
#include "menusystem/menus/cashwidget.h"
#include "menusystem/menus/exitactionswidget.h"
#include "menusystem/menus/genericproduct.h"
#include "menusystem/menus/xzwidget.h"
#include "menusystem/menus/freepricewidget.h"
#include "menusystem/menus/killticketswidget.h"
#include "menusystem/menus/receiptrecoverwidget.h"
#include "menusystem/menus/invitationallocatorwidget.h"
#include "menusystem/menus/paymodeswidget.h"

#include <QSplashScreen>
#include <QLayout>
#include <QList>
#include <QMap>
#include <QFile>
#include <QPushButton>

#include <QApplication>
#include <QCursor>
#include <QTimer>
#include <QPushButton>

extern "C"{
    #include <stdio.h>
}

#include <iostream>
using namespace std;

extern AuthCore *auth;

constexpr int SECONDS_STOP {180};
constexpr int TIMER_RESOLUTION {100};
constexpr int TIMER_NO_EVENTS { SECONDS_STOP*( 1000 / TIMER_RESOLUTION )};

const QString SalesScreen::PRODUCT_MENU {"PRODUCT_MENU"};
const QString SalesScreen::AUTH_MENU {"AUTH_MENU"};
const QString SalesScreen::FREE_PRICE_MENU {"FREE_PRICE_MENU"};
const QString SalesScreen::CASH_MENU {"CASH_MENU"};
const QString SalesScreen::XZ_MENU {"XZ_MENU"};
const QString SalesScreen::CHANGE_MONEY_MENU {"CHANGE_MONEY_MENU"};
const QString SalesScreen::EXIT_MENU {"EXIT_MENU"};
const QString SalesScreen::KILL_TICKETS_MENU {"KILL_TICKETS_MENU"};
const QString SalesScreen::RECEIPT_MENU {"RECEIPT_MENU"};
const QString SalesScreen::INVITATION_MENU {"INVITATION_MENU"};
const QString SalesScreen::ADMIN_MENU {"ADMIN_MENU"};
const QString SalesScreen::GENERIC_PRODUCT_MENU {"GENERIC_PRODUCT_MENU"};
const QString SalesScreen::PAY_MODE_MENU {"PAY_MODE_MENU"};

SalesScreen::SalesScreen(
    MenuPage *parent,
    QSplashScreen& splash_screen,
    const QString& name ):
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    parent->addWidget(this, objectName());
    setSizePolicy(parent->sizePolicy());

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::XCORE_SET_DEFAULT_PRODUCT_OFFER, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::BARCORE_PROCESS_CORE, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::SETCORE_MODE, this);
    gsm->publishGenericSignal(GSIGNAL::AUTH_ENABLE_FINGERPRINT, this);
    gsm->publishGenericSignal(GSIGNAL::AUTH_DISABLE_FINGERPRINT, this);
    gsm->subscribeToGenericSignal(GSIGNAL::LOAD_BUTTONS, this);
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, this);

    connect(this, &SalesScreen::splashRequested, &splash_screen, &QSplashScreen::showMessage);
    emit splashRequested(tr("Loading Modules ..."), Qt::AlignBottom | Qt::AlignRight , Qt::darkBlue);

    timer = new QTimer(this);
    connect( timer, &QTimer::timeout, this, &SalesScreen::checkPendingEvents);

    auth = new  AuthCore(this, "Auth");
    auth->setUserId("1");

    //Initialising nTPV core modules
    auto color = Qt::darkBlue;
    auto alignment = Qt::AlignBottom | Qt::AlignRight;

    emit splashRequested( tr("System Core"), alignment , color);
    new CashDevice(this, "CashBox");

    emit splashRequested( tr("EventLog Core"), alignment , color);
    event_log_core = new EventLogCore(this, "event_log_core");

    emit splashRequested( tr("Bar Core"), alignment , color);
    bar_core = new BarCore(this, "bar_core");
    bar_core->initCore();

    emit splashRequested( tr("Ticket Core"), alignment , color);
    ticket_core = new TicketCore(this, "ticket_core");

    //Creating Menus Screens(products, Admins, Cash Movement ...)
    menu_stack = new MenuStack(central_frame, "SALES_MENU_MANAGER");
    qobject_cast<QVBoxLayout*>(central_frame->layout())->insertWidget(0, menu_stack);

    //Filling the menu stack
    emit splashRequested(tr("Product Screens"), alignment , color);
    createProductDialog();

    emit splashRequested(tr("Authentication"), alignment , color);
    createAuthenticationDialog();

    emit splashRequested(tr("Free Price"), alignment , color);
    createFreepriceDialog();
    createBalanceCollectionDialog();
    createCashMovementDialog();
    createExitActionDialog();
    createReportDialog();

    emit splashRequested(tr("Tickets Cancellation"), alignment , color);
    createTicketCancellationDialog();

    emit splashRequested(tr("Orders Control"), alignment , color);
    createReceiptRecoverDialog();

    emit splashRequested(tr("Invitations and Offers"), alignment , color);
    createInvitationDialog();
    createPayByCashButton();
    createGenericProductDialog();

    emit splashRequested(tr("System Setting"), alignment , color);
    createAdminDialog();

    emit splashRequested(tr("Payment Modes"), alignment , color);
    createPaySelectionDialog();

    menu_stack->setCurrentPage(PRODUCT_MENU);

    //Orders and Commands Widgets
    orderPicker = new OrderPickerView(order_frame, "OrderPickerView");
    auto orderPicker_layout = new QHBoxLayout(order_frame);
    orderPicker_layout->addWidget(orderPicker);
    orderPicker_layout->setContentsMargins(0,0,0,0);

    upper_buttons = new ButtonSetWidget(productScreenStack, upper_buttons_frame, "Upper Buttons");
    auto upper_buttons_frame_layout = new QVBoxLayout(upper_buttons_frame);
    upper_buttons_frame_layout->addWidget(upper_buttons);

    numkey = new NumKeyboardBox(numkey_frame);
    auto numkey_layout = new QHBoxLayout(numkey_frame);
    numkey_layout->setContentsMargins(2,2,2,2);
    numkey_layout->addWidget(numkey);

    shortcut_layout = new QHBoxLayout(shortcut_buttons_frame);

    connect(orderPicker->order(), SIGNAL(dataChanged(XmlConfig*)), bar_core, SLOT(dataChangedSlot(XmlConfig*)));
    connect(productScreenStack, SIGNAL(productDefinition(XmlConfig*)), this, SLOT(receiveProduct(XmlConfig*)));
    connect(bar_core, SIGNAL(dataChanged(XmlConfig*)), orderPicker->order(), SLOT(changeData(XmlConfig*)));
    connect(bar_core, SIGNAL(ticket(XmlConfig*)), ticket_core, SLOT(receiveCoreDataSlot(XmlConfig*)));

    connect(next_button,SIGNAL(pressed()), productScreenStack, SLOT(nextScreen()));
    connect(previous_button, SIGNAL(pressed()), productScreenStack, SLOT(prevScreen()));

    next_button->setIcon(QPixmap("controls:next_48.png"));
    previous_button->setIcon(QPixmap("controls:prev_48.png"));

    gsm->subscribeToGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, menu_stack);
    emit splashRequested(tr("Keypads"),  Qt::AlignBottom | Qt::AlignRight , Qt::darkBlue);
    createShortcutButton();
    createOpenCashDeviceButton();
    numkey->setFocus();

    FileAlterationWatcher *watcher = new FileAlterationWatcher(this, "watcher");
    watcher->startWatching();
}

void SalesScreen::createShortcutButton(){
    if (!shortcut_group)
        shortcut_group = new GenericButtonGroup(productScreenStack, shortcut_buttons_frame, "buttongroup");
    else
        shortcut_group->clear();
    shortcut_group->readConfigFrom("shortcut_buttons_frame", Files::configFilePath("buttons"));
}

void SalesScreen::createOpenCashDeviceButton(){
    if (!paymode_group)
        paymode_group = new GenericButtonGroup(productScreenStack, openbox_and_payment_mode_buttons_frame,"buttongroup");
    else
        paymode_group->clear();
    paymode_group->readConfigFrom("openbox_and_payment_mode_buttons_frame", Files::configFilePath("buttons"));
}

void SalesScreen::createPayByCashButton(){
//    GenericButton *collect_cash_button =  new GenericButton("Collect Cash", payment_frame, "cobrar_metalico");
    auto cash_collection_button =  new GenericButton("", payment_frame, "cobrar_metalico");
    cash_collection_button->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    cash_collection_button->setFixedHeight(95);
    cash_collection_button->setIcon(QPixmap("payments:1000xaf.jpeg"));
    cash_collection_button->setIconSize(QSize(200,95));
    cash_collection_layout->addWidget(cash_collection_button);
    connect(cash_collection_button, &GenericButton::clicked, this, &SalesScreen::receiveCash);
}

void SalesScreen::createProductDialog(){
    auto page = new MenuPage(menu_stack, PRODUCT_MENU);
    page->setLayoutType(MenuPage::VBOX);
    page->layout()->setContentsMargins(0,0,0,0);
    productScreenStack = new ProductScreenStack(page, "ProductScreenStack");

    connect(productScreenStack, &ProductScreenStack::splashRequested, this, &SalesScreen::splashRequested);
        productScreenStack->initScreenStack(Files::configFilePath("bar"));
    disconnect(productScreenStack, &ProductScreenStack::splashRequested, this, &SalesScreen::splashRequested);

    page->addWidget(productScreenStack, "ProductScreenStack");
    menu_stack->addPage(page, PRODUCT_MENU);
}

void SalesScreen::createAuthenticationDialog(){
    auto page = new MenuPage(menu_stack, AUTH_MENU);
    page->setLayoutType(MenuPage::VBOX);
    NumCodeWidget *numCode = new NumCodeWidget(page, "NumCodeWidget");
    page->addWidget(numCode, "NumCodeWidget");
    menu_stack->addPage(page, AUTH_MENU);
}

void SalesScreen::createBalanceCollectionDialog(){
    auto page = new MenuPage(menu_stack, CASH_MENU);
    page->setLayoutType(MenuPage::VBOX);
    CashWidget *cash_widget = new CashWidget(bar_core, page, "CashWidget");
    page->addWidget(cash_widget, "CashWidget");
    menu_stack->addPage(page, CASH_MENU);
}

void SalesScreen::createGenericProductDialog(){
    auto page = new MenuPage(menu_stack, GENERIC_PRODUCT_MENU);
    page->setLayoutType(MenuPage::VBOX);
    GenericProduct *g_product = new GenericProduct("varios", page, "GenericProductWidget");
    page->addWidget(g_product,"GenericProductWidget");
    menu_stack->addPage(page, GENERIC_PRODUCT_MENU);
}

void SalesScreen::createCashMovementDialog(){
    auto page = new MenuPage(menu_stack, CHANGE_MONEY_MENU);
    page->setLayoutType(MenuPage::VBOX);
    ChangeMoneyWidget *changeMoney = new ChangeMoneyWidget(page, "ChangeMoneyWidget");
    page->addWidget(changeMoney,"ChangeMoneyWidget");
    menu_stack->addPage(page, CHANGE_MONEY_MENU);
}

void SalesScreen::createExitActionDialog(){
    auto page = new MenuPage(menu_stack, EXIT_MENU);
    page->setLayoutType(MenuPage::VBOX);
    ExitActionsWidget *exitActions = new ExitActionsWidget(page, "ExitActionsWidget");
    page->addWidget(exitActions,"ExitActionsWidget");
    menu_stack->addPage(page, EXIT_MENU);
}

void SalesScreen::createReportDialog(){
    MenuPage *page = new MenuPage(menu_stack, XZ_MENU);
    page->setLayoutType(MenuPage::VBOX);
    XZWidget *xz = new XZWidget(page,"XZWidget");
    page->addWidget(xz, "XZWidget");
    menu_stack->addPage(page, XZ_MENU);
}

void SalesScreen::createFreepriceDialog(){
    auto page = new MenuPage(menu_stack, FREE_PRICE_MENU);
    page->setLayoutType(MenuPage::VBOX);
    FreePriceWidget *freePrice = new FreePriceWidget(page, "FreePriceWidget");
    page->addWidget(freePrice, "FreePriceWidget");
    menu_stack->addPage(page, FREE_PRICE_MENU);
}

void SalesScreen::createTicketCancellationDialog(){
    auto page = new MenuPage(menu_stack, KILL_TICKETS_MENU);
    page->setLayoutType(MenuPage::VBOX);
    KillTicketsWidget *killTickets = new KillTicketsWidget(page, "KillTicketsWidget");
    page->addWidget(killTickets, "KillTicketsWidget");
    menu_stack->addPage(page, KILL_TICKETS_MENU);
}

void SalesScreen::createReceiptRecoverDialog(){
    auto page = new MenuPage(menu_stack, RECEIPT_MENU);
    page->setLayoutType(MenuPage::VBOX);
    ReceiptRecoverWidget *receipt = new ReceiptRecoverWidget(page, "ReceiptRecoverWidget");
    page->addWidget(receipt,"ReceiptRecoverWidget");
    menu_stack->addPage(page, RECEIPT_MENU);
}

void SalesScreen::createPaySelectionDialog(){
    auto page = new MenuPage(menu_stack, PAY_MODE_MENU);
    page->setLayoutType(MenuPage::VBOX);
    PayModes *pay_mode = new PayModes( admin_panel->toggle_cash, bar_core, page, "PayModeWidget");
    page->addWidget(pay_mode, pay_mode->objectName());
    menu_stack->addPage(page, PAY_MODE_MENU);
}

void SalesScreen::createInvitationDialog(){
    auto page = new MenuPage(menu_stack, INVITATION_MENU);
    page->setLayoutType(MenuPage::VBOX);
    auto invitation = new InvitationAllocatorWidget(page, "InvitationAllocatorWidget");
    page->addWidget(invitation, invitation->objectName());
    menu_stack->addPage(page, INVITATION_MENU);
}

void SalesScreen::createAdminDialog(){
    auto page = new MenuPage(menu_stack, ADMIN_MENU);
    page->setLayoutType(MenuPage::VBOX);
    admin_panel = new AdminWidget(productScreenStack, page, "AdminWidget");
    page->addWidget(admin_panel, admin_panel->objectName());
    menu_stack->addPage(page, ADMIN_MENU);
}

void SalesScreen::receiveProduct(XmlConfig *xml){

    if (!xml) return;

    int units = numkey->getNumber();
    numkey->clear();

    if (units > 1){
        bar_core->receiveProduct( xml, units);
        productScreenStack->setDefaultScreen();
    }
    else{
        bar_core->receiveProduct(xml);
        productScreenStack->setDefaultScreen();
    }
}

void SalesScreen::receiveCash(){
    XmlConfig xml;

    if ( admin_panel->toggle_cash->isDown() ){
        xml.createElement("pay_type", "metalico");
        emit genericDataSignal(GDATASIGNAL::BARCORE_PROCESS_CORE, &xml);

        if ( admin_panel->toggle_out_screen->isDown() ){
            XmlConfig xml;
            xml.createElement("name", MainScreen::LOGIN_SCREEN);
            emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
        }
    }
    else{
        xml.createElement("name", CASH_MENU);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
    }
}

void SalesScreen::showEvent(QShowEvent *event){
    counter = 0;
    timer->start(TIMER_RESOLUTION);
    setDefaultState();
    if (auth->getUserId() == "1"){
        XmlConfig xml ;
        xml.createElement("name", AUTH_MENU);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
        setEnabledLateralWidgets(false);
    }
    QWidget::showEvent(event);
}

void SalesScreen::hideEvent(QHideEvent *event){
    timer->stop();
    bar_core->exitAndSaveReceipt();
    QWidget::hideEvent(event);
}

void SalesScreen::checkPendingEvents(){

    if ( qApp->hasPendingEvents() )
        counter = 0;
    else{
        counter++;
        if ( counter > TIMER_NO_EVENTS ){
            XmlConfig xml;
            xml.createElement("name", MainScreen::LOGIN_SCREEN);
            emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
            counter = 0;
        }
    }
}

/**
*       the defaultState has the following properties:
*       - the order is clear ( no products are selected )
*       - the extracore should be empty of extra options, offers and prices
*       - all product are deactivated at the prodselect/ScreenStack
*       - the product screen stack must be at the main screen
*       - the default product screen must be at the product screen
*       - the product screen must be in normal mode.
*       - the buttons
*               toggle_derrama;
*               toggle_anulation;
*         must be dettogled if they are toggledOn
*       - the core must be working in normal mode
*/

void SalesScreen::setDefaultState(){

    if ( ! admin_panel->toggle_derrama->isDown() )
        admin_panel->toggle_derrama->toggle();

    if ( ! admin_panel->toggle_anulation->isDown() )
        admin_panel->toggle_anulation->toggle();

    bar_core->initCore();
    bar_core->initExtras();
    productScreenStack->productDefinitionSlot(0);
    productScreenStack->setDefaultScreen();
    numkey->clear();

    XmlConfig xml;
    xml.createElement("mode","normal");
    emit genericDataSignal(GDATASIGNAL::SETCORE_MODE, &xml);

    xml.deleteElement("mode");
    xml.createElement("name", PRODUCT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);

    setEnabledLateralWidgets(true);
}

void SalesScreen::setEnabledLateralWidgets(bool enabled){
    numkey_frame->setEnabled(enabled);
    order_frame->setEnabled(enabled);
    upper_buttons_frame->setEnabled(enabled);
    shortcut_buttons_frame->setEnabled(enabled);
    payment_frame->setEnabled(enabled);
    movement_buttons_frame->setEnabled(enabled);
}

void SalesScreen::setVisibleLateralWidgets(bool visible){
    command_frame->setVisible(visible);
//    order_frame->setVisible(visible);  //keep the order visible
    shortcut_buttons_frame->setVisible(visible);
    movement_buttons_frame->setVisible(visible);
}

void SalesScreen::genericSignalSlot(const QString& signal_name){
    if (signal_name == GSIGNAL::LOAD_BUTTONS){
        createShortcutButton();
        createOpenCashDeviceButton();
    }
}

void SalesScreen::genericDataSignalSlot(const QString& signal_name, XmlConfig *xml){
    if ( signal_name == GDATASIGNAL::MAINWIDGET_SETENABLED){
        xml->delDomain();
        auto enabled =  ( xml->readString("enabled") == "true");
        setEnabledLateralWidgets(enabled);
        setVisibleLateralWidgets(enabled);
    }
    else if ( signal_name == GDATASIGNAL::LATERALWIDGET_SET_VISIBLE){
        xml->delDomain();
        auto visible = ( xml->readString("visible") == "true" );
        setVisibleLateralWidgets( visible);
    }
}
