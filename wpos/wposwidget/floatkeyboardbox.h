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

#ifndef FLOATKEYBOARDBOX_H
#define FLOATKEYBOARDBOX_H

#include <QWidget>
#include "basickeyboardbox.h"

/**
  *@author Carlos Manzanedo
  */

class FloatKeyboardBox : public BasicKeyboardBox {
    Q_OBJECT
public:
    explicit FloatKeyboardBox(QWidget *parent=nullptr);
    ~FloatKeyboardBox();

    double getNumber();
    void setNumber(const double num);
public slots:
    //if the num == -1 then the ce button has been pressed (use - numbers to speccial buttons)
    void receivedNum(int);
    void clear();

signals:
    void numChanged(double);

protected:
    void keyPressEvent(QKeyEvent *e);
    void initAspect();

private:
    double number;
    QString string_number;
};

#endif
