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

static const  int KEY_0   {0};
static const  int KEY_1   {1};
static const  int KEY_2   {2};
static const  int KEY_3   {3};
static const  int KEY_4   {4};
static const  int KEY_5   {5};
static const  int KEY_6   {6};
static const  int KEY_7   {7};
static const  int KEY_8   {8};
static const  int KEY_9   {9};
static const  int KEY_CE  {-1};
static const  int KEY_DOT {-2};

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

    signalMapper->setMapping(ui->button_0, KEY_0);
    signalMapper->setMapping(ui->button_1, KEY_1);
    signalMapper->setMapping(ui->button_2, KEY_2);
    signalMapper->setMapping(ui->button_3, KEY_3);
    signalMapper->setMapping(ui->button_4, KEY_4);
    signalMapper->setMapping(ui->button_5, KEY_5);
    signalMapper->setMapping(ui->button_6, KEY_6);
    signalMapper->setMapping(ui->button_7, KEY_7);
    signalMapper->setMapping(ui->button_8, KEY_8);
    signalMapper->setMapping(ui->button_9, KEY_9);
    signalMapper->setMapping(ui->button_ce, KEY_CE);
    signalMapper->setMapping(ui->button_dot, KEY_DOT);

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
    connect(signalMapper, &QSignalMapper::mappedInt,this, &NumPad::reeadKey);
}

void NumPad::clear(){
    m_is_dirty = false;
    ui->lcd->display(0);
}

void NumPad::showDisplay(){
    ui->displayFrame->setVisible(true);
}

void NumPad::hideDisplay(){
    ui->displayFrame->setVisible(false);
}

void NumPad::keyPressEvent(QKeyEvent *event){
    auto text = event->text();
    if      (text == "0")  reeadKey( KEY_0 );
    else if (text == "1")  reeadKey( KEY_1 );
    else if (text == "2")  reeadKey( KEY_2 );
    else if (text == "3")  reeadKey( KEY_3 );
    else if (text == "4")  reeadKey( KEY_4 );
    else if (text == "5")  reeadKey( KEY_5 );
    else if (text == "6")  reeadKey( KEY_6 );
    else if (text == "7")  reeadKey( KEY_7 );
    else if (text == "8")  reeadKey( KEY_8 );
    else if (text == "9")  reeadKey( KEY_9 );
    else if (event->key() == 4103 ) reeadKey( KEY_CE );
    else QWidget::keyPressEvent(event);
}

void  NumPad::display(double value){
    ui->lcd->display(value);
}

void NumPad::setSegmentStyle(
    QLCDNumber::SegmentStyle style)
{
    ui->lcd->setSegmentStyle(style);
}

void NumPad::setDigitCount(unsigned count){
    ui->lcd->setDigitCount(count);
}

void NumPad::cefontIsDotfont(){
    ui->button_dot->setFont(ui->button_ce->font());
}

void NumPad::dotInit( const QString& value){
    ui->button_dot->setText(value);
}

void NumPad::dotVisible(bool visibility){
   ui->button_dot->setVisible(visibility);
}

void NumPad::removeKey(QWidget* key){
    ui->buttonLayout->removeWidget(key);
}

void NumPad::addKey(
    QWidget* key,
    int row, int column,
    int rowSpan, int columnSpan)
{
    ui->buttonLayout->addWidget(key, row, column, rowSpan, columnSpan);
}

int NumPad::ce() const{
    return KEY_CE;
}

int NumPad::dot() const{
    return KEY_DOT;
}

QWidget* NumPad::dotKey() const{
    return ui->button_dot;
}

QWidget* NumPad::ceKey() const{
    return ui->button_ce;
}
