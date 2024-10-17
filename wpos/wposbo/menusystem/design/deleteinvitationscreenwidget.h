/***************************************************************************
                          bsldeletescreeninvitationswidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef DELETEINVITATIONSCREENWIDGET_H
#define DELETEINVITATIONSCREENWIDGET_H

#include <QWidget>
#include <QMenu>

#include "ui_deletescreeninvitationsinterface.h"

class DeleteInvitationScreenWidget :
        public QMenu,
        private Ui::DeleteScreenInvitationsInterface
{
    Q_OBJECT
public:
    explicit DeleteInvitationScreenWidget(QWidget *parent=0, const QString& name=QString{});
    ~DeleteInvitationScreenWidget() = default;

    void setScreen(const QString& screen);
    void showSelectScreen();

signals:
    void screenDeleted(bool deleted);

private slots:
    void acceptSlot();
    void cancelSlot();
    void deleteSlot();

};

#endif
