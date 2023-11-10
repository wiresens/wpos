/***************************************************************************
                          basickeyboardbox.h  -  description
                             -------------------
    begin                :  Aug 20 2003
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

#ifndef BASICKEYBOARDBOX_H
#define BASICKEYBOARDBOX_H

#include "ui_numkeyboard.h"
/**
  *@author Carlos Manzanedo
  */
const int NUMBER_0=0;
const int NUMBER_1=1;
const int NUMBER_2=2;
const int NUMBER_3=3;
const int NUMBER_4=4;
const int NUMBER_5=5;
const int NUMBER_6=6;
const int NUMBER_7=7;
const int NUMBER_8=8;
const int NUMBER_9=9;
const int NUMBER_CE=-1;
const int NUMBER_DOT=-2;

class QSignalMapper;

class BasicKeyboardBox :
    public QWidget,
    protected Ui::NumKeyboard
{
    Q_OBJECT
public:
    explicit BasicKeyboardBox(QWidget* parent = nullptr);
    ~BasicKeyboardBox();

    bool initMode();
    void setInitMode(bool status = true);

public slots:
    //if the num == -1 then the CE button has been pressed ( use negative numbers for special buttons)
    virtual void clear();
    virtual void hideNumber(bool state);

protected:
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);

    QSignalMapper* signalMapper;
    QVector<QWidget*> buttons;
    bool init_mode{false};
};

#endif
