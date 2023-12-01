/***************************************************************************
                          bsldeleteofferproductwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "deleteofferwidget.h"

#include <libbslxml/xmlconfig.h>

#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QRadioButton>

static const QString& DELETE_OFFERS_LIST_DTD  {"/etc/ntpv_backoffice/dtds/products_offerslist.dtd"};

#include <iostream>
using namespace std;

DeleteOfferWidget::DeleteOfferWidget(QWidget *parent, const QString& name):
    QWidget(parent)

{
    setupUi(this);
    setObjectName(name);

    delete_stack->setCurrentWidget(delete_offer_page);
    connect(delete_button, &QPushButton::released, this, &DeleteOfferWidget::deleteSlot);
    connect(cancel_button, &QPushButton::released, this, &DeleteOfferWidget::cancelSlot);
    connect(accept_button, &QPushButton::released, this, &DeleteOfferWidget::acceptSlot);
}

void DeleteOfferWidget::setOffer(const QString& type, const QString& name){
    delete_stack->setCurrentWidget(delete_offer_page);
    offer_type_label->setText(type);
    offer_name_label->setText(name);
    offer_name_label->show();

    offer_type = type;
    offer_name = name;
    delete_offer->show();
    delete_offer->setChecked(true);
}

void DeleteOfferWidget::deleteSlot(){

    if(delete_type->isChecked()) return deleteType();
    if(delete_offer->isChecked()) return deleteOffer();
}

void DeleteOfferWidget::deleteOffer(){

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("offers");
    xml.createElementSetDomain("offer");
    xml.createElement("offer_type", offer_type);
    xml.createElement("offer_name", offer_name);
    xml.releaseDomain("offer");
    xml.releaseDomain("offers");

    if(!xml.validateXmlWithDTD(DELETE_OFFERS_LIST_DTD, true)){
        cerr << "file does not conform xml file type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    if(! product_offer_module.deleteOffer(xml.toString())){
        delete_stack->setCurrentWidget(error_page);
        return;
    }

    offer_type_label->clear();
    offer_name_label->clear();
    offer_type.clear();
    offer_name.clear();
    emit(offerDeleted(true));
}

void DeleteOfferWidget::deleteType(){

    if(! product_offer_module.deleteOfferType(offer_type)) {
        delete_stack->setCurrentWidget(error_page);
        return;
    }

    offer_type_label->clear();
    offer_name_label->clear();
    offer_type.clear();
    offer_name.clear();
    emit(offerDeleted(true));
}

void DeleteOfferWidget::acceptSlot(){
    delete_stack->setCurrentWidget(delete_offer_page);
    emit(offerDeleted(false));
}

void DeleteOfferWidget::showSelectOffer(){
    delete_stack->setCurrentWidget(select_offer_page);
}

void DeleteOfferWidget::cancelSlot(){
    emit(offerDeleted(false));
}


void DeleteOfferWidget::showOfferType(const QString& offer_type){
    offer_type_label->setText(offer_type);
    delete_type->setChecked(true);

    delete_offer->hide();
    offer_name_label->hide();
}
