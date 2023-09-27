/***************************************************************************
                          numkeyboardbox.h  -  description
                             -------------------
    begin                : lun jun 30 2003
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

#ifndef NUMKEYBOARDBOX_H
#define NUMKEYBOARDBOX_H

#include <QWidget>
#include "basickeyboardbox.h"
#include <QSignalMapper>
#include <QPushButton>
#include <QLCDNumber>
#include <QLayout>
#include <QEvent>

/**
  *@author Carlos Manzanedo
  */

class QSignalMapper;

class NumKeyboardBox : public BasicKeyboardBox{
    Q_OBJECT
public:
    explicit  NumKeyboardBox(QWidget *parent);
    ~NumKeyboardBox();

    int getNumber();

public slots:
    //if the num == -1 then the ce button has been pressed (use - numbers to speccial buttons)
    void receivedNum(int);
    void clear();

signals:
    void numChanged(int);

protected:
    void keyPressEvent(QKeyEvent *e);
    void initialize();
    int number;
};
#endif
