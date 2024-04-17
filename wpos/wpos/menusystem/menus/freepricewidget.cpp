/***************************************************************************
                          bslfreepricewidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "freepricewidget.h"
#include "salesscreen.h"

#include <wposgui/common/global.h>
#include <wposgui/keyboard/floatkeyboard.h>
#include <wposcore/genericsignalmanager.h>
#include <libbslxml/xmlconfig.h>

#include <QLayout>
#include <QFrame>
#include <QFile>
#include <QString>
#include <QPushButton>
#include <QGroupBox>
#include <QColor>

FreePriceWidget::FreePriceWidget(QWidget *parent, const QString& name) :
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    QVBoxLayout *numpad_frame_layout = new QVBoxLayout(numpad_frame);
    key_pad = new FloatKeyboard(numpad_frame);
    numpad_frame_layout->addWidget(key_pad);

    cancel_button->setIcon(QPixmap("controls:button_cancel.png"));
    ok_button->setIcon(QPixmap("controls:button_ok_48.png"));

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::XCORE_SET_PRODUCT_PRICE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::PRODSELECT_COLOR_MODE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED,this);

    // At firts sight, this dialog should have tax_16 as default option
    tax_16_button->setDown(true);
    toggleButtonsState();

    // Standart connections
    connect(ok_button, &QPushButton::released, this, &FreePriceWidget::handleAccepted);
    connect(cancel_button, &QPushButton::released, this, &FreePriceWidget::handleCancelled);
    connect(tax_16_button, &QPushButton::released, this, &FreePriceWidget::toggleButtonsState);
    connect(tax_7_button, &QPushButton::released, this, &FreePriceWidget::toggleButtonsState);
    connect(tax_4_button, &QPushButton::released, this, &FreePriceWidget::toggleButtonsState);
}

FreePriceWidget::~FreePriceWidget(){
    delete key_pad;
}

void FreePriceWidget::handleAccepted(){

    //pass the new price to the extracore
    XmlConfig xml ;
    xml.createElement("mode", "special");
    emit genericDataSignal(GDATASIGNAL::PRODSELECT_COLOR_MODE, &xml);

    xml.deleteElement("mode");
    xml.createElement("price", QString::number(key_pad->value()));

    if (tax_16_button->isDown()){
        xml.createElement("tax_type","iva16");
        xml.createElement("tax","0.16");
    }
    else if (tax_7_button->isDown()){
        xml.createElement("tax_type","iva7");
        xml.createElement("tax","0.07");
    }
    else if (tax_4_button->isDown()){
        xml.createElement("tax_type","iva4");
        xml.createElement("tax","0.04");
    }
    emit genericDataSignal(GDATASIGNAL::XCORE_SET_PRODUCT_PRICE, &xml);

    XmlConfig xml2;
    xml2.createElement("name", SalesScreen::PRODUCT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml2);

    xml2.deleteElement("name");
    xml2.createElement("enabled","true");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml2);

    key_pad->clear();
}

void FreePriceWidget::handleCancelled(){
    key_pad->clear();

    //delete the xml and create a new XML
    std::unique_ptr<XmlConfig> xml {new XmlConfig()};
    xml->delDomain();
    xml->createElement("name", SalesScreen::PRODUCT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, xml.get());

    xml->deleteElement("name");
    xml->createElement("enabled","true");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, xml.get());
}

void FreePriceWidget::toggleButtonsState(){

    for(auto child : taxes_button_group->children()){
        auto button = qobject_cast<QAbstractButton*>(child);
        if( !button ) continue;
        if( button->isDown())
            button->setPalette(QPalette(Colors::MONEY_BUTTON_DWN_COLOR));
        else
            button->setPalette(QPalette(Colors::MONEY_BUTTON_UP_COLOR));
        button->setBackgroundRole(QPalette::Button);
    }
}
