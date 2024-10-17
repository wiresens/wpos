/***************************************************************************
                          bslinvitationallocatorwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#ifndef INVITATIONALLOCATORWIDGET_H
#define INVITATIONALLOCATORWIDGET_H

#include <wposcore/hlist.h>

#include "ui_invitationallocatorwidget.h"
#include <QIcon>
#include <QList>
#include <QEvent>

class XmlConfig;
class QListWidgetItem;

class ItemOfferData {
public:
    QString type;
    QString name;
};

class InvitationAllocatorWidget : public QWidget, private Ui::InvitationAllocatorWidget {
    Q_OBJECT
public:
    explicit InvitationAllocatorWidget(QWidget *parent=0, const QString& name = QString{});
    ~InvitationAllocatorWidget();

public slots:
    void raiseMenu(QListWidgetItem *item);
    void invitationSelected(const QModelIndex &index);
    void cancelSlot();

    void genericSignalSlot(const QString& signal_name);

signals:
    void genericDataSignal(const QString &signal_name,XmlConfig *xml);
    
protected:
    void showEvent(QShowEvent *e);
    void parseXmlDescription();

    HList<QListWidgetItem> menu_items;
    QMap<QString, QWidget*> menus;
    QList<QListWidget*> iconviews;
    QList<QWidget*> pages;
    QMap<QString, ItemOfferData*> data_items;
    QListWidgetItem *default_item{};
};

#endif
