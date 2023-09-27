/***************************************************************************
                          spkeyboard.h  -  description
                             -------------------
    begin                : dom feb 8 2004
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

#ifndef SPKEYBOARD_H
#define SPKEYBOARD_H

#include "ui_spkeyboardwidget.h"

#include <QObject>
#include <QtWidgets/QWidget>
#include <wposcore/hlist.h>

/**
  *@author Carlos Manzanedo Rueda
  */

class QSignalMapper;
class QPushButton;

class SpKeyboard : public QWidget, private Ui::SpKeyboardWidget{
    Q_OBJECT
public:
    explicit  SpKeyboard(QWidget *parent = nullptr, const QString& name = QString());
    ~SpKeyboard();

    void init();

public slots:
    void sendEvents(bool on);
    void exitWithEnter(bool on);
    void clearSlot();

protected slots:
    void keyClicked(int);
    void exitSlot();

signals:
    void exitClicked();
    void textChanged(const QString& text);
    void enterPressed(const QString &text);

protected:
    HList<QPushButton> *button_list {nullptr};
    QSignalMapper *signal_mapper {nullptr};
    bool send_events {false};
    QObject *o_parent {nullptr};
    QString state;
    bool may {false};
    bool exit_with_enter {false};
};

#endif
