/***************************************************************************
                          npopupkeyboard.h  -  description
                             -------------------
    begin                : lun jun 7 2004
    copyright            : (C) 2004 by Carlos Manzanedo
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

#ifndef NPOPUPKEYBOARD_H
#define NPOPUPKEYBOARD_H

#include <QMenu>
#include <QStringList>
#include <wposcore/hlist.h>

class NKeyboard;
class QString;
class ProductExtraInfo;

class NPopUpKeyboard : public QMenu{
    Q_OBJECT

public:
    explicit NPopUpKeyboard(QWidget *parent=0);

    explicit NPopUpKeyboard(
        const QString& text,
        QWidget *parent=0
    );

    void showMenu(const QPoint& pos);

    bool isAtTable();
    int getTable();
    QString getLounge();

    HList<ProductExtraInfo>*
    getOptionsFromLounge(const QString& lounge);

signals:
    void exitClicked();
    void textChanged(const QString &);
    void loungeRequested(bool on);

public slots:
    void setText(const QString& text);
    void showLounge();
    void showKeyBoard();
    void setUsedTables(const QStringList &tbl_codes);

protected slots:
    void enterPressedSlot(const QString &text);

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private:
    NKeyboard *m_nkeyboard{};
    QAction* m_action;
};

#endif
