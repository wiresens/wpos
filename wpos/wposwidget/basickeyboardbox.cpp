/***************************************************************************
                          basickeyboardbox.cpp  -  description
                             -------------------
    begin                :  Aug 20 2003
    copyright            : (C) 2003 by Carlos Manzanedo
    email                : support@bluesock.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "basickeyboardbox.h"
#include <QSignalMapper>
#include <QPushButton>
#include <QLCDNumber>
#include <QLayout>

#include <iostream>
using namespace std;

BasicKeyboardBox::BasicKeyboardBox(QWidget *parent):
    QWidget(parent),
    init_mode(false)
{
    //     signal_mapper = new QSignalMapper(this,"mapper");
    setupUi(this);
    signalMapper = new QSignalMapper(this);
    signalMapper->setObjectName(QString("mapper"));
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    initMapping();
}

BasicKeyboardBox::~BasicKeyboardBox(){}

void BasicKeyboardBox::clear(){
    lcd->display((int)0);
}

void BasicKeyboardBox::hideNumber(bool state){
    if (!state) number_frame->show();
    else number_frame->hide();
}

void BasicKeyboardBox::initMapping(){
    //assign mapping

    button_0->setFocusProxy(this);
    button_1->setFocusProxy(this);
    button_2->setFocusProxy(this);
    button_3->setFocusProxy(this);
    button_4->setFocusProxy(this);
    button_5->setFocusProxy(this);
    button_6->setFocusProxy(this);
    button_7->setFocusProxy(this);
    button_8->setFocusProxy(this);
    button_9->setFocusProxy(this);
    button_ce->setFocusProxy(this);
    button_dot->setFocusProxy(this);

    signalMapper->setMapping(button_0,NUMBER_0);
    signalMapper->setMapping(button_1,NUMBER_1);
    signalMapper->setMapping(button_2,NUMBER_2);
    signalMapper->setMapping(button_3,NUMBER_3);
    signalMapper->setMapping(button_4,NUMBER_4);
    signalMapper->setMapping(button_5,NUMBER_5);
    signalMapper->setMapping(button_6,NUMBER_6);
    signalMapper->setMapping(button_7,NUMBER_7);
    signalMapper->setMapping(button_8,NUMBER_8);
    signalMapper->setMapping(button_9,NUMBER_9);
    signalMapper->setMapping(button_ce,NUMBER_CE);
    signalMapper->setMapping(button_dot,NUMBER_DOT);

    //connect buttons with the signal_mapper
    connect(button_0,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_1,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_2,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_3,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_4,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_5,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_6,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_7,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_8,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_9,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_ce,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_ce,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(button_dot,SIGNAL(released()),signalMapper,SLOT(map()));
}


void BasicKeyboardBox::enterEvent(QEvent*){
    setFocus();
}

void BasicKeyboardBox::leaveEvent(QEvent*){
    clearFocus();
}

bool BasicKeyboardBox::initMode(){
    return init_mode;
}

void BasicKeyboardBox::setInitMode(bool on){
    init_mode = on;
}
