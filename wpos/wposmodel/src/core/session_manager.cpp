// file      :  session_manager.cpp
// birth     :  5/1/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "user.h"
#include "session_manager.h"

namespace wpos {
using namespace model;

    UserPtr SessionManager::authenticate(const string& login, const string& pwd){
        UserPtr user_ptr = logIn(login, pwd);
        if (user_ptr)
            startSession(*user_ptr);
        else  notifyAuthFailiure();
        return user_ptr;
    }

    UserPtr SessionManager::logIn(const string &login, const string &pwd){
        return User::connect(login, pwd);
    }

    void SessionManager::endSession(const User& user){ user.disconnect(); }
}
