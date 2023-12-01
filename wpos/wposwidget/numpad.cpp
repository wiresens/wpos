/***************************************************************************
                          numpad.cpp  -  description
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

#include "ui_numpad.h"
#include "numpad.h"
#include <QSignalMapper>
#include <QKeyEvent>

#include <iostream>
using namespace std;

static const  int NUMBER_0   {0};
static const  int NUMBER_1   {1};
static const  int NUMBER_2   {2};
static const  int NUMBER_3   {3};
static const  int NUMBER_4   {4};
static const  int NUMBER_5   {5};
static const  int NUMBER_6   {6};
static const  int NUMBER_7   {7};
static const  int NUMBER_8   {8};
static const  int NUMBER_9   {9};
static const  int NUMBER_CE  {-1};
static const  int NUMBER_DOT {-2};

NumPad::NumPad(QWidget *parent)
    : QWidget{parent}
{
    ui = new Ui::NumPad();
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

    signalMapper->setMapping(ui->button_0, NUMBER_0);
    signalMapper->setMapping(ui->button_1, NUMBER_1);
    signalMapper->setMapping(ui->button_2, NUMBER_2);
    signalMapper->setMapping(ui->button_3, NUMBER_3);
    signalMapper->setMapping(ui->button_4, NUMBER_4);
    signalMapper->setMapping(ui->button_5, NUMBER_5);
    signalMapper->setMapping(ui->button_6, NUMBER_6);
    signalMapper->setMapping(ui->button_7, NUMBER_7);
    signalMapper->setMapping(ui->button_8, NUMBER_8);
    signalMapper->setMapping(ui->button_9, NUMBER_9);
    signalMapper->setMapping(ui->button_ce, NUMBER_CE);
    signalMapper->setMapping(ui->button_dot, NUMBER_DOT);

    //connect buttons with the QSignalMapper
    connect(ui->button_0, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->button_1, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->button_2, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->button_3, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->button_4, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->button_5, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->button_6, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->button_7, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->button_8, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->button_9, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->button_ce, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->button_dot, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));

    //prepare the map signal
    connect(signalMapper, &QSignalMapper::mappedInt,this, &NumPad::receiveDigit);
}

void NumPad::clear(){
    dirty_ = false;
    ui->lcd->display(0);
}

void NumPad::showDisplay(){
    ui->displayFrame->setVisible(true);
}

void NumPad::hideDisplay(){
    ui->displayFrame->setVisible(false);
}

void NumPad::keyPressEvent(QKeyEvent *event){
    if      (event->text() == "0")  receiveDigit(NUMBER_0);
    else if (event->text() == "1")  receiveDigit(NUMBER_1);
    else if (event->text() == "2")  receiveDigit(NUMBER_2);
    else if (event->text() == "3")  receiveDigit(NUMBER_3);
    else if (event->text() == "4")  receiveDigit(NUMBER_4);
    else if (event->text() == "5")  receiveDigit(NUMBER_5);
    else if (event->text() == "6")  receiveDigit(NUMBER_6);
    else if (event->text() == "7")  receiveDigit(NUMBER_7);
    else if (event->text() == "8")  receiveDigit(NUMBER_8);
    else if (event->text() == "9")  receiveDigit(NUMBER_9);
    else if (event->key() == 4103 ) receiveDigit(NUMBER_CE);
}

int NumPad::ce() const{
    return NUMBER_CE;
}

int NumPad::dot() const{
    return NUMBER_DOT;
}
