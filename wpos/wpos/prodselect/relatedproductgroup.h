/***************************************************************************
                          productdata.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef RELATEDPRODUCTGROUP_H
#define RELATEDPRODUCTGROUP_H

#include <wposcore/hlist.h>

#include <QStringList>

class ProductExtraInfo;
class XmlConfig;
class QString;

class RelatedProductGroup {

protected:
    explicit RelatedProductGroup(XmlConfig &xml);
    RelatedProductGroup();

public: 
    virtual ~RelatedProductGroup();

    virtual void setProductName(const QString& _name);

    bool initProduct(XmlConfig &xml);
    void reset();

    void append(RelatedProductGroup *relative);
    void removeProductAt(const int pos);
    bool removeProduct(const QString& name);
    int count() const;

    QString baseName() const;
    QStringList getCombinations() const;

    RelatedProductGroup* getProductAt(int pos) const;
    RelatedProductGroup* getProduct(const QString& name) const;

private:
    friend class Product;
    QString base_product_name;

    HList<RelatedProductGroup> relatedGroup;
    HList<ProductExtraInfo> extraInfos;
};

#endif
