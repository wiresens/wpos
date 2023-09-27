/***************************************************************************
                          productdata.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRODUCTDATA_H
#define PRODUCTDATA_H

#include <wposcore/hlist.h>

#include <QStringList>
#define UNKNOWN QString("STRANGER")

class OptionNode;
class XmlConfig;
class QString;


class ProductData {
public: 

     virtual ~ProductData();

     virtual bool initProduct(XmlConfig *xml);
     virtual bool reset();

     virtual QString getProductName();
     virtual bool setProductName(const QString& _name);
     virtual QStringList getCombinations();

     virtual bool append(ProductData *product);
     virtual ProductData* getProductAt(const int pos);
     virtual ProductData* getProduct(const QString& name);
     virtual bool removeProductAt(const int pos);
     virtual bool removeProduct(const QString& name);
     virtual int count();


 protected:
    explicit ProductData(XmlConfig *xml);
    ProductData();

private:
    friend class Product;
    QString product_name;

    HList<ProductData> *list;
    HList<OptionNode> *options;
};

#endif
