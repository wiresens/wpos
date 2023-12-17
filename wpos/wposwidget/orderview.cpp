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

#include <wposwidget/iwidgetconfighelper.h>
#include <libbslxml/xmlconfig.h>
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
    XmlConfig orderXmlDesc (ordersConfig);

    if ( !QFile::exists(ordersConfig) || !orderXmlDesc.wellFormed() ){
        cerr << "CRITICAL ERROR : file " << ordersConfig.toStdString()
            << " not found. Cannot properly initialize order.";
        return;
    }

    parseXmlDescription(orderXmlDesc);
    setSizePolicy(parent->sizePolicy());

    m_header  = new OrderHeaderView(orderXmlDesc, this, "ORDER_HEADER");
    m_content = new OrderContentView(orderXmlDesc, this, "ORDER_TABLE");
    m_footer  = new OrderFooterView(orderXmlDesc, this, "ORDER_TOTAL");

    m_header->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    m_content->setSizePolicy(sizePolicy());
    m_footer->setSizePolicy(m_header->sizePolicy());

    layout->addWidget(m_header);
    layout->addWidget(m_content);
    layout->addWidget(m_footer);

    // Some signals connections
    connect(m_content, &OrderContentView::totalChanged, m_footer, &OrderFooterView::setTotal);
    connect(m_content, &OrderContentView::contentChanged, this, &OrderView::orderChanged);
}

void OrderView::parseXmlDescription(XmlConfig &orderXmlDesc){
    IWidgetConfigHelper widgetConfigurer;

    // Looks for widget background color
    auto backgroundcolor = orderXmlDesc.readString("backgroundcolor");
    if(!backgroundcolor.isEmpty()) widgetConfigurer.setBackgroundColor(*this, QColor(backgroundcolor));

    //Set widget size
    auto height = orderXmlDesc.readString("height");
    auto width = orderXmlDesc.readString("width");
    widgetConfigurer.setSize(*this, width.toInt(), height.toInt());

    // Apply  size policy if there is one
    auto hsizepolicy = orderXmlDesc.readString("hsizepolicy");
    auto vsizepolicy = orderXmlDesc.readString("vsizepolicy");
    widgetConfigurer.setSizePolicy(*this, hsizepolicy, vsizepolicy);

    // Create the layout for this widget
    auto tmpString = orderXmlDesc.readString("layout").toLower();
    if( tmpString == "hbox") layout = new QHBoxLayout(this);
    else layout = new QVBoxLayout(this);

    // Apply margins and spacing
    widgetConfigurer.setLayoutMargin(*this, orderXmlDesc.readString("margin").toInt());
    widgetConfigurer.setLayoutSpacing(*this, orderXmlDesc.readString("spacing").toInt());

    // Apply frame shape and shadow
    widgetConfigurer.setFrameShape(*this, orderXmlDesc.readString("frameshape"));
    widgetConfigurer.setFrameShadow(*this, orderXmlDesc.readString("frameshadow"));
}

void OrderView::updateOrder(XmlConfig *xml){
    m_header->setStaffInfo(xml);
    m_content->updateProductsInfo(xml);
}
