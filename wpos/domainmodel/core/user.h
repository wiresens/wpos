#ifndef USER_H
#define USER_H

#include "../persistable.h"
#include "../daterange.h"
#include "party.h"

namespace ws {
namespace model{

using namespace odb::core;
class User;
using UserPtr = std::shared_ptr<User>;

class SessionManager{
public:
    void authenticate(const User& user);
    void logOut(const User& user){ endSession(user);}

    virtual ~SessionManager() = default;

protected:
    virtual bool logIn(const User& user);
    virtual void startSession(const User&){}
    virtual void endSession(const User&){}
    virtual void notifyAuthtenticationError(){}
};

//class User : public Persistable<User> {
class User : public Persistable{
    friend class odb::access;
    friend void SessionManager::authenticate(const User& user);

public:
    User(User&&) = default;
    User& operator=(User&&) = default;

    static UserPtr exist(const string& login, const string& pwd);
    static User newUser(PersonPtr employee, const string& login, const string& pwd);

    User( const string& login, const string& pwd):
        login_{login},
        pwd_hash_{pwdHash(pwd)}
    {}

    User( PersonPtr employee, const string& login, const string& pwd):
        login_{login}, pwd_hash_{pwdHash(pwd)},
        pwd_timeout_{ nowLocal()}, employee_{employee}
    {
        if( exist(login_, pwd_hash_)) throw DuplicateAuthTokenException{};
    }

    virtual void persist() override;

    void setLogin(const string& login) { login_ = login ;}
    const string& login() const { return  login_ ;}

    const TimeStamp& lastLogin() const { return last_login_ ;}

    void setPassWord(const string& pwd){  pwd_hash_ = pwdHash(pwd);}
    const string& passWord() const { return  pwd_hash_ ;}

protected:
    void updatelastLogin() const{
        conected_ = true;
        last_login_ = nowLocal();
    }

    //We will use a sha512 algorithm to has password
    string pwdHash( const string& pwd){ return pwd;}

private:
    User() = default;

private:
    ulong id_{0};
    string login_;
    string pwd_hash_;
    mutable TimeStamp last_login_;
    TimeStamp pwd_timeout_;
    bool enabeled_{true};
    mutable bool conected_{false};
    PersonPtr employee_;
};

}
}

#endif // USER_H
