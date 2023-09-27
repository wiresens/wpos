/***************************************************************************
                          offer.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRODUCTOFFERSMODULEDB_H
#define PRODUCTOFFERSMODULEDB_H

#include <wposcore/basicdatabase.h>
#include <libbslxml/xmlconfig.h>

#include <QObject>
#include <QString>
#include <QList>

struct ProductOfferData{
    ProductOfferData() = default;
//    ProductOfferData(const ProductOfferData& data) = default;
//    ProductOfferData& operator=(ProductOfferData &data) = default;
//    ~ProductOfferData() = default;

    ProductOfferData(const ProductOfferData *data): ProductOfferData(*data){}
    QString offer_type;
    QString description_type;
    QString cpp_operator;
    QString offer_name;
    QString description_offer;
    QString logo;
    double value{0.0};
    bool is_default{false};
};

class ProductOffersModuleDB : public BasicDatabase {
public: 
    ProductOffersModuleDB(QString _name_connection,
                          QString _path_connection,
                          QString _database,
                          QString _username,
                          QString _passwd);

    ProductOffersModuleDB(const QString& _connection_name,
                          XmlConfig *xml);

    ProductOffersModuleDB(const QString& _connection_name,
                          const QString& configuration_path);

    ~ProductOffersModuleDB();

    bool existOffer(const QString& offer_type, const QString& offer_name);

    bool existOfferType(const QString& offer_type);

    bool deleteOffer(const QString& offer_type, const QString& offer_name);

    bool deleteOfferType(const QString& offer_type);

    QString getDescriptionOffer(const QString& offer_type, const QString& offer_name);

    QString getDescriptionOfferType(const QString& offer_type);

    QString getCppOperator(const QString& offer_type);

    QString getLogoOffer(const QString& offer_type, const QString& offer_name);

    ProductOfferData* getOffer(const QString& offer_type, const QString& offer_name);

    QList<ProductOfferData*>* getOffers(const QString& offer_type);

    QList<ProductOfferData*>* getAllOffers();

    ProductOfferData* getOfferType(const QString& offer_type);

    QStringList* getOfferTypes();

    bool insertOffer(const QString& offer_type, const QString& offer_name, const QString& description,const QString& logo="");

    bool insertOfferType(const QString& offer_type, const QString& description, const QString& cpp_operator);

    int getNumOfferTypes();

    bool updateOffer(const QString& offer_type, const QString& offer_name, const QString& description,const QString& logo="");

    bool updateOfferType(const QString& offer_type, const QString& description, const QString& cpp_operator);

    QList<ProductOfferData*>* getProductOffers(const QString& product_code);

    bool existProductAndOffer(const QString& product_code, const QString& offer_type, const QString& offer_name);

    bool insertOfferToProduct(const QString& product_code, const QString& offer_type,
                              const QString& offer_name, double value, const QString& is_default="f");

    bool updateOfferToProduct(const QString& product_code, const QString& offer_type,
                              const QString& offer_name, double value, const QString& is_default="f");

    bool deleteOfferToProduct(const QString& product_code, const QString& offer_type,
                              const QString& offer_name);


    bool setAllProductOfferValue(const QString& offer_type,const QString& offer_name, const QString& value);

    bool updateAllProductOfferValue(const QString& offer_type,const QString& offer_name,const QString& value);
};

#endif
