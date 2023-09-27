/***************************************************************************
                          bslgenericproduct.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "genericproduct.h"

#include <wposwidget/global.h>
#include <wposwidget/floatkeyboardbox.h>
#include <wposcore/genericsignalmanager.h>

#include "salesscreen.h"

#include <libbslxml/xmlconfig.h>

#include <QLayout>
#include <QFrame>
#include <QFile>
#include <QString>
#include <QPushButton>
#include <QGroupBox>
#include <QColor>

GenericProduct::GenericProduct(const QString& product_name,
                                     QWidget *parent,
                                     const QString& name):
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    QVBoxLayout *numpad_frame_layout = new QVBoxLayout(numpad_frame);
    key_pad = new FloatKeyboardBox(numpad_frame);
    numpad_frame_layout->addWidget(key_pad);

    generic_product_name = product_name;
    cancel_button->setIcon(QPixmap("/usr/share/ntpv/apps/button_cancel.png"));
    ok_button->setIcon(QPixmap("/usr/share/ntpv/apps/button_ok_48.png"));

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::XCORE_SET_PRODUCT_PRICE, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::BARCORE_SEND_PRODUCT, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, this);


    // At firts sight, this dialog should be have tax_16 as default option
    tax_16_button->setDown(true);
    toggleButtonsState();

    // Standart connections
    /**
     *    AS AT BslFreePriceProduct, the tax types should be read from the db and the program
     *     should create a button for each type of tax... in spain main taxes for pubs and bar are
     *    16% 4% and 7% and there are hardcoded
     */
    connect(ok_button,SIGNAL(released()),this,SLOT(handleAccepted()));
    connect(cancel_button,SIGNAL(released()),this,SLOT(handleCancelled()));
    connect(tax_16_button,SIGNAL(released()),this,SLOT(toggleButtonsState()));
    connect(tax_7_button, SIGNAL(released()),this,SLOT(toggleButtonsState()));
    connect(tax_4_button, SIGNAL(released()),this,SLOT(toggleButtonsState()));

}

GenericProduct::~GenericProduct(){}

void GenericProduct::handleAccepted(){

    //delete the xml and create a new XML
    //pass to the extracore the new price
    {
        XmlConfig xml ;
        xml.createElement("price", QString::number(key_pad->getNumber()));
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
    }

    {
        XmlConfig xml ;
        xml.createElement("name", SalesScreen::PRODUCT_MENU);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
    }

    {
        XmlConfig xml ;
        xml.createElement("enabled", "true");
        emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);
    }

    //pass to the extracore the new price
    {
        XmlConfig xml ;
        xml.createElement("product.articles.article.name",generic_product_name);
        emit genericDataSignal(GDATASIGNAL::BARCORE_SEND_PRODUCT, &xml);
    }

    key_pad->clear();
}

void GenericProduct::handleCancelled(){

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

void GenericProduct::toggleButtonsState(){

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
