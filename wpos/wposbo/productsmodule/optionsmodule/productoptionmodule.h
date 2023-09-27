/***************************************************************************
                          productoption.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRODUCTOPTIONMODULE_H
#define PRODUCTOPTIONMODULE_H

#include "productoptionmoduleinterface.h"

#include "database/productoptionsmoduledb.h"

#include <wposcore/basicdatabase.h>
#include <libbslxml/xmlconfig.h>

#include <QtCore/QObject>
#include <QString>

class ProductOptionsModuleDB;

class ProductOptionModule :
        public QObject,
        public ProductOptionModuleInterface {

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.DBusBOProductOption")

public:
    static const QString DBusObjectPath;

    explicit ProductOptionModule(QObject *parent= 0, const QString& name = QString());
    ~ProductOptionModule() = default;

public slots:
    bool deleteOption(const QString& xml_string) override;

    bool deleteOptionType(const QString& option_type)override;

    bool existOption(const QString& xml_string)override;

    bool existOptionType(const QString& type)override;

    QString getDescriptionOption(const QString& xml_string)override;

    QString getDescriptionOptionType(const QString& option_type)override;

    QString getOption(const QString& xml_string)override;

    QString getOptions(const QString& option_type)override;

    QString getAllOptions()override;

    QString getOptionType(const QString& option_type)override;

    QString getOptionTypes()override;

    bool insertOption(const QString& xml_string)override;

    bool insertOptionType(const QString& xml_string)override;

    bool updateOption(const QString& xml_string) override;

    bool updateOptionType(const QString& xml_string) override;

    bool existProductAndOption(const QString& xml_string) override;

    QString getProductOptions(const QString& product_code) override;

    bool insertOptionsToProduct(const QString& xml_string) override;

    bool updateOptionsToProduct(const QString& xml_string) override;

    bool deleteOptionsToProduct(const QString& product_code, const QString& option_type, const QString& option_name) override;

    void setAllProductOptionValue(const QString& option_type, const QString& option_name, const QString& value) override;

    void updateAllProductOptionValue(const QString& option_type, const QString& option_name, const QString& value) override;

private:
    QString getStringXmlOption(ProductOptionData *option);

private:
    ProductOptionsModuleDB *db;
};

#endif
