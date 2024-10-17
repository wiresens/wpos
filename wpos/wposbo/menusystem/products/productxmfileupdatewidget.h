/***************************************************************************
                          updatexml.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef UPDATEXML_H
#define UPDATEXML_H

#include "ui_productxmfileupdatewidgetbase.h"

#include <QWidget>

class ProductModule;
class ProductXmFileUpdateWidget :
        public QWidget,
        private Ui::ProductXmFileUpdateWidgetBase
{
    Q_OBJECT

public:
    explicit ProductXmFileUpdateWidget(ProductModule *_product_mod, QWidget *parent=0, const QString &name=QString{});
    ~ProductXmFileUpdateWidget() = default;

public slots:
    void acceptSlot();
    void updatingProduct(int num, const QString &product);

protected:
    void showEvent(QShowEvent *e);

private:
    ProductModule *product_mod{};
    int products{-1};
};

#endif
