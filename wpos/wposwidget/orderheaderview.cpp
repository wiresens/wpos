/***************************************************************************
                          bslorderheaderwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Jose Ignacio Ferrero-Carrera
Modified by Carlos Manzanedo Rueda.

%LICENCIA%
 ***************************************************************************/

#include "orderheaderview.h"
#include <wposwidget/iwidgetconfighelper.h>

#include <QLayout>
#include <QLabel>
#include <QPixmap>
#include <QColor>
#include <QSizePolicy>
#include <QFont>
#include <QApplication>

#include <xmlconfig.h>

#include <iostream>
using namespace std;

OrderHeaderView::OrderHeaderView(
    XmlConfig &headerXmlDesc,
    QWidget *parent,
    QString name) :
    QFrame(parent)
{
    setObjectName(name);

    // Default profile for order is obtained from the description
    //XML <PART 2> Extracting values from XML
    parseXmlDescription(headerXmlDesc);
}

void OrderHeaderView::parseXmlDescription(XmlConfig &headerXmlDesc) {
    IWidgetConfigHelper widgetConfigurer;

    headerXmlDesc.pushDomain();
    headerXmlDesc.delDomain();
    headerXmlDesc.setDomain("headerdescription.global");
    widgetConfigurer.setBackgroundColor(*this, QColor(headerXmlDesc.readString("backgroundcolor")));

    // Apply  size policy if there is one
    auto hsizepolicy = headerXmlDesc.readString("hsizepolicy");
    auto vsizepolicy = headerXmlDesc.readString("vsizepolicy");
    widgetConfigurer.setSizePolicy(*this, hsizepolicy, vsizepolicy);

    //add the photo
    headerXmlDesc.delDomain();
    headerXmlDesc.setDomain("headerdescription.photo");
    QString text = headerXmlDesc.readString("text");
    QString pixmap = headerXmlDesc.readString("pixmap");
    photoLabel = new QLabel(this);

    photoLabel = photoLabel;


    labelsLayout = new QHBoxLayout(this);
    labelsLayout->addWidget(photoLabel);

    if (!text.isEmpty()) photoLabel->setText(text);
    if (!pixmap.isEmpty()) photoLabel->setPixmap(QPixmap(pixmap));

    auto aux_string = headerXmlDesc.readString("backgroundcolor");
    if(!aux_string.isEmpty())
        widgetConfigurer.setBackgroundColor(*photoLabel, QColor(aux_string));

    // Set pthoto size
    auto height = headerXmlDesc.readString("height");
    auto width = headerXmlDesc.readString("width");
    widgetConfigurer.setSize(*photoLabel, width.toInt(), height.toInt());

    //Align of photo label
    auto halign = headerXmlDesc.readString("halign");
    auto valign = headerXmlDesc.readString("valign");
    widgetConfigurer.setAlignment(*photoLabel, halign, valign);

    //Apply  size policy if there is one
    hsizepolicy = headerXmlDesc.readString("hsizepolicy");
    vsizepolicy = headerXmlDesc.readString("vsizepolicy");
    widgetConfigurer.setSizePolicy(*photoLabel, hsizepolicy, vsizepolicy);

    widgetConfigurer.setFont(*photoLabel, headerXmlDesc);

    // Add each label to the layout of this widget
    headerXmlDesc.delDomain();
    headerXmlDesc.setDomain("headerdescription.global");

    //add the frame
    textFrame = new QFrame(this);
    textFrame->setObjectName("headerlabelFrame");
    labelsLayout->addWidget(textFrame);

    // Create the layout for this widget
    aux_string = headerXmlDesc.readString("layout");
    if( !aux_string.isEmpty()){
        if(aux_string == "HBox")      textLayout = new QHBoxLayout(textFrame);
        else if(aux_string == "VBox") textLayout = new QVBoxLayout(textFrame);
    }

    // Apply margins
    auto margin = headerXmlDesc.readString("margin");
    auto spacing = headerXmlDesc.readString("spacing");
    widgetConfigurer.setLayoutMargin(*textFrame, margin.toInt());
    widgetConfigurer.setLayoutSpacing(*textFrame, spacing.toInt());

    widgetConfigurer.setFrameShape(*textFrame, headerXmlDesc.readString("frameshape"));
    widgetConfigurer.setFrameShadow(*textFrame, headerXmlDesc.readString("frameshadow"));

    headerXmlDesc.delDomain();
    headerXmlDesc.setDomain("headerdescription.labels");

    // Looks for labels and create them
    for(auto i=0; i < headerXmlDesc.howManyTags("label"); i++){
        QString name = headerXmlDesc.readString("label["+QString::number(i)+"].name");
        QString text = headerXmlDesc.readString("label["+QString::number(i)+"].text");
        QString pixmap = headerXmlDesc.readString("label["+QString::number(i)+"].pixmap");

        QLabel *textLabel = new QLabel(textFrame);

        if( name == "staff_identity") identityLabel = textLabel;
        else if( name == "order_description") descLabel = textLabel;

        textLayout->addWidget(textLabel);

        if( !text.isEmpty())  textLabel->setText(text);
        if( !pixmap.isEmpty()) textLabel->setPixmap(QPixmap(pixmap));

        QString backgroundcolor = headerXmlDesc.readString("label["+QString::number(i)+".backgroundcolor");
        widgetConfigurer.setBackgroundColor(*textLabel, QColor(backgroundcolor));

        width = headerXmlDesc.readString("label["+QString::number(i)+".width");
        height = headerXmlDesc.readString("label["+QString::number(i)+".height");
        widgetConfigurer.setSize(*textLabel, width.toInt(), height.toInt());

        //Align of labels
        halign = headerXmlDesc.readString("label["+QString::number(i)+"].halign");
        valign = headerXmlDesc.readString("label["+QString::number(i)+"].valign");
        widgetConfigurer.setAlignment(*textLabel, halign, valign);

        // Apply size policy if there is one
        hsizepolicy = headerXmlDesc.readString("label["+QString::number(i)+"].hsizepolicy");
        vsizepolicy = headerXmlDesc.readString("label["+QString::number(i)+"].vsizepolicy");
        widgetConfigurer.setSizePolicy(*textLabel, hsizepolicy, vsizepolicy);

        widgetConfigurer.setFont(*textLabel, headerXmlDesc, i);
    }

    headerXmlDesc.popDomain();
}

void OrderHeaderView::showPhoto(bool show_it){}

void OrderHeaderView::setStaffInfo(XmlConfig *xml){
    xml->pushDomain();
    xml->delDomain();

    QString name = xml->readString("employee.name");
    QString order_description = xml->readString("description");
    xml->popDomain();

    if(name.isEmpty()) return;

    QFont font = QApplication::font();
    font.setBold(true);

    identityLabel->setFont(font);
    identityLabel->setText(name);

    if ( !order_description.isEmpty()){
        descLabel->show();
        descLabel->setText("Table : "+order_description);
    }
    else
        descLabel->hide();

}
