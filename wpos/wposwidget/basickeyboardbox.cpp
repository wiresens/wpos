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
#include "ui_numkeyboard.h"
#include <QSignalMapper>
#include <QPushButton>
#include <QLCDNumber>
#include <QLayout>

#include <iostream>
using namespace std;

BasicKeyboardBox::BasicKeyboardBox(QWidget *parent)
    : QWidget{parent}
{

    ui = new Ui::NumKeyboard();
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    ui->button_0->setFocusProxy(this);
    ui->button_1->setFocusProxy(this);
    ui->button_2->setFocusProxy(this);
    ui->button_3->setFocusProxy(this);
    ui->button_4->setFocusProxy(this);
    ui->button_5->setFocusProxy(this);
    ui->button_6->setFocusProxy(this);
    ui->button_7->setFocusProxy(this);
    ui->button_8->setFocusProxy(this);
    ui->button_9->setFocusProxy(this);
    ui->button_ce->setFocusProxy(this);
    ui->button_dot->setFocusProxy(this);

    signalMapper = new QSignalMapper(this);

    signalMapper->setMapping(ui->button_0,NUMBER_0);
    signalMapper->setMapping(ui->button_1,NUMBER_1);
    signalMapper->setMapping(ui->button_2,NUMBER_2);
    signalMapper->setMapping(ui->button_3,NUMBER_3);
    signalMapper->setMapping(ui->button_4,NUMBER_4);
    signalMapper->setMapping(ui->button_5,NUMBER_5);
    signalMapper->setMapping(ui->button_6,NUMBER_6);
    signalMapper->setMapping(ui->button_7,NUMBER_7);
    signalMapper->setMapping(ui->button_8,NUMBER_8);
    signalMapper->setMapping(ui->button_9,NUMBER_9);
    signalMapper->setMapping(ui->button_ce,NUMBER_CE);
    signalMapper->setMapping(ui->button_dot,NUMBER_DOT);

    //connect buttons with the QSignalMapper
    connect(ui->button_0,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_1,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_2,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_3,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_4,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_5,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_6,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_7,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_8,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_9,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_ce,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_ce,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(ui->button_dot,SIGNAL(released()),signalMapper,SLOT(map()));
}

BasicKeyboardBox::~BasicKeyboardBox(){
    delete ui;
}

void BasicKeyboardBox::clear(){
    ui->display->display((int)0);
}

void BasicKeyboardBox::hideNumber(bool state){
    if (!state) ui->number_frame->show();
    else ui->number_frame->hide();
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
