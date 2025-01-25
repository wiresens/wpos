/***************************************************************************
                          ndelofferwidget.h  -  description
                             -------------------
    begin                : vie abr 23 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef NDELOFFERWIDGET_H
#define NDELOFFERWIDGET_H

#include "productsmodule/offersmodule/productoffermodule.h"
#include "ui_offerdeletionwidgetbase.h"

class OfferDeletionWidget : public QWidget,
                            private Ui::OfferDeletionWidgetBase {

    Q_OBJECT

public:
    enum OfferColumn {
        Icon = 0,
        Name = 1
    };

    explicit OfferDeletionWidget(QWidget* parent = 0, const QString& name = QString {});
    ~OfferDeletionWidget() = default;

public slots:
    void clear();
    void handleOfferListSelected();
    void handleOfferTreeSelected();

    void previousOffer();
    void nextOffer();
    void previousType();
    void nextType();

    void deleteOfferType();
    void acceptSlot();

protected:
    virtual void showEvent(QShowEvent* e) override; // allmost
    bool hasAllValues();

    void getOfferTypes(); // idem
    void showOffers(const QString& offer_type);

private:
    ProductOfferModule mod;
};

#endif
