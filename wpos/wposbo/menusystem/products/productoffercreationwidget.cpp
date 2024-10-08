/***************************************************************************
                          bsladdofferwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "productoffercreationwidget.h"
#include "menusystem/utils.h"

#include "database/productoffersmoduledb.h"

#include <wposgui/keyboard/floatkeyboard.h>
#include <libbslxml/xmlconfig.h>

#include <QDir>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QCheckBox>
#include <QTextEdit>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QListWidget>
#include <QPixmap>
#include <QStackedWidget>
#include <QLabel>
#include <QLocale>

#include <iostream>
using namespace std;

const QString& SELECT_OFFER_TYPE = QObject::tr("Select a type of offer");
const QString& SELECT_OFFER = QObject::tr("Select an offer");
const QString& OFFERS_LIST_DTD = "/etc/ntpv_backoffice/dtds/products_offerslist.dtd";

const QString& OFFER_ICON_PATH = "controls32:";
const double ICON_SIZE = 30.00;
const int MAX_OFFER_TYPES = 10;

static const QString& XML_DESCRIPTION {"/etc/ntp_backoffice/mainscreen_description.xml"};

ProductOfferCreationWidget::ProductOfferCreationWidget(
        double _product_price,
        QWidget *parent,
        const QString& name):
    QWidget(parent), product_price {_product_price}
{
    setupUi(this);
    setObjectName(name);

    logo->hide();
    logo_label->hide();
    offer_default->hide();

    QHBoxLayout *layout{};
    float_keyboard_offer = new FloatKeyboard(numblock_offer_frame);
    float_keyboard_offer->setObjectName("float_keyboard_offer");

    if(!(layout = qobject_cast<QHBoxLayout *>( numblock_offer_frame->layout())))
        layout = new QHBoxLayout(numblock_offer_frame);

    layout->addWidget(float_keyboard_offer);
    connect(save_offer_button, &QPushButton::clicked, this, &ProductOfferCreationWidget::saveOfferButtonClicked);
    connect(offer_type_combo, &QComboBox::textActivated, this, &ProductOfferCreationWidget::offerTypeActivated);
    connect(too_offer_button, &QPushButton::clicked, this, &ProductOfferCreationWidget::tooOfferButtonClicked);
    connect(accept_error_button, &QPushButton::clicked, this, &ProductOfferCreationWidget::acceptErrorButtonClicked);
    offer_stack->setCurrentWidget(offer_page);
}

void ProductOfferCreationWidget::offerTypeActivated(const QString& text){
    if( text == SELECT_OFFER_TYPE){
        offer_combo->clear();
        offer_combo->addItem(SELECT_OFFER_TYPE);
    }
    else  updateOffers();
}

void ProductOfferCreationWidget::saveOfferButtonClicked(){

    auto offer = new ProductOfferData;
    offer->offer_type = offer_type_combo->currentText();
    offer->description_type = description_offer_type->text();
    offer->offer_name = offer_combo->currentText();
    offer->description_offer = description_offer->text();
    //       offer->is_default = offer_default->isChecked();

    offer->logo.clear();
    if ( logo && logo->currentItem() )
        offer->logo = (logo->currentItem())->text();

    auto value = float_keyboard_offer->value() - product_price;
    offer->value = value;
    //     offer->is_default = offer_default->isChecked();

    offers.append(offer);
    //     offer_default->setChecked(false);
    float_keyboard_offer->clear();
    emit(offerSaved(offer));
    //@benes    delete offer;
}

void ProductOfferCreationWidget::updateOffers(){
    auto xml_string = model.getOffers(offer_type_combo->currentText());
    if(xml_string.isEmpty()) return;
    QString description = model.getDescriptionOfferType(offer_type_combo->currentText());

    XmlConfig xml;
    if(!xml.readXmlFromString(xml_string)){
        cerr << "cannot convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OFFERS_LIST_DTD, true)){
        cerr << "xml does not validate against xml " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.delDomain();
    xml.setDomain("offers[0]");
    QStringList offers_aux;
    for(auto i = 0; i < xml.howManyTags("offer"); i++)
        offers_aux.append(xml.readString("offer[" + QString::number(i) + "].offer_name"));

    offer_combo->clear();
    offer_combo->addItem(SELECT_OFFER);
    offer_combo->addItems(offers_aux);
    description_offer_type->setText(description);
}

void ProductOfferCreationWidget::setProductPrice(double _product_price){
    product_price = _product_price;
}

void ProductOfferCreationWidget::initOfferTypes(){

    QString xml_string = model.getOfferTypes();
    XmlConfig xml;
    if(!xml.readXmlFromString(xml_string)){
        cerr << "cannot convert the string into xml " << __PRETTY_FUNCTION__ << ": " <<__LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OFFERS_LIST_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    QStringList offer_types;
    xml.setDomain("offers");
    for(auto i = 0; i < xml.howManyTags("offer"); i++)
        offer_types.append(xml.readString("offer[" + QString::number(i) + "].offer_type"));

    offer_type_combo->clear();
    offer_type_combo->addItem(QString(SELECT_OFFER_TYPE));
    offer_type_combo->addItems(offer_types);
    offer_combo->clear();
    offer_combo->addItem(QString(SELECT_OFFER_TYPE));
}

void ProductOfferCreationWidget::clearDefaults(){
    for(auto* offer : offers)
        offer->is_default = false;
}

bool ProductOfferCreationWidget::saveOffersProduct(const QString& product_code){
    clearWidget();
    if( offers.isEmpty()) return true;

    XmlConfig xml;
    xml.delDomain();
    xml.createElement("product_code", product_code);
    xml.createElementSetDomain("offers");

    for(auto* offer : offers){
        xml.createElementSetDomain("offer");
        xml.createElement("offer_type", offer->offer_type);
        xml.createElement("description_type", offer->description_type);
        xml.createElement("offer_name", offer->offer_name);
        xml.createElement("description_offer", offer->description_offer);
        xml.createElement("cpp_operator", offer->cpp_operator);
        xml.createElement("value", QString::number(offer->value));
        if(offer->is_default)
            xml.createElement("default", "t");
        else
            xml.createElement("default", "f");
        xml.releaseDomain("offer");
    }
    xml.releaseDomain("offers");

    if(!xml.validateXmlWithDTD(OFFERS_LIST_DTD, true)){
        cerr << "XML does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return false;
    }
    return  model.insertOffersToProduct(xml.toString());
}

void ProductOfferCreationWidget::clear(){
    clearWidget();
    offers.clear();
}

void ProductOfferCreationWidget::setEditable(bool editable){
    offer_type_combo->setEditable(editable);
    offer_combo->setEditable(editable);

    if(!editable){
        delete logo_label;
        delete logo;
        logo = 0;
    }
}

bool ProductOfferCreationWidget::saveOffer(ProductOfferData *offer){
    if(!offer)  return true;

    XmlConfig xml;
    xml.createElementSetDomain("offers");
    xml.createElementSetDomain("offer");
    xml.createElement("offer_type", offer->offer_type);
    xml.createElement("description_type", offer->description_type);
    xml.createElement("offer_name", offer->offer_name);
    xml.createElement("description_offer", offer->description_offer);
    xml.createElement("cpp_operator", offer->cpp_operator);
    xml.createElement("value", QString::number(offer->value));

    if(offer->is_default) xml.createElement("default", "t");
    else xml.createElement("default", "f");
    xml.createElement("logo", offer->logo);

    if( !xml.validateXmlWithDTD(OFFERS_LIST_DTD, true) ){
        cerr << "El xml no tiene una estructura valida " << __PRETTY_FUNCTION__ << ": " << __LINE__ <<  endl;
        xml.debug();
        return false;
    }

    return model.insertOffer(xml.toString());
}

void ProductOfferCreationWidget::saveOffers(){

    if( offers.isEmpty())   return;
    for(auto* offer : offers)
    {
        XmlConfig xml;
        xml.createElementSetDomain("offers");
        xml.createElementSetDomain("offer");
        xml.createElement("offer_type", offer->offer_type);
        xml.createElement("description_type", offer->description_type);
        xml.createElement("offer_name", offer->offer_name);
        xml.createElement("description_offer", offer->description_offer);
        xml.createElement("cpp_operator", offer->cpp_operator);
        xml.createElement("value", QString::number(offer->value));
        if(offer->is_default)  xml.createElement("default", "t");
        else xml.createElement("default", "f");
        xml.createElement("logo", offer->logo);
        xml.releaseDomain("offer", false);
        xml.releaseDomain("offers", false);

        if(!xml.validateXmlWithDTD(OFFERS_LIST_DTD, true)){
            cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml.debug();
            return;
        }
        model.insertOffer(xml.toString());
    }
    clear();
}

QList<ProductOfferData*>* ProductOfferCreationWidget::getOffers(){
    return &offers;
}

//void BslAddOfferWidget::initLogos(){

//    QString absolute_file_path;
//    logo->clear();

//    QStringList files =  QDir(OFFER_ICON_PATH).entryList(QStringList("*.png"), QDir::Files, QDir::Name);
//    for(const auto& file : files){
//        absolute_file_path = OFFER_ICON_PATH + file;

//        QPixmap pixmap(absolute_file_path);
//        QMatrix matrix;
//        auto width = ICON_SIZE / pixmap.width();
//        auto height = ICON_SIZE / pixmap.height();
//        matrix.scale(width, height);

//        auto item = new QListWidgetItem(QPixmap(pixmap.transformed( matrix )), file,  logo);
//        logo->addItem(item);
//    }
//}

void ProductOfferCreationWidget::initLogos(){

    QString absolute_file_path;
    logo->clear();

    QStringList files =  QDir(OFFER_ICON_PATH).entryList(QStringList("*.png"), QDir::Files, QDir::Name);
    for(const auto& file : files){
        absolute_file_path = OFFER_ICON_PATH + file;
        auto item = new QListWidgetItem(cropedIcon(absolute_file_path,ICON_SIZE), file,  logo);
        logo->addItem(item);
    }
}

void ProductOfferCreationWidget::setOffer(const QString& offer_type, const QString& offer_name){

    QString xml_string;
    {
        XmlConfig xml;
        xml.createElementSetDomain("offers");
        xml.createElementSetDomain("offer");
        xml.createElement("offer_type", offer_type);
        xml.createElement("offer_name", offer_name);
        xml.releaseDomain("offer", false);
        xml.releaseDomain("offers", true);

        if(!xml.validateXmlWithDTD(OFFERS_LIST_DTD, true)){
            cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml.debug();
            return;
        }

        xml_string = model.getOffer(xml.toString());
    }

    XmlConfig xml;
    if( !xml.readXmlFromString(xml_string) ){
        cerr << "Cannot convert xml into string " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if( !xml.validateXmlWithDTD(OFFERS_LIST_DTD, true )){
        cerr << "XML does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.delDomain();
    xml.setDomain("offers[0]");
    xml.setDomain("offer[0]");

    offer_type_combo->setCurrentText(xml.readString("offer_type"));
    description_offer_type->setText(xml.readString("description_type"));
    offer_combo->setCurrentText(xml.readString("offer_name"));
    description_offer->setText(xml.readString("description_offer"));

    if( !xml.readString("logo").isEmpty() ){
        QList<QListWidgetItem*> items = logo->findItems(xml.readString("logo"), Qt::MatchExactly);
        if( !items.isEmpty())
            logo->setCurrentItem(items.first());
    }
    float_keyboard_offer->reeadKey(xml.readString("value").toDouble());
}

void ProductOfferCreationWidget::showError(const QString& offer_type, const QString& offer_name){
    offer_type_label->setText(offer_type);
    offer_name_label->setText(offer_name);
    offer_stack->setCurrentWidget(error_page);
}

void ProductOfferCreationWidget::showTooOffers(){
    offer_stack->setCurrentWidget(too_offers_page);
}

bool ProductOfferCreationWidget::canInsertOfferType(const QString& offer_type){

    if(model.existOfferType(offer_type)) return true;
    return model.getNumOfferTypes() < MAX_OFFER_TYPES;
}

void ProductOfferCreationWidget::clearWidget(){
    offer_combo->setCurrentText(SELECT_OFFER);
    description_offer->clear();
    float_keyboard_offer->clear();
    if(logo)  logo->clearSelection();
}

void ProductOfferCreationWidget::tooOfferButtonClicked(){
    offer_stack->setCurrentWidget(offer_page);
}

void ProductOfferCreationWidget::acceptErrorButtonClicked(){
    offer_stack->setCurrentWidget(offer_page);
}

void ProductOfferCreationWidget::checkDefaultOffers(){
    int i;

    for(i = 0; i < offers.count(); i++)
        if( offers[i]->is_default) break;

    if ( i == offers.count() ) offers[0]->is_default = true;
}

void ProductOfferCreationWidget::showEvent(QShowEvent *e){
    clear();
    QWidget::showEvent(e);
}
