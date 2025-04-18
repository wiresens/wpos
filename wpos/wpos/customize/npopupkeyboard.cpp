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
#include <wposcore/productextrainfo.h>

NPopUpKeyboard::NPopUpKeyboard(
    QWidget *parent) :
    QMenu(parent)
{
    m_nkeyboard = new NKeyboard(this);
    m_action = addMenu(m_nkeyboard);
    m_nkeyboard->setHasSendEvents(false);
    m_nkeyboard->setExitWithEnter(true);

    connect(m_nkeyboard, &NKeyboard::textChanged,  this, &NPopUpKeyboard::textChanged);
    connect(m_nkeyboard, &NKeyboard::enterPressed, this, &NPopUpKeyboard::enterPressedSlot);
    connect(m_nkeyboard, &NKeyboard::loungeRequested, this, &NPopUpKeyboard::loungeRequested);
    connect(m_nkeyboard, &NKeyboard::exitClicked,  this, &NPopUpKeyboard::exitClicked);
}

NPopUpKeyboard::NPopUpKeyboard(
    const QString& text,
    QWidget *parent):
    NPopUpKeyboard{parent}
{
    m_nkeyboard->setText(text);
}

void NPopUpKeyboard::enterPressedSlot(const QString &text){
    emit textChanged(text);
    emit exitClicked();
}

void NPopUpKeyboard::showMenu(const QPoint& pos){
    m_nkeyboard->popup( pos, m_action );
}

void NPopUpKeyboard::showEvent(QShowEvent *event){
    m_nkeyboard->clearSlot();
    QMenu::showEvent(event);
}

void NPopUpKeyboard::hideEvent(QHideEvent *event){
    close();
    QMenu::hideEvent(event);
}

void NPopUpKeyboard::setText(const QString& text){
    m_nkeyboard->setText(text);
}

void NPopUpKeyboard::showLounge(){
    m_nkeyboard->showLounge();
}

void NPopUpKeyboard::showKeyBoard(){
    m_nkeyboard->showKeyBoard();
}

void NPopUpKeyboard::setUsedTables(const QStringList& tbl_codes)
{
    m_nkeyboard->setUsedTables(tbl_codes);
}

bool NPopUpKeyboard::isAtTable(){
    return m_nkeyboard->isAtTable();
}

QString NPopUpKeyboard::getLounge(){
    return m_nkeyboard->getLounge();
}

int NPopUpKeyboard::getTable(){
    return m_nkeyboard->getTable();
}

HList<ProductExtraInfo>*
NPopUpKeyboard::getOptionsFromLounge(
    const QString& lounge)
{
    return m_nkeyboard->getOptionsFromLounge(lounge);
}
