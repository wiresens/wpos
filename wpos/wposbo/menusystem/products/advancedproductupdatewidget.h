/***************************************************************************
                          nupdateselectionwidget.h  -  description
                             -------------------
    begin                : lun may 3 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef NUPDATESELECTIONWIDGET_H
#define NUPDATESELECTIONWIDGET_H

#include "advancedproducteditionwidget.h"

class ProductModule;
class AdvancedProductInsertionWidget;

class AdvancedProductUpdateWidget :
        public AdvancedProductEditionWidget
{
    Q_OBJECT

public:
    explicit AdvancedProductUpdateWidget(ProductModule *product_model, QWidget *parent=0, const QString& name= QString{});
    ~AdvancedProductUpdateWidget() = default;

public slots:    
    virtual void acceptSlot() override;

protected slots:
    void nUpdateAcceptSlot();
    void nUpdateCancelSlot();

private:
    AdvancedProductInsertionWidget *product_insertion_form{};
};

#endif
