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
class QStringList;

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
};


class BarCoreDB : public BasicDatabase  {
public: 
     BarCoreDB(const QString& _connection_name,
                       const QString& _hostname,const QString& _database,
                       const QString& _username,const QString& _passwd);

     BarCoreDB(const QString& _connection_name,const QString& configuration_path);
     ~BarCoreDB();

     QString getName(const QString& product_code);
     QString getName(const QStringList article_list);

     bool getProductAtPrinter(const QString& product_code, const QString& printer);

     QString getPrice(const QString& product_code);
     QString getTax(const QString& product_code);
     QString getTaxName(const QString& product_code);

     QString getOptionModifier(const QString& product_code,const QString& option_type,const QString& option_value);

     bool checkOption(const QString& product_code,const QString& option_type,const QString& option_value);
     OfferData* getOffer(const QString& product_code,const QString& offer_type,const QString& offer_name);
     
};

#endif
