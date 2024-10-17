/***************************************************************************
                          bslupdateproductswidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef BSLUPDATEPRODUCTSWIDGET_H
#define BSLUPDATEPRODUCTSWIDGET_H

#include "basicproductcreationwidget.h"

class ProductModule;
class IngredientData;
class ProductData;

class BasicProductUpdateWidget  : public BasicProductCreationWidget{
    Q_OBJECT

public: 
    explicit BasicProductUpdateWidget(ProductModule* _product_mod, QWidget *parent={}, const QString& name=QString{});
    ~BasicProductUpdateWidget() = default;

    bool updateProduct();
    void setProduct(ProductData* prod);
    void showUpdate();
    void showUpdateComposition();
    void hideWidget();
    QPushButton* next() const;
    QPushButton* previous() const;
    QPushButton* cancel() const;

private:
    bool createComposition(const QString& code);
    bool updateCompositionProduct();
    bool updateUnitaryProduct();

    QList<IngredientData*>* getIngredientsProduct(const QString& product_code);

    void showEvent(QShowEvent *e);
    bool isProductEmpty(ProductData *prod);

    ProductModule* product_mod{};
    QList<IngredientData*> *ingredients_list_old{};
};

#endif
