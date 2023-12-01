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
#include <iwidgetconfighelper.h>

#include <QLayout>
#include <QLabel>

#include <xmlconfig.h>
#include <iostream>

using namespace std;

OrderFooterView::OrderFooterView(
    XmlConfig *xml,
    QWidget *parent,
    const QString& name)
    :QFrame(parent)
{
    setObjectName(name);
    parseXmlDescription(xml);
    labels[2]->setVisible(false); //We hide the currency  symbol for the time being.
}

void OrderFooterView::parseXmlDescription(XmlConfig *xml){

    IWidgetConfigHelper widgetConfigHelper;
    xml->delDomain(); //releaseDomain() : Position ourself at the root of the XML tree
    xml->setDomain("totaldescription.global");
    auto aux_string = xml->readString("backgroundcolor");
    if(!aux_string.isEmpty()) widgetConfigHelper.setBackgroundColor(*this, QColor(aux_string));

    widgetConfigHelper.setSize(*this, width(), 50);

    auto hsizepolicy = xml->readString("hsizepolicy");
    auto vsizepolicy = xml->readString("vsizepolicy");
    widgetConfigHelper.setSizePolicy(*this, hsizepolicy, vsizepolicy);

    layout = new QHBoxLayout(this);

    aux_string = xml->readString("margin");
    if (!aux_string.isEmpty())
        layout->setMargin(aux_string.toInt());

    aux_string = xml->readString("spacing");
    if (!aux_string.isEmpty())
        layout->setSpacing(aux_string.toInt());

    widgetConfigHelper.setFrameShape(*this, xml->readString("frameshape"));
    widgetConfigHelper.setFrameShadow(*this, xml->readString("frameshadow"));

    xml->delDomain();
    xml->setDomain("totaldescription.labels"); //goto

    // Looks for labels and create them
    for(auto  i=0; i < xml->howManyTags("label") ; i++){
        QString aux_name = xml->readString("label["+QString::number(i)+"].name");
        QString aux_text = xml->readString("label["+QString::number(i)+"].text");

        QLabel* label = new QLabel(this);
        label->setObjectName(aux_name);
        labels.append(label, aux_name);
        layout->addWidget(label);
        if(!aux_text.isEmpty()) label->setText(aux_text);

        aux_string = xml->readString("label["+QString::number(i)+".backgroundcolor");
        if(!aux_string.isEmpty()) widgetConfigHelper.setBackgroundColor(*this, QColor(aux_string));

        auto width_str = xml->readString("label["+QString::number(i)+".width");
        auto height_str = xml->readString("label["+QString::number(i)+".height");
        if ( !width_str.isEmpty() || !height_str.isEmpty())
            widgetConfigHelper.setSize(*label, width_str.toInt(), height_str.toInt());

        auto halign = xml->readString("label["+QString::number(i)+"].halign");
        auto valign = xml->readString("label["+QString::number(i)+"].valign");
        widgetConfigHelper.setAlignment(*label, halign, valign);

        auto hsizepolicy = xml->readString("label["+QString::number(i)+"].hsizepolicy");
        auto vsizepolicy = xml->readString("label["+QString::number(i)+"].vsizepolicy");

        auto  hstretch = xml->readString("label["+QString::number(i)+"].hstretch").toInt();
        auto  vstretch = xml->readString("label["+QString::number(i)+"].vstretch").toInt();
        widgetConfigHelper.setSizePolicy(*label, hsizepolicy, vsizepolicy, hstretch, vstretch);

        // Sets the font
        QFont font("SansSerif");
        aux_string = xml->readString("label["+QString::number(i)+"].fontfamily");
        font.setFamily(aux_string);

        aux_string = xml->readString("label["+QString::number(i)+"].fontsize");
        font.setPointSize(aux_string.toInt() + 1);

        aux_string = xml->readString("label["+QString::number(i)+"].bold");
        font.setBold(  aux_string == "true" );

        aux_string = xml->readString("label["+QString::number(i)+"].underline");
        font.setUnderline(aux_string == "true");

        aux_string = xml->readString("label["+QString::number(i)+"].italic");
        font.setItalic(aux_string == "true");

        aux_string = xml->readString("label["+QString::number(i)+"].strikeout");
        font.setStrikeOut(aux_string == "true");
        label->setFont(font);
    }
}

void OrderFooterView::totalBillSlot(float amount){
    for( auto label :  labels){
        if( label->objectName() == "total_amount")
            label->setText(QString::number( amount, 'f', 2));
    }
}
