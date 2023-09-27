/***************************************************************************
                          bslselectproductwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef BSLSELECTPRODUCTWIDGET_H
#define BSLSELECTPRODUCTWIDGET_H

#include "ui_basicproducteditionwidgetbase.h"

#include <QString>

class BasicProductUpdateWidget;
class ProductModule;
class ProductData;

class BasicProductEditionWidget :
        public QWidget,
        private Ui::BasicProductEditionWidgetBase
{
    Q_OBJECT

public:
    enum OperationType{
        DeleteProduct = 0,
        UpdateProduct =1
    };

    enum Column{
        Icon = 0,
        Name = 1,
        Code = 2
    };

    BasicProductEditionWidget(ProductModule *_product_mod, int _action,
                           QWidget *parent=0, const QString& name= QString());

    ~BasicProductEditionWidget() = default;

public slots:
    void productUpdatedSlot(QString);
    void productDeletedSlot(QString);

private slots:
    void productNameChanged(const QString& text);
    void productSelectedChanged(QTreeWidgetItem* item);
    void acceptSlot();
    void cancelSlot();
    void nextSlot();
    void previousSlot();
    void clear();

    void upClickedSlot();
    void downClickedSlot();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void init();
    void showSelectProduct();
    void deleteProduct();
    void getProductFromItem(QTreeWidgetItem *item);

    ProductData *product_data{};
    ProductModule *product_mod{};
    BasicProductUpdateWidget *update_product{};
    QString last_name;

    int action;
};

#endif
