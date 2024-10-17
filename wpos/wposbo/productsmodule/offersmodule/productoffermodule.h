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

    explicit ProductOfferModule(QObject *parent= 0, const QString& name= QString{});
     ~ProductOfferModule() = default;

public slots:
    Q_SCRIPTABLE bool deleteOffer(const QString& xml_string) override;
    Q_SCRIPTABLE bool deleteOfferType(const QString& offer_type) override;
    Q_SCRIPTABLE QString getDescriptionOffer(const QString& xml_string) override;

    Q_SCRIPTABLE QString getDescriptionOfferType(const QString& offer_type) override;
    Q_SCRIPTABLE QString getCppOperator(const QString& offer_type) override;

    Q_SCRIPTABLE QString getLogoOffer(const QString& xml_string) override;
    Q_SCRIPTABLE QString getLogo(const QString& type, const QString& name) override;

    Q_SCRIPTABLE QString getAllOffers() override;
    Q_SCRIPTABLE QString getOffer(const QString& xml_string) override;
    Q_SCRIPTABLE QString getOffers(const QString& offer_type) override;
    Q_SCRIPTABLE QString getOfferType(const QString& offer_type) override;
    Q_SCRIPTABLE QString getOfferTypes() override;

    Q_SCRIPTABLE bool insertOffer(const QString& xml_string) override;
    Q_SCRIPTABLE bool insertOfferType(const QString& xml_string) override;

    Q_SCRIPTABLE int getNumOfferTypes() override;

    Q_SCRIPTABLE bool updateOffer(const QString& xml_string) override;
    Q_SCRIPTABLE bool updateOfferType(const QString& xml_string) override;

    Q_SCRIPTABLE QString getProductOffers(const QString& product_code) override;
    Q_SCRIPTABLE bool existOffer(const QString& xml_string) override;
    Q_SCRIPTABLE bool existOfferType(const QString& type) override;
    Q_SCRIPTABLE bool existProductAndOffer(const QString& xml_string) override;

    Q_SCRIPTABLE bool insertOffersToProduct(const QString& xml_string) override;
    Q_SCRIPTABLE bool updateOffersToProduct(const QString& xml_string) override;
    Q_SCRIPTABLE bool deleteOffersToProduct(const QString& product_code, const QString& offer_type, const QString& offer_name) override;

    Q_SCRIPTABLE void setAllProductOfferValue(const QString& offer_type, const QString& offer_name, const QString& value) override;
    Q_SCRIPTABLE void updateAllProductOfferValue(const QString& offer_type, const QString& offer_name, const QString& value) override;

 private:
      QString getStringXmlOffer(ProductOfferData* offer);

private:
      ProductOffersModuleDB *db;
};

#endif
