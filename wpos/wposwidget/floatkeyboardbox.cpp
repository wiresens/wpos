/***************************************************************************
                          numkeyboardbox.cpp  -  description
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

#include "ui_numkeyboard.h"
#include "floatkeyboardbox.h"
#include "floatkeyboardbox.h"
#include <QSignalMapper>
#include <QPushButton>
#include <QLCDNumber>
#include <QRegExp>
#include <QLayout>
#include <QString>
#include <QKeyEvent>

#define MAX_DOUBLE 3000.00

#include <iostream>

#define DECIMALS 2

extern "C"{
#include <math.h>
}

using namespace std;

FloatKeyboardBox::FloatKeyboardBox(QWidget *parent) : BasicKeyboardBox(parent), number(0) {
    //prepare the map signal
    connect(signalMapper,SIGNAL(mapped(int)),this,SLOT(receivedNum(int)));
    this->initAspect();
}

FloatKeyboardBox::~FloatKeyboardBox(){}

void FloatKeyboardBox::clear(){
    string_number="";
    number = 0.0;
    ui->display->display(number);
    emit numChanged(number);
}

double FloatKeyboardBox::getNumber(){
    return number;
}

void FloatKeyboardBox::setNumber(const double num){
    string_number = QString::number(num,'f',2);
    number = num;
    ui->display->display(number);
    emit numChanged(number);
}


void FloatKeyboardBox::receivedNum(int num){
    double aux_d=0.0;

    if (init_mode){
        clear();
        setInitMode(false);
    }

    //set the number to 0
    if (num == NUMBER_CE){
        this->clear();
        emit numChanged(number);
    }
    else if (num == NUMBER_DOT){
        QString aux_string_number = string_number;
        aux_string_number+= "00";
        aux_d = pow((double)10.0,(int)DECIMALS);
        aux_d = aux_string_number.toDouble() / aux_d ;

        if (aux_d < MAX_DOUBLE){
            string_number = aux_string_number;
            number= aux_d;
            ui->display->display(number);
            emit numChanged(number);
        }
    }
    else{
        QString aux_string_number = string_number;
        aux_string_number+=QString::number(num);
        aux_d = pow((double)10.0,(int)DECIMALS);
        aux_d = aux_string_number.toDouble() / aux_d ;

        if (aux_d < MAX_DOUBLE){
            string_number = aux_string_number;
            number= aux_d;
            ui->display->display(number);
            emit numChanged(number);
        }
    }
}

void FloatKeyboardBox::initAspect(){
    QFont font;
    font = ui->button_ce->font();
    ui->button_dot->setFont(font);
    ui->button_dot->setText(" 0 0 ");

    ui->display->setSegmentStyle(QLCDNumber::Filled);
    ui->display->setDigitCount(7);
}

void FloatKeyboardBox::keyPressEvent(QKeyEvent *e){
    if (e->text() == "0")
        receivedNum(NUMBER_0);
    else if (e->text() == "1")
        receivedNum(NUMBER_1);
    else if (e->text() == "2")
        receivedNum(NUMBER_2);
    else if (e->text() == "3")
        receivedNum(NUMBER_3);
    else if (e->text() == "4")
        receivedNum(NUMBER_4);
    else if (e->text() == "5")
        receivedNum(NUMBER_5);
    else if (e->text() == "6")
        receivedNum(NUMBER_6);
    else if (e->text() == "7")
        receivedNum(NUMBER_7);
    else if (e->text() == "8")
        receivedNum(NUMBER_8);
    else if (e->text() == "9")
        receivedNum(NUMBER_9);
    else if (e->key() == 4103 )
        receivedNum(NUMBER_CE);
}
