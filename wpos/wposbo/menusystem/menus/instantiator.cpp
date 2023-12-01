
/***************************************************************************
                          bslinstantiator.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "instantiator.h"
#include "supportwidget.h"
#include "techwidget.h"

#include "menusystem/database/databaseconfigwidget.h"

#include "menusystem/design/submenuinfoframe.h"
#include "menusystem/design/posdesignselectorwidget.h"
#include "menusystem/design/productscreendesignwidget.h"
#include "menusystem/design/productscreendesignwidget.h"
#include "menusystem/design/invitationscreendesignwidget.h"
#include "menusystem/design/nticketdesignwidget.h"
#include "menusystem/design/shortcutbuttonwidget.h"

#include "menusystem/offers_options/offercreationwidget.h"
#include "menusystem/offers_options/offerdeletionwidget.h"
#include "menusystem/offers_options/offerupdatewidget.h"
#include "menusystem/offers_options/optioncreationwidget.h"
#include "menusystem/offers_options/optiondeletionwidget.h"
#include "menusystem/offers_options/optionupdatewidget.h"

#include "menusystem/products/productinsertionwidget.h"
#include "menusystem/products/advancedproductdeletionwidget.h"
#include "menusystem/products/advancedproductupdatewidget.h"
#include "menusystem/products/basicproductcreationwidget.h"
#include "menusystem/products/basicproducteditionwidget.h"
#include "menusystem/products/productxmfileupdatewidget.h"

#include "menusystem/reports/perszwidget.h"
#include "menusystem/reports/persticketwidget.h"
#include "menusystem/reports/nprofitreportwidget.h"
//#include "menusystem/reports/nprodandofferreportwidget.h"

//#include "menusystem/system/fx2000configwidget.h"
#include "menusystem/system/memstickwidget.h"
#include "menusystem/system/devicesconfigwidget.h"

#include "menusystem/users/usercreatewidget.h"
#include "menusystem/users/userselectwidget.h"

#include "databasemodule/databasemodule.h"
#include "usersmodule/usermodule.h"
#include "productsmodule/productmodule.h"

#include <libbslxml/xmlconfig.h>
#include <QSplashScreen>

extern QSplashScreen *splash;

Instantiator::Instantiator(QWidget* parent, const QString& name):
    QObject(parent),
    parent{parent}
{

    // Initialization and default values
    setObjectName(name);
    connect(this, &Instantiator::splashRequested, splash, &QSplashScreen::showMessage);

    emit splashRequested(tr("D-Bus Module"), splash_align, splash_color);
    initDcopModules();

    emit splashRequested(tr("System Module"),splash_align, splash_color);
    initSysConfMenu();

    emit splashRequested(tr("Personel Module"),splash_align, splash_color);
    initUsersMenu();

    emit splashRequested(tr("Offers Module"), splash_align, splash_color);
    initOffersAndOptionsMenu();

    emit splashRequested(tr("Products Module:"), splash_align, splash_color);
    initProductsMenu();

    emit splashRequested(tr("Screens Configuration Module"), splash_align, splash_color);
    initDesignMenu();

    emit splashRequested(tr("Listings Module"), splash_align, splash_color);
    initHistoricMenu();

    emit splashRequested(tr("Information Module"), splash_align, splash_color);
    initReportMenu();

    emit splashRequested(tr("Stick Operations Module"), splash_align, splash_color);
    initStickMenu();
    initSupportMenu();

    disconnect(this, &Instantiator::splashRequested, splash, &QSplashScreen::message);
}

void Instantiator::initDcopModules(){
    db_mod      = new DatabaseModule(this, "databsemodule");
    user_mod    = new UserModule(this, "usermodule");
    product_mod = new ProductModule(this, "productmodule");
}

void Instantiator::initSysConfMenu(){
    QWidget* widget = new SubMenuInfoFrame(nullptr, "INFO");
    menus.append(widget, "SYSCONF_SUBMENU");

    //@benes    auto config_widget = new FX2000ConfigWidget(user_mod, nullptr, "FX2000_CONFIG_WIDGET");
    //@benes    menus.append(config_widget,"SYSCONF_SUBMENU_FX2000DEVICE");

    widget = new NTicketDesignWidget(nullptr,"NTICKET_DESIGN_WIDGET");
    menus.append(widget, "SYSCONF_SUBMENU_TICKETDESIGN_WIDGET");

    widget = new DatabaseConfigWidget(db_mod,nullptr, "DDBB_WIDGET");
    menus.append(widget,"SYSCONF_SUBMENU_DDBB_SUBMENU_GENERAL");

    widget = new DevicesConfig( nullptr, "DEVICES_CONFIG_WIDGET");
    menus.append(widget, "SYSCONF_SUBMENU_DEVICES_CONFIG_WIDGET");

}

void Instantiator::initUsersMenu(){
    QWidget* widget = new SubMenuInfoFrame( nullptr,  "INFO");
    menus.append(widget, "USER_SUBMENU");

    widget = new UserCreationWidget(user_mod,  nullptr,  "USERS_WIDGET");
    menus.append(widget, "USER_SUBMENU_ADD");

    widget = new UserSelectWidget(db_mod,user_mod, UserSelectWidget::Deleter, nullptr, "SELECT_USERS_MOD_WIDGET");
    menus.append(widget, "USER_SUBMENU_DELETE");

    widget = new UserSelectWidget(db_mod,user_mod, UserSelectWidget::Updater, nullptr, "SELECT_USERS_MOD_WIDGET");
    menus.append(widget, "USER_SUBMENU_MODIFY");
}

void Instantiator::initProductsMenu(){

    // Main information for this menu
    QWidget* widget = new SubMenuInfoFrame( nullptr,  "INFO");
    menus.append(widget, "PRODUCTS_SUBMENU");

    emit splashRequested(tr("Modulo Productos: Unitarios"), splash_align, splash_color);
    widget = new ProductInsertionWidget(product_mod, ProductInsertionWidget::Unitary,  nullptr,  "UNITARY_PRODUCT_INSERT_WIDGET");
    menus.append(widget,"PRODUCTS_SUBMENU_ADD_UNITARY");

    emit splashRequested(tr("Modulo Productos: Compuestos"), splash_align, splash_color);
    widget = new ProductInsertionWidget(product_mod, ProductInsertionWidget::Composed,  nullptr,  "COMPOSITION_PRODUCT_INSERT_WIDGET");
    menus.append(widget,"PRODUCTS_SUBMENU_ADD_COMPOSITE");

    emit splashRequested(tr("Modulo Productos: Actualizacion"), splash_align, splash_color);
    widget = new AdvancedProductUpdateWidget(product_mod,  nullptr, "UPDATE_PRODUCT_WIDGET");
    menus.append(widget, "PRODUCTS_SUBMENU_MODIFY");

    emit splashRequested(tr("Modulo Productos: Borrado"), splash_align, splash_color);
    widget = new AdvancedProductDeletionWidget(product_mod,  nullptr,  "DELETE_PRODUCT_WIDGET");
    menus.append(widget, "PRODUCTS_SUBMENU_DELETE");

    widget = new ProductXmFileUpdateWidget(product_mod,  nullptr, "update_xml");
    menus.append(widget,"PRODUCTS_SUBMENU_XML_UPDATE");
}

void Instantiator::initDesignMenu(){
    // Main information for this menu
    QWidget* widget = new SubMenuInfoFrame( nullptr, "INFO");
    menus.append(widget,"DESIGN_SUBMENU");

    widget = new ProductScreenDesignWidget( nullptr,  "SCREEN_PRODUCTS");
    menus.append(widget, "DESIGN_SUBMENU_SCREEN");

    widget = new InvitationScreenDesignWidget( nullptr,  "SCREEN_OFFERS");
    menus.append(widget, "DESIGN_SUBMENU_OFFERS");

    widget = new ShortcutButtonWidget( nullptr, "SHORTCUTBUTTONS_WIDGET");
    menus.append(widget, "DESIGN_SUBMENU_BUTTONS");
}

void Instantiator::initOffersAndOptionsMenu(){
    QWidget* widget = new SubMenuInfoFrame( nullptr, "INFO");
    menus.append(widget, "OFFERS_OPTIONS_SUBMENU");

    widget = new OfferCreationWidget( nullptr,  "ADD_OFFER");
    menus.append(widget, "OFFERS_OPTIONS_SUBMENU_ADD_OFFER");

    widget = new OfferUpdateWidget( nullptr,  "ADD_OFFER");
    menus.append(widget, "OFFERS_OPTIONS_SUBMENU_UPDATE_OFFER");

    widget = new OfferDeletionWidget( nullptr,  "ADD_OFFER");
    menus.append(widget, "OFFERS_OPTIONS_SUBMENU_DEL_OFFER");

    widget = new OptionCreationWidget( nullptr, "ADD_OPTION");
    menus.append(widget, "OFFERS_OPTIONS_SUBMENU_ADD_OPTION");

    widget = new OptionDeletionWidget( nullptr, "ADD_OPTION");
    menus.append(widget, "OFFERS_OPTIONS_SUBMENU_DEL_OPTION");

    widget = new OptionUpdateWidget( nullptr, "ADD_OPTION");
    menus.append(widget, "OFFERS_OPTIONS_SUBMENU_UPDATE_OPTION");
}

void Instantiator::initHistoricMenu(){

    // Main information for this menu
    QWidget* widget = new SubMenuInfoFrame( nullptr, "INFO");
    menus.append( widget, "HISTORIC_SUBMENU");

    widget = new PersTicketWidget( nullptr, "TICKET_HISTORIC_SUBMENU");
    menus.append(widget, "TICKET_HISTORIC_SUBMENU");

    widget = new PersZWidget( nullptr,  "Z_HISTORIC_SUBMENU");
    menus.append(widget, "Z_HISTORIC_SUBMENU");
}

void Instantiator::initReportMenu(){

    // Main information for this menu
    QWidget* widget = new SubMenuInfoFrame( nullptr, "INFO");
    menus.append(widget,"REPORT_SUBMENU");

    widget = new NProfitReportWidget(user_mod, nullptr, "PROFIT_REPORT");
    menus.append(widget, "PROFIT_SUBMENU");

    //@benes    widget = new NProdAndOfferReportWidget(user_mod, nullptr, "PROD_AND_OFFER_REPORT");
    //@benes    menus.append(widget, "PRODUCT_AND_OFFERS_SUBMENU");
}

void Instantiator::initStickMenu(){

    // Main information for this menu
    QWidget* widget = new SubMenuInfoFrame( nullptr,  "INFO");
    menus.append( widget, "STICK_SUBMENU");

    widget = new MemStickWidget( nullptr, "STICK_SUBMENU");
    menus.append( widget, "STICK_SUBMENU");
}

void Instantiator::initSupportMenu(){
    // Main information for this menu
    QWidget* widget = new SubMenuInfoFrame( nullptr,  "INFO");
    menus.append(widget, "SUPPORT_SUBMENU");

    widget = new SupportWidget( nullptr,  "SUPPORT_WIDGET");
    menus.append(widget, "SUPPORT_SUBMENU");

    widget = new TechWidget( nullptr,  "TECHWIDGET");
    menus.append( widget, "TECH_TEAM_SUBMENU");
}

QWidget *Instantiator::getMenu(
    QWidget *parent,
    const QString &menuName)
{
    QWidget *widget =  menus.find(menuName);
    if(widget) widget->setParent(parent);
    return widget;
}

bool Instantiator::hasMenu(const QString &menuName){
    return menus.find(menuName);

}

void Instantiator::showMenu(const QString& pageName){
    emit showPageMenu(pageName);
}
