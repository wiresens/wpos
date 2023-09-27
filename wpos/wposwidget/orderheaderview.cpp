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

OrderHeaderView::OrderHeaderView(XmlConfig *xml, QWidget *parent, QString name) :
    QFrame(parent),
    widgets  {new HList<QLabel>()}
{
    setObjectName(name);

    // Default profile for order is obtained from the description
    //XML <PART 2> Extracting values from XML
    parseXmlDescription(xml);
}

OrderHeaderView::~OrderHeaderView(){}

void OrderHeaderView::parseXmlDescription(XmlConfig *xml) {

    total_layout = new QHBoxLayout(this);
    xml->pushDomain();

    xml->delDomain();
    xml->setDomain("headerdescription.global");

    // Looks for widget background color
    QString aux_string = xml->readString("backgroundcolor");
    if(!aux_string.isEmpty()){
        QPalette palette;
        palette.setColor(backgroundRole(), QColor(aux_string));
        setPalette(palette);
    }

//    // looks for widget height
//    aux_string = xml->readString("height");
//    if (!aux_string.isEmpty())
//        setFixedHeight(aux_string.toInt());

//    // looks for widget width
//    aux_string = xml->readString("width");
//    if (!aux_string.isEmpty())
//        setFixedWidth(aux_string.toInt());

    // Apply horizontal size policy if there is one
    aux_string = xml->readString("hsizepolicy");
    if(!aux_string.isEmpty()){
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
    aux_string = xml->readString("vsizepolicy");
    if(!aux_string.isEmpty()){
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

    //add the photo
    xml->delDomain();
    xml->setDomain("headerdescription.photo");
    QString aux_name = xml->readString("name");
    QString aux_text = xml->readString("text");
    QString aux_pixmap = xml->readString("pixmap");
    QLabel *label = new QLabel(this);
    label->setObjectName(aux_name);
    total_layout->addWidget(label);
    widgets->append(label, aux_name);

    if (!aux_text.isEmpty())
        label->setText(aux_text);

    if (!aux_pixmap.isEmpty())
        label->setPixmap(QPixmap(aux_pixmap));

    aux_string = xml->readString("backgroundcolor");
    if(!aux_string.isEmpty()){
        QPalette palette;
        palette.setColor(backgroundRole(), QColor(aux_string));
        label->setPalette(palette);
    }
    // looks for widget height
    aux_string = xml->readString("height");
    if (!aux_string.isEmpty())
        label->setFixedHeight(aux_string.toInt());

    // looks for widget width
//    aux_string = xml->readString("width");
//    if (!aux_string.isEmpty())
//        label->setFixedWidth(aux_string.toInt());

    // Horizontal align of labels
    aux_string = xml->readString("halign");
    if (!aux_string.isEmpty()){
        if(aux_string == "Left")
            label->setAlignment(Qt::AlignLeft);
        else if(aux_string == "Center")
            label->setAlignment(Qt::AlignHCenter);
        else if(aux_string == "Right")
            label->setAlignment(Qt::AlignRight);
    }

    // Vertical align of labels
    aux_string = xml->readString("valign");
    if (!aux_string.isEmpty()){
        if(aux_string == "Top")
            label->setAlignment(Qt::AlignTop);
        else if(aux_string == "Center")
            label->setAlignment(Qt::AlignVCenter);
        else if(aux_string == "Bottom")
            label->setAlignment(Qt::AlignBottom);
    }

    // Apply horizontal size policy if there is one
    aux_string = xml->readString("hsizepolicy");
    if (!aux_string.isEmpty()){
        if(aux_string == "Expanding")
            label->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
        else if(aux_string == "Preferred")
            label->sizePolicy().setHorizontalPolicy(QSizePolicy::Preferred);
        else if(aux_string == "Fixed")
            label->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
        else if(aux_string == "Minimum")
            label->sizePolicy().setHorizontalPolicy(QSizePolicy::Minimum);
        else if(aux_string == "Maximum")
            label->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
    }

    // Apply vertical size policy if there is one
    aux_string = xml->readString("vsizepolicy");
    if (!aux_string.isEmpty()){
        if(aux_string == "Expanding")
            label->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
        else if(aux_string == "Preferred")
            label->sizePolicy().setVerticalPolicy(QSizePolicy::Preferred);
        else if(aux_string == "Fixed")
            label->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
        else if(aux_string == "Minimum")
            label->sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
        else if(aux_string == "Maximum")
            label->sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);
    }

    // Sets the font family
    aux_string = xml->readString("fontfamily");
    QFont font ;
    if (!aux_string.isEmpty() && aux_string == "SansSerif" )
        font.setFamily("SansSerif");

    // and font size
    aux_string = xml->readString("fontsize");
    if(!aux_string.isEmpty())
        font.setPixelSize(aux_string.toInt());

    // and bold
    aux_string = xml->readString("bold");
    if(!aux_string.isEmpty())
        font.setBold(true);

    // and underline
    aux_string = xml->readString("underline");
    if(!aux_string.isEmpty())
        font.setUnderline(true);

    // and italic
    aux_string = xml->readString("italic");
    if(!aux_string.isEmpty())
        font.setItalic(true);

    // and strikeout
    aux_string = xml->readString("strikeout");
    if(!aux_string.isEmpty()){
        font.setStrikeOut(true);
    }

    label->setFont(font);

    // Add each label to the layout of this widget


    xml->delDomain();
    xml->setDomain("headerdescription.global");

    //add the frame
    labels_frame = new QFrame(this);
    labels_frame->setObjectName("headerlabelFrame");
    total_layout->addWidget(labels_frame);


    // Create the layout for this widget
    aux_string = xml->readString("layout");
    if(!aux_string.isEmpty()){
        if(aux_string == "HBox")
            layout = new QHBoxLayout(labels_frame);
        else if(aux_string == "VBox")
            layout = new QVBoxLayout(labels_frame);
    }

    // Apply margins
    aux_string = xml->readString("margin");
    if (!aux_string.isEmpty())
        QWidget::layout()->setMargin(aux_string.toInt());

    // and spacings for widget childs of this widget
    aux_string = xml->readString("spacing");
    if (!aux_string.isEmpty())
        QWidget::layout()->setSpacing(aux_string.toInt());

    // Apply frame shape
    aux_string = xml->readString("frameshape");
    if (!aux_string.isEmpty()){
        if(aux_string == "NoFrame"){
            setFrameShape(QFrame::NoFrame);
            labels_frame->setFrameShape(QFrame::NoFrame);
        }
        else if(aux_string == "Box"){
            setFrameShape(QFrame::Box);
            labels_frame->setFrameShape(QFrame::Box);
        }
        else if(aux_string == "StyledPanel"){
            setFrameShape(QFrame::StyledPanel);
            labels_frame->setFrameShape(QFrame::StyledPanel);
        }
        else if(aux_string == "HLine"){
            setFrameShape(QFrame::HLine);
            labels_frame->setFrameShape(QFrame::HLine);
        }
    }

    // and frame shadow for this widget
    aux_string = xml->readString("frameshadow");
    if (!aux_string.isEmpty()){
        if(aux_string == "Plain"){
            setFrameShadow(QFrame::Plain);
            labels_frame->setFrameShadow(QFrame::Plain);
        }
        else if(aux_string == "Raised"){
            setFrameShadow(QFrame::Raised);
            labels_frame->setFrameShadow(QFrame::Raised);
        }
        else if(aux_string == "Sunken"){
            setFrameShadow(QFrame::Sunken);
            labels_frame->setFrameShadow(QFrame::Sunken);
        }
    }


    xml->delDomain();
    xml->setDomain("headerdescription.labels");

    // Looks for labels and create it
    int num_labels = xml->howManyTags("label");
    for(int i=0; i < num_labels; i++){
        QString aux_name = xml->readString("label["+QString::number(i)+"].name");
        QString aux_text = xml->readString("label["+QString::number(i)+"].text");
        QString aux_pixmap = xml->readString("label["+QString::number(i)+"].pixmap");
        QLabel *label = new QLabel(labels_frame);
        label->setObjectName(aux_name);
        widgets->append(label, aux_name);
        if(!aux_text.isEmpty())
            label->setText(aux_text);
        if(!aux_pixmap.isEmpty())
            label->setPixmap(QPixmap(aux_pixmap));

        // Looks for widget background color
        aux_string = xml->readString("label["+QString::number(i)+".backgroundcolor");
        if (!aux_string.isEmpty()){
            QPalette palette;
            palette.setColor(backgroundRole(), QColor(aux_string));
            label->setPalette(palette);
        }
        // looks for widget height
        aux_string = xml->readString("label["+QString::number(i)+".height");
        if(!aux_string.isEmpty())
            label->setFixedHeight(aux_string.toInt());

        // looks for widget width
//        aux_string = xml->readString("label["+QString::number(i)+".width");
//        if (!aux_string.isEmpty())
//            label->setFixedWidth(aux_string.toInt());

        // Horizontal align of labels
        aux_string = xml->readString("label["+QString::number(i)+"].halign");
        if (!aux_string.isEmpty()){
            if(aux_string == "Left")
                label->setAlignment(Qt::AlignLeft);
            else if(aux_string == "Center")
                label->setAlignment(Qt::AlignHCenter);
            else if(aux_string == "Right")
                label->setAlignment(Qt::AlignRight);
        }

        // Vertical align of labels
        aux_string = xml->readString("label["+QString::number(i)+"].valign");
        if(!aux_string.isEmpty()){
            if (aux_string == "Top")
                label->setAlignment(Qt::AlignTop);
            else if (aux_string == "Center")
                label->setAlignment(Qt::AlignVCenter);
            else if (aux_string == "Bottom")
                label->setAlignment(Qt::AlignBottom);
        }

        // Apply horizontal size policy if there is one
        aux_string = xml->readString("label["+QString::number(i)+"].hsizepolicy");
        if (!aux_string.isEmpty()){
            if(aux_string == "Expanding")
                label->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
            else if(aux_string == "Preferred")
                label->sizePolicy().setHorizontalPolicy(QSizePolicy::Preferred);
            else if(aux_string == "Fixed")
                label->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
            else if(aux_string == "Minimum")
                label->sizePolicy().setHorizontalPolicy(QSizePolicy::Minimum);
            else if(aux_string == "Maximum")
                label->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
        }

        // Apply vertical size policy if there is one
        aux_string = xml->readString("label["+QString::number(i)+"].vsizepolicy");
        if (!aux_string.isEmpty()){
            if(aux_string == "Expanding")
                label->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
            else if(aux_string == "Preferred")
                label->sizePolicy().setVerticalPolicy(QSizePolicy::Preferred);
            else if(aux_string == "Fixed")
                label->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
            else if(aux_string == "Minimum")
                label->sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
            else if(aux_string == "Maximum")
                label->sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);
        }

        QFont font;
        // Sets the font family
        aux_string = xml->readString("label["+QString::number(i)+"].fontfamily");
        if (!aux_string.isEmpty()){
            if(aux_string == "SansSerif"){
                font.setFamily("SansSerif");
            }
        }

        // and font size
        aux_string = xml->readString("label["+QString::number(i)+"].fontsize");
        if (!aux_string.isEmpty())
            font.setPixelSize(aux_string.toInt());

        // and bold
        aux_string = xml->readString("label["+QString::number(i)+"].bold");
        if (!aux_string.isEmpty())
            font.setBold(true);


        // and underline
        aux_string = xml->readString("label["+QString::number(i)+"].underline");
        if (!aux_string.isEmpty())
            font.setUnderline(true);

        // and italic
        aux_string = xml->readString("label["+QString::number(i)+"].italic");
        if (!aux_string.isEmpty())
            font.setItalic(true);

        // and strikeout
        aux_string = xml->readString("label["+QString::number(i)+"].strikeout");
        if (!aux_string.isEmpty())
            font.setStrikeOut(true);

        label->setFont(font);

        // Add each label to the layout of this widget
        layout->addWidget(label);
    }

    xml->popDomain();
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

    for( auto i=0; i< widgets->count(); i++){
        QLabel *label = (*widgets)[i];
        QString aux_label = label->objectName();
        if(aux_label == "staff_photo"){ }
        else if(aux_label == "staff_identity"){
            label->setFont(font);
            label->setText(name);
        }
        else if ( aux_label == "order_description" ){
            if ( !order_description.isEmpty()){
                label->show();
                label->setText("description : "+order_description);
            }
            else
                label->hide();
        }
    }
}
