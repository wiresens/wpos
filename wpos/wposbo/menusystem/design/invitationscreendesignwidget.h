/***************************************************************************
                          bslscreeninvitationsdesignwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef INVITATIONSCREENDESIGNWIDGET_H
#define INVITATIONSCREENDESIGNWIDGET_H

#include "ui_screeninvitationsdesigninterface.h"

#include "productsmodule/offersmodule/productoffermodule.h"
#include "database/productoffersmoduledb.h"

#include <libbslxml/xmlconfig.h>
#include <wposcore/hlist.h>

class ProductOfferScreenData {
public:

    ProductOfferScreenData(const ProductOfferScreenData *data):
        ProductOfferScreenData(*data){}

    ProductOfferScreenData() = default;
//    ~ProductOfferScreenData() = default;
//    ProductOfferScreenData& operator=(ProductOfferScreenData& data) = default;

    HList<ProductOfferData> product_offers;
    QString name;
    QString text;
    QString pixmap;
    bool enabled{true};
    bool visible{true};
    bool is_default{false};
};

class BslDDIconView;

class ProductOfferCreationWidget;
class AddInvitationScreenWidget;
class DeleteOfferWidget;
class DeleteInvitationScreenWidget;

class QMenu;
class QToolBox;
class QScrollArea;
class QListWidget;
class QListWidgetItem;

class InvitationScreenDesignWidget :
        public QWidget,
        private Ui::ScreenInvitationsDesignInterface {
    Q_OBJECT
public:
    explicit InvitationScreenDesignWidget(QWidget *parent = 0, const QString& name = QString{});
    ~InvitationScreenDesignWidget() = default;

public slots:
    void acceptSlot();
    void cancelSlot();

    void leftButtonClicked();
    void rightButtonClicked();

//@benes    void offerDropped(QDropEvent *e, const QList<QIconDragItem> & lst);
    void screenChanged(QListWidgetItem *item, QListWidgetItem* previous);

    void deleteOfferFromScreen();

    void newScreenClicked();
    void showPopNewScreen();
    void hidePopNewScreen();

    void screenCreatedSlot(ProductOfferScreenData *new_screen);
    void screenUpdatedSlot(ProductOfferScreenData *new_screen,const QString& old_screen);

    void updateScreenClicked();
    void showPopUpdateScreen();
    void hidePopUpdateScreen();

    void deleteScreenClicked();
    void showPopDeleteScreen();
    void hidePopDeleteScreen();

    void screenDeletedSlot(bool deleted);

    void startShowing();
    void offerReadedSlot(int num,const QString& screen_name,const QString offer_label);

signals:
    void cancel();
    void offerReaded(int num,const QString& screen_name,const QString offer_label);

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

    void setEnabledScreen(bool enabled, const int group_pressed);

    void showConfig();
    void showToolBox();
    void showViews();
    void showScreen(const QString& screen_name);

    void clear();
    void clearToolBox();
    void clearViews();

    void writeXml();
    void readXml();

    ProductOfferData* getOffer(QString offer_type,QString offer_name);
    QString getMainWidgetName();

private:
    AddInvitationScreenWidget *invitations_menu_item{};
    QMenu *invitations_menu{};

    AddInvitationScreenWidget *update_invitations_menu_item{};
    QMenu *update_invitations_menu{};

    DeleteInvitationScreenWidget *delete_invitations_widget{};
    QMenu *delete_invitations_menu{};

    HList<ProductOfferScreenData> screen_data_list{};

    QToolBox *offers_box{};
    QScrollArea *scroll_screen{};
    QListWidget *screens_view{};
    BslDDIconView *screen_selected{};
    bool first_show{false};
};

#endif
