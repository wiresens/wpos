/***************************************************************************
                          nupdateproductwidget.h  -  description
                             -------------------
    begin                : lun may 3 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef NUPDATEPRODUCTWIDGET_H
#define NUPDATEPRODUCTWIDGET_H

#include "productinsertionwidget.h"

/**
  *@author Carlos Manzanedo
  */

class AdvancedProductInsertionWidget : public ProductInsertionWidget  {
    Q_OBJECT
public:
    explicit AdvancedProductInsertionWidget(ProductModule *product_module , int mode=0, QWidget *parent=0, const QString& name = QString());
    ~AdvancedProductInsertionWidget() = default;

public slots:
    void clear();
    void setMode(int num);
    void setProductWithName(const QString& product);
    void setProduct(const QString& product_code);
    void numkeyChangedSlot(double);

    void acceptSlot();

protected:
    void showEvent(QShowEvent *e);
    void setComposition(const QString& product_code);
    void setOptions(const QString& product_code);
    void setOffers(const QString& product_code);
    void setLogo(const QString& logo);

    bool saveProduct();
    bool saveProductComposition();

    QString p_code;
};

#endif
