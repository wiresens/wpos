
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
//#include "menusystem/system/memstickwidget.h"
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
    menus{new HList<QWidget>()},
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

QWidget *Instantiator::getMenu(QWidget *parent, const QString &menu_name)
{
    QWidget *widget =  menus->find(menu_name);
    if(widget) widget->setParent(parent);
    return widget;
}

bool Instantiator::hasMenu(const QString &menu_name){
    QWidget *widget =  this->menus->find(menu_name);
    return widget;
}

void Instantiator::initDcopModules(){
    db_mod = new DatabaseModule(this,"databsemodule");
    user_mod = new UserModule(this,"usermodule");
    product_mod = new ProductModule(this, "productmodule");
}

void Instantiator::initSysConfMenu(){
    auto info = new SubMenuInfoFrame(0,"INFO");
    menus->append(info,"SYSCONF_SUBMENU");

//@benes    auto config_widget = new FX2000ConfigWidget(user_mod, 0, "FX2000_CONFIG_WIDGET");
//@benes    menus->append(config_widget,"SYSCONF_SUBMENU_FX2000DEVICE");

    auto ticket_widtet = new NTicketDesignWidget(0,"NTICKET_DESIGN_WIDGET");
    menus->append(ticket_widtet,"SYSCONF_SUBMENU_TICKETDESIGN_WIDGET");

    auto ddbb_widget = new DatabaseConfigWidget(db_mod,0,"DDBB_WIDGET");
    menus->append(ddbb_widget,"SYSCONF_SUBMENU_DDBB_SUBMENU_GENERAL");

    auto devices_config = new DevicesConfig(0,"DEVICES_CONFIG_WIDGET");
    menus->append(devices_config,"SYSCONF_SUBMENU_DEVICES_CONFIG_WIDGET");

}

void Instantiator::initUsersMenu(){
    auto info = new SubMenuInfoFrame(0,"INFO");
    menus->append(info,"USER_SUBMENU");

    auto users_widget = new UserCreationWidget(user_mod ,0,"USERS_WIDGET");
    menus->append(users_widget,"USER_SUBMENU_ADD");

    auto delete_users = new UserSelectWidget(db_mod,user_mod, UserSelectWidget::Deleter,0,"SELECT_USERS_MOD_WIDGET");
    menus->append(delete_users,"USER_SUBMENU_DEL");

    auto update_users = new UserSelectWidget(db_mod,user_mod, UserSelectWidget::Updater,0,"SELECT_USERS_MOD_WIDGET");
    menus->append(update_users,"USER_SUBMENU_MODIFY");
}

void Instantiator::initProductsMenu(){

    // Main information for this menu
    auto info = new SubMenuInfoFrame(0,"INFO");
    menus->append(info,"PRODUCTS_SUBMENU");

    emit splashRequested(tr("Modulo Productos: Unitarios"), splash_align, splash_color);
    auto unitary_product_widget = new ProductInsertionWidget(product_mod, ProductInsertionWidget::Unitary, 0, "UNITARY_PRODUCT_INSERT_WIDGET");
    menus->append(unitary_product_widget,"PRODUCTS_SUBMENU_ADD_UNITARY");

    emit splashRequested(tr("Modulo Productos: Compuestos"), splash_align, splash_color);
    auto  composition_product_widget = new ProductInsertionWidget(product_mod, ProductInsertionWidget::Composed, 0, "COMPOSITION_PRODUCT_INSERT_WIDGET");
    menus->append(composition_product_widget,"PRODUCTS_SUBMENU_ADD_COMPOSITE");

    emit splashRequested(tr("Modulo Productos: Actualizacion"), splash_align, splash_color);
    auto update_product_widget = new AdvancedProductUpdateWidget(product_mod, 0,"UPDATE_PRODUCT_WIDGET");
    menus->append(update_product_widget, "PRODUCTS_SUBMENU_MODIFY");

    emit splashRequested(tr("Modulo Productos: Borrado"), splash_align, splash_color);
    auto delete_product_widget = new AdvancedProductDeletionWidget(product_mod, 0, "DELETE_PRODUCT_WIDGET");
    menus->append(delete_product_widget, "PRODUCTS_SUBMENU_DEL");

    auto update_xml = new ProductXmFileUpdateWidget(product_mod, 0,"update_xml");
    menus->append(update_xml,"PRODUCTS_SUBMENU_XML_UPDATE");
}

void Instantiator::initDesignMenu(){
    // Main information for this menu
    auto info = new SubMenuInfoFrame(0,"INFO");
    menus->append(info,"DESIGN_SUBMENU");

    auto screen = new ProductScreenDesignWidget(0, "SCREEN_PRODUCTS");
    menus->append(screen, "DESIGN_SUBMENU_SCREEN");

    auto offers = new InvitationScreenDesignWidget(0, "SCREEN_OFFERS");
    menus->append(offers, "DESIGN_SUBMENU_OFFERS");

    auto shortcut_buttons = new ShortcutButtonWidget(0,"SHORTCUTBUTTONS_WIDGET");
    menus->append(shortcut_buttons, "DESIGN_SUBMENU_BUTTONS");
}

void Instantiator::initOffersAndOptionsMenu(){
    auto info = new SubMenuInfoFrame(0,"INFO");
    menus->append(info,"OFFERS_OPTIONS_SUBMENU");

    auto add_offer = new OfferCreationWidget(0, "ADD_OFFER");
    menus->append(add_offer, "OFFERS_OPTIONS_SUBMENU_ADD_OFFER");

    auto update_offer = new OfferUpdateWidget(0, "ADD_OFFER");
    menus->append(update_offer, "OFFERS_OPTIONS_SUBMENU_UPDATE_OFFER");

    auto del_offer = new OfferDeletionWidget(0, "ADD_OFFER");
    menus->append(del_offer, "OFFERS_OPTIONS_SUBMENU_DEL_OFFER");

    auto add_option = new OptionCreationWidget(0,"ADD_OPTION");
    menus->append(add_option, "OFFERS_OPTIONS_SUBMENU_ADD_OPTION");

    auto del_option = new OptionDeletionWidget(0,"ADD_OPTION");
    menus->append(del_option, "OFFERS_OPTIONS_SUBMENU_DEL_OPTION");

    auto update_option = new OptionUpdateWidget(0,"ADD_OPTION");
    menus->append(update_option, "OFFERS_OPTIONS_SUBMENU_UPDATE_OPTION");
}

void Instantiator::initHistoricMenu(){

    // Main information for this menu
    auto   info = new SubMenuInfoFrame(0,"INFO");
    menus->append(info,"HISTORIC_SUBMENU");

    auto ticket = new PersTicketWidget(0,"TICKET_HISTORIC_SUBMENU");
    menus->append(ticket, "TICKET_HISTORIC_SUBMENU");

    auto z = new PersZWidget(0, "Z_HISTORIC_SUBMENU");
    menus->append(z, "Z_HISTORIC_SUBMENU");
}

void Instantiator::initReportMenu(){

    // Main information for this menu
    auto info = new SubMenuInfoFrame(0,"INFO");
    menus->append(info,"REPORT_SUBMENU");

    auto profits = new NProfitReportWidget(user_mod,0,"PROFIT_REPORT");
    menus->append(profits, "PROFIT_SUBMENU");

//@benes    auto prod_and_offer = new NProdAndOfferReportWidget(user_mod,0,"PROD_AND_OFFER_REPORT");
//@benes    menus->append(prod_and_offer, "PRODUCT_AND_OFFERS_SUBMENU");
}

void Instantiator::initStickMenu(){

    // Main information for this menu
     auto info = new SubMenuInfoFrame(0,"INFO");
     menus->append(info,"STICK_SUBMENU");

//@benes    auto stick_widget = new MemStickWidget(0,"STICK_SUBMENU");
//@benes    menus->append(stick_widget, "STICK_SUBMENU");
}

void Instantiator::initSupportMenu(){
    // Main information for this menu
    auto info = new SubMenuInfoFrame(0,"INFO");
    menus->append(info,"SUPPORT_SUBMENU");

    auto support = new SupportWidget(0,"SUPPORT_WIDGET");
    menus->append(support, "SUPPORT_SUBMENU");

    auto tech = new TechWidget(0,"TECHWIDGET");
    menus->append(tech, "TECH_TEAM_SUBMENU");
}

void Instantiator::showMenu(const QString& page_name){
    emit showPageMenu(page_name);
}
