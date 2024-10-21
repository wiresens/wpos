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

#include <libbslxml/xmlconfig.h>
#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>

#include <QPushButton>
#include <QLayout>

#include <iostream>

OrderPickerView::OrderPickerView(QWidget *parent, QString name):
    QFrame(parent)
{
    setObjectName(name);
    // Default profile for order is obtained from the description XML. <PART 2> extracting values from XML

    //attach signals
    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericSignal(GSIGNAL::BARCORE_DELETE_ACTUAL_RECEIPT,this);

    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE,this);
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
    auto file_path = cfg::xmlFileByKey( cfg::XMLKey::AdvancedOrders );
    XmlConfig xml ( file_path );
    if ( !QFile::exists( file_path ) || !xml.wellFormed() ){
        std::cerr << "[FATAL ERROR]: Missing  Advanced order xml file "
             << file_path.toStdString()
             << "Can't initialize order in proper way.";
        return;
    }

    // Looks for widget background color
    QString tmpStr{xml.readString("backgroundcolor")};
    if( !tmpStr.isEmpty() ){
        setPalette(QPalette(QColor(tmpStr)));
        setBackgroundRole(QPalette::Window);
    }

    // looks for widget height
    tmpStr = xml.readString("height");
    if( !tmpStr.isEmpty()) setFixedHeight(tmpStr.toInt());

    // looks for widget width
    tmpStr = xml.readString("width");
    if(!tmpStr.isEmpty() ) setFixedWidth(tmpStr.toInt());

    // Apply horizontal size policy if there is one
    tmpStr = xml.readString("hsizepolicy");
    if( !tmpStr.isEmpty() ){
        if(tmpStr == "Expanding")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
        else if(tmpStr == "Preferred")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Preferred);
        else if(tmpStr == "Fixed")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
        else if(tmpStr == "Minimum")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Minimum);
        else if(tmpStr == "Maximum")
            sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
    }

    // Apply vertical size policy if there is one
    tmpStr = xml.readString("vsizepolicy");
    if( !tmpStr.isEmpty() ){
        if(tmpStr == "Expanding")
            sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
        else if(tmpStr == "Preferred")
            sizePolicy().setVerticalPolicy(QSizePolicy::Preferred);
        else if(tmpStr == "Fixed")
            sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
        else if(tmpStr == "Minimum")
            sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
        else if(tmpStr == "Maximum")
            sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);
    }

    // Create the layout for this widget
    tmpStr = xml.readString("layout");
    if( !tmpStr.isEmpty() ){
        if(tmpStr == "HBox")
            m_main_blayout = new QHBoxLayout(this);
        else if(tmpStr == "VBox")
            m_main_blayout = new QVBoxLayout(this);
        m_main_blayout->setObjectName("MAIN_LAYOUT");
    }

    // ADD HERE THE ORDER, WHEN THE MAIN LAYOUT HAS BEEN CREATED
    // Create the visual order widget
    m_order = new OrderView(this, "ORDER");
    m_main_blayout->addWidget(m_order);

    // Apply margins
    tmpStr = xml.readString("margin");
    if( !tmpStr.isEmpty() ){
        auto margin = tmpStr.toInt();
        QWidget::layout()->setContentsMargins(margin,margin,margin,margin);
    }

    // and spacings for widget childs of this widget
    tmpStr = xml.readString("spacing");
    if( !tmpStr.isEmpty() )
        QWidget::layout()->setSpacing(tmpStr.toInt());

    // Apply frame shape
    tmpStr = xml.readString("frameshape");
    if( !tmpStr.isEmpty() ){
        if(tmpStr == "NoFrame")
            setFrameShape(QFrame::NoFrame);
        else if(tmpStr == "Box")
            setFrameShape(QFrame::Box);
        else if(tmpStr == "StyledPanel")
            setFrameShape(QFrame::StyledPanel);
        else if(tmpStr == "HLine")
            setFrameShape(QFrame::HLine);
    }

    // and frame shadow for this widget
    tmpStr = xml.readString("frameshadow");
    if( !tmpStr.isEmpty() ){
        if(tmpStr == "Plain")
            setFrameShadow(QFrame::Plain);
        else if(tmpStr == "Raised")
            setFrameShadow(QFrame::Raised);
        else if(tmpStr == "Sunken")
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
    tmpStr = xml.readString("backgroundcolor");
    if( !tmpStr.isEmpty() ){
        QPalette palette;
        palette.setColor(QPalette::Window, QColor(tmpStr));
        setPalette(palette);
        m_buttons_frame->setPalette(QColor(tmpStr));
    }
    // looks for widget height
    tmpStr = xml.readString("height");
    if(!tmpStr.isEmpty())
        m_buttons_frame->setFixedHeight(tmpStr.toInt());

    // looks for widget width
    //    aux_string = xml.readString("width");
    //    if(!aux_string.isEmpty())
    //        buttons_container->setFixedWidth(aux_string.toInt());

    // Apply horizontal size policy if there is one
    tmpStr = xml.readString("hsizepolicy");
    if(!tmpStr.isEmpty()){
        if(tmpStr == "Expanding")
            m_buttons_frame->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
        else if(tmpStr == "Preferred")
            m_buttons_frame->sizePolicy().setHorizontalPolicy(QSizePolicy::Preferred);
        else if(tmpStr == "Fixed")
            m_buttons_frame->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
        else if(tmpStr == "Minimum")
            m_buttons_frame->sizePolicy().setHorizontalPolicy(QSizePolicy::Minimum);
        else if(tmpStr == "Maximum")
            m_buttons_frame->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
    }

    // Apply vertical size policy if there is one
    tmpStr = xml.readString("vsizepolicy");
    if(!tmpStr.isEmpty()){
        if(tmpStr == "Expanding")
            m_buttons_frame->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
        else if(tmpStr == "Preferred")
            m_buttons_frame->sizePolicy().setVerticalPolicy(QSizePolicy::Preferred);
        else if(tmpStr == "Fixed")
            m_buttons_frame->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
        else if(tmpStr == "Minimum")
            m_buttons_frame->sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
        else if(tmpStr == "Maximum")
            m_buttons_frame->sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);
    }

    // Set number of rows
    int rows = 0;
    int columns = 0;
    tmpStr = xml.readString("rows");
    if(!tmpStr.isEmpty())
        rows = tmpStr.toInt();

    // and number of columns
    tmpStr = xml.readString("columns");
    if(!tmpStr.isEmpty())
        columns = tmpStr.toInt();

    // Apply margins
    tmpStr = xml.readString("margin");
    if(!tmpStr.isEmpty())
    {
        auto margin = tmpStr.toInt();
        m_buttons_frame->layout()->setContentsMargins(margin,margin,margin,margin);
    }

    // and spacings for widget childs of this widget
    tmpStr = xml.readString("spacing");
    if(!tmpStr.isEmpty())
        m_buttons_frame->layout()->setSpacing(tmpStr.toInt());

    // Apply frame shape
    tmpStr = xml.readString("frameshape");
    if(!tmpStr.isEmpty()){
        if(tmpStr == "NoFrame")
            m_buttons_frame->setFrameShape(QFrame::NoFrame);
        else if(tmpStr == "Box")
            m_buttons_frame->setFrameShape(QFrame::Box);
        else if(tmpStr == "StyledPanel")
            m_buttons_frame->setFrameShape(QFrame::StyledPanel);
        else if(tmpStr == "HLine")
            m_buttons_frame->setFrameShape(QFrame::HLine);
    }

    // and frame shadow for this widget
    tmpStr = xml.readString("frameshadow");
    if(!tmpStr.isEmpty()){
        if(tmpStr == "Plain")
            m_buttons_frame->setFrameShadow(QFrame::Plain);
        else if(tmpStr == "Raised")
            m_buttons_frame->setFrameShadow(QFrame::Raised);
        else if(tmpStr == "Sunken")
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
        tmpStr = xml.readString("button["+QString::number(i)+".backgroundcolor");
        if(!tmpStr.isEmpty()){
            QPalette palette;
            palette.setColor(QPalette::Button, QColor(tmpStr));
            button->setPalette(palette);
        }

        // looks for widget height
        tmpStr = xml.readString("button["+QString::number(i)+".height");
        //        if(!aux_string.isEmpty())
        button->setFixedHeight(40);

        // looks for widget width
        //        aux_string = xml.readString("button["+QString::number(i)+".width");
        //        if(!aux_string.isEmpty())
        //            button->setFixedWidth(aux_string.toInt());

        // Apply horizontal size policy if there is one
        tmpStr = xml.readString("button["+QString::number(i)+"].hsizepolicy");
        if(!tmpStr.isEmpty()){
            if(tmpStr == "Expanding")
                button->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
            else if(tmpStr == "Preferred")
                button->sizePolicy().setHorizontalPolicy(QSizePolicy::Preferred);
            else if(tmpStr == "Fixed")
                button->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
            else if(tmpStr == "Minimum")
                button->sizePolicy().setHorizontalPolicy(QSizePolicy::Minimum);
            else if(tmpStr == "Maximum")
                button->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
        }

        // Apply vertical size policy if there is one
        tmpStr = xml.readString("button["+QString::number(i)+"].vsizepolicy");
        if(!tmpStr.isEmpty()){
            if(tmpStr == "Expanding")
                button->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
            else if(tmpStr == "Preferred")
                button->sizePolicy().setVerticalPolicy(QSizePolicy::Preferred);
            else if(tmpStr == "Fixed")
                button->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
            else if(tmpStr == "Minimum")
                button->sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
            else if(tmpStr == "Maximum")
                button->sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);
        }

        // Sets the font family
        tmpStr = xml.readString("button["+QString::number(i)+"].fontfamily");
        QFont font;
        if(!tmpStr.isEmpty()){
            if(tmpStr == "SansSerif")
                font.setFamily("SansSerif");
        }

        // and font size
        tmpStr = xml.readString("button["+QString::number(i)+"].fontsize");
        bool ok = false;
        if (!tmpStr.isEmpty()){
            auto tmpInt = tmpStr.toInt(&ok);
            if (ok && tmpInt > 0)  font.setPixelSize(tmpInt);
        }

        // and bold
        tmpStr = xml.readString("button["+QString::number(i)+"].bold");
        if(!tmpStr.isEmpty())
            font.setBold(true);

        // and underline
        tmpStr = xml.readString("button["+QString::number(i)+"].underline");
        if(!tmpStr.isEmpty())
            font.setUnderline(true);

        // and italic
        tmpStr = xml.readString("button["+QString::number(i)+"].italic");
        if(!tmpStr.isEmpty())
            font.setItalic(true);

        // and strikeout
        tmpStr = xml.readString("button["+QString::number(i)+"].strikeout");
        if(!tmpStr.isEmpty())
            font.setStrikeOut(true);

        button->setFont(font);

        // Check if this button is visible
        tmpStr = xml.readString("button["+QString::number(i)+"].visible");
        if(!tmpStr.isEmpty()){
            if (tmpStr == "true")
                button->show();
            else
                button->hide();
        }

        // check if button is enabled
        tmpStr = xml.readString("button["+QString::number(i)+"].enabled");
        if(!tmpStr.isEmpty()){
            if(tmpStr == "true")
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

OrderView* OrderPickerView::createOrder(QWidget* /*parent*/, XmlConfig &/*xml*/){
    return nullptr;
}

QFrame* OrderPickerView::createControls(QWidget* /*parent*/, XmlConfig &/*xml*/){
    return nullptr;
}

OrderView *OrderPickerView::order(){
    return m_order;
}

void OrderPickerView::dataChangedSlot(XmlConfig *xml){
    m_order->updateOrder(xml);
}

void OrderPickerView::exitMainWindowSlot(){
    QString screen_name{"LOGIN_SCREEN"};
    XmlConfig xml;
    xml.createElement("name", screen_name);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);
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
