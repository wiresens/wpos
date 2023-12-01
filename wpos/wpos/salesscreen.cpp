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
#include "salesscreen.h"
#include "menusmanager.h"

#include "auth/authcore.h"
#include "barcore/filemanager.h"
#include "barcore/barcore.h"
#include "barcore/cashdevice.h"
#include "barcore/eventlogcore.h"
#include "barcore/filealterationwatcher.h"
#include "ticket/ticketcore.h"

#include "prodselect/productscreen.h"
#include "prodselect/productscreenstack.h"
#include "prodselect/product.h"

#include "customize/buttonsetwidget.h"
#include "customize/menubutton.h"
#include "customize/genericbutton.h"
#include "customize/genericbuttongroup.h"

#include "menusystem/menus/adminwidget.h"
#include "menusystem/menus/pincodewidget.h"
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

#include <wposwidget/menustack.h>
#include <wposwidget/menupage.h>
#include <wposwidget/numkeyboard.h>
#include <wposwidget/orderpickerview.h>
#include <wposwidget/orderview.h>
#include <wposwidget/ordercontentview.h>
#include <wposwidget/visualproduct.h>

#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>

#include <libbslxml/xmlconfig.h>

//#include <QApplication>
//#include <QSplashScreen>
//#include <QPushButton>
//#include <QCursor>
//#include <QLayout>
//#include <QTimer>
//#include <QMap>
//#include <QList>
//#include <QFile>

extern AuthCore *authCore;

constexpr int SECONDS_STOP {180};
constexpr int TIMER_RESOLUTION {100};
constexpr int TIMER_NO_EVENTS { SECONDS_STOP*( 1000 / TIMER_RESOLUTION )};
static const QString ROOT_ID{"1"};

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

SalesScreen::SalesScreen(MenuPage *parent,
    QSplashScreen& splashScreen,
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

    connect(this, &SalesScreen::splashRequested, &splashScreen, &QSplashScreen::showMessage);
    emit splashRequested(tr("Loading Modules ..."), Qt::AlignBottom | Qt::AlignRight , Qt::darkBlue);

    timer = new QTimer(this);
    connect( timer, &QTimer::timeout, this, &SalesScreen::checkPendingEvents);

    authCore = new  AuthCore(this, "AuthCore");
    authCore->loadUserById(ROOT_ID);

    //Initialising wPOS core modules
    auto color = Qt::darkBlue;
    auto alignment = Qt::AlignBottom | Qt::AlignRight;

    emit splashRequested( tr("Cash Device"), alignment , color);
    new CashDevice(this, "cashDevice");

    emit splashRequested( tr("EventLog Core"), alignment , color);
    eventLogCore = new EventLogCore(this, "eventLogCore");

    emit splashRequested( tr("Bar Core"), alignment , color);
    barCore = new BarCore(this, "barCore");
    barCore->reInitialise();

    emit splashRequested( tr("Ticket Core"), alignment , color);
    ticketCore = new TicketCore(this, "ticketCore");

    //Creating Menus Screens(products, Admins, Cash Movement ...)
    menuStack = new MenuStack(central_frame, "SALES_MENU_MANAGER");
    qobject_cast<QVBoxLayout*>(central_frame->layout())->insertWidget(0, menuStack);

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

    menuStack->setCurrentPage(PRODUCT_MENU);

    //Orders and Commands Widgets
    orderPicker = new OrderPickerView(order_frame, "OrderPickerView");
    auto orderPicker_layout = new QHBoxLayout(order_frame);
    orderPicker_layout->addWidget(orderPicker);
    orderPicker_layout->setContentsMargins(0,0,0,0);

    upperButtons = new ButtonSetWidget(productScreenStack, upper_buttons_frame, "Upper Buttons");
    auto upper_buttons_frame_layout = new QVBoxLayout(upper_buttons_frame);
    upper_buttons_frame_layout->addWidget(upperButtons);

    numkey = new NumKeyboard(numkey_frame);
    auto numkey_layout = new QHBoxLayout(numkey_frame);
    numkey_layout->setContentsMargins(2,2,2,2);
    numkey_layout->addWidget(numkey);

    shortcutLayout = new QHBoxLayout(shortcut_buttons_frame);

    connect(orderPicker->order(), &OrderView::dataChanged, barCore, &BarCore::dataChangedSlot);
    connect(productScreenStack, &ProductScreenStack::productDefinition, this, &SalesScreen::receiveProduct);
    connect(barCore, &BarCore::dataChanged, orderPicker->order(), &OrderView::changeData);
    connect(barCore, &BarCore::ticket, ticketCore, &TicketCore::receiveCoreDataSlot);

    connect(next_button, &QPushButton::pressed, productScreenStack, &ProductScreenStack::nextScreen);
    connect(previous_button, &QPushButton::pressed, productScreenStack, &ProductScreenStack::prevScreen);

    next_button->setIcon(QPixmap("controls:next_48.png"));
    previous_button->setIcon(QPixmap("controls:prev_48.png"));

    gsm->subscribeToGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, menuStack);
    emit splashRequested(tr("Keypads"),  Qt::AlignBottom | Qt::AlignRight , Qt::darkBlue);
    createShortcutButton();
    createOpenCashDeviceButton();
    numkey->setFocus();

    FileAlterationWatcher *watcher = new FileAlterationWatcher(this, "watcher");
    watcher->startWatching();
}

void SalesScreen::receiveProduct(XmlConfig *xml){

    if (!xml) return;

    int units = numkey->value();
    numkey->clear();

    if (units > 1){
        barCore->receiveProduct( xml, units);
        productScreenStack->setDefaultScreen();
    }
    else{
        barCore->receiveProduct(xml);
        productScreenStack->setDefaultScreen();
    }
}

void SalesScreen::receiveCash(){
    XmlConfig xml;

    if ( adminPanel->toggle_cash->isDown() ){
        xml.createElement("pay_type", "metalico");
        emit genericDataSignal(GDATASIGNAL::BARCORE_PROCESS_CORE, &xml);

        if ( adminPanel->toggle_out_screen->isDown() ){
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

/**
*       Default State  Properties:
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

    if ( ! adminPanel->toggle_derrama->isDown() )
        adminPanel->toggle_derrama->toggle();

    if ( ! adminPanel->toggle_anulation->isDown() )
        adminPanel->toggle_anulation->toggle();

    barCore->reInitialise();
    barCore->initExtras();
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

void SalesScreen::showEvent(QShowEvent *event){
    counter = 0;
    timer->start(TIMER_RESOLUTION);
    setDefaultState();
    if (authCore->userId() == "1"){
        XmlConfig xml ;
        xml.createElement("name", AUTH_MENU);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
        setEnabledLateralWidgets(false);
    }
    QWidget::showEvent(event);
}

void SalesScreen::hideEvent(QHideEvent *event){
    timer->stop();
    barCore->exitAndSaveReceipt();
    QWidget::hideEvent(event);
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

void SalesScreen::createProductDialog(){
    auto page = new MenuPage(menuStack, PRODUCT_MENU);
    page->setLayoutType(MenuPage::VBOX);
    page->layout()->setContentsMargins(0,0,0,0);
    productScreenStack = new ProductScreenStack(page, "ProductScreenStack");

    connect(productScreenStack, &ProductScreenStack::splashRequested, this, &SalesScreen::splashRequested);
    productScreenStack->initScreenStack(Files::configFilePath("bar"));
    disconnect(productScreenStack, &ProductScreenStack::splashRequested, this, &SalesScreen::splashRequested);

    page->addWidget(productScreenStack, "ProductScreenStack");
    menuStack->addPage(page, PRODUCT_MENU);
}

void SalesScreen::createAuthenticationDialog(){
    auto page = new MenuPage(menuStack, AUTH_MENU);
    page->setLayoutType(MenuPage::VBOX);
    PinCodeWidget *numCode = new PinCodeWidget(page, "NumCodeWidget");
    page->addWidget(numCode, "NumCodeWidget");
    menuStack->addPage(page, AUTH_MENU);
}

void SalesScreen::createFreepriceDialog(){
    auto page = new MenuPage(menuStack, FREE_PRICE_MENU);
    page->setLayoutType(MenuPage::VBOX);
    FreePriceWidget *freePrice = new FreePriceWidget(page, "FreePriceWidget");
    page->addWidget(freePrice, "FreePriceWidget");
    menuStack->addPage(page, FREE_PRICE_MENU);
}

void SalesScreen::createBalanceCollectionDialog(){
    auto page = new MenuPage(menuStack, CASH_MENU);
    page->setLayoutType(MenuPage::VBOX);
    CashWidget *cash_widget = new CashWidget(barCore, page, "CashWidget");
    page->addWidget(cash_widget, "CashWidget");
    menuStack->addPage(page, CASH_MENU);
}

void SalesScreen::createCashMovementDialog(){
    auto page = new MenuPage(menuStack, CHANGE_MONEY_MENU);
    page->setLayoutType(MenuPage::VBOX);
    ChangeMoneyWidget *changeMoney = new ChangeMoneyWidget(page, "ChangeMoneyWidget");
    page->addWidget(changeMoney,"ChangeMoneyWidget");
    menuStack->addPage(page, CHANGE_MONEY_MENU);
}

void SalesScreen::createExitActionDialog(){
    auto page = new MenuPage(menuStack, EXIT_MENU);
    page->setLayoutType(MenuPage::VBOX);
    ExitActionsWidget *exitActions = new ExitActionsWidget(page, "ExitActionsWidget");
    page->addWidget(exitActions,"ExitActionsWidget");
    menuStack->addPage(page, EXIT_MENU);
}

void SalesScreen::createReportDialog(){
    MenuPage *page = new MenuPage(menuStack, XZ_MENU);
    page->setLayoutType(MenuPage::VBOX);
    XZWidget *xz = new XZWidget(page,"XZWidget");
    page->addWidget(xz, "XZWidget");
    menuStack->addPage(page, XZ_MENU);
}

void SalesScreen::createTicketCancellationDialog(){
    auto page = new MenuPage(menuStack, KILL_TICKETS_MENU);
    page->setLayoutType(MenuPage::VBOX);
    KillTicketsWidget *killTickets = new KillTicketsWidget(page, "KillTicketsWidget");
    page->addWidget(killTickets, "KillTicketsWidget");
    menuStack->addPage(page, KILL_TICKETS_MENU);
}

void SalesScreen::createReceiptRecoverDialog(){
    auto page = new MenuPage(menuStack, RECEIPT_MENU);
    page->setLayoutType(MenuPage::VBOX);
    ReceiptRecoverWidget *receipt = new ReceiptRecoverWidget(page, "ReceiptRecoverWidget");
    page->addWidget(receipt,"ReceiptRecoverWidget");
    menuStack->addPage(page, RECEIPT_MENU);
}

void SalesScreen::createInvitationDialog(){
    auto page = new MenuPage(menuStack, INVITATION_MENU);
    page->setLayoutType(MenuPage::VBOX);
    auto invitation = new InvitationAllocatorWidget(page, "InvitationAllocatorWidget");
    page->addWidget(invitation, invitation->objectName());
    menuStack->addPage(page, INVITATION_MENU);
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

void SalesScreen::createGenericProductDialog(){
    auto page = new MenuPage(menuStack, GENERIC_PRODUCT_MENU);
    page->setLayoutType(MenuPage::VBOX);
    GenericProduct *g_product = new GenericProduct("varios", page, "GenericProductWidget");
    page->addWidget(g_product,"GenericProductWidget");
    menuStack->addPage(page, GENERIC_PRODUCT_MENU);
}

void SalesScreen::createAdminDialog(){
    auto page = new MenuPage(menuStack, ADMIN_MENU);
    page->setLayoutType(MenuPage::VBOX);
    adminPanel = new AdminWidget(productScreenStack, page, "AdminWidget");
    page->addWidget(adminPanel, adminPanel->objectName());
    menuStack->addPage(page, ADMIN_MENU);
}

void SalesScreen::createPaySelectionDialog(){
    auto page = new MenuPage(menuStack, PAY_MODE_MENU);
    page->setLayoutType(MenuPage::VBOX);
    PayModes *pay_mode = new PayModes( adminPanel->toggle_cash, barCore, page, "PayModeWidget");
    page->addWidget(pay_mode, pay_mode->objectName());
    menuStack->addPage(page, PAY_MODE_MENU);
}

void SalesScreen::createShortcutButton(){
    if (!shortcutGroup)
        shortcutGroup = new GenericButtonGroup(productScreenStack, shortcut_buttons_frame, "buttongroup");
    else
        shortcutGroup->clear();
    shortcutGroup->readConfigFrom("shortcut_buttons_frame", Files::configFilePath("buttons"));
}

void SalesScreen::createOpenCashDeviceButton(){
    if (!paymodeGroup)
        paymodeGroup = new GenericButtonGroup(productScreenStack, openbox_and_payment_mode_buttons_frame,"buttongroup");
    else
        paymodeGroup->clear();
    paymodeGroup->readConfigFrom("openbox_and_payment_mode_buttons_frame", Files::configFilePath("buttons"));
}
