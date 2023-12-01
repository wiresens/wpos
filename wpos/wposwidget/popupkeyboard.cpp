/***************************************************************************
                          popupkeyboard.cpp  -  description
                             -------------------
    begin                : vie feb 13 2004
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

#include "popupkeyboard.h"
#include <QLayout>
#include <QString>

#include "spkeyboard.h"

PopUpKeyboard::PopUpKeyboard(QWidget *parent, const QString& name) :
    QMenu(parent){
    setObjectName(name);
    kb = new SpKeyboard(this);
    addAction( QString(""), kb , (const char*)nullptr);

    kb->sendEvents(false);
    kb->exitWithEnter(true);
    connect(kb, &SpKeyboard::textChanged, this, &PopUpKeyboard::textChanged);
    connect(kb, &SpKeyboard::enterPressed, this, &PopUpKeyboard::enterPressedSlot);
    connect(kb, &SpKeyboard::exitClicked, this, &PopUpKeyboard::exitClicked);
}

PopUpKeyboard::~PopUpKeyboard(){
//        QObject::disconnect(kb, SIGNAL(textChanged(const QString &)),this,SIGNAL(textChanged(const QString &)));
//        QObject::disconnect(kb,SIGNAL(enterPressed(const QString &)),this, SLOT(enterPressedSlot(const QString &)));
//        QObject::disconnect(kb,SIGNAL(exitClicked()),this,SIGNAL(exitClicked()));
//        delete kb;
}

void PopUpKeyboard::enterPressedSlot(const QString &text_string){
     emit textChanged( text_string );
     emit exitClicked();
}

void PopUpKeyboard::showEvent(QShowEvent *e){
     kb->clearSlot();
     QMenu::showEvent(e);
}

void PopUpKeyboard::hideEvent(QHideEvent *e){
     close();
     QMenu::hideEvent(e);
}
