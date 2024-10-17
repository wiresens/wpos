/***************************************************************************
                          bsldeleteofferproductwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef DELETEOFFERWIDGET_H
#define DELETEOFFERWIDGET_H

#include "ui_deleteofferproductinterface.h"
#include "productsmodule/offersmodule/productoffermodule.h"

class ProductOfferModule;

class DeleteOfferWidget :
                          public QWidget,
                          private Ui::DeleteOfferProductInterface
{
    Q_OBJECT

public:
    explicit DeleteOfferWidget(QWidget *parent=0, const QString& name = QString{});
    ~DeleteOfferWidget() = default;

    void setOffer(const QString& type, const QString& name);
    void showSelectOffer();
    void showOfferType(const QString& offer_type);

signals:
    void offerDeleted(bool deleted);
    void typeDeleted(bool deleted);

private slots:
    void deleteSlot();
    void cancelSlot();
    void acceptSlot();

private:
    void deleteOffer();
    void deleteType();

    ProductOfferModule product_offer_module{};
    QString offer_type, offer_name;
};

#endif
