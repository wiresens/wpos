// file      :  salesscreen.cpp
// birth     :  06/19/2023
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_saleswidget.h"
#include "mainscreen.h"
#include "salesscreen.h"

#include "auth/authcore.h"
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

#include <wposgui/windows/menusmanager.h>
#include <wposgui/windows/menustack.h>
#include <wposgui/windows/menupage.h>
#include <wposgui/keyboard/numkeyboard.h>
#include <wposgui/order/orderpickerview.h>
#include <wposgui/order/orderview.h>
#include <wposgui/order/ordercontentview.h>
#include <wposgui/visualproduct.h>

#include <wposcore/genericsignalmanager.h>
#include <wposcore/idle_filter.h>
#include <wposcore/config.h>

#include <libbslxml/xmlconfig.h>

extern AuthCore *global_auth_core;

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

SalesScreen::SalesScreen(
    MenuPage *parent,
    QSplashScreen& splashScreen,
    const QString& name ):
    QWidget(parent),
    ui{ new Ui::SalesWidget}
{
    ui->setupUi(this);
    setObjectName(name);
    parent->addWidget(this, objectName());
    setSizePolicy(parent->sizePolicy());

    auto idle_filter = new IdleFilter(this, 5);
    qApp->installEventFilter( idle_filter );
    connect( idle_filter, &IdleFilter::appIdled,
        this, &SalesScreen::showGreeter, Qt::QueuedConnection);

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::XCORE_SET_DEFAULT_PRODUCT_OFFER, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::BARCORE_PROCESS_CORE, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::SETCORE_MODE, this);

    gsm->publishGenericSignal(GSIGNAL::AUTH_ENABLE_FINGERPRINT, this);
    gsm->publishGenericSignal(GSIGNAL::AUTH_DISABLE_FINGERPRINT, this);

    gsm->subscribeToGenericSignal(GSIGNAL::LOAD_BUTTONS, this);
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, this);

    connect(this, &SalesScreen::splashRequested, &splashScreen, &QSplashScreen::showMessage);
    emit splashRequested(tr("Loading Modules ..."), Qt::AlignBottom | Qt::AlignRight , Qt::darkBlue);

    global_auth_core = new  AuthCore(this, "AuthCore");
    global_auth_core->loadUserById(ROOT_ID);

    //Initialising wPOS core modules
    auto color = Qt::darkBlue;
    auto alignment = Qt::AlignBottom | Qt::AlignRight;

    emit splashRequested( tr("Cash Device"), alignment , color);
    new CashDevice(this, "CashDevice");

    emit splashRequested( tr("EventLog Core"), alignment , color);
    m_eventlog_core = new EventLogCore(this, "EventLogCore");

    emit splashRequested( tr("Bar Core"), alignment , color);
    m_bar_core = new BarCore(this, "BarCore");
    m_bar_core->reInitialise();

    emit splashRequested( tr("Ticket Core"), alignment , color);
    m_ticket_core = new TicketCore(this, "TicketCore");

    //Creating Menus Screens(products, Admins, Cash Movement ...)
    m_screens = new MenuStack(ui->central_frame, "SALES_MENU_MANAGER");
    qobject_cast<QVBoxLayout*>(ui->central_frame->layout())->insertWidget(0, m_screens);

    //Filling the menu stack
    emit splashRequested(tr("Product Screens"), alignment , color);
    m_product_screens = createProductDialog(m_screens);

    emit splashRequested(tr("Authentication"), alignment , color);
    createAuthenticationDialog(m_screens);

    emit splashRequested(tr("Free Price"), alignment , color);
    createFreepriceDialog(m_screens);
    createBalanceCollectionDialog(m_screens);
    createCashMovementDialog(m_screens);
    createExitActionDialog(m_screens);
    createReportDialog(m_screens);

    emit splashRequested(tr("Tickets Cancellation"), alignment , color);
    createTicketCancellationDialog(m_screens);

    emit splashRequested(tr("Orders Control"), alignment , color);
    createReceiptRecoverDialog(m_screens);

    emit splashRequested(tr("Invitations and Offers"), alignment , color);
    createInvitationDialog(m_screens);
    createPayByCashButton();
    createGenericProductDialog(m_screens);

    emit splashRequested(tr("System Setting"), alignment , color);
    createAdminDialog(m_screens);

    emit splashRequested(tr("Payment Modes"), alignment , color);
    createPaySelectionDialog(m_screens);

    m_screens->setCurrentPage(PRODUCT_MENU);

    //Orders and Commands Widgets
    m_order_picker_view = new OrderPickerView(ui->order_frame, "OrderPickerView");
    auto orderPicker_layout = new QHBoxLayout(ui->order_frame);
    orderPicker_layout->addWidget(m_order_picker_view);
    orderPicker_layout->setContentsMargins(0,0,0,0);

    m_upper_btns = new ButtonSetWidget(m_product_screens, ui->upper_buttons_frame, "Upper Buttons");
    auto upper_buttons_frame_layout = new QVBoxLayout(ui->upper_buttons_frame);
    upper_buttons_frame_layout->addWidget(m_upper_btns);

    m_num_keyboard = new NumKeyboard(ui->numkey_frame);
    auto numkey_layout = new QHBoxLayout(ui->numkey_frame);
    numkey_layout->setContentsMargins(2,2,2,2);
    numkey_layout->addWidget(m_num_keyboard);

    m_shortcut_layout = new QHBoxLayout(ui->shortcut_buttons_frame);

    connect(m_order_picker_view->order(), &OrderView::orderChanged, m_bar_core, &BarCore::addProductDefaultOption);
    connect(m_product_screens, &ProductScreenStack::productDefinition, this, &SalesScreen::receiveProduct);
    connect(m_bar_core, &BarCore::orderChanged, m_order_picker_view->order(), &OrderView::updateOrder);
    connect(m_bar_core, &BarCore::ticket, m_ticket_core, &TicketCore::receiveCoreData);

    connect(ui->next_button, &QPushButton::pressed, m_product_screens, &ProductScreenStack::nextScreen);
    connect(ui->previous_button, &QPushButton::pressed, m_product_screens, &ProductScreenStack::prevScreen);

    ui->next_button->setIcon(QPixmap("controls:next_48.png"));
    ui->previous_button->setIcon(QPixmap("controls:prev_48.png"));

    gsm->subscribeToGenericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, m_screens);
    emit splashRequested(tr("Keypads"),  Qt::AlignBottom | Qt::AlignRight , Qt::darkBlue);
    createShortcutButton();
    createOpenCashDeviceButton();
    m_num_keyboard->setFocus();

    FileAlterationWatcher *watcher = new FileAlterationWatcher(this, "FileAlterationWatcher");
    watcher->startWatching();
}

void SalesScreen::receiveProduct(XmlConfig *xml){

    if (!xml) return;

    int units = m_num_keyboard->value();
    m_num_keyboard->clear();

    if (units > 1){
        m_bar_core->receiveProduct( xml, units);
        m_product_screens->setDefaultScreen();
    }
    else{
        m_bar_core->receiveProduct(xml);
        m_product_screens->setDefaultScreen();
    }
}

void SalesScreen::receiveCash(){
    XmlConfig xml;

    if ( m_admin_wgt->toggle_cash->isDown() ){
        xml.createElement("pay_type", "metalico");
        emit genericDataSignal(GDATASIGNAL::BARCORE_PROCESS_CORE, &xml);

        if ( m_admin_wgt->toggle_out_screen->isDown() ){
            XmlConfig xml;
            xml.createElement("name", MainScreen::LOGIN_SCREEN);
            emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);
        }
    }
    else{
        xml.createElement("name", CASH_MENU);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);
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

    if ( ! m_admin_wgt->toggle_derrama->isDown() )
        m_admin_wgt->toggle_derrama->toggle();

    if ( ! m_admin_wgt->toggle_anulation->isDown() )
        m_admin_wgt->toggle_anulation->toggle();

    m_bar_core->reInitialise();
    m_bar_core->initExtras();
    m_product_screens->productDefinitionSlot(0);
    m_product_screens->setDefaultScreen();
    m_num_keyboard->clear();

    XmlConfig xml;
    xml.createElement("mode","normal");
    emit genericDataSignal(GDATASIGNAL::SETCORE_MODE, &xml);

    xml.deleteElement("mode");
    xml.createElement("name", PRODUCT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);

    enableLateralWidgets(true);
    showLateralWidgets( true);
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
        enableLateralWidgets(enabled);
        showLateralWidgets(enabled);
    }
    // else if ( signal_name == GDATASIGNAL::LATERALWIDGET_SET_VISIBLE){
    //     xml->delDomain();
    //     auto visible = ( xml->readString("visible") == "true" );
    //     showLateralWidgets( visible);
    // }
}

void SalesScreen::showGreeter(){
    XmlConfig xml;
    xml.createElement("name", MainScreen::LOGIN_SCREEN);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);
}

void SalesScreen::showEvent(QShowEvent *event){
    setDefaultState();
    if (global_auth_core->userId() == "1"){
        XmlConfig xml ;
        xml.createElement("name", AUTH_MENU);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);
        enableLateralWidgets(false);
    }
    QWidget::showEvent(event);
}

void SalesScreen::hideEvent(QHideEvent *event){
    m_bar_core->exitAndSaveReceipt();
    QWidget::hideEvent(event);
}

void SalesScreen::enableLateralWidgets(bool enabled){
    ui->numkey_frame->setEnabled(enabled);
    ui->order_frame->setEnabled(enabled);
    ui->upper_buttons_frame->setEnabled(enabled);
    ui->shortcut_buttons_frame->setEnabled(enabled);
    ui->payment_frame->setEnabled(enabled);
    ui->movement_buttons_frame->setEnabled(enabled);
}

void SalesScreen::showLateralWidgets(bool visible){
    ui->command_frame->setVisible(visible);
    // ui->order_frame->setVisible(visible);  //keep the order visible
    ui->shortcut_buttons_frame->setVisible(visible);
    ui->movement_buttons_frame->setVisible(visible);
}

ProductScreenStack* SalesScreen::createProductDialog(MenuStack* screens){
    auto page = new MenuPage(screens, PRODUCT_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    page->layout()->setContentsMargins(0,0,0,0);
    auto product_screens = new ProductScreenStack(page, "ProductScreenStack");

    connect(product_screens, &ProductScreenStack::splashRequested,
        this, &SalesScreen::splashRequested);
    product_screens->initScreenStack(cfg::xmlFileByKey(cfg::XMLKey::Bar));
    disconnect(product_screens, &ProductScreenStack::splashRequested,
        this, &SalesScreen::splashRequested);

    page->addWidget(product_screens, product_screens->objectName());
    screens->addPage(page, PRODUCT_MENU);
    return product_screens;
}

void SalesScreen::createAuthenticationDialog(MenuStack* screens){
    auto page = new MenuPage(screens, AUTH_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    auto numCode = new PinCodeWidget(page, "PinCodeWidget");
    page->addWidget(numCode, numCode->objectName());
    screens->addPage(page, AUTH_MENU);
}

void SalesScreen::createFreepriceDialog(MenuStack* screens){
    auto page = new MenuPage(screens, FREE_PRICE_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    auto freePrice = new FreePriceWidget(page, "FreePriceWidget");
    page->addWidget(freePrice, freePrice->objectName());
    screens->addPage(page, FREE_PRICE_MENU);
}

void SalesScreen::createBalanceCollectionDialog(MenuStack* screens){
    auto page = new MenuPage(screens, CASH_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    auto cash_widget = new CashWidget(m_bar_core, page, "CashWidget");
    page->addWidget(cash_widget, cash_widget->objectName());
    screens->addPage(page, CASH_MENU);
}

void SalesScreen::createCashMovementDialog(MenuStack* screens){
    auto page = new MenuPage(screens, CHANGE_MONEY_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    ChangeMoneyWidget *changeMoney = new ChangeMoneyWidget(page, "ChangeMoneyWidget");
    page->addWidget(changeMoney,"ChangeMoneyWidget");
    screens->addPage(page, CHANGE_MONEY_MENU);
}

void SalesScreen::createExitActionDialog(MenuStack* screens){
    auto page = new MenuPage(screens, EXIT_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    ExitActionsWidget *exitActions = new ExitActionsWidget(page, "ExitActionsWidget");
    page->addWidget(exitActions,"ExitActionsWidget");
    screens->addPage(page, EXIT_MENU);
}

void SalesScreen::createReportDialog(MenuStack* screens){
    MenuPage *page = new MenuPage(screens, XZ_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    XZWidget *xz = new XZWidget(page,"XZWidget");
    page->addWidget(xz, "XZWidget");
    screens->addPage(page, XZ_MENU);
}

void SalesScreen::createTicketCancellationDialog(MenuStack* screens){
    auto page = new MenuPage(screens, KILL_TICKETS_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    KillTicketsWidget *killTickets = new KillTicketsWidget(page, "KillTicketsWidget");
    page->addWidget(killTickets, "KillTicketsWidget");
    screens->addPage(page, KILL_TICKETS_MENU);
}

void SalesScreen::createReceiptRecoverDialog(MenuStack* screens){
    auto page = new MenuPage(screens, RECEIPT_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    ReceiptRecoverWidget *receipt = new ReceiptRecoverWidget(page, "ReceiptRecoverWidget");
    page->addWidget(receipt,"ReceiptRecoverWidget");
    screens->addPage(page, RECEIPT_MENU);
}

void SalesScreen::createInvitationDialog(MenuStack* screens){
    auto page = new MenuPage(screens, INVITATION_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    auto invitation = new InvitationAllocatorWidget(page, "InvitationAllocatorWidget");
    page->addWidget(invitation, invitation->objectName());
    screens->addPage(page, INVITATION_MENU);
}

void SalesScreen::createPayByCashButton(){
    //    GenericButton *collect_cash_button =  new GenericButton("Collect Cash", payment_frame, "cobrar_metalico");
    auto cash_collection_button =  new GenericButton("", ui->payment_frame, "cobrar_metalico");
    cash_collection_button->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    cash_collection_button->setFixedHeight(95);
    cash_collection_button->setIcon(QPixmap("payments:1000xaf.jpeg"));
    cash_collection_button->setIconSize(QSize(200,95));
    ui->cash_collection_layout->addWidget(cash_collection_button);
    connect(cash_collection_button, &GenericButton::clicked, this, &SalesScreen::receiveCash);
}

void SalesScreen::createGenericProductDialog(MenuStack* screens){
    auto page = new MenuPage(screens, GENERIC_PRODUCT_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    GenericProduct *g_product = new GenericProduct("varios", page, "GenericProductWidget");
    page->addWidget(g_product,"GenericProductWidget");
    screens->addPage(page, GENERIC_PRODUCT_MENU);
}

void SalesScreen::createAdminDialog(MenuStack* screens){
    auto page = new MenuPage(screens, ADMIN_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    m_admin_wgt = new AdminWidget(m_product_screens, page, "AdminWidget");
    page->addWidget(m_admin_wgt, m_admin_wgt->objectName());
    screens->addPage(page, ADMIN_MENU);
}

void SalesScreen::createPaySelectionDialog(MenuStack* screens){
    auto page = new MenuPage(screens, PAY_MODE_MENU);
    page->setLayoutType(MenuPage::LayoutType::VBOX);
    PayModes *pay_mode = new PayModes( m_admin_wgt->toggle_cash, m_bar_core, page, "PayModeWidget");
    page->addWidget(pay_mode, pay_mode->objectName());
    screens->addPage(page, PAY_MODE_MENU);
}

void SalesScreen::createShortcutButton(){
    if (!m_shortcut_grp)
        m_shortcut_grp = new GenericButtonGroup(m_product_screens, ui->shortcut_buttons_frame, "buttongroup");
    else
        m_shortcut_grp->clear();
    m_shortcut_grp->readConfigFrom("shortcut_buttons_frame", cfg::xmlFileByKey(cfg::XMLKey::Buttons));
}

void SalesScreen::createOpenCashDeviceButton(){
    if (!m_paymode_grp)
        m_paymode_grp = new GenericButtonGroup(m_product_screens, ui->openbox_and_payment_mode_buttons_frame,"buttongroup");
    else
        m_paymode_grp->clear();
    m_paymode_grp->readConfigFrom("openbox_and_payment_mode_buttons_frame", cfg::xmlFileByKey(cfg::XMLKey::Buttons));
}
