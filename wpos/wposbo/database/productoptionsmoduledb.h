/***************************************************************************
                          productoptionsmoduledb.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRODUCTOPTIONSMODULEDB_H
#define PRODUCTOPTIONSMODULEDB_H

#include <wposcore/basicdatabase.h>

#include <QString>
#include <QList>

struct ProductOptionData{
    QString option_type;
    QString description_type;
    QString option_name;
    QString description_option;
    double value;
    bool is_default;
};


class ProductOptionsModuleDB :
        public BasicDatabase
{
public:
    ProductOptionsModuleDB(QString _name_connection, QString _path_connection, QString _database, QString _username, QString _passwd);
    ProductOptionsModuleDB(const QString& _connection_name, XmlConfig *xml);
    ProductOptionsModuleDB(const QString& _connection_name, const QString& configuration_path);
    ~ProductOptionsModuleDB();

    bool existOption(const QString& option_type, const QString& option_name);

    bool existOptionType(const QString& option_type);

    bool deleteOption(const QString& option_type, const QString& option_name);

    bool deleteOptionType(const QString& option_type);

    QString getDescriptionOption(const QString& option_type, const QString& option_name);

    QString getDescriptionOptionType(const QString& option_type);

    ProductOptionData* getOption(const QString& option_type, const QString& option_name);

    QStringList* getOptions(const QString& option_type);

    ProductOptionData* getOptionType(const QString& option_type);

    QList<ProductOptionData*>* getAllOptions();

    QStringList* getOptionTypes();

    bool insertOption(const QString& option_type, const QString& option_name,
                      const QString& option_description);

    bool insertOptionType(const QString& option_type, const QString& description_type);

    bool updateOption(const QString& option_type, const QString& option_name,
                      const QString& description_type);

    bool updateOptionType(const QString& option_type, const QString& description_type);

    bool existProductAndOption(const QString& product_code, const QString& option_type,
                               const QString& option_name);

    QList<ProductOptionData*>* getProductOptions(const QString& product_code);

    bool insertOptionToProduct(const QString& product_code, const QString& option_type,
                               const QString& option_name, double value, const QString& is_default);

    bool updateOptionToProduct(const QString& product_code, const QString& option_type,
                               const QString& option_name, double value, const QString& is_default);

    bool deleteOptionToProduct(const QString& product_code, const QString& option_type,
                               const QString& option_name);

    bool setAllProductOptionValue(const QString& option_type,const QString& option_name, const QString& value);

    bool updateAllProductOptionValue(const QString& option_type,const QString& option_name,const QString& value);

};

#endif
