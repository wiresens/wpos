/***************************************************************************
                          bslorderwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Jose Ignacio Ferrero-Carrera
Modified by Carlos Manzanedo Rueda.

%LICENCIA%
 ***************************************************************************/

#include "orderview.h"
#include "orderheaderview.h"
#include "ordercontentview.h"
#include "orderfooterview.h"

#include <xmlconfig.h>
#include <wposcore/config.h>

#include <QFile>
#include <QLayout>
#include <QColor>

#include <iostream>

using namespace std;

OrderView::OrderView(QWidget *parent, const QString& name):
    QFrame(parent)
{
    setObjectName(name);
    auto ordersConfig = Files::configFilePath("orders");
    XmlConfig xml (ordersConfig);

    if ( !QFile::exists(ordersConfig) || !xml.wellFormed() ){
        cerr << "Can't find " << ordersConfig.toStdString() << " file. [CRITICAL ERROR] Can't initialize order in proper way.";
        return;
    }

    parseXmlDescription(&xml);
    setSizePolicy(parent->sizePolicy());

    order_header = new OrderHeaderView(&xml, this, "ORDER_HEADER");
    order_content = new OrderContentView(&xml, this, "ORDER_TABLE");
    order_total = new OrderFooterView(&xml, this, "ORDER_TOTAL");

    order_header->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    order_content->setSizePolicy(sizePolicy());
    order_total->setSizePolicy(order_header->sizePolicy());

    layout->addWidget(order_header);
    layout->addWidget(order_content);
    layout->addWidget(order_total);

    // Some signals connections
    connect(order_content, &OrderContentView::totalBillSignal, order_total, &OrderFooterView::totalBillSlot);
    connect(order_content, &OrderContentView::dataChanged, this, &OrderView::dataChanged);
}

void OrderView::parseXmlDescription(XmlConfig *xml){
    bool ok;
    int aux_num;
    QString aux_string = "";

    // Looks for widget background color
    aux_string = xml->readString("backgroundcolor");
    if (!aux_string.isEmpty()){      //FIXME , check if the aux_string can be converted into QColor
        QPalette palette;
        palette.setColor(backgroundRole(), QColor(aux_string));
        setPalette(palette);
    }

    // looks for widget height
    aux_string = xml->readString("height");
    aux_num = aux_string.toInt(&ok);
    if (ok)
        setFixedHeight(aux_num);

    // looks for widget width
    aux_string = xml->readString("width");
    aux_num = aux_string.toInt(&ok);
//    if (ok) setFixedWidth(aux_num);

    // Apply horizontal size policy if there is one
    aux_string = (xml->readString("hsizepolicy")).toLower();
    if (!aux_string.isEmpty()){
        if (aux_string == "expanding")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
        else if (aux_string == "preferred")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Preferred);
        else if (aux_string == "fixed")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
        else if (aux_string == "minimum")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Minimum);
        else if (aux_string == "maximum")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
    }

    // Apply vertical size policy if there is one
    aux_string = (xml->readString("vsizepolicy")).toLower();
    if (!aux_string.isEmpty()){
        if (aux_string == "expanding")
            sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
        else if (aux_string == "preferred")
            sizePolicy().setVerticalPolicy(QSizePolicy::Preferred);
        else if ( aux_string == "fixed")
            sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
        else if ( aux_string == "minimum")
            sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
        else if ( aux_string == "maximum")
            sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);
    }

    // Create the layout for this widget
    aux_string = (xml->readString("layout")).toLower();
    if (!aux_string.isEmpty()){
        if(aux_string == "hbox")
            layout = new QHBoxLayout(this);
        else if(aux_string == "vbox")
            layout = new QVBoxLayout(this);
    }

    // Apply margins
    aux_string = xml->readString("margin");
    aux_num = aux_string.toInt(&ok);
    if (ok)
        QWidget::layout()->setMargin(aux_num);

    // and spacings for widget childs of this widget
    aux_string = xml->readString("spacing");
    aux_num = aux_string.toInt(&ok);
    if (ok)
        QWidget::layout()->setSpacing(aux_num);

    // Apply frame shape
    aux_string = (xml->readString("frameshape")).toLower();
    if (!aux_string.isEmpty()){
        if (aux_string == "noframe")
            setFrameShape(QFrame::NoFrame);
        else if (aux_string == "box")
            setFrameShape(QFrame::Box);
        else if (aux_string == "styledpanel")
            setFrameShape(QFrame::StyledPanel);
        else if (aux_string == "hline")
            setFrameShape(QFrame::HLine);
    }

    // and frame shadow for this widget
    aux_string = (xml->readString("frameshadow")).toLower();
    if (!aux_string.isEmpty()){
        if(aux_string == "plain")
            setFrameShadow(QFrame::Plain);
        else if(aux_string == "raised")
            setFrameShadow(QFrame::Raised);
        else if(aux_string == "sunken")
            setFrameShadow(QFrame::Sunken);
    }
}

void OrderView::changeData(XmlConfig *xml){
    order_header->setStaffInfo(xml);
    order_content->updateProductsInfo(xml);
}

OrderContentView *OrderView::orderContent(){
    return order_content;
}

OrderHeaderView *OrderView::orderHeader(){
    return order_header;
}
