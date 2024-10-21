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
    cefontIsDotfont();
    dotInit(" 0 0 ");
//    setDigitCount(7);
    setSegmentStyle(QLCDNumber::Filled);
}

void FloatKeyboard::reeadKey(int digit){

    if (isDirty()) clear();

    if ( digit == ce() ) clear();
    else{
        auto tmpStr = m_value_str;
        if ( digit == dot()) tmpStr += "00";
        else tmpStr += QString::number(digit);
        double tmpNumber = tmpStr.toDouble() / std::pow(10.0, DECIMALS) ;

        if ( tmpNumber < MAX_DOUBLE){
            m_value_str = tmpStr;
            m_value = tmpNumber;
            display(m_value);
            emit valueChanged(m_value);
        }
    }
}

void FloatKeyboard::clear(){
    m_value_str.clear();
    m_value = 0.0;
    NumPad::clear();
    emit valueChanged(m_value);
}

void FloatKeyboard::setValue(double value){
    m_value = value;
    m_value_str = QString::number(value, 'f', 2);
    display(m_value);
    emit valueChanged(m_value);
}
