/***************************************************************************
                          bslcashwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "ui_cashwidget.h"
#include "cashwidget.h"

#include "mainscreen.h"
#include "salesscreen.h"
#include "barcore/barcore.h"

#include <wposgui/keyboard/floatkeyboard.h>
#include <wposcore/genericsignalmanager.h>
#include <libbslxml/xmlconfig.h>

#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QApplication>
#include <QtGui/QPixmap>
#include <QtCore/QFile>
#include <QtCore/QTimer>

static const quint16 LOOK_DELAY {3000};
static const quint16 DECIMALS   {2};
static const quint16 TIME_OUT   {25};

CashWidget::CashWidget(BarCore *barCore,
    QWidget *parent,
    const QString& name):
    QWidget(parent),
    ui{ new Ui::CashWidget},
    barCore{barCore}
{
    ui->setupUi(this);
    setObjectName(name);
    float_keyboard  = new FloatKeyboard(this);
    float_keyboard->setObjectName("change_cash_numpad");
    float_keyboard->hideDisplay();
    QHBoxLayout *numpad_layout = qobject_cast<QHBoxLayout *>( ui->numpad_frame->layout() );

    if (!numpad_layout)
        numpad_layout = new QHBoxLayout(ui->numpad_frame);
    numpad_layout->addWidget(float_keyboard);

    // Initialize all images
    auto pixmap = QPixmap("payments:pay.jpeg");
    ui->ok_button->setIcon(pixmap);
//    ui->ok_button->setIconSize(pixmap.rect().size());
//    ui->ok_button->setFixedWidth(150);
    ui->cancel_button->setIcon(QPixmap("controls:button_cancel.png"));

    //generic signal connector
    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::BARCORE_PROCESS_CORE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED,this);
    // gsm->publishGenericDataSignal(GDATASIGNAL::LATERALWIDGET_SET_VISIBLE, this);
    gsm->publishGenericSignal(GSIGNAL::OPEN_CASHBOX,this);
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::CASH_MENU_SPEED,this);

    //normal signals and slots
    connect(ui->cancel_button, &QPushButton::released, this, &CashWidget::cancelSlot);
    connect(ui->ok_button, &QPushButton::released, this, &CashWidget::accepSlot);
    connect(float_keyboard, &FloatKeyboard::valueChanged, this, &CashWidget::numkeyChangedSlot);
    //     connect(quick_button, SIGNAL(released()), this, SLOT(quickAccept()));
    //  button->addContent("name",PRODUCT_SCREEN);
}

CashWidget::~CashWidget(){
    delete ui;
}

void CashWidget::setNewPrice(double price){
    ui->total_receivable_value_label->clear();
    QFont font = QApplication::font();
    font.setBold(true);
    font.setPointSize(30);

    ui->total_receivable_value_label->setFont(font);
    ui->total_receivable_value_label->setText(QString::number(price,'f', 2));

    cash = 0;
    ui->to_return_value_label->setFont(font);
    ui->to_return_value_label->setText(QString::number(cash));

    ui->delivered_value_label->setFont(font);
    ui->delivered_value_label->setText(QString::number(cash));
}

void CashWidget::numkeyChangedSlot(double _cash){
    QString aux;
    aux = QString::number(_cash,'f', 2);
    ui->delivered_value_label->setText(aux);

    cash = _cash - actual_price;

    if ( cash < 0.0 ){
        ui->ok_button->setEnabled(false);
        ui->to_return_value_label->setText(QString::number(0));
        return;
    }

    ui->ok_button->setEnabled(true);
    ui->to_return_value_label->setText(QString::number(cash,'f',2));
}

void CashWidget::accepSlot(){
    if ((float_keyboard->value() - actual_price ) < 0)
        return;

    emit genericSignal(GSIGNAL::OPEN_CASHBOX);
    sendTicketSignal();
    cashing_completed = true;
    ui->cancel_button->setIcon(QPixmap("controls:button_ok_48.png"));
    float_keyboard->setEnabled(false);
    ui->ok_button->hide();
}

void CashWidget::cancelSlot(){
    XmlConfig xml;
    if (!cashing_completed){
        xml.createElement("name", SalesScreen::PRODUCT_MENU);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);
        xml.deleteElement("name");
        xml.createElement("enabled", "true");
        emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);
    }
    else{
        if (!fast_cashing){
            xml.createElement("name", MainScreen::LOGIN_SCREEN);
            emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);
        }
        else{
            xml.createElement("name", SalesScreen::PRODUCT_MENU);
            emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);

            xml.deleteElement("name");
            xml.createElement("enabled", "true");
            emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);

//            xml.deleteElement("enabled");
//            xml.createElement("visible", "true");
//            emit genericDataSignal(GDATASIGNAL::LATERALWIDGET_SET_VISIBLE, &xml);
        }
    }
}

void CashWidget::showEvent(QShowEvent *event){

    float_keyboard->clear();
    ui->ok_button->setEnabled(false);
    ui->ok_button->show();
    ui->cancel_button->setIcon(QPixmap("controls:button_cancel.png"));
    float_keyboard->setEnabled(true);
    cashing_completed = false;

    if (!barCore->hasProducts()){
        QTimer::singleShot(TIME_OUT, this, &CashWidget::cancelSlot);
        QWidget::showEvent(event);
        return;
    }

    actual_price = QString::number(barCore->total(), 'f', 2).toDouble();
    XmlConfig xml;
    xml.createElement("enabled", "false");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);

//    xml.deleteElement("enabled");
//    xml.createElement("visible", "false");
//    emit genericDataSignal(GDATASIGNAL::LATERALWIDGET_SET_VISIBLE, &xml);

    //the price is 0... nothing to pay
    if (actual_price <= 0){
        sendTicketSignal();
        xml.deleteElement("enabled");
        if (!fast_cashing){
            xml.createElement("name", MainScreen::LOGIN_SCREEN);
            emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);
        }
        else{
            xml.createElement("name", SalesScreen::PRODUCT_MENU);
            emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);
            xml.deleteElement("name");
            xml.createElement("enabled", "true");
            emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);
        }
        return;
    }

    ui->ok_button->setEnabled(false);
    setNewPrice(actual_price);
    QWidget::showEvent(event);
}

void CashWidget::sendTicketSignal(){
    XmlConfig xml ;
    xml.createElement("pay_type", "metalico");
    emit genericDataSignal(GDATASIGNAL::BARCORE_PROCESS_CORE, &xml);
}

void CashWidget::quickAccept(){
    sendTicketSignal();
}

void CashWidget::genericDataSignalSlot(const QString& signal, XmlConfig *xml){
    if ( signal == GDATASIGNAL::CASH_MENU_SPEED){
        xml->delDomain();
        QString aux = xml->readString("state");
        if (aux == "on")
            fast_cashing = true;
        else if (aux == "off")
            fast_cashing = false;
    }
}
