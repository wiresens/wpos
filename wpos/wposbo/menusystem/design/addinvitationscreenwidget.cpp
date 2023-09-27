/***************************************************************************
                          bsladdscreeninvitationswidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "addinvitationscreenwidget.h"
#include "invitationscreendesignwidget.h"
#include "menusystem/utils.h"

#include <libbslxml/xmlconfig.h>

#include <QString>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>

#include <QPushButton>
#include <QCheckBox>
#include <QListWidget>
#include <QMenu>
#include <QLayout>
#include <QDir>
#include <QLabel>
#include <QPixmap>
#include <QMatrix>
#include <QTimer>

static const QString& INVITATION_XML = "/etc/ntpv_backoffice/invitations_description.xml";
static const double ICON_SIZE = 32.00;
static const QString ICON_PATH = "/usr/share/ntpv/apps/offers/";

AddInvitationScreenWidget::AddInvitationScreenWidget(QWidget *parent, const QString& name):
    QMenu(parent)
{
    setupUi(this);
    setObjectName(name);

//@benes    logo_view->arrangeItemsInGrid(QSize(70,50),true);
//@benes    logo_view->setMaxItemTextLength(70);
//@benes    logo_view->setWordWrapIconText(true);

    up_button->setAutoRepeat(true);
    down_button->setAutoRepeat(true);

    up_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/up.png"));
    down_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/down.png"));
    pix_label1->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/info.png"));
    pix_label2->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/info.png"));
    ok_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_ok_48.png"));
    cancel_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_cancel.png"));

    connect(ok_button, SIGNAL(clicked()), this, SLOT(apply()));
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(screen_name_label,SIGNAL(textChanged(const QString&)), this, SLOT(nameChangedSlot(const QString&)));
    connect(up_button,SIGNAL(clicked()),this,SLOT(upButtonSlot()));
    connect(down_button,SIGNAL(clicked()),this,SLOT(downButtonSlot()));

}

void AddInvitationScreenWidget::nameChangedSlot(const QString& name){
    if (name.isEmpty()) ok_button->setEnabled(false);
    else  ok_button->setEnabled(true);
}

void AddInvitationScreenWidget::apply(){
    ProductOfferScreenData *screen = 0;
    QString last_screen_name;

    auto text = screen_name_label->text();
    auto item = logo_view->currentItem();

    if ( text.isEmpty() || !item) return;

    if (updated_screen){
        screen = updated_screen;
        last_screen_name = screen->text;
    }
    else
        screen = new ProductOfferScreenData();

    screen->text = text;
    screen->name = text+"_entry";
    screen->visible = true;
    screen->enabled = true;
    screen->is_default = entry_default->isChecked();

    if(item) screen->pixmap = ICON_PATH+item->text();
    else  screen->pixmap.clear();

    clear();
    if (updated_screen) emit screenUpdated(screen,last_screen_name);
    else  emit screenCreated(screen);
}

void AddInvitationScreenWidget::cancel(){
    clear();
    if (updated_screen) emit screenUpdated(0, QString());
    else emit screenCreated(0);
}

void AddInvitationScreenWidget::clear(){
    screen_name_label->clear();
    logo_view->clear();
    entry_default->setChecked(false);
    ok_button->setEnabled(false);
}

void AddInvitationScreenWidget::showLogos(){
//@benes    logo_view->setMaxItemTextLength(70);
    logo_view->clear();
    QStringList files = QDir(ICON_PATH).entryList(QStringList("*.png"), QDir::Readable | QDir::Files, QDir::Name);

    QListWidgetItem* item{};
    for(const auto& filename : files){
        auto abs_file_name = ICON_PATH + filename;
        item = new QListWidgetItem(cropedIcon(abs_file_name, ICON_SIZE), filename, logo_view );
//        item->setDragEnabled(false);
    }

    logo_view->setSelectionMode(QAbstractItemView::SingleSelection);

    logo_view->setGridSize(QSize(50,50));
    logo_view->resize(360,160);
    logo_view->show();

    if (!selected_logo.isEmpty()){
        auto text = selected_logo;
        auto pos = text.lastIndexOf("/");
        text = text.mid( pos + 1 , text.length() - pos);
        auto items = logo_view->findItems(text, Qt::MatchContains);
        if (!items.empty()){
            item = items.first();
            logo_view->setCurrentItem(item);
            item->setHidden(false);
            item->setSelected(true);
        }
        selected_logo.clear();
    }
}

void AddInvitationScreenWidget::showEvent(QShowEvent *e){
    QTimer::singleShot(10, this,SLOT(showLogos()));
    QWidget::showEvent(e);
}

void AddInvitationScreenWidget::hideEvent(QHideEvent *e){
    delete updated_screen;
    QWidget::hideEvent(e);
}

void AddInvitationScreenWidget::setScreen(const ProductOfferScreenData *screen){
    delete updated_screen;

    if (!screen) return;
    updated_screen = new ProductOfferScreenData(screen);
    screen_name_label->setText(updated_screen->text);
    selected_logo = updated_screen->pixmap;
}

void AddInvitationScreenWidget::upButtonSlot(){
    auto item = logo_view->currentItem();
    auto items = logo_view->selectedItems();

    if ( !item || !items.isEmpty() ) return;
    else  item = items.first();

    logo_view->setCurrentItem(item);
    item->setHidden(false);
    item->setSelected(true);
}

void AddInvitationScreenWidget::downButtonSlot(){
    auto item = logo_view->currentItem();
    auto items = logo_view->selectedItems();

    if ( !item || !items.isEmpty() ) return;
    else  item = items.last();

    logo_view->setCurrentItem(item);
    item->setHidden(false);
    item->setSelected(true);
}
