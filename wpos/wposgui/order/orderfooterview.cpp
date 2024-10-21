/***************************************************************************
                          bslordertotalwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Jose Ignacio Ferrero-Carrera
Modified by Carlos Manzanedo Rueda.

%LICENCIA%
 ***************************************************************************/

#include "orderfooterview.h"
#include "common/iwidgetconfighelper.h"

#include <QLayout>
#include <QLabel>

#include <libbslxml/xmlconfig.h>
#include <iostream>

using namespace std;

OrderFooterView::OrderFooterView(
    XmlConfig &footerXmlDesc,
    QWidget *parent,
    const QString& name)
    :QFrame(parent)
{
    setObjectName(name);
    parseXmlDescription(footerXmlDesc);
    currencyLabel->setVisible(false); //We hide the currency  symbol for the time being.
}

void OrderFooterView::parseXmlDescription(XmlConfig &footerXmlDesc){

    IWidgetConfigHelper widgetConfigurer;
    footerXmlDesc.delDomain(); //releaseDomain() : Position ourself at the root of the XML tree
    footerXmlDesc.setDomain("totaldescription.global");
    auto aux_string = footerXmlDesc.readString("backgroundcolor");
    if(!aux_string.isEmpty()) widgetConfigurer.setBackgroundColor(*this, QColor(aux_string));

    widgetConfigurer.setSize(*this, width(), 50);

    auto hsizepolicy = footerXmlDesc.readString("hsizepolicy");
    auto vsizepolicy = footerXmlDesc.readString("vsizepolicy");
    widgetConfigurer.setSizePolicy(*this, hsizepolicy, vsizepolicy);

    layout = new QHBoxLayout(this);

    auto margin = footerXmlDesc.readString("margin");
    auto spacing = footerXmlDesc.readString("spacing");

    widgetConfigurer.setLayoutMargin(*this, margin.toInt());
    widgetConfigurer.setLayoutSpacing(*this, spacing.toInt());

    widgetConfigurer.setFrameShape(*this, footerXmlDesc.readString("frameshape"));
    widgetConfigurer.setFrameShadow(*this, footerXmlDesc.readString("frameshadow"));

    footerXmlDesc.delDomain();
    footerXmlDesc.setDomain("totaldescription.labels"); //goto

    // Looks for labels and create them
    for(auto  i=0; i < footerXmlDesc.howManyTags("label") ; i++){
        QString name = footerXmlDesc.readString("label["+QString::number(i)+"].name");
        QString text = footerXmlDesc.readString("label["+QString::number(i)+"].text");

        QLabel* label = new QLabel(this);
        if( name == "total_amount") amountLabel = label;
        else if( name == "currency_symbol") currencyLabel = label;

        layout->addWidget(label);
        if(!text.isEmpty()) label->setText(text);

        auto backgroundcolor = footerXmlDesc.readString("label["+QString::number(i)+".backgroundcolor");
        widgetConfigurer.setBackgroundColor(*label, QColor(backgroundcolor));

        auto width = footerXmlDesc.readString("label["+QString::number(i)+".width");
        auto height = footerXmlDesc.readString("label["+QString::number(i)+".height");
        widgetConfigurer.setSize(*label, width.toInt(), height.toInt());

        auto halign = footerXmlDesc.readString("label["+QString::number(i)+"].halign");
        auto valign = footerXmlDesc.readString("label["+QString::number(i)+"].valign");
        widgetConfigurer.setAlignment(*label, halign, valign);

        auto hsizepolicy = footerXmlDesc.readString("label["+QString::number(i)+"].hsizepolicy");
        auto vsizepolicy = footerXmlDesc.readString("label["+QString::number(i)+"].vsizepolicy");

        auto  hstretch = footerXmlDesc.readString("label["+QString::number(i)+"].hstretch").toInt();
        auto  vstretch = footerXmlDesc.readString("label["+QString::number(i)+"].vstretch").toInt();
        widgetConfigurer.setSizePolicy(*label, hsizepolicy, vsizepolicy, hstretch, vstretch);

        widgetConfigurer.setFont(*label, footerXmlDesc, i);
    }
}

void OrderFooterView::setTotal(float amount){
    amountLabel->setText(QString::number( amount, 'f', 2));
}
