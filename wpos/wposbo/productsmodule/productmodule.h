/***************************************************************************
                          product.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRODUCTMODULE_H
#define PRODUCTMODULE_H

#include "productmoduleinterface.h"

#include <libbslxml/xmlconfig.h>

#include <QObject>
#include <QString>
#include <QList>

class ProductOptionData;

class ProductModule :
        public QObject,
        public ProductModuleInterface {

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.DBusBOProduct")

public:
    static const QString DBusObjectPath;

    explicit ProductModule(QObject *parent= 0, const QString& name = QString());
    ~ProductModule() = default;

    bool isUnitaryProduct(QString product_code);

public slots:
    virtual bool deleteProduct(const QString& product_code) override;

    virtual QString getProduct(const QString& product_code) override;
    virtual QString getProductName(const QString& product_code) override;

    virtual QString getProductLike(const QString& product_name) override;

    virtual QString getFamilies() override;

    virtual QString getLogo(const QString& product_name) override;
    virtual QString getLogoFromProductCode(const QString& product_code) override;

    virtual QString getLogos(const QString& xml_string) override;

    virtual QString getTaxes() override;

    virtual QString getProductsExtend() override;

    virtual QString getProducts() override;

    virtual QString getUnitaryProducts() override;

    virtual bool insertProduct(const QString& xml_string) override;

    virtual bool updateProduct(const QString& xml_string) override;

    virtual QString getCompositions() override;

    virtual bool insertProductComposition(const QString& xml_string) override;

    virtual bool updateProductComposition(const QString& xml_string) override;

    virtual bool existProductComposition(const QString& xml_string) override;

    virtual QString getIngredients(const QString& product_code) override;

    virtual QString getCompositionsWithIngredient(const QString& ingredient_code) override;

    virtual QString getProductCodeByProductName(const QString& product_name) override;

    virtual int getNumberOfProducts() override;

    virtual void saveBarXml() override;

    virtual void setProductAtPrinter(const QString& product_name, const QString& printer, bool on) override;
    virtual bool getProductAtPrinter(const QString& product_name, const QString& printer) override;

signals:
    /**
        *this signal is emited when a user has been created.
        *@param QString with the employee_id of the user that has been created
        */
    void productCreated(QString);

    /**
        *this signal is emited when a user has been deleted.
        *@param QString with the employee_id of the user that has been deleted
        */
    void productDeleted(QString);

    /**
        *this signal is emited when a user has been Updated.
        *@param QString with the employee_id of the user that has been updated
        */
    void productUpdated(QString);
    void productAtXml(int progress, const QString& product_name);

protected:
    int productPosition(const QString& name, XmlConfig *xml);
    void writeOptions(QList<ProductOptionData*> *options, XmlConfig *xml);
    //        void writeOffers(QList<ProductOfferData*> *offers, XmlConfig *xml);
};

#endif
