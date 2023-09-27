/***************************************************************************
                          bslmenusmanager.h  -  description
                             -------------------
        begin                 : mon Jun 2 2003
        copyright          : (C) 2003 by Napsis S.L.
        email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef MENUSMANAGER_H
#define MENUSMANAGER_H

#include "ui_menusmanager.h"
#include <QList>
#include <QMap>
#include <QWidget>
#include <QStackedWidget>

class MenusManager : public QWidget , private Ui::MenusManager {
    Q_OBJECT
public:
    explicit  MenusManager(QWidget *parent=0, const QString& name = QString());
    ~MenusManager();
    QWidget *getCurrentMenu();
    void addMenu(const QString &name, QWidget *w);

public slots:
    void showScreenSlot(const QString &menu, const QString &subMenu);
    void setMenu(QWidget *w);
    void setMenu(int id);
    void setMenu(QString name);
    void setEnabled(bool enabled);

signals:
    void showScreen(const QString &subMenu);

private:
    QList<QWidget*> menusList{};
    QMap<QString, QWidget*> menusDict{};
};

#endif
