/***************************************************************************
                          ndelproductwidget.h  -  description
                             -------------------
    begin                : lun may 3 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef NDELPRODUCTWIDGET_H
#define NDELPRODUCTWIDGET_H

#include "ui_advancedproducteditionwidgetbase.h"

class ProductModule;
class QTreeWidgetItem;

class AdvancedProductEditionWidget : public QWidget,
                                     protected Ui::AdvancedProductEditionWidgetBase {
    Q_OBJECT

public:
    enum ProductColumn {
        Icon = 0,
        Name = 1,
        Code = 2
    };

    explicit AdvancedProductEditionWidget(ProductModule* product_model, QWidget* parent = 0, const QString& name = QString {});
    ~AdvancedProductEditionWidget() = default;

public slots:
    void clear();
    void upButtonSlot();
    void downButtonSlot();

    virtual void acceptSlot() = 0;
    void cancelSlot();

    void productSelectedSlot();
    void nameChangedSlot(const QString& text);
    void searchButtonSlot();

protected slots:
    void startShowing();
    void setProgressSlot(int num, const QString& product);

signals:
    void progressSignal(int num, const QString& product);

protected:
    virtual void showEvent(QShowEvent* e) override;
    void loadList();
    bool isItemSelected();
    QTreeWidgetItem* selectedItem() const;

    inline ProductModule& productModel() const
    {
        return *product_model;
    }

private:
    ProductModule* product_model;
};

#endif
