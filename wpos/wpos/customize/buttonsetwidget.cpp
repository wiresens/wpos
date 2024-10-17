/***************************************************************************
                          bslbuttonsetwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "dbusreceiptprimitive_interface.h"
#include "dbusreceiptmediator_interface.h"

#include "buttonsetwidget.h"
#include "genericbutton.h"
#include "genericbuttongroup.h"

#include "prodselect/productscreenstack.h"
#include "prodselect/product.h"

#include <libbslxml/xmlconfig.h>
#include <wposcore/productextrainfo.h>
#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>

#include <QFrame>
#include <QLayout>
#include <QEvent>
#include <QFile>
#include <QFileSystemWatcher>

#include <iostream>
using namespace std;

using IReceiptPrimitive = com::wiresens::wpos::dbusreceipt::DBusReceiptPrimitive;
using IReceiptMediator  = com::wiresens::wpos::dbusreceipt::DBusReceiptMediator;
static const QString dbus_service = "com.wiresens.wpos.dbusreceipt";
static const QString dbus_object_remote = "/wpos/dbusreceipt/DBusReceiptPrimitive";
static const QString dbus_object_local = "/wpos/dbusreceipt/DBusReceiptMediator";

ButtonSetWidget::ButtonSetWidget(
        ProductScreenStack *stack,
        QWidget* parent,
        const QString& name)
    :QWidget(parent),
    m_product_screen_stack {stack}
{
    setupUi(this);
    setObjectName(name);
    // Set each button group exclusive, I've not succeeded to do that yet
    //@benes    setExclusive(true);
    //@benes    setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    createButtons();
    GenericSignalManager::instance()->subscribeToGenericSignal(GSIGNAL::LOAD_BUTTONS, this);
}

ButtonSetWidget::~ButtonSetWidget(){}

void ButtonSetWidget::createButtons(){
    auto configFile = cfg::xmlFileByKey(cfg::XMLKey::Buttons);
    if (!m_generic_btn_grp){
        m_generic_btn_grp = new GenericButtonGroup(m_product_screen_stack, this, "upper_layout");
        m_generic_btn_grp->readConfigFrom("upper_buttons", configFile);
    }
    else{
        m_generic_btn_grp->clear();
        m_generic_btn_grp->readConfigFrom("upper_buttons", configFile);
    }
}

void ButtonSetWidget::showEvent(QShowEvent *event){

    auto commands_button = m_generic_btn_grp->buttonByName("dbusreceipt");
    if (commands_button){
        auto bus = QDBusConnection::sessionBus();
        auto remote_iface = new IReceiptPrimitive(dbus_service, dbus_object_remote, bus, this);
        auto local_iface  = new IReceiptMediator(dbus_service, dbus_object_local, bus, this);

        if ( remote_iface->isValid() && local_iface->isValid())
            commands_button->show();
        else{
            std::clog << "ButtonSetWidget::showEvent : dbusreceipt service not running , hidding Command Button\n";
            commands_button->hide();
        }
    }
    QWidget::showEvent(event);
}

void ButtonSetWidget::genericSignalSlot(const QString& signal_name){
    if (signal_name == GSIGNAL::LOAD_BUTTONS)
        createButtons();
}
