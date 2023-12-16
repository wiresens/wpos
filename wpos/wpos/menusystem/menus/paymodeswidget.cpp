/***************************************************************************
                          paymodewidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "paymodeswidget.h"


#include "mainscreen.h"
#include "salesscreen.h"

#include "barcore/barcore.h"
#include "customize/genericbutton.h"

#include <wposcore/genericsignalmanager.h>
#include <xmlconfig.h>

#include <QSignalMapper>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QTimer>

#include <iostream>
using namespace std;

PayModes::PayModes(
    QPushButton* _toggle_cash,
    BarCore *_core,
    QWidget *parent,
    const QString& name ):
    QWidget(parent), core{_core},
    toggle_cash{_toggle_cash}
{
    setupUi(this);
    setObjectName(name);
    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::BARCORE_PROCESS_CORE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED,this);

    cancel_button->setIcon(QPixmap("controls:button_cancel.png"));

    //proteccion_civil_button->hide();

    QVector<QPushButton*> paymode_buttons {
        mastercard_button,
        visa_button,
        amex_button,
        fourb_button,
        cash_button,
        gourmet_button,
        restaurant_button,
        sodexho_button,
        proteccion_civil_button
    };

    QVector<QString> paymode_pixmap {
        QString("master_card.png"),
        QString("visa.svg"),
        QString("american_express.svg"),
        QString("orange_money.png"),
        QString("google_pay.png") ,
        QString("airtel.svg"),
        QString("paypal.svg")
    };

    for( auto i = 0; i < paymode_pixmap.size(); i++ ){
        auto pixmap_str = QString("payments:") + paymode_pixmap[i];
        paymode_buttons[i]->setIcon(QPixmap(pixmap_str));
        paymode_buttons[i]->setIconSize(QSize{150, 100});
    }

    connect(mastercard_button, &QPushButton::clicked, this, &PayModes::mastercardClickedSlot);
    connect(visa_button, &QPushButton::clicked,this, &PayModes::visaClickedSlot);
    connect(amex_button, &QPushButton::clicked,this, &PayModes::amexClickedSlot);
    connect(fourb_button, &QPushButton::clicked,this, &PayModes::fourbClickedSlot);
    connect(cash_button, &QPushButton::clicked,this, &PayModes::cashClickedSlot);
    connect(gourmet_button, &QPushButton::clicked,this, &PayModes::gourmetClickedSlot);
    connect(restaurant_button, &QPushButton::clicked,this, &PayModes::restaurantClickedSlot);
    connect(sodexho_button, &QPushButton::clicked,this, &PayModes::sodexhoClickedSlot);
    connect(proteccion_civil_button, &QPushButton::clicked,this, &PayModes::proceccionCivilClickedSlot);

    connect(cancel_button, &QPushButton::clicked,this, &PayModes::cancelClickedSlot);

}

PayModes::~PayModes(){}

void PayModes::setNewPrice(double price){
    total_receivable_value_label->setText((QString::number(price,'f',2))+" € ");
}

void PayModes::mastercardClickedSlot(){
    sendTicketSignal("mastercard");
}

void PayModes::visaClickedSlot(){
    sendTicketSignal("visa");
}

void PayModes::amexClickedSlot(){
    sendTicketSignal("american express");
}

void PayModes::fourbClickedSlot(){
    sendTicketSignal("4b");
}

void PayModes::gourmetClickedSlot(){
    sendTicketSignal("cheque gourmet");
}

void PayModes::restaurantClickedSlot(){
    sendTicketSignal("ticket restaurant");
}

void PayModes::sodexhoClickedSlot(){
    sendTicketSignal("sodexho pass");
}

void PayModes::proceccionCivilClickedSlot(){
    sendTicketSignal("proteccion civil");
}

void PayModes::cashClickedSlot(){
    XmlConfig xml ;
    if (!toggle_cash->isDown())
        sendTicketSignal("metalico");
    else{
        xml.createElement("name", SalesScreen::CASH_MENU);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
    }
}

void PayModes::cancelClickedSlot(){
    XmlConfig xml ;
    xml.createElement("name", SalesScreen::PRODUCT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);

    xml.deleteElement("name");
    xml.createElement("enabled", "true");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);
}

void PayModes::sendTicketSignal(const QString& pay_mode){

    XmlConfig xml ;
    xml.createElement("pay_type", pay_mode);
    emit genericDataSignal(GDATASIGNAL::BARCORE_PROCESS_CORE, &xml );

    xml.deleteElement("pay_type");
    xml.createElement("name", SalesScreen::PRODUCT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);

    xml.deleteElement("name");
    xml.createElement("enabled","true");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);
}

void PayModes::showEvent(QShowEvent *e){

    double actual_price = 0.0;
    if ( !core->hasProducts() ){
        QTimer::singleShot(50, this, &PayModes::cancelClickedSlot);
        return;
    }

    actual_price = core->total();

    //the price is 0... nothing to pay
    XmlConfig xml;
    if (actual_price <= 0){
        sendTicketSignal("metalico");
        xml.createElement("name", SalesScreen::PRODUCT_MENU);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
    }
    else
        setNewPrice(actual_price);
}
