/***************************************************************************
                          floatkeyboard.cpp  -  description
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
#include "floatkeyboard.h"
#include <QLayout>
#include <QKeyEvent>

#include <cmath>

static const float MAX_DOUBLE {3000.00};
static const int DECIMALS {2};

FloatKeyboard::FloatKeyboard(
    QWidget *parent):
    NumPad(parent)
{
    ui->button_dot->setFont(ui->button_ce->font());
    ui->button_dot->setText(" 0 0 ");
//    ui->display->setDigitCount(7);
    ui->lcd->setSegmentStyle(QLCDNumber::Filled);
}

void FloatKeyboard::receiveDigit(int digit){

    if (isDirty()) clear();

    if ( digit == ce() ) clear();
    else{
        auto tmpStr = numStr;
        if ( digit == dot()) tmpStr += "00";
        else tmpStr += QString::number(digit);
        double tmpNumber = tmpStr.toDouble() / std::pow(10.0, DECIMALS) ;

        if ( tmpNumber < MAX_DOUBLE){
            numStr = tmpStr;
            value_ = tmpNumber;
            ui->lcd->display(value_);
            emit valueChanged(value_);
        }
    }
}

void FloatKeyboard::clear(){
    numStr.clear();
    value_ = 0.0;
    NumPad::clear();
    emit valueChanged(value_);
}

void FloatKeyboard::setValue(double value){
    numStr = QString::number(value, 'f', 2);
    value_ = value;
    ui->lcd->display(value_);
    emit valueChanged(value_);
}
