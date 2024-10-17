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

    explicit ProductOptionModule(QObject *parent= 0, const QString& name = QString{});
    ~ProductOptionModule() = default;

public slots:
    Q_SCRIPTABLE bool deleteOption(const QString& xml_string) override;

    Q_SCRIPTABLE bool deleteOptionType(const QString& option_type) override;

    Q_SCRIPTABLE bool existOption(const QString& xml_string) override;

    Q_SCRIPTABLE bool existOptionType(const QString& type) override;

    Q_SCRIPTABLE QString getDescriptionOption(const QString& xml_string) override;

    Q_SCRIPTABLE QString getDescriptionOptionType(const QString& option_type) override;

    Q_SCRIPTABLE QString getOption(const QString& xml_string) override;

    Q_SCRIPTABLE QString getOptions(const QString& option_type) override;

    Q_SCRIPTABLE QString getAllOptions() override;

    Q_SCRIPTABLE QString getOptionType(const QString& option_type) override;

    Q_SCRIPTABLE QString getOptionTypes() override;

    Q_SCRIPTABLE bool insertOption(const QString& xml_string) override;

    Q_SCRIPTABLE bool insertOptionType(const QString& xml_string) override;

    Q_SCRIPTABLE bool updateOption(const QString& xml_string) override;

    Q_SCRIPTABLE bool updateOptionType(const QString& xml_string) override;

    Q_SCRIPTABLE bool existProductAndOption(const QString& xml_string) override;

    Q_SCRIPTABLE QString getProductOptions(const QString& product_code) override;

    Q_SCRIPTABLE bool insertOptionsToProduct(const QString& xml_string) override;

    Q_SCRIPTABLE bool updateOptionsToProduct(const QString& xml_string) override;

    Q_SCRIPTABLE bool deleteOptionsToProduct(const QString& product_code, const QString& option_type, const QString& option_name) override;

    Q_SCRIPTABLE void setAllProductOptionValue(const QString& option_type, const QString& option_name, const QString& value) override;

    Q_SCRIPTABLE void updateAllProductOptionValue(const QString& option_type, const QString& option_name, const QString& value) override;

private:
    QString getStringXmlOption(ProductOptionData *option);

private:
    ProductOptionsModuleDB *db;
};

#endif
