/***************************************************************************
                          shortcutbuttonwidget.h  -  description
                             -------------------
    begin                : Sat feb 5 2005
    copyright            : (C) 2005 by Carlos Manzanedo
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SHORTCUTBUTTONWIDGET_H
#define SHORTCUTBUTTONWIDGET_H

#include "ui_shortcutbuttoninterface.h"
#include <QWidget>
#include <QtCore/QObject>

#include <wposcore/hlist.h>

class QListWidgetItem;
class BslDDTable;
class BslDDIconView;

class ShortcutButtonData {
public:
    enum ButtonType {
        Product = 0,
        Offer = 1,
        Option = 2,
        SpecialButton = 3,
        ReadedFromXml = 4
    };

    QString text;
    QString type;
    QString name;
    QString code;
    QString pixmap;
    ButtonType button_type { ShortcutButtonData::Product };
    int num {};
    bool state { false };
};

class ShortcutButtonWidget : public QWidget,
                             private Ui::ShortcutButtonInterface {

    Q_OBJECT
public:
    enum SpecialButtons {
        FreePrice = 0,
        GenericProduct = 1,
        OpenCashBox = 2,
        PrintAtKitchen = 3
    };

    explicit ShortcutButtonWidget(QWidget* parent = 0, const QString& name = QString {});
    ~ShortcutButtonWidget();

public slots:
    void acceptSlot();
    void reloadSlot();

    void selectFirstSlot();
    void scrollUpSlot();
    void scrollDownSlot();
    void selectLastSlot();

protected slots:
    void draggedText(int x, int y, const QString& text);
    void loadShortcutButtonsSlot();
    void loadProductsSlot();
    void loadOffersSlot();
    void loadOptionsSlot();
    void loadSpecialButtons();

    void writeConfig();
    void startShowing();
    void itemReadedSlot(int num, const QString& text1, const QString& text2);

signals:
    void itemReaded(int num, const QString& text1, const QString& text2);

protected:
    void clear();
    void clearTable();

    void readButtonsXml();
    void showButtonsAtList();

    void showEvent(QShowEvent* e);

    BslDDTable* shortcut_button_table {};
    BslDDIconView* products_iconview {};
    BslDDIconView* offers_iconview {};
    BslDDIconView* options_iconview {};
    BslDDIconView* special_buttons_iconview {};

    HList<ShortcutButtonData>* button_list {};
    bool first_show { true };
};

#endif
