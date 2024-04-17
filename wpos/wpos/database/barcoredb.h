/***************************************************************************
                          barcoredb.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef BARCOREDB_H
#define BARCOREDB_H

#include <basicdatabase.h>
class QString;
struct ProductPriceData{
    QString price;
    QString tax_type;
    QString tax;
};

struct OfferData{
    QString offer_type;
    QString offer_name;
    QString offer_mode;
    QString offer_discount;
    int item_code;

    bool isNull() const {
        return offer_type.isEmpty() &&
               offer_name.isEmpty() &&
               offer_mode.isEmpty() &&
               offer_discount.isEmpty();
    }
};

class BarCoreDB : public BasicDatabase  {
public:
    BarCoreDB() = default;
    BarCoreDB(
        const QString& connection_name,
        const QString& hostname,
        const QString& database,
        const QString& username,
        const QString& passwd);

    BarCoreDB(
        const QString& connection,
        const QString& configFile);

    QString getName(const QString& product_code);
    QString getName(const QStringList &article_list);

    bool getProductAtPrinter(const QString& product_code, const QString& printer);

    QString getPrice(const QString& product_code);
    QString getTax(const QString& product_code);
    QString getTaxName(const QString& product_code);

    QString getOptionModifier(const QString& product_code,const QString& option_type,const QString& option_value);

    bool checkOption(const QString& product_code,const QString& option_type,const QString& option_value);
//    OfferData* getOffer(const QString& product_code,const QString& offer_type,const QString& offer_name);
    OfferData getOffer(const QString& product_code,const QString& offer_type,const QString& offer_name) const;

};

#endif
