/***************************************************************************
                          popupkeyboard.h  -  description
                             -------------------
    begin                : vie feb 13 2004
    copyright            : (C) 2004 by Napsis S.L.
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

#ifndef POPUPKEYBOARD_H
#define POPUPKEYBOARD_H

#include <QMenu>
/**
  *@author carlos Manzanedo Rueda
  */

class SpKeyboard;
class QString;

class PopUpKeyboard : public QMenu{
    Q_OBJECT
public:
    explicit PopUpKeyboard(
        QWidget *parent = nullptr,
        const QString& name = QString{}
    );
    ~PopUpKeyboard();

signals:
    void textChanged(const QString &);
    void exitClicked();

protected slots:
    void enterPressedSlot(const QString &text_string);

protected:
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;
    SpKeyboard *kb;
};
#endif
