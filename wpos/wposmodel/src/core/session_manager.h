// file      :  session_manager.h
// birth     :  5/1/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <memory>

namespace wpos {
namespace model{
    class User;
    using UserPtr = std::shared_ptr<User>;
    using ConstUserPtr = std::shared_ptr<const User>;
}

using namespace model;
using std::string;

//Will be incorporated in the application server deamon
//to manage user sessions via the network
class SessionManager{
public:
    UserPtr authenticate(const string &login, const string &pwd);
    void logOut(const User& user){ endSession(user);}

    virtual ~SessionManager() = default;

protected:
    virtual UserPtr logIn(const string& login, const string& pwd);
    virtual void startSession(const User&){}
    virtual void endSession(const User& user);
    virtual void notifyAuthFailiure(){}
};
}
#endif // SESSION_MANAGER_H
