/***************************************************************************
                          bsldeletescreeninvitationswidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "deleteinvitationscreenwidget.h"

#include <QLabel>
#include <QStackedWidget>
#include <QPushButton>

DeleteInvitationScreenWidget::DeleteInvitationScreenWidget(
        QWidget *parent, const QString& name):
    QMenu(parent)
{
    setupUi(this);
    setObjectName(name);
    screen_name_label->clear();
    connect(accept_button, SIGNAL(released()), this, SLOT(acceptSlot()));
    connect(cancel_button, SIGNAL(released()), this, SLOT(cancelSlot()));
    connect(delete_button, SIGNAL(released()), this, SLOT(deleteSlot()));
}

void DeleteInvitationScreenWidget::setScreen(const QString& screen){
    delete_stack->setCurrentWidget(delete_screen_page);
    screen_name_label->setText(screen);
}

void DeleteInvitationScreenWidget::showSelectScreen(){
    delete_stack->setCurrentWidget(select_screen_page);
}

void DeleteInvitationScreenWidget::acceptSlot(){
    delete_stack->setCurrentWidget(delete_screen_page);
    emit screenDeleted(false);
}

void DeleteInvitationScreenWidget::cancelSlot(){
    emit screenDeleted(false);
}

void DeleteInvitationScreenWidget::deleteSlot(){
    emit screenDeleted(true);
}
