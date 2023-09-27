/***************************************************************************
                          bsladdscreeninvitationswidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef ADDINVITATIONSCREENWIDGET_H
#define ADDINVITATIONSCREENWIDGET_H

#include "ui_newscreeninvitationsinterface.h"
#include <QObject>
#include <QMenu>

class ProductOfferScreenData;
class XmlConfig;
class QListWidget;

class AddInvitationScreenWidget :
        public QMenu ,
        private Ui::NewScreenInvitationsInterface{
    Q_OBJECT
public:
    explicit AddInvitationScreenWidget(QWidget *parent=0, const QString& name = QString());
    ~AddInvitationScreenWidget() = default;

public slots:
    void apply();
    void cancel();
    void clear();
    void setScreen(const ProductOfferScreenData *screen);
    void showLogos();

signals:
    void screenCreated(ProductOfferScreenData *new_screen);
    void screenUpdated(ProductOfferScreenData *screen, const QString& last_screen);

protected slots:
    void upButtonSlot();
    void downButtonSlot();
    void nameChangedSlot(const QString &text);

protected:

    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

    ProductOfferScreenData *updated_screen{};
    QString selected_logo;
};

#endif
