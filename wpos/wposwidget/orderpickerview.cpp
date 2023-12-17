/***************************************************************************
                          bsladvancedorderwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Jose Ignacio Ferrero-Carrera
Modified by Carlos Manzanedo Rueda.

%LICENCIA%
 ***************************************************************************/

#include "orderpickerview.h"
#include "orderheaderview.h"
#include "orderview.h"
#include "ordercontentview.h"

#include <xmlconfig.h>
#include <wposcore/genericsignalmanager.h>

#include <QPushButton>
#include <QLayout>

#include <iostream>

using namespace std;

#define ORDER_MANAGER_DESCRIPTION_XML "/etc/ntpv/advanced_order_description.xml"
#define LOGIN_SCREEN "LOGIN_SCREEN"

OrderPickerView::OrderPickerView(QWidget *parent, QString name):
    QFrame(parent)
{
    setObjectName(name);
    // Default profile for order is obtained from the description XML. <PART 2> extracting values from XML

    //attach signals
    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericSignal(GSIGNAL::BARCORE_DELETE_ACTUAL_RECEIPT,this);

    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::BARCORE_PRINT_ORDER_AT_SPECIAL_PRINTER,this);

    setSizePolicy(parent->sizePolicy());
    parseXmlDescription();

    //make the connections for all buttons
    QPushButton *button = m_buttons.find("down_button");
    if (button){
        button->setAutoRepeat(true);
        connect(button, &QPushButton::clicked, m_order->contentView(), &OrderContentView::selectDown);
    }

    button = m_buttons.find("up_button");
    if (button){
        button->setAutoRepeat(true);
        connect(button, &QPushButton::clicked, m_order->contentView(), &OrderContentView::selectUp);
    }

    button = m_buttons.find("plus_button");
    if (button){
        connect( button, &QPushButton::clicked, m_order->contentView(), &OrderContentView::incrementQuantity);
    }

    button = m_buttons.find("less_button");
    if (button){
        connect(button, &QPushButton::clicked, m_order->contentView(), &OrderContentView::decrementQuantity);
    }

    button = m_buttons.find("del_row_button");
    if (button){
        connect( button, &QPushButton::clicked, m_order->contentView(), &OrderContentView::delRow);
    }

    button = m_buttons.find("clear_order_button");
    if (button){
        connect(button, &QPushButton::clicked,this, &OrderPickerView::deleteReceiptSlot);
    }

    button = m_buttons.find("exit_mainwindow_button");
    if (button){
        connect(button, &QPushButton::clicked,this, &OrderPickerView::exitMainWindowSlot);
    }

    button = m_buttons.find("printer_order_button");
    if (button){
        connect(button, &QPushButton::clicked, m_order->contentView(), &OrderContentView::printTicket);
    }

    button = m_buttons.find("print_at_kitchen_button");
    if (button){
        connect(button, &QPushButton::clicked, this, &OrderPickerView::printAtKitchen);
    }
}

void OrderPickerView::parseXmlDescription(){

    int row{0}, col{0};

    XmlConfig xml (ORDER_MANAGER_DESCRIPTION_XML);
    if ( !QFile::exists(ORDER_MANAGER_DESCRIPTION_XML) || !xml.wellFormed() ){
        cerr << "Can't parse " << ORDER_MANAGER_DESCRIPTION_XML << "file. [CRITICAL ERROR] Can't initialize order in proper way.";
        return;
    }

    // Looks for widget background color
    QString aux_string{xml.readString("backgroundcolor")};
    if( !aux_string.isEmpty() ){
        setPalette(QPalette(QColor(aux_string)));
        setBackgroundRole(QPalette::Window);
    }

    // looks for widget height
    aux_string = xml.readString("height");
    if( !aux_string.isEmpty()) setFixedHeight(aux_string.toInt());

    // looks for widget width
    aux_string = xml.readString("width");
    if(!aux_string.isEmpty() ) setFixedWidth(aux_string.toInt());

    // Apply horizontal size policy if there is one
    aux_string = xml.readString("hsizepolicy");
    if( !aux_string.isEmpty() ){
        if(aux_string == "Expanding")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
        else if(aux_string == "Preferred")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Preferred);
        else if(aux_string == "Fixed")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
        else if(aux_string == "Minimum")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Minimum);
        else if(aux_string == "Maximum")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
    }

    // Apply vertical size policy if there is one
    aux_string = xml.readString("vsizepolicy");
    if( !aux_string.isEmpty() ){
        if(aux_string == "Expanding")
            sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
        else if(aux_string == "Preferred")
            sizePolicy().setVerticalPolicy(QSizePolicy::Preferred);
        else if(aux_string == "Fixed")
            sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
        else if(aux_string == "Minimum")
            sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
        else if(aux_string == "Maximum")
            sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);
    }

    // Create the layout for this widget
    aux_string = xml.readString("layout");
    if( !aux_string.isEmpty() ){
        if(aux_string == "HBox")
            m_main_blayout = new QHBoxLayout(this);
        else if(aux_string == "VBox")
            m_main_blayout = new QVBoxLayout(this);
        m_main_blayout->setObjectName("MAIN_LAYOUT");
    }

    // ADD HERE THE ORDER, WHEN THE MAIN LAYOUT HAS BEEN CREATED
    // Create the visual order widget
    m_order = new OrderView(this, "ORDER");
    m_main_blayout->addWidget(m_order);

    // Apply margins
    aux_string = xml.readString("margin");
    if( !aux_string.isEmpty() )
        QWidget::layout()->setMargin(aux_string.toInt());

    // and spacings for widget childs of this widget
    aux_string = xml.readString("spacing");
    if( !aux_string.isEmpty() )
        QWidget::layout()->setSpacing(aux_string.toInt());

    // Apply frame shape
    aux_string = xml.readString("frameshape");
    if( !aux_string.isEmpty() ){
        if(aux_string == "NoFrame")
            setFrameShape(QFrame::NoFrame);
        else if(aux_string == "Box")
            setFrameShape(QFrame::Box);
        else if(aux_string == "StyledPanel")
            setFrameShape(QFrame::StyledPanel);
        else if(aux_string == "HLine")
            setFrameShape(QFrame::HLine);
    }

    // and frame shadow for this widget
    aux_string = xml.readString("frameshadow");
    if( !aux_string.isEmpty() ){
        if(aux_string == "Plain")
            setFrameShadow(QFrame::Plain);
        else if(aux_string == "Raised")
            setFrameShadow(QFrame::Raised);
        else if(aux_string == "Sunken")
            setFrameShadow(QFrame::Sunken);
    }

    xml.delDomain();
    xml.setDomain("buttons");

    m_buttons_frame = new QFrame(this);
    m_buttons_frame->setObjectName("ORDER_BUTTONS_CONTAINER");
    m_main_blayout->addWidget(m_buttons_frame);
    m_buttons_glayout = new QGridLayout(m_buttons_frame);
    m_buttons_glayout->setObjectName("ORDER_BUTTONS_FRAME_LAYOUT");

    // -------------------------------------- BUTTONS CONTAINER FRAME --------------------------------------------

    // Looks for widget background color
    aux_string = xml.readString("backgroundcolor");
    if( !aux_string.isEmpty() ){
        QPalette palette;
        palette.setColor(QPalette::Window, QColor(aux_string));
        setPalette(palette);
        m_buttons_frame->setPalette(QColor(aux_string));
    }
    // looks for widget height
    aux_string = xml.readString("height");
    if(!aux_string.isEmpty())
        m_buttons_frame->setFixedHeight(aux_string.toInt());

    // looks for widget width
    //    aux_string = xml.readString("width");
    //    if(!aux_string.isEmpty())
    //        buttons_container->setFixedWidth(aux_string.toInt());

    // Apply horizontal size policy if there is one
    aux_string = xml.readString("hsizepolicy");
    if(!aux_string.isEmpty()){
        if(aux_string == "Expanding")
            m_buttons_frame->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
        else if(aux_string == "Preferred")
            m_buttons_frame->sizePolicy().setHorizontalPolicy(QSizePolicy::Preferred);
        else if(aux_string == "Fixed")
            m_buttons_frame->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
        else if(aux_string == "Minimum")
            m_buttons_frame->sizePolicy().setHorizontalPolicy(QSizePolicy::Minimum);
        else if(aux_string == "Maximum")
            m_buttons_frame->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
    }

    // Apply vertical size policy if there is one
    aux_string = xml.readString("vsizepolicy");
    if(!aux_string.isEmpty()){
        if(aux_string == "Expanding")
            m_buttons_frame->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
        else if(aux_string == "Preferred")
            m_buttons_frame->sizePolicy().setVerticalPolicy(QSizePolicy::Preferred);
        else if(aux_string == "Fixed")
            m_buttons_frame->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
        else if(aux_string == "Minimum")
            m_buttons_frame->sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
        else if(aux_string == "Maximum")
            m_buttons_frame->sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);
    }

    // Set number of rows
    int rows = 0;
    int columns = 0;
    aux_string = xml.readString("rows");
    if(!aux_string.isEmpty())
        rows = aux_string.toInt();

    // and number of columns
    aux_string = xml.readString("columns");
    if(!aux_string.isEmpty())
        columns = aux_string.toInt();

    // Apply margins
    aux_string = xml.readString("margin");
    if(!aux_string.isEmpty())
        m_buttons_frame->layout()->setMargin(aux_string.toInt());

    // and spacings for widget childs of this widget
    aux_string = xml.readString("spacing");
    if(!aux_string.isEmpty())
        m_buttons_frame->layout()->setSpacing(aux_string.toInt());

    // Apply frame shape
    aux_string = xml.readString("frameshape");
    if(!aux_string.isEmpty()){
        if(aux_string == "NoFrame")
            m_buttons_frame->setFrameShape(QFrame::NoFrame);
        else if(aux_string == "Box")
            m_buttons_frame->setFrameShape(QFrame::Box);
        else if(aux_string == "StyledPanel")
            m_buttons_frame->setFrameShape(QFrame::StyledPanel);
        else if(aux_string == "HLine")
            m_buttons_frame->setFrameShape(QFrame::HLine);
    }

    // and frame shadow for this widget
    aux_string = xml.readString("frameshadow");
    if(!aux_string.isEmpty()){
        if(aux_string == "Plain")
            m_buttons_frame->setFrameShadow(QFrame::Plain);
        else if(aux_string == "Raised")
            m_buttons_frame->setFrameShadow(QFrame::Raised);
        else if(aux_string == "Sunken")
            m_buttons_frame->setFrameShadow(QFrame::Sunken);
    }

    // -------------------------------------- BUTTONS ITSELF ------------------------------------------------------------

    // Looks for buttons and create it
    int num_labels = xml.howManyTags("button");
    for(int i=0; i< num_labels; i++){
        QString aux_name = xml.readString("button["+QString::number(i)+"].name");
        QString aux_text = xml.readString("button["+QString::number(i)+"].text");
        QString aux_pixmap = xml.readString("button["+QString::number(i)+"].pixmap");

        QPushButton *button = new QPushButton(m_buttons_frame);
        button->setObjectName(aux_name);
        m_buttons.append(button, aux_name);

        if( ! aux_text.isEmpty() )
            button->setText(aux_text);

        if(! aux_pixmap.isEmpty() ){
            button->setIcon(QPixmap(aux_pixmap));
            button->setIconSize(QSize{32,32});
        }

        // Looks for widget background color
        aux_string = xml.readString("button["+QString::number(i)+".backgroundcolor");
        if(!aux_string.isEmpty()){
            QPalette palette;
            palette.setColor(QPalette::Button, QColor(aux_string));
            button->setPalette(palette);
        }

        // looks for widget height
        aux_string = xml.readString("button["+QString::number(i)+".height");
        //        if(!aux_string.isEmpty())
        button->setFixedHeight(40);

        // looks for widget width
        //        aux_string = xml.readString("button["+QString::number(i)+".width");
        //        if(!aux_string.isEmpty())
        //            button->setFixedWidth(aux_string.toInt());

        // Apply horizontal size policy if there is one
        aux_string = xml.readString("button["+QString::number(i)+"].hsizepolicy");
        if(!aux_string.isEmpty()){
            if(aux_string == "Expanding")
                button->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
            else if(aux_string == "Preferred")
                button->sizePolicy().setHorizontalPolicy(QSizePolicy::Preferred);
            else if(aux_string == "Fixed")
                button->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
            else if(aux_string == "Minimum")
                button->sizePolicy().setHorizontalPolicy(QSizePolicy::Minimum);
            else if(aux_string == "Maximum")
                button->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
        }

        // Apply vertical size policy if there is one
        aux_string = xml.readString("button["+QString::number(i)+"].vsizepolicy");
        if(!aux_string.isEmpty()){
            if(aux_string == "Expanding")
                button->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
            else if(aux_string == "Preferred")
                button->sizePolicy().setVerticalPolicy(QSizePolicy::Preferred);
            else if(aux_string == "Fixed")
                button->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
            else if(aux_string == "Minimum")
                button->sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
            else if(aux_string == "Maximum")
                button->sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);
        }

        // Sets the font family
        aux_string = xml.readString("button["+QString::number(i)+"].fontfamily");
        QFont font;
        if(!aux_string.isEmpty()){
            if(aux_string == "SansSerif")
                font.setFamily("SansSerif");
        }

        // and font size
        aux_string = xml.readString("button["+QString::number(i)+"].fontsize");
        if(!aux_string.isEmpty())
            font.setPixelSize(aux_string.toInt());

        // and bold
        aux_string = xml.readString("button["+QString::number(i)+"].bold");
        if(!aux_string.isEmpty())
            font.setBold(true);

        // and underline
        aux_string = xml.readString("button["+QString::number(i)+"].underline");
        if(!aux_string.isEmpty())
            font.setUnderline(true);

        // and italic
        aux_string = xml.readString("button["+QString::number(i)+"].italic");
        if(!aux_string.isEmpty())
            font.setItalic(true);

        // and strikeout
        aux_string = xml.readString("button["+QString::number(i)+"].strikeout");
        if(!aux_string.isEmpty())
            font.setStrikeOut(true);

        button->setFont(font);

        // Check if this button is visible
        aux_string = xml.readString("button["+QString::number(i)+"].visible");
        if(!aux_string.isEmpty()){
            if (aux_string == "true")
                button->show();
            else
                button->hide();
        }

        // check if button is enabled
        aux_string = xml.readString("button["+QString::number(i)+"].enabled");
        if(!aux_string.isEmpty()){
            if(aux_string == "true")
                button->setEnabled(true);
            else
                button->setEnabled(false);
        }

        // Looks for position of this button in the grid
        QString aux_string_rowpos = xml.readString("button["+QString::number(i)+"].gridrowpos");
        QString aux_string_rowcol = xml.readString("button["+QString::number(i)+"].gridcolumnpos");

        if( ! aux_string_rowpos.isEmpty()  &&  !aux_string_rowcol.isEmpty()){
            row = aux_string_rowpos.toInt();
            col =  aux_string_rowcol.toInt();
        }

        // Add each button to the layout of this widget
        m_buttons_glayout->addWidget(button, row, col);
    }
}

OrderView* OrderPickerView::createOrder(QWidget* parent, XmlConfig &xml){

}

QFrame* OrderPickerView::createControls(QWidget* parent, XmlConfig &xml){

}

OrderView *OrderPickerView::order(){
    return m_order;
}

void OrderPickerView::dataChangedSlot(XmlConfig *xml){
    m_order->updateOrder(xml);
}

void OrderPickerView::exitMainWindowSlot(){
    XmlConfig xml;
    xml.createElement("name", LOGIN_SCREEN);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}

void OrderPickerView::deleteReceiptSlot(){
    emit genericSignal(GSIGNAL::BARCORE_DELETE_ACTUAL_RECEIPT);
    m_order->contentView()->reset();
    m_order->headerView()->setStaffInfo(m_order->contentView()->orderAsXml());
}

void OrderPickerView::printAtKitchen(){
    XmlConfig xml;
    xml.createElement("type","kitchen");
    emit genericDataSignal(GDATASIGNAL::BARCORE_PRINT_ORDER_AT_SPECIAL_PRINTER, &xml);
}
