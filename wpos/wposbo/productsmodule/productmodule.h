// file      :  productmodule.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef PRODUCTMODULE_H
#define PRODUCTMODULE_H

#include "database/productoptionsmoduledb.h"
#include "productmoduleinterface.h"

#include <libbslxml/xmlconfig.h>

#include <QList>
#include <QObject>
#include <QString>

class ProductModule : public QObject,
                      public ProductModuleInterface {

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.DBusBOProduct")

public:
    static const QString DBusObjectPath;

    explicit ProductModule(QObject* parent = 0, const QString& name = QString {});
    ~ProductModule() = default;

    bool isUnitaryProduct(QString product_code);

public slots:
    Q_SCRIPTABLE bool deleteProduct(const QString& product_code) override;

    Q_SCRIPTABLE QString getProduct(const QString& product_code) override;
    Q_SCRIPTABLE QString getProductName(const QString& product_code) override;

    Q_SCRIPTABLE QString getProductLike(const QString& product_name) override;

    Q_SCRIPTABLE QString getFamilies() override;

    Q_SCRIPTABLE QString getLogo(const QString& product_name) override;
    Q_SCRIPTABLE QString getLogoFromProductCode(const QString& product_code) override;

    Q_SCRIPTABLE QString getLogos(const QString& xml_string) override;

    Q_SCRIPTABLE QString getTaxes() override;

    Q_SCRIPTABLE QString getProductsExtend() override;

    Q_SCRIPTABLE QString getProducts() override;

    Q_SCRIPTABLE QString getUnitaryProducts() override;

    Q_SCRIPTABLE bool insertProduct(const QString& xml_string) override;

    Q_SCRIPTABLE bool updateProduct(const QString& xml_string) override;

    Q_SCRIPTABLE QString getCompositions() override;

    Q_SCRIPTABLE bool insertProductComposition(const QString& xml_string) override;

    Q_SCRIPTABLE bool updateProductComposition(const QString& xml_string) override;

    Q_SCRIPTABLE bool existProductComposition(const QString& xml_string) override;

    Q_SCRIPTABLE QString getIngredients(const QString& product_code) override;

    Q_SCRIPTABLE QString getCompositionsWithIngredient(const QString& ingredient_code) override;

    Q_SCRIPTABLE QString getProductCodeByProductName(const QString& product_name) override;

    Q_SCRIPTABLE int getNumberOfProducts() override;

    Q_SCRIPTABLE void saveBarXml() override;

    Q_SCRIPTABLE void setProductAtPrinter(const QString& product_name, const QString& printer, bool on) override;
    Q_SCRIPTABLE bool getProductAtPrinter(const QString& product_name, const QString& printer) override;

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
    int productPosition(const QString& name, XmlConfig* xml);
    void writeOptions(QList<ProductOptionData*>* options, XmlConfig* xml);
    //        void writeOffers(QList<ProductOfferData*> *offers, XmlConfig *xml);
};

#endif
