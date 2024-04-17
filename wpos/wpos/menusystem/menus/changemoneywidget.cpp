/***************************************************************************
                          bslchangemoneywidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "salesscreen.h"
#include "changemoneywidget.h"

#include <wposgui/common/global.h>

#include <wposgui/keyboard/floatkeyboard.h>
#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>

#include <libbslxml/xmlconfig.h>

#include <QLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QFile>
#include <QEvent>

#include <iostream>
using namespace std;



ChangeMoneyWidget::ChangeMoneyWidget(QWidget *parent, const QString& name) :
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);

    // Register signal to close this panel, wich in fact means changing to ADM_MENU
    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::EVENTLOG,this);

    QVBoxLayout *numpad_frame_layout = new QVBoxLayout(numpad_frame);
    keyPad = new FloatKeyboard(numpad_frame);

    // Initialize all images
    ok_button->setIcon(QPixmap("controls:button_ok_48.png"));
    cancel_button->setIcon(QPixmap("controls:button_cancel.png"));

    // Create the connection to the database
    db = new ChangeMoneyWidgetDB ("BslChangeMoneyWidgetConnection",Files::configFilePath("database"));

    numpad_frame_layout->addWidget(keyPad);

    // Default to add_up
    operation_type = ADD_UP;
    db->connect();
    money_in_cash = db->getMoneyInCash();
    db->disConnect();

    // Set in the current_change_label the current cash
    current_change_label->setText (QString::number (money_in_cash)+tr(" E","€"));

    // Set new_change_label to money_in_cash euros on startup
    new_change_label->setText(QString::number (money_in_cash)+tr(" E","€"));

    // Set total_change_label to money_in_cash euros on startup
    total_change_label->setText (QString::number (money_in_cash)+ tr(" E","€"));

    // Establish standart connections
    connect(keyPad, &FloatKeyboard::valueChanged,       this, &ChangeMoneyWidget::updateChangeSlot);
    connect(ok_button, &QPushButton::clicked,           this, &ChangeMoneyWidget::acceptChangeSlot);
    connect(cancel_button, &QPushButton::clicked,       this, &ChangeMoneyWidget::rejectChangeSlot);
    connect(sustractMoneyButton, &QPushButton::toggled, this, &ChangeMoneyWidget::sustractMoneySlot);
    connect(addMoneyButton, &QPushButton::toggled,      this, &ChangeMoneyWidget::addMoneySlot);
}

void ChangeMoneyWidget::updateChangeSlot(double _change){
    double total=0.0;
    double change;


    variation = _change;
    variation_change_label->setText(QString::number(variation,'f',2)+tr(" E","€"));

    db->connect();
    money_in_cash = db->getMoneyInCash();
    change = db->getMoneyPayType("metalico");
    db->disConnect();
    change = money_in_cash - change;


    switch (operation_type) {
    case ADD_UP:
        total = money_in_cash + variation;
        change_at_cashbox_label->setText(QString::number(change+_change,'f',2));
        break;
    case SUBSTRACT:
        total = money_in_cash - variation;
        change_at_cashbox_label->setText(QString::number(change-_change,'f',2));
        break;
    }
    total_change_label->setText(QString::number(total,'f',2)+tr(" E","€"));
}

void ChangeMoneyWidget::acceptChangeSlot(){
    QString operation;
    double var = variation;
    XmlConfig *xml = 0;

    xml = new XmlConfig();
    operation = "money_add";
    if (operation_type == SUBSTRACT){
        operation = "money_substract";
        var *= -1;
    }

    xml->delDomain();
    xml->createElement("event_type",operation);
    xml->createElement("cash_quantity",QString::number(var,'f',2));
    emit genericDataSignal(GDATASIGNAL::EVENTLOG,xml);
    delete xml;
    xml = 0;

    xml = new XmlConfig();
    //   Prepare an xml to emit a signal that means change to ADM_MENU
    xml->delDomain();
    xml->createElement("name", SalesScreen::ADMIN_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE,xml);
    delete xml;
    xml = 0;
}

void ChangeMoneyWidget::rejectChangeSlot(){
    XmlConfig *xml = 0;
    xml = new XmlConfig();
    // Prepare an xml to emit a signal that means change to ADM_MENU
    xml->createElement("name", SalesScreen::ADMIN_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE,xml);
    delete xml;
}

void ChangeMoneyWidget::sustractMoneySlot(){
    // Set visual elements in this dialog to reflect a sustract money action
    keyPad->setEnabled(true);
    toggleButtonsStateSlot();
    oper_sign_label->setText("-");

    // Set the operation type to be SUSTRACT money from cash
    operation_type = SUBSTRACT;

    // Call the update
    updateChangeSlot (keyPad->value());
}

void ChangeMoneyWidget::addMoneySlot(){
    // Set visual elements in this dialog to reflect an add money action
    keyPad->setEnabled(true);
    toggleButtonsStateSlot();
    oper_sign_label->setText("+");

    // Set the operation type to be ADD money from cash
    operation_type = ADD_UP;

    // Call the update
    updateChangeSlot(keyPad->value());
}

void ChangeMoneyWidget::showEvent(QShowEvent *event){
    double change;
    // Restore initial visual state
    keyPad->clear();
    keyPad->setEnabled(false);
    addMoneyButton->setDown(false);
    sustractMoneyButton->setDown(false);
    toggleButtonsStateSlot();

    // Get how many money is in the cash
    db->connect();
    money_in_cash = db->getMoneyInCash();
    change = db->getMoneyPayType("metalico");
    db->disConnect();

    change = money_in_cash - change;

    // Set labels to a reset state, to give user the sensation of a new opening of a dialog
    current_change_label->setText(QString::number(money_in_cash,'f',2)+ tr(" €","€"));
    new_change_label->setText(QString::number(money_in_cash,'f',2));
    variation_change_label->setText("0.00");
    total_change_label->setText(QString::number(money_in_cash,'f',2)+tr(" €","€"));
    money_at_cashbox_label->setText(QString::number(money_in_cash,'f',2)+tr(" €","€"));
    change_at_cashbox_label->setText(QString::number(change,'f',2)+tr(" €","€"));

    QWidget::showEvent(event);
}

void ChangeMoneyWidget::toggleButtonsStateSlot(){

    for(auto child : ButtonGroup1->children()){
        auto button = qobject_cast<QPushButton *>(child);
        if( !button) continue;

        if( ! button->isChecked() )
            button->setPalette(QPalette(Colors::MONEY_BUTTON_DWN_COLOR));
        else
            button->setPalette(QPalette(Colors::MONEY_BUTTON_UP_COLOR));
        button->setBackgroundRole(QPalette::Button);
    }
}


