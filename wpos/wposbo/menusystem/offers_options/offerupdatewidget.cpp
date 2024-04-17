/***************************************************************************
                          nupdateofferwidget.cpp  -  description
                             -------------------
    begin                : Sat abr 24 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "offerupdatewidget.h"

#include "productsmodule/offersmodule/productoffermodule.h"
#include "database/productoffersmoduledb.h"

#include <wposgui/keyboard/numkeyboard.h>
#include <wposgui/keyboard/floatkeyboard.h>
#include <libbslxml/xmlconfig.h>

#include <QMessageBox>

#include <iostream>
using std::cerr;
using std::endl;

static const QString& OFFERS_LIST_DTD  {"/etc/ntpv_backoffice/dtds/products_offerslist.dtd"};


OfferUpdateWidget::OfferUpdateWidget(QWidget *parent, const QString& name ) :
    OfferEditionWidget(parent, name)
{
    title_label->setText(tr("Update Offer"));
    offer_lineedit->hide();
    offer_combobox->setEditable(false);

    connect(offer_combobox, &QComboBox::textActivated,
            this, &OfferUpdateWidget::typeChangedSlot);
    connect(offer_name_combobox, &QComboBox::textActivated,
            this, &OfferUpdateWidget::offerChangedSlot);
}

void OfferUpdateWidget::clear(){
    offer_name_combobox->clear();
    OfferEditionWidget::clear();
}

void OfferUpdateWidget::typeChangedSlot(const QString& type){

    auto cpp_operator = mod.getCppOperator(type);
    if (cpp_operator.isEmpty()){
        offer_name_combobox->clear();
        button_group->setEnabled(true);
        ok_button->setEnabled(hasAllValues());
        return;
    }

    if (cpp_operator == "=" ) fixed_radiobutton->setChecked(true);
    else if ( cpp_operator == "x" ) percent_radiobutton->setChecked(true);
    button_group->setEnabled(false);
    radioButtonSlot();
    ok_button->setEnabled(hasAllValues());

    //different, put hook method
    showOffersFromType(type);
    searchIcon(offer_combobox->currentText(), offer_name_combobox->currentText());
}

void OfferUpdateWidget::offerChangedSlot(const QString &){
    searchIcon(offer_combobox->currentText(), offer_name_combobox->currentText());
    ok_button->setEnabled(hasAllValues());
}

bool OfferUpdateWidget::searchIcon(const QString& type, const QString& name){

    XmlConfig xml;
    xml.createElementSetDomain("offers.offer");
    xml.createElement("offer_type",type);
    xml.createElement("offer_name",name);

    xml.delDomain();
    QString xml_string = mod.getLogoOffer(xml.toString());

    if ( !xml_string.isEmpty() ){
        auto items = logo_view->findItems(xml_string, Qt::MatchContains);
        if ( !items.isEmpty() ){
            auto item = items.first();
            logo_view->setCurrentItem(item);
            logo_view->scrollToItem(item);
            item->setSelected(true);
            return true;
        }
    }
    return false;
}

bool OfferUpdateWidget::hasAllValues(){
    return !offer_combobox->currentText().isEmpty()
            && !offer_name_combobox->currentText().isEmpty()
            && logo_view->currentItem();
}

void OfferUpdateWidget::acceptSlot(){

    if ( !save() ){
        QString text = tr("Failed to update offer\n An error occured while updating the offer\n");
        QMessageBox::information(this, tr("Failed to update the offer"), text, QMessageBox::Ok);
        return;
    }

    if (group_box->isChecked()){
        QString msg = tr("This offer will apply to all products\n Do you want to apply the new value to all products ?");
        if( QMessageBox::question( this, tr("Apply to all"), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No ) return;

        QString value;
        if (fixed_radiobutton->isChecked()){
            value = QString::number(float_kb->value(),'f',2);
        }
        else if (percent_radiobutton->isChecked()){
            double percentage = double (100 - num_kb->value()) /100;
            value = QString::number(percentage,'f', 2);
        }
        mod.updateAllProductOfferValue(offer_combobox->currentText(), offer_name_combobox->currentText(),value);
    }
    clear();
    getOfferTypes();
    loadIcons();
}

bool OfferUpdateWidget::save(){

    if ( !hasAllValues() )  return false;
    auto item = logo_view->currentItem();

    XmlConfig xml;
    xml.createElementSetDomain("offers.offer");
    xml.createElement("offer_type", offer_combobox->currentText());
    xml.createElement("description_type", offer_combobox->currentText()+"_offer_type");
    xml.createElement("offer_name", offer_name_combobox->currentText());
    xml.createElement("description_offer", offer_lineedit->text()+"_offer_name");

    if ( fixed_radiobutton->isChecked() ) xml.createElement("cpp_operator", "=");
    else if ( percent_radiobutton->isChecked() ) xml.createElement("cpp_operator", "x");

    xml.createElement("logo", item->text());
    if( !xml.validateXmlWithDTD(OFFERS_LIST_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ <<  endl;
        xml.debug();
        return false;
    }

    return mod.updateOffer(xml.toString());
}

void OfferUpdateWidget::showOffersFromType(const QString& type){
    auto xml_string = mod.getOffers(type);
    if (xml_string.isEmpty())  return;

    XmlConfig xml;
    if(!xml.readXmlFromString(xml_string)){
        cerr << "can not convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OFFERS_LIST_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.delDomain();
     QStringList offers;
    for(auto i = 0; i < xml.howManyTags("offer"); i++)
        offers  << xml.readString("offer["+QString::number(i)+"].offer_name");

    offer_name_combobox->clear();
    offer_name_combobox->addItems(offers);
}
