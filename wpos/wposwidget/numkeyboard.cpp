/***************************************************************************
                          numkeyboard.cpp  -  description
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

#include "ui_numpad.h"
#include "numkeyboard.h"
#include <QLayout>
#include <QKeyEvent>

static const int MAX_INT {10000000};

NumKeyboard::NumKeyboard(
    QWidget *parent ) :
    NumPad(parent)
{
    ui->buttonLayout->removeWidget(ui->button_dot);
    ui->button_dot->hide();
    ui->buttonLayout->removeWidget(ui->button_ce);
    ui->buttonLayout->addWidget(ui->button_ce, 3, 1, 1, 2);
    ui->lcd->setSegmentStyle(QLCDNumber::Flat);
}

void NumKeyboard::receiveDigit(int digit){

    if ( isDirty()) clear();

    if ( digit == ce() || digit == dot()) clear();
    else{
        auto result = ( value_ * 10) + digit;
        if (result > MAX_INT) return;
        value_ = result;
        ui->lcd->display(value_);
        emit valueChanged(value_);
    }
}

void NumKeyboard::clear(){
    value_ = 0;
    NumPad::clear();
    emit valueChanged(value_);
}
