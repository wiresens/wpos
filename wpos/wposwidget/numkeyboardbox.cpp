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

#include "numkeyboardbox.h"
#include <QSignalMapper>
#include <QPushButton>
#include <QLCDNumber>
#include <QLayout>
#include <QKeyEvent>

#define MAXNUMBER 10000000000

#include <iostream>

using namespace std;

NumKeyboardBox::NumKeyboardBox(QWidget *parent ) : BasicKeyboardBox(parent), number(0) {
    //prepare the map signal
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(receivedNum(int)));
    initialize();
}

NumKeyboardBox::~NumKeyboardBox(){
}

void NumKeyboardBox::clear(){
    number = 0;
    display->display((int) number);
    emit numChanged(number);
}

int NumKeyboardBox::getNumber(){
    return number;
}

void NumKeyboardBox::receivedNum(int num){
    int result;
    if (init_mode){
        clear();
        setInitMode(false);
    }
    //set the number to 0
    if (num == NUMBER_CE){
        number = 0;
        display->display(number);
    }
    else if (num == NUMBER_DOT){
        number = 0;
        display->display(number);
    }
    else{
        result = (number * 10) + num;
        if (result>MAXNUMBER)
            return;
        number = result;
        display->display(number);
    }
    emit numChanged(number);
}

void NumKeyboardBox::initialize(){
    display->setSegmentStyle(QLCDNumber::Flat);
//    button_dot->hide();
//    button_ce->hide();

    QGridLayout *layout = qobject_cast<QGridLayout *>( frame15->layout() );

    layout->removeWidget(button_dot);
    button_dot->hide();
    layout->removeWidget(button_ce);
    layout->addWidget(button_ce,3,1,1,2);
}

void NumKeyboardBox::keyPressEvent(QKeyEvent *e){
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

