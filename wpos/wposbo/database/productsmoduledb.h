/***************************************************************************
                          product.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRODUCTSMODULEDB_H
#define PRODUCTSMODULEDB_H

#include <wposcore/basicdatabase.h>

#include <QString>

template<typename T> class QList;
template<typename Key, typename Value> class QMap;

template <typename Value>
using QPtrMap = QMap<QString, Value*>;

template <typename Value>
using QPtrList = QList<Value*>;

struct ProductData{
    ProductData( const ProductData *product): ProductData(*product){}
    ProductData() = default;
//    ~ProductData() = default;
//    ProductData(const ProductData& p) = default;
//    ProductData& operator=(const ProductData& p) = default;

    QString code;
    QString name;
    QString tax;
    QString logo;
    QString description;
    double price{0.0};
};

struct IngredientData{
    QString ingredient_code;
    double quantity;
};

class ProductsModuleDB : public BasicDatabase {
public: 
    ProductsModuleDB(QString _name_connection,
                     QString _path_connection,
                     QString _database,
                     QString _username,
                     QString _passwd);

    ProductsModuleDB(const QString& _connection_name,
                     XmlConfig *xml);

    ProductsModuleDB(const QString& _connection_name,
                     const QString& configuration_path);

    virtual ~ProductsModuleDB() = default;

    bool existProduct(const QString& product_code);

    bool deleteProduct(const QString& product_code);

    ProductData* getProduct(const QString& product_code);

    QString getProductLike(const QString& product_name);

    QStringList* getFamilies();

    ProductData* getLogo(const QString& product_name);

    QPtrList<ProductData>* getLogos(QStringList products_name);

    QStringList* getTaxes();

    QPtrList<ProductData>* getProducts();

    QPtrList<ProductData>* getProductsExtend();

    QPtrList<ProductData>* getUnitaryProducts();

    bool isUnitaryProduct(const QString& product_code);

    bool insertProduct(ProductData *product);

    bool updateProduct(ProductData *product);

    QPtrList<ProductData>* getCompositions();

    bool insertProductComposition(const QString& composition_code,
                                  const QString& ingredient_code,
                                  double quantity, int ordering=0);

    bool updateProductComposition(const QString& composition_code,
                                  const QString& ingredient_code,
                                  double quantity,int ordering=0);

    bool existProductComposition(const QString& compositon_code,
                                 const QString& ingredient_code);

    QPtrList<IngredientData>* getIngredients(const QString& composition_code);

    QPtrList<ProductData>* getCompositionsWithIngredient(const QString& ingredient_code);

    QString getProductCodeByProductName(const QString& product_name);

    int getNumberOfProducts();

    void setProductAtPrinter(const QString& product_name,const QString& printer,bool on);

    bool getProductAtPrinter(const QString& product_name,const QString& printer);
};

#endif
