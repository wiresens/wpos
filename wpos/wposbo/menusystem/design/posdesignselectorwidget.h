/***************************************************************************
                          bslposdesignselectorwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef POSDESIGNSELECTORWIDGET_H
#define POSDESIGNSELECTORWIDGET_H

#include "ui_posdesignselectorinterface.h"

class ProductScreenDesignWidget;
class InvitationScreenDesignWidget;

class POSDesignSelectorWidget :
        public QWidget,
        private Ui::POSDesignSelectorInterface
{
    Q_OBJECT

public:
    explicit POSDesignSelectorWidget(QWidget *parent = 0, const QString& name = QString{});
    ~POSDesignSelectorWidget() = default;

private slots:
    void actionSelectedChanged();
    void cancel();
    virtual void showEvent(QShowEvent *e) override;

private:
    ProductScreenDesignWidget *screen;
    InvitationScreenDesignWidget *offers;
    QMap<QListWidgetItem*, QString> submenu_item_relantionships;

};

#endif
