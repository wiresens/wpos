/***************************************************************************
                          bslposdesignselectorwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "posdesignselectorwidget.h"

#include "productscreendesignwidget.h"
#include "invitationscreendesignwidget.h"

#include <QListWidget>
#include <QPixmap>
#include <QStackedWidget>
#include <QLabel>
#include <QLocale>

const QString& DESIGN_SCREEN = QObject::tr("Product Screen Setting");
const QString& DESIGN_OFFERS = QObject::tr("Offer Screen Setting");

#include <iostream>
using namespace std;

POSDesignSelectorWidget::POSDesignSelectorWidget(QWidget *parent, const QString&name)
    : QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    action_stack->setCurrentWidget(select_action);

    //Initializations and default selections
    QPixmap default_pixmap("controls48:design1.png");

    actions_iconview->clear();
    // Item that raise <Comand Designer> menu
    auto item = new QListWidgetItem(actions_iconview);
    item->setText(tr("Command Management"));
    item->setIcon(default_pixmap);
    submenu_item_relantionships.insert(item, QString());
    // Item that raise <Screen Products Designer> menu
    item = new QListWidgetItem(actions_iconview);
    item->setText(DESIGN_SCREEN);
    item->setIcon(default_pixmap);
    submenu_item_relantionships.insert(item, "DESIGN_SUBMENU_POS_SELECTOR_SUBMENU_SPD");
    // Item that raise <> menu
    item = new QListWidgetItem(actions_iconview);
    item->setText(tr("Quick Access Area Management"));
    item->setIcon(default_pixmap);
    submenu_item_relantionships.insert(item,"");
    // Item that raise <> menu
    item = new QListWidgetItem(actions_iconview);
    item->setText(tr("Action Menu Management"));
    item->setIcon(default_pixmap);
    submenu_item_relantionships.insert(item,"");
    // Item that raise <> menu
    item = new QListWidgetItem(actions_iconview);
    item->setText(tr("Calculator Setting"));
    item->setIcon(default_pixmap);
    submenu_item_relantionships.insert(item,"");

    item = new QListWidgetItem(actions_iconview);
    item->setText(DESIGN_OFFERS);
    item->setIcon(default_pixmap);
    submenu_item_relantionships.insert(item,"");

    connect(actions_iconview, &QListWidget::itemSelectionChanged, this, &POSDesignSelectorWidget::actionSelectedChanged);
}

void POSDesignSelectorWidget::actionSelectedChanged(){
    QString action_selected;

    auto act_view = qobject_cast<QListWidget*>(sender());
    auto selection = act_view->selectedItems();

    if(selection.isEmpty()) return;
    action_selected = selection.first()->text();

    if(action_selected == DESIGN_SCREEN){
        action_stack->setCurrentWidget(action);

        if(!screen){
            screen = new ProductScreenDesignWidget(action, "SCREEN_PRODUCTS");
            connect(screen, &ProductScreenDesignWidget::cancel, this, &POSDesignSelectorWidget::cancel);
        }

        title->setText(action_selected);
        screen->show();
    }

    if(action_selected == DESIGN_OFFERS){
        action_stack->setCurrentWidget(action);

        if(!offers){
            offers = new InvitationScreenDesignWidget(action, "SCREEN_OFFERS");
            connect(offers, &InvitationScreenDesignWidget::cancel, this, &POSDesignSelectorWidget::cancel);
        }

        title->setText(action_selected);
        offers->show();
    }
}

void POSDesignSelectorWidget::cancel(){
    action_stack->setCurrentWidget(select_action);
    actions_iconview->clearSelection();
}

void POSDesignSelectorWidget::showEvent(QShowEvent *e){
    action_stack->setCurrentWidget(select_action);
    actions_iconview->clearSelection();

    QWidget::showEvent(e);
}
