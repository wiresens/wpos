/***************************************************************************
                          offer.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRODUCT_OFFER_MODULE_H
#define PRODUCT_OFFER_MODULE_H

#include "productoffermoduleinterface.h"
#include <QtCore/QObject>
#include <QString>

class ProductOffersModuleDB;
class ProductOfferData;

class ProductOfferModule :
        public QObject,
        virtual public ProductOfferModuleInterface {

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.DBusBOProductOffer")

public:
    static const QString DBusObjectPath;

    explicit ProductOfferModule(QObject *parent= 0, const QString& name= QString());
     ~ProductOfferModule() = default;

public slots:
     virtual bool deleteOffer(const QString& xml_string) override;
     virtual bool deleteOfferType(const QString& offer_type) override;
     virtual QString getDescriptionOffer(const QString& xml_string) override;

     virtual QString getDescriptionOfferType(const QString& offer_type) override;
     virtual QString getCppOperator(const QString& offer_type) override;

     virtual QString getLogoOffer(const QString& xml_string) override;
     virtual QString getLogo(const QString& type, const QString& name) override;

     virtual QString getAllOffers() override;
     virtual QString getOffer(const QString& xml_string) override;
     virtual QString getOffers(const QString& offer_type) override;
     virtual QString getOfferType(const QString& offer_type) override;
     virtual QString getOfferTypes() override;

     virtual bool insertOffer(const QString& xml_string) override;
     virtual bool insertOfferType(const QString& xml_string) override;

     virtual int getNumOfferTypes() override;

     virtual bool updateOffer(const QString& xml_string) override;
     virtual bool updateOfferType(const QString& xml_string) override;

     virtual QString getProductOffers(const QString& product_code) override;
     virtual bool existOffer(const QString& xml_string) override;
     virtual bool existOfferType(const QString& type) override;
     virtual bool existProductAndOffer(const QString& xml_string) override;

     virtual bool insertOffersToProduct(const QString& xml_string) override;
     virtual bool updateOffersToProduct(const QString& xml_string) override;
     virtual bool deleteOffersToProduct(const QString& product_code, const QString& offer_type, const QString& offer_name) override;

     virtual void setAllProductOfferValue(const QString& offer_type, const QString& offer_name, const QString& value) override;
     virtual void updateAllProductOfferValue(const QString& offer_type, const QString& offer_name, const QString& value) override;

 private:
      QString getStringXmlOffer(ProductOfferData* offer);

private:
      ProductOffersModuleDB *db;
};

#endif
