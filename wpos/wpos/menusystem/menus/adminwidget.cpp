/***************************************************************************
                          bsladmonpanelwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "adminwidget.h"

#include "auth/authcore.h"
#include "mainscreen.h"
#include "salesscreen.h"

#include "prodselect/productscreenstack.h"
#include "customize/genericbuttongroup.h"
#include "barcore/filemanager.h"
#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>
#include <QApplication>

#include <QLayout>
#include <QFrame>
#include <QFile>
#include <QPushButton>
#include <QPixmap>
#include <QEvent>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>

#include <xmlconfig.h>

#include <iostream>
using namespace std;

extern AuthCore *auth;
extern FileManager *file_manager;

AdminWidget::AdminWidget(ProductScreenStack *stack,QWidget* parent, const QString& name):
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    cancel_button->setIcon(QPixmap("/usr/share/ntpv/apps/button_cancel.png"));

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED,this);
    gsm->subscribeToGenericSignal(GDATASIGNAL::MAINWIDGET_SAVE_BUTTON_STATE,this);
    gsm->publishGenericSignal(GSIGNAL::LOAD_BUTTONS,this);
    gsm->publishGenericSignal(GSIGNAL::LOAD_PRODUCTS,this);
    gsm->publishGenericSignal(GSIGNAL::LOAD_OFFERS,this);
    gsm->publishGenericSignal(GSIGNAL::LOAD_DEVICES,this);


    connect(cancel_button, &QPushButton::released, this, &AdminWidget::cancelSlot);
    connect(qApp, &QApplication::aboutToQuit, this, &AdminWidget::saveToggleButtonStatus);
    init(stack);
    loadToggleButtonStatus();

    process = new QProcess(this);
    process->setObjectName("backoffice_process");

}

AdminWidget::~AdminWidget(){}

void AdminWidget::init(ProductScreenStack *stack){

    auto configFile = Files::configFilePath("buttons");

    management = new GenericButtonGroup(stack, gestion_inside_frame, "admin_management");
    management->readConfigFrom("admin_management", configFile);

    operations = new GenericButtonGroup(stack, operation_inside_frame, "admin_operation");
    if (!operations->readConfigFrom("admin_operation", configFile)){
        exit(2);
    }

    cancel_operations = new GenericButtonGroup(stack, anulation_inside_frame,"admin_cancel_operation");
    if (!cancel_operations->readConfigFrom("admin_cancel_operation", configFile)){
        exit(2);
    }

    printer_operations = new GenericButtonGroup(stack, printer_inside_frame,"admin_printer");
    if (!printer_operations->readConfigFrom("admin_printer", configFile)){
        exit(2);
    }

    controls = new GenericButtonGroup(stack,control_inside_frame,"admin_controls");
    if (!controls->readConfigFrom("admin_controls", configFile)){
        exit(2);
    }

    admin_operations = new GenericButtonGroup(stack, admon_inside_frame, "admin_admin_operation");
    if (!admin_operations->readConfigFrom("admin_admin_operation", configFile)){
        exit(2);
    }

    auto caption = tr("Button %1 Not Found");
    auto text = tr("Failed to load admin button configuration profile %1");
    toggle_derrama = cancel_operations->find("toggle_derrama");
    if (!toggle_derrama){
        qDebug() << "wpos::error toggle_derrama" << Qt::endl;
        QMessageBox::warning(this, caption.arg("toggle_derrama"), text.arg("toggle_derrama"), QMessageBox::NoButton);
        exit(1);
    }

    toggle_anulation = cancel_operations->find("toggle_anulation");
    if (!toggle_anulation){
        qDebug() << "wpos::error toggle_anulation" << Qt::endl;
        QMessageBox::warning(this, caption.arg("toggle_anulation"), text.arg("toggle_anulation"), QMessageBox::NoButton);
        exit(1);
    }

    toggle_cash = controls->find("toggle_cash");
    if (!toggle_cash){
        qDebug() << "wpos::error toggle_cash" << Qt::endl;
        QMessageBox::warning(this, caption.arg("toggle_cash"), text.arg("toggle_cash"), QMessageBox::NoButton);
        exit(1);
    }

    toggle_out_screen = controls->find("toggle_out_screen");
    if (!toggle_out_screen){
        qDebug() << "wpos::error toggle_out_screen" << Qt::endl;
        QMessageBox::warning(this, caption.arg("toggle_out_screen"), text.arg("toggle_out_screen"), QMessageBox::NoButton);
        exit(1);
    }

    toggle_waiters = controls->find("toggle_waiters");
    if (!toggle_waiters){
        qDebug() << "wpos::error toggle_waiters" << Qt::endl;
        QMessageBox::warning(this, caption.arg("toggle_waiters"), text.arg("toggle_waiters"), QMessageBox::NoButton);
        exit(1);
    }

    toggle_fx2000 = controls->find("toggle_fx2000");
    if (!toggle_fx2000){
        qDebug() << "wpos::error toggle_fx2000" << Qt::endl;
        QMessageBox::warning(this, caption.arg("toggle_fx2000"), text.arg("toggle_fx2000"), QMessageBox::NoButton);
        exit(1);
    }

    toggle_printing = printer_operations->find("toggle_printing");
    if (! toggle_printing){
        qDebug() << "wpos::error toggle_printing" << Qt::endl;
        QMessageBox::warning(this, caption.arg("toggle_printing"), text.arg("toggle_printing"), QMessageBox::NoButton);
        exit(1);
    }

    toggle_double_printing = printer_operations->find("toggle_double_printing");

    QPushButton *button  = admin_operations->find("ntpv_backoffice");

    if (button)
        connect(button, &QPushButton::clicked, this, &AdminWidget::launchBackOffice);

    button = admin_operations->find("xterm");
    if (button)
        connect(button, &QPushButton::clicked ,this, &AdminWidget::launchXterm);
}

void AdminWidget::loadToggleButtonStatus(){

    QFile file ( Files::configFilePath("button_state"));
    if (!file.exists())  return;

    std::unique_ptr<XmlConfig> xml { new XmlConfig( Files::configFilePath("button_state")) };
    xml->delDomain();
    if (!xml->setDomain("admin_buttons"))
        return;

    auto count = xml->howManyTags("button");
    for (auto i=0; i<count; i++){
        auto aux = xml->readString("button["+QString::number(i)+"].name");
        auto aux_state = xml->readString("button["+QString::number(i)+"].state");

        if ( aux == "toggle_cash" && aux_state=="on" )
            toggle_cash->toggle();

        else if ( aux == "toggle_waiters" && aux_state=="on" )
            toggle_waiters->toggle();

        else if ( aux == "toggle_printing" && aux_state=="on" )
            toggle_printing->toggle();

        else if ( aux == "toggle_fx2000" && aux_state=="on" )
            toggle_fx2000->toggle();

        else if ( aux == "toggle_double_printing" && aux_state=="on" )
            toggle_double_printing->toggle();

        else if ( aux == "toggle_out_screen" && aux_state=="on" )
            toggle_out_screen->toggle();
    }
    xml->delDomain();
}

void AdminWidget::cancelSlot(){

    XmlConfig xml;
    if ( auth && auth->getUserId()  == "1" ){
        xml.createElement("name", MainScreen::LOGIN_SCREEN);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
        return;
    }

    xml.deleteElement("name");
    xml.createElement("name", SalesScreen::PRODUCT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);

    xml.deleteElement("name");
    xml.createElement("enabled", "true");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);
}

void AdminWidget::launchBackOffice(){
//    process->setArguments( QStringList() << "~/Workspace/study/ntpv/ntpvbo/wposbo");
    connect(process, SIGNAL(finished(int)), this, SLOT(backOfficeEndSlot()));
    process->setProgram("/home/benes/Workspace/study/ntpv/ntpvbo/wposbo");
    process->start();
}

int AdminWidget::launchXterm(){
    return system("/usr/bin/xterm");
}

void AdminWidget::backOfficeEndSlot( ){
    disconnect(process, SIGNAL(finished(int)), this, SLOT(backOfficeEndSlot()));
    raiseConfigWidget();
    emit genericSignal(GSIGNAL::LOAD_BUTTONS);
    emit genericSignal(GSIGNAL::LOAD_PRODUCTS);
    emit genericSignal(GSIGNAL::LOAD_OFFERS);
    emit genericSignal(GSIGNAL::LOAD_DEVICES);
    file_manager->saveFiles();
    raiseMainWidget();
}

void AdminWidget::saveToggleButtonStatus(){

    QFile file( Files::configFilePath("button_state"));
    if (file.exists()) file.remove();

    XmlConfig xml( Files::configFilePath("button_state"));
    xml.delDomain();
    xml.createElementSetDomain("admin_buttons");

    //toggle_cash
    xml.createElementSetDomain("button");
    xml.createElement("name","toggle_cash");
    if (toggle_cash->isDown())
        xml.createElement("state","on");
    else
        xml.createElement("state", "off");

    xml.releaseDomain("button");

    //toggle_waiters
    xml.createElementSetDomain("button");
    xml.createElement("name","toggle_waiters");
    if (toggle_waiters->isDown())
        xml.createElement("state","on");
    else
        xml.createElement("state","off");
    xml.releaseDomain("button");


    //toggle_printing
    xml.createElementSetDomain("button");
    xml.createElement("name","toggle_printing");
    if (toggle_printing->isDown())
        xml.createElement("state","on");
    else
        xml.createElement("state","off");
    xml.releaseDomain("button");

    //toggle_fx2000
    xml.createElementSetDomain("button");
    xml.createElement("name","toggle_fx2000");
    if (toggle_fx2000->isDown())
        xml.createElement("state","on");
    else
        xml.createElement("state","off");
    xml.releaseDomain("button");

    //toggle_double_printing
    xml.createElementSetDomain("button");
    xml.createElement("name","toggle_double_printing");
    if (toggle_double_printing->isDown())
        xml.createElement("state","on");
    else
        xml.createElement("state","off");
    xml.releaseDomain("button");

    xml.createElementSetDomain("button");
    xml.createElement("name","toggle_out_screen");
    if (toggle_out_screen->isDown())
        xml.createElement("state","on");
    else
        xml.createElement("state","off");
    xml.releaseDomain("button");

    xml.delDomain();
    xml.save();
}

void AdminWidget::genericSignalSlot(const QString& signal){
    if ( signal == GDATASIGNAL::MAINWIDGET_SAVE_BUTTON_STATE){
        saveToggleButtonStatus();
    }
}

void AdminWidget::raiseMainWidget(){
    XmlConfig xml ;
    xml.createElement("name", MainScreen::SALES_SCREEN);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}

void AdminWidget::raiseConfigWidget(){
    XmlConfig xml;
    xml.createElement("name", MainScreen::READ_CONFIG_SCREEN);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}
