/***************************************************************************
                          ProductOfferInterface.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef H_PRODUCT_OFFER_MODULE_INTERFACE
#define H_PRODUCT_OFFER_MODULE_INTERFACE

#include <QString>

class ProductOfferModuleInterface {
    /**
     *    deleteOffer delete the offer specificated on the xml of the param
     *    @param QString xml with properties of the offer to delete
     *    @return bool: true if the offer was deleted succesfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer's type</offer_type>
     *                   <offer_name>offer's name</offer_name>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool deleteOffer(const QString&) = 0;

    /**
     *    deleteOfferType delete de offer's type specificated on the param
     *    @param QString name of offer's type
     *    @return bool: true if the offer's type was deleted
     */
    virtual bool deleteOfferType(const QString&) = 0;

    /**
     *    getDescriptionOffer return the offer's description of the offer
     *    especificated into the xml
     *    @param QString xml with offer's type and offer's name
     *    @return QString description of this offer
     *
     *    param:
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *                   <offer>
     *                        <offer_type>option's type</offer_type>
     *                        <offer_name>option's name</offer_name>
     *                   </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getDescriptionOffer(const QString&) = 0;

    /**
     *    getDescriptionOfferType return the description of the offer's type
     *    specificated for the param
     *    @param QString offer's type
     *    @return QString description of the offer's type specificated
     */
    virtual QString getDescriptionOfferType(const QString&) = 0;

    /**
     *    getCppOperator return the cpp operator of the offer's type
     *    specificated into the param
     *    @param QString offer's type
     *    @return QString description of the offer's type specificated
     */
    virtual QString getCppOperator(const QString&) = 0;

    /**
     *    getLogoOffer return the path of the offer indicated into the xml's param
     *    @param QString xml with the offer's code and the offer's type code of
     *    the offer to search
     *    @return QString path of the logo's offer
     *
     *    param:
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer's type code</offer_type>
     *                   <offer_name>offer's name</offer_name>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getLogoOffer(const QString&) = 0;
    virtual QString getLogo(const QString&, const QString&) = 0;

    /**
     *    getOffer       return all properties of the offer with code equal to the code
     *    specificated into the xml of the param
     *    @param QString xml with the offer's code and the offer's type code of
     *    the offer to search
     *    @return QString xml with all properties of the offer specificated
     *
     *    param:
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer's type code</offer_type>
     *                   <offer_name>offer's name</offer_name>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     *
     *    return:
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer's type</offer_type>
     *                   <offer_name>offer's name</offer_name>
     *                   <description_offer>offer's description</description_offer>
     *                   <logo>offer_logo</logo>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getOffer(const QString&) = 0;

    /**
     *    getAllOffers return all properties of the offer's type with code equals to the
     *    code specificated into the param
     *    @param QString code of the offer's type
     *    @return QString xml with all properties of this offer's type
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>code of offer's type</offer_type>
     *                   <offer_name></offer_name>
     *                   <logo></logo>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getAllOffers() = 0;

    /**
     *    getOffers return all offers of the offer's type specificated on the param
     *    @param QString name of offer's type
     *    @return QString: xml with all offers of this offer's type
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer1's type</offer_type>
     *                   <description_type>description of offer1's type</description_type>
     *                   <offer_name>offer1's name</offer_name>
     *                   <description_offer>offer1's description</description_offer>
     *                   <cpp_operator>cpp operator of the offer1</cpp_operator>
     *              </offer>
     *              <offer>
     *                   <offer_type>offer2's type</offer_type>
     *                   <description_type>description of offer2's type</description_type>
     *                   <offer_name>offer2's name</offer_name>
     *                   <description_offer>offer2's description</description_offer>
     *                   <cpp_operator>cpp operator of the offer2</cpp_operator>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getOffers(const QString&) = 0;

    /**
     *    getOfferType return all properties of the offer's type with code equals to the
     *    code specificated into the param
     *    @param QString code of the offer's type
     *    @return QString xml with all properties of this offer's type
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>code of offer's type</offer_type>
     *                   <description_type>description of offer's type</description_type>
     *                   <offer_name></offer_name>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getOfferType(const QString&) = 0;

    /**
     *    getOfferTypes return all offer's types
     *    @return QString xml with all offer's types
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer's type1</offer_type>
     *                   <offer_name></offer_name>
     *              </offer>
     *              <offer>
     *                   <offer_type>offer's type2</offer_type>
     *                   <offer_name></offer_name>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getOfferTypes() = 0;

    /**
     *    insertOffer insert into the database the offer specificated on the xml of
     *    the param
     *    @param QString xml with the offer's properties
     *    @return bool: return true if the offer was inserted successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer's type</offer_type>
     *                   <description_type>description of offer's type</description_type>
     *                   <offer_name>offer's name</offer_name>
     *                   <description_offer>offer's description</description_offer>
     *                   <cpp_operator>cpp operator of the offer</cpp_operator>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool insertOffer(const QString&) = 0;

    /***
     *    insertOffer insert into the database the offer's type specificated on the
     *    xml of the param
     *    @param QString xml with the properties of offer's type
     *    @return bool: return true if the offer's type was inserted successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer's type</offer_type>
     *                   <description_type>description of offer's type</description_type>
     *                   <offer_name></offer_name>
     *                   <cpp_operator>operator of offer's type</cpp_operator>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool insertOfferType(const QString&) = 0;

    /**
     *    getNumOfferTypes return the number of offers' type
     *    @return int: number of offers' type
     */
    virtual int getNumOfferTypes() = 0;

    /**
     *    updateOffer update into the database the offer's properties of the offer
     *    specificated on the xml of the param
     *    @param QString xml with the properties of the offer to update
     *    @return bool: true if the offer was updated successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer's type</offer_type>
     *                   <description_type>description of offer's type</description_type>
     *                   <offer_name>offer's name</offer_name>
     *                   <description_offer>offer's description</description_offer>
     *                   <cpp_operator>cpp operator of the offer</cpp_operator>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool updateOffer(const QString&) = 0;

    /**
     *    updateOfferType update into the database the properties of the offer's
     *    type specificated on the xml of the param
     *    @param QString xml with the properties of the offer's type to update
     *    @return bool: true if the offer was updated successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer's type</offer_type>
     *                   <description_type>description of the offer's type</description_type>
     *                   <offer_name></offer_name>
     *                   <cpp_operator>operator of the offer's type</cpp_operator>
     *              <offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool updateOfferType(const QString&) = 0;

    /**
     *    getProductOffers return the offers of the product specificated on the
     *    param
     *    @param QString product's code
     *    @return QString xml with all offers of the product specificated
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer1's type</offer_type>
     *                   <description_type>description of offer1's type</description_type>
     *                   <cpp_operator>cpp operator of the offer1</cpp_operator>
     *                   <offer_name>offer1's name</offer_name>
     *                   <description_offer>offer1's description</description_offer>
     *                   <value>offer1's value</value>
     *                   <default>if the offer1 is default</default>
     *              </offer>
     *              <offer>
     *                   <offer_type>offer2's type</offer_type>
     *                   <description_type>description of offer2's type</description_type>
     *                   <cpp_operator>cpp operator of the offer2</cpp_operator>
     *                   <offer_name>offer2's name</offer_name>
     *                   <description_offer>offer2's description</description_offer>
     *                   <value>offer2's value</value>
     *                   <default>if the offer2 is default</default>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getProductOffers(const QString&) = 0;

    /**
     *    existOffer return true if this offer exist into database
     *    @param QString xml with all properties of this offer
     *    @return bool: true if this offer exist into database
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer's type</offer_type>
     *                   <offer_name>offer's name</offer_name>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool existOffer(const QString&) = 0;

    /**
     *    existOfferType return true if this offer's type exist into database
     *    @param QString with offer's type
     *    @return bool: true if this offer's type exist into database
     */
    virtual bool existOfferType(const QString&) = 0;

    /**
     *    existProductAndOffer return true if this product has this offer.
     *    @param QString xml with the product's code and properties of the offer
     *    @return bool: true if the product has this offer
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <product_code>product's code</product_code>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer's type</offer_type>
     *                   <offer_name>offer's name</offer_name>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool existProductAndOffer(const QString&) = 0;

    /**
     *    insertOffersToProduct insert into the database this offers for this product
     *    @param QString xml with the product's code and the properties of the offers
     *    to insert
     *    @return bool: true if all offers was inserted successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <product_code>product's code</product_code>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer1's type</offer_type>
     *                   <description_type>description of offer1's type</description_type>
     *                   <offer_name>offer1's name</offer_name>
     *                   <description_offer>offer1's description</description_offer>
     *                   <value>offer1's value</value>
     *                   <default>if the offer1 is default</default>
     *                   <cpp_operator>cpp operator of the offer1</cpp_operator>
     *              </offer>
     *              <offer>
     *                   <offer_type>offer2's type</offer_type>
     *                   <description_type>description of offer2's type</description_type>
     *                   <offer_name>offer2's name</offer_name>
     *                   <description_offer>offer2's description</description_offer>
     *                   <value>offer2's value</value>
     *                   <default>if the offer2 is default</default>
     *                   <cpp_operator>cpp operator of the offer2</cpp_operator>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool insertOffersToProduct(const QString&) = 0;

    /**
     *    updateOffersToProduct update into the database this offers for this product
     *    @param QString xml with the product's code and the properties of the offers
     *    to update
     *    @return bool: true if all offers was updated successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <product_code>product's code</product_code>
     *         <offers>
     *              <offer>
     *                   <offer_type>offer1's type</offer_type>
     *                   <description_type>description of offer1's type</description_type>
     *                   <offer_name>offer1's name</offer_name>
     *                   <description_offer>offer1's description</description_offer>
     *                   <value>offer1's value</value>
     *                   <default>if the offer1 is default</default>
     *              </offer>
     *              <offer>
     *                   <offer_type>offer2's type</offer_type>
     *                   <description_type>description of offer2's type</description_type>
     *                   <offer_name>offer2's name</offer_name>
     *                   <description_offer>offer2's description</description_offer>
     *                   <value>offer2's value</value>
     *                   <default>if the offer2 is default</default>
     *              </offer>
     *         </offers>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OFFERS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, offers) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT offers (offer*) >
     *    <!ELEMENT offer (offer_type, description_type?, offer_name, description_offer?, cpp_operator?, value?, default?) >
     *    <!ELEMENT offer_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT offer_name (#PCDATA) >
     *    <!ELEMENT description_offer (#PCDATA) >
     *    <!ELEMENT cpp_operator (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool updateOffersToProduct(const QString&) = 0;

    /**
     *  delete offer from a product.
     * @param QString product_code
     * @param QString offer_type
     * @param QString offer_name
     * @return true if the option was deleted
     */
    virtual bool deleteOffersToProduct(const QString&, const QString&, const QString&) = 0;

    /**
     *    this is a brute force method... it aplies to all products a given offer
     *    the offer should exists.
     *    @param QString Offer_type with the offer type
     *    @param QString Offer_name, with the offer name
     *    @param QString value with the offer value.
     */
    virtual void setAllProductOfferValue(const QString&, const QString&, const QString&) = 0;

    /**
     *    this is a brute force method... it aplies to all products that already have the offer
     *    a given offer, a new value .The offer should exists.
     *    @param QString Offer_type with the offer type
     *    @param QString Offer_name, with the offer name
     *    @param QString value with the offer value.
     */
    virtual void updateAllProductOfferValue(const QString&, const QString&, const QString&) = 0;
};

#endif
