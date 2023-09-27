/***************************************************************************
                          npopupkeyboard.cpp  -  description
                             -------------------
    begin                : lun jun 7 2004
    copyright            : (C) 2004 by Carlos Manzanedo
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

#include "npopupkeyboard.h"
#include <QLayout>
#include <QString>
#include "nkeyboard.h"
#include <optionnode.h>

NPopUpKeyboard::NPopUpKeyboard(
        QWidget *parent,
        const QString& name) :
    QMenu(parent)
{
    setObjectName(name);
    keyboard = new NKeyboard(this, "Keyboard");
    keyboard->setSendEvents(false);
    keyboard->setExitWithEnter(true);

    connect(keyboard, SIGNAL(textChanged(const QString &)), this, SIGNAL(textChanged(const QString &)));
    connect(keyboard, SIGNAL(enterPressed(const QString &)), this, SLOT(enterPressedSlot(const QString &)));
    connect(keyboard, SIGNAL(loungeSignal(bool)), this, SIGNAL(loungeSignal(bool)));
    connect(keyboard, SIGNAL(exitClicked()), this, SIGNAL(exitClicked()));

    addMenu(keyboard);
}

NPopUpKeyboard::NPopUpKeyboard(
        const QString& text,
        QWidget *parent,
        const QString& name):
    NPopUpKeyboard{parent, name}
{
    keyboard->setText(text);
}

NPopUpKeyboard::~NPopUpKeyboard(){}

void NPopUpKeyboard::enterPressedSlot(const QString &text){
    emit textChanged(text);
    emit exitClicked();
}

void NPopUpKeyboard::showEvent(QShowEvent *event){
    keyboard->clearSlot();
    QMenu::showEvent(event);
}

void NPopUpKeyboard::hideEvent(QHideEvent *event){
    close();
    QMenu::hideEvent(event);
}

void NPopUpKeyboard::setText(const QString& text){
    keyboard->setText(text);
}

void NPopUpKeyboard::loungeButtonClicked(){
    keyboard->loungeButtonClicked();
}

void NPopUpKeyboard::kbButtonClicked(){
    keyboard->kbButtonClicked();
}

void NPopUpKeyboard::setUsedList(QStringList list){
    keyboard->setUsedTables(list);
}

bool NPopUpKeyboard::isAtTable(){
    return keyboard->isAtTable();
}

QString NPopUpKeyboard::getLounge(){
    return keyboard->getLounge();
}

int NPopUpKeyboard::getTable(){
    return keyboard->getTable();
}

HList<OptionNode>* NPopUpKeyboard::getOptionsFromLounge(const QString& lounge){
    return keyboard->getOptionsFromLounge(lounge);
}
