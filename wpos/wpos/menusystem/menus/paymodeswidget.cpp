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

struct PayRecord{
    QPushButton* btn;
    const QString icon;
};

PayModes::PayModes(
    QPushButton* _toggle_cash,
    BarCore *_core,
    QWidget *parent,
    const QString& name ):
    QWidget(parent),
    core{_core},
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

    QVector<PayRecord> pay_records{
        { cash_button,              QString("1000xaf.jpeg") } ,
        { proteccion_civil_button , QString("orange_money.png") } ,
        { fourb_button,             QString("mtn_momo.png") } ,
        { restaurant_button,        QString("airtel.svg") } ,
        { mastercard_button,        QString("master_card.png") } ,
        { visa_button,              QString("visa.png") } ,
        { gourmet_button,           QString("google_pay.png") } ,
        { amex_button,              QString("american_express.svg") } ,
        { sodexho_button,           QString("paypal.svg") }
    };

    for( auto &record : pay_records){
        auto icon_file_name = QString("payments:") + record.icon;
        record.btn->setIcon(QPixmap(icon_file_name));
        record.btn->setIconSize(QSize{150, 100});
    }
    connectButtons();
}

void PayModes::setNewPrice(double price){
    total_receivable_value_label->setText((QString::number(price,'f',2))+" € ");
}

void PayModes::mastercardPay(){
    sendTicketSignal("mastercard");
}

void PayModes::visaPay(){
    sendTicketSignal("visa");
}

void PayModes::amexPay(){
    sendTicketSignal("american express");
}

void PayModes::fourbPay(){
    sendTicketSignal("4b");
}

void PayModes::gourmetPay(){
    sendTicketSignal("cheque gourmet");
}

void PayModes::restaurantPay(){
    sendTicketSignal("ticket restaurant");
}

void PayModes::sodexhoPay(){
    sendTicketSignal("sodexho pass");
}

void PayModes::proceccionPay(){
    sendTicketSignal("proteccion civil");
}

void PayModes::cashPay(){
    XmlConfig xml ;
    xml.createElement("name", SalesScreen::CASH_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}

// void PayModes::cashPay(){
//     XmlConfig xml ;
//     if (!toggle_cash->isDown())
//         sendTicketSignal("metalico");
//     else{
//         xml.createElement("name", SalesScreen::CASH_MENU);
//         emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
//     }
// }

void PayModes::cancelPayment(){
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
        QTimer::singleShot(50, this, &PayModes::cancelPayment);
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

void PayModes::connectButtons(){
    connect(mastercard_button, &QPushButton::clicked, this, &PayModes::mastercardPay);
    connect(visa_button, &QPushButton::clicked,this, &PayModes::visaPay);
    connect(amex_button, &QPushButton::clicked,this, &PayModes::amexPay);
    connect(fourb_button, &QPushButton::clicked,this, &PayModes::fourbPay);
    connect(cash_button, &QPushButton::clicked, this, &PayModes::cashPay);
    connect(gourmet_button, &QPushButton::clicked,this, &PayModes::gourmetPay);
    connect(restaurant_button, &QPushButton::clicked,this, &PayModes::restaurantPay);
    connect(sodexho_button, &QPushButton::clicked,this, &PayModes::sodexhoPay);
    connect(proteccion_civil_button, &QPushButton::clicked,this, &PayModes::proceccionPay);

    connect(cancel_button, &QPushButton::clicked,this, &PayModes::cancelPayment);
}
