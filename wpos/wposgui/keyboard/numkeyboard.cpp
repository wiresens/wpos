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
    removeKey( dotKey() );
    dotVisible(false);
    removeKey( ceKey() );
    addKey( ceKey() , 3, 1, 1, 2);
    setSegmentStyle(QLCDNumber::Flat);
}

void NumKeyboard::reeadKey(int digit){

    if ( isDirty()) clear();

    if ( digit == ce() || digit == dot()) clear();
    else{
        auto result = ( m_value * 10) + digit;
        if (result > MAX_INT) return;
        m_value = result;
        display(m_value);
        emit valueChanged(m_value);
    }
}

void NumKeyboard::clear(){
    m_value = 0;
    NumPad::clear();
    emit valueChanged(m_value);
}
