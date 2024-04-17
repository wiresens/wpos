#ifndef USER_H
#define USER_H

#include "../persistable.h"
#include "../daterange.h"
#include "exceptions.h"
#include "party.h"

namespace wpos {
namespace model{

using namespace odb::core;
class User;
using UserPtr = std::shared_ptr<User>;
using ConstUserPtr = std::shared_ptr<const User>;

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

//class User : public Persistable<User> {
class User : public Persistable{
    friend class odb::access;
    friend UserPtr SessionManager::authenticate(const string&, const string&);

public:
    User(User&&) = default;
    User& operator=(User&&) = default;

    static UserPtr find(const string& login, const string& pwd);
    static User newUser(PersonPtr employee, const string& login, const string& pwd);

    User( PersonPtr employee, const string& login, const string& pwd):
        login_{login}, password_{pwdHash(pwd)},
        pwd_timeout_{ nowLocal()}, employee_{employee}
    {
        if(login.empty() || pwd.empty()) throw EmptyValueException{};
        if( find(login_, password_)) throw DuplicateAuthTokenException{};
    }

    virtual void persist() override;

    void setLogin(const string& login) { login_ = login ;}
    const string& login() const { return  login_ ;}

    const TimeStamp& lastLogin() const { return last_login_ ;}

    void setPassword(const string& pwd){  password_ = pwdHash(pwd);}
    const string& password() const { return  password_ ;}

    Person& employee() const { return  *employee_ ;}
    PersonPtr employeePtr() const { return  employee_ ;}

    void enable() { enabeled_ = true; }

    void disable() { enabeled_ = false;}

    bool isConnected() const{
        return connected_;
    }

    void disconnect() const{
        connected_ = false;
    }

protected:
    void updatelastLogin() const{
        connected_  = true;
        last_login_ = nowLocal();
    }

    //We will use a sha512 algorithm to has password
    string pwdHash( const string& pwd){ return pwd;}

private:
    User() = default;

public:
    const ulong id{0};

private:
    string login_;
    string password_;
    mutable TimeStamp last_login_;
    TimeStamp pwd_timeout_;
    bool enabeled_{true};
    mutable bool connected_{false};
    PersonPtr employee_;
};

}
}

#endif // USER_H
