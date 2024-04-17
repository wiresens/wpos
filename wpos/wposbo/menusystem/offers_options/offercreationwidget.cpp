#include "offercreationwidget.h"

#include <wposgui/keyboard/numkeyboard.h>
#include <wposgui/keyboard/floatkeyboard.h>
#include <libbslxml/xmlconfig.h>

#include <QMessageBox>

#include <iostream>
using std::cerr;
using std::endl;

static const QString& OFFERS_LIST_DTD  {"/etc/ntpv_backoffice/dtds/products_offerslist.dtd"};

OfferCreationWidget::OfferCreationWidget(
        QWidget* parent, const QString& name):
    OfferEditionWidget(parent, name)
{
    offer_name_combobox->hide();

    connect(offer_combobox, &QComboBox::currentTextChanged,this, &OfferEditionWidget::typeChangedSlot);
    connect(offer_lineedit, &QLineEdit::textChanged, this, &OfferEditionWidget::offerChangedSlot);
}

void OfferCreationWidget::clear(){
    offer_lineedit->clear();
    OfferEditionWidget::clear();
}

bool OfferCreationWidget::hasAllValues(){
    return !offer_combobox->currentText().isEmpty()
            && !offer_lineedit->text().isEmpty()
            && logo_view->currentItem();
}

void OfferCreationWidget::typeChangedSlot(const QString& type){

    auto cpp_operator = mod.getCppOperator(type);
    if (cpp_operator.isEmpty()){
        button_group->setEnabled(true);
        ok_button->setEnabled(hasAllValues());
        return;
    }

    if (cpp_operator == "=") fixed_radiobutton->setChecked(true);
    else if (cpp_operator=="x")  percent_radiobutton->setChecked(true);
    button_group->setEnabled(false);
    radioButtonSlot();
    ok_button->setEnabled(hasAllValues());
}

void OfferCreationWidget::offerChangedSlot(const QString &){
    ok_button->setEnabled(hasAllValues());
}

void OfferCreationWidget::acceptSlot(){
    if (!hasAllValues()) return;

    XmlConfig xml;
    xml.createElementSetDomain("offers.offer");
    xml.createElement("offer_type", offer_combobox->currentText());
    xml.createElement("offer_name", offer_lineedit->text());
    xml.delDomain();

    if ( mod.existOffer(xml.toString()) ){
        auto msg = tr("The offer you tried to enter already exists\n\You want to update the offer data ?");
        if( QMessageBox::question( this, tr("Existing offer"), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No ) return;
    }

    if ( !save() ){
        auto msg = tr("Failed to insert offer\nthere was a failure to insert or update the offer");
        QMessageBox::information(this, tr("Failed to create the offer"), msg, QMessageBox::Ok);
        return;
    }

    if (group_box->isChecked()){

        auto msg = tr("This offer applies to all products\nDo you want to apply the new value to all products ?");
        if( QMessageBox::question( this, tr("Apply to all","Apply to all products"), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No ) return;

        QString value;
        if (fixed_radiobutton->isChecked())
            value = QString::number(float_kb->value(),'f',2);
        else if (percent_radiobutton->isChecked())
        {
            double percentage = double ( 100 - num_kb->value()) / 100;
            value = QString::number(percentage,'f',2);
        }
        mod.setAllProductOfferValue(offer_combobox->currentText(), offer_lineedit->text(), value);
    }
    clear();
    getOfferTypes();
    loadIcons();
}

/**
*       1. Chech if the offer already exists
*       2. Insert the offer at the database
*       3. If groupbox is checked set all the product offers with the correct price
*/
bool OfferCreationWidget::save(){
    if (!hasAllValues()) return false;
    QListWidgetItem *item = logo_view->currentItem();

    XmlConfig xml;
    xml.createElementSetDomain("offers.offer");
    xml.createElement("offer_type", offer_combobox->currentText());
    xml.createElement("description_type", offer_combobox->currentText()+"_offer_type");
    xml.createElement("offer_name", offer_lineedit->text());
    xml.createElement("description_offer", offer_lineedit->text()+"_offer_name");

    if (fixed_radiobutton->isChecked()) xml.createElement("cpp_operator","=");
    else if (percent_radiobutton->isChecked())  xml.createElement("cpp_operator","x");

    xml.createElement("logo", item->text());
    if(!xml.validateXmlWithDTD(OFFERS_LIST_DTD, true)){
        cerr << "file does not conform xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ <<  endl;
        xml.debug();
        return false;
    }
    return mod.insertOffer(xml.toString());
}
