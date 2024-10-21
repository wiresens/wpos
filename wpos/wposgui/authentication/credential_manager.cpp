// file      :  credential_manager.cpp
// birth     :  2/19/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "credential_manager.h"
#include "greeter_dialog.h"

#include "windows/basic_main_window.h"

#include <QEvent>

CredentialManager::CredentialManager(BasicMainWindow* managed)
    : QObject{managed},
    managedWindow{managed},
    greeter{ new GreeterDialog{}}
{
    managed->installEventFilter(this);   
}

bool CredentialManager::eventFilter(QObject *watched, QEvent *event){
    static bool shown {false};
    if( watched == managedWindow && event->type() == QEvent::Show){
        if( ! shown ) {
            logIn();
            shown = true;
        }
    }
    return QObject::eventFilter(watched, event);
}

void CredentialManager::logIn(){
    greeter->showFullScreen();
    auto sucessful =  greeter->authenticate();
    if( !sucessful ) exit(EXIT_SUCCESS);
}
