/***************************************************************************
                          extracore.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "extracore.h"

#include <wposcore/genericsignalmanager.h>

#include "wpos/database/barcoredb.h"
#include <xmlconfig.h>
#include <productextrainfo.h>

#include <QString>
#include <QStringList>
#include <QFile>


#include <iostream>
namespace std{}
using namespace std;

ExtraCore::ExtraCore(QObject *parent, const QString& name):
    QObject(parent),
    price_state(true)
{
    setObjectName(name);
    auto gsm = GenericSignalManager::instance();
        gsm->subscribeToGenericDataSignal(GDATASIGNAL::XCORE_SET_DEFAULT_PRODUCT_OFFER, this);
        gsm->subscribeToGenericDataSignal(GDATASIGNAL::XCORE_SET_PRODUCT_OPTION, this);
        gsm->subscribeToGenericDataSignal(GDATASIGNAL::XCORE_SET_PRODUCT_PRICE, this);
        gsm->subscribeToGenericDataSignal(GDATASIGNAL::XCORE_SET_PRODUCT_OFFER, this);
        gsm->subscribeToGenericDataSignal(GDATASIGNAL::SETCORE_MODE, this);

        gsm->subscribeToGenericSignal(GDATASIGNAL::XCORE_DEL_OPTIONS, this);
        gsm->subscribeToGenericSignal(GSIGNAL::XCORE_CLEAR_EXTRAS, this);

        gsm->publishGenericDataSignal(GDATASIGNAL::PRODSELECT_COLOR_MODE, this);
}

QStringList ExtraCore::getOptionsNames(){

    QStringList names;
    for(auto* node : options){
        QString aux = node->getOptionType();
        if (aux.isEmpty()) continue;
        names.append(aux);
    }
    return names;
}

QString ExtraCore::getOptionValue(const QString& option_name){

    ProductExtraInfo* node = options.find(option_name);

    if (node)
        return node->getDefaultOption();
    return QString();
}

QStringList ExtraCore::getFixedOptionsNames(){

    QStringList names;
    for(auto *node : fixed_options){
        QString name = node->getOptionType();
        if (name.isEmpty()) continue;
        names.append(name);
    }

    return names;
}

QString ExtraCore::getFixedOptionValue(const QString& option_name){

    ProductExtraInfo* node = fixed_options.find(option_name);
    if (node)
        return node->getDefaultOption();
    return QString();

}

bool ExtraCore::clearOptionList(bool fixed){
    options.clear();
    if (fixed)
        fixed_options.clear();
    return true;
}

ProductPriceData* ExtraCore::getPrice(){
    return custom_price;
}

bool ExtraCore::getPriceMode(){
    return price_state;
}

void ExtraCore::setPriceMode(bool state){
    price_state = state;
}

double ExtraCore::getModifiedPrice(double price){
    if ( !getPriceMode() )
        return (price * (-1));
    return price;
}

bool ExtraCore::setPrice(const ProductPriceData* price_data){

    bool  ret {clearPrice()};
    custom_price = new ProductPriceData{*price_data};
    //    *custom_price = *price_data;
    return ret;
}

bool ExtraCore::clearPrice(){

    if (custom_price){
        XmlConfig xml ;
        xml.createElement("mode", "last");
        emit genericDataSignal(GDATASIGNAL::PRODSELECT_COLOR_MODE, &xml);
        delete custom_price;
        return true;
    }
    return false;
}

void ExtraCore::genericSignalSlot(const QString& signal_name){
    //the option list have been selected to be cleared
    if (signal_name == GDATASIGNAL::XCORE_DEL_OPTIONS)
        options.clear();
    else if (signal_name == GSIGNAL::XCORE_CLEAR_EXTRAS)
        clearExtras();
}

void ExtraCore::genericDataSignalSlot(const QString& signal_name, XmlConfig* xml){
    if (!xml) return;
    /**
     *    new options to add at the options list, the core will take all this options and will
     *    replace it at the xml to the order and the ticket
     */
    if (signal_name == GDATASIGNAL::XCORE_SET_PRODUCT_OPTION){
        readOptionFromXml(xml);
    }
    else if (signal_name == GDATASIGNAL::XCORE_SET_PRODUCT_OFFER){
        readOfferFromXml(xml);
    }
    /**
     *    the core will change the price of the latest product that have been inserted with
     *    the price that this slot receives
     */
    else if (signal_name == GDATASIGNAL::XCORE_SET_PRODUCT_PRICE){
        readPriceFromXml(xml);
    }
    else if (signal_name == GDATASIGNAL::SETCORE_MODE){

        xml->delDomain();
        QString aux { xml->readString("mode") };

        if (aux == "normal") setPriceMode(true);
        else if (aux == "product_anulation")  setPriceMode(false);
    }
    else if (signal_name == GDATASIGNAL::XCORE_SET_DEFAULT_PRODUCT_OFFER){

        xml->delDomain();
        QString operation {xml->readString("operation")};

        if (operation == "set"){
            QString type {xml->readString("type")};
            QString name {xml->readString("name")};

            if ( !type.isEmpty() && !name.isEmpty() )
                setDefaultOffer(type, name);
        }
        else if (operation == "del") delDefaultOffer();
    }
}

void ExtraCore::readPriceFromXml(XmlConfig* xml){

    clearPrice();
    xml->pushDomain();
    xml->delDomain();
    QString price {xml->readString("price")};
    QString tax_type  {xml->readString("tax_type")};
    QString tax {xml->readString("tax")};
    xml->popDomain();

    bool ok{false};
    double num = price.toDouble(&ok);

    if (!ok)  return;

    num = tax.toDouble(&ok);

    if ( !ok || num > 1.0 || num < 0.0)
        return;

    //check the price and tax.
    ProductPriceData data;
    data.price = price;
    data.tax_type = tax_type;
    data.tax = tax;

    setPrice(&data);
}

void ExtraCore::readOfferFromXml(XmlConfig* xml){

    clearOffer();
    xml->pushDomain();
    xml->delDomain();
    QString type {xml->readString("type")};
    QString name {xml->readString("name")};
    xml->popDomain();

    if ( type.isEmpty() || name.isEmpty() )  return;

    OfferData data;
    data.offer_type = type;
    data.offer_name = name;
    setOffer(&data);
}

void ExtraCore::readOptionFromXml(XmlConfig* xml){


    QString name {xml->readString("type")};
    QString value  {xml->readString("value")};

    if ((name.isEmpty())||(value.isEmpty()))
        return;

    ProductExtraInfo* node {new ProductExtraInfo(name)};
    node->addOption(value, true);
    options.append(node, node->getOptionType() );
}

void ExtraCore::readFixedOptionFromXml(XmlConfig* xml){

    QString name  {xml->readString("type")};
    QString value = {xml->readString("value")};

    if (name.isEmpty() || value.isEmpty()) return;

    ProductExtraInfo *node {new ProductExtraInfo(name)};
    node->addOption(value, true);
    fixed_options.append(node, node->getOptionType() );
}

OfferData* ExtraCore::getOffer(){
    if (custom_offer)  return custom_offer;
    if (default_offer) return default_offer;
    return nullptr;
}

bool ExtraCore::setOffer(const OfferData* offer){

    bool ret {clearPrice()};
    custom_offer = new OfferData{*offer};
    //    *custom_offer = *offer;
    return ret;
}

bool ExtraCore::clearOffer(){
    if (custom_offer){
        XmlConfig xml;
        xml.createElement("mode","last");
        emit genericDataSignal(GDATASIGNAL::PRODSELECT_COLOR_MODE, &xml);
        delete custom_offer;
        custom_offer = nullptr;
        return true;
    }
    return false;
}

bool ExtraCore::setDefaultOffer(const QString& offer_type, const QString& offer_name){
    default_offer = new OfferData;
    default_offer->offer_type = offer_type;
    default_offer->offer_name = offer_name;

    //maybe some kind of test to check if the offer_type and offer_name exists will be necesary
    return true;
}

bool ExtraCore::delDefaultOffer(){

    if (default_offer){
        delete default_offer;
        default_offer = nullptr;
    }
    return true;
}

void ExtraCore::clearExtras(){
    clearPrice();
    clearOffer();
    clearOptionList(true);
    delDefaultOffer();
}
