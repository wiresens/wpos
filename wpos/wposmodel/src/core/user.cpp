#include "user.h"
#include "user-odb.hxx"

namespace wpos{
namespace model{

UserPtr SessionManager::authenticate(const string& login, const string& pwd){
    UserPtr user_ptr = logIn(login, pwd);
    if (user_ptr){
        if( !user_ptr->isConnected() ) user_ptr->updatelastLogin();
        startSession(*user_ptr);
    }
    else  notifyAuthFailiure();
    return user_ptr;
}

UserPtr SessionManager::logIn(const string &login, const string &pwd){
    return User::exist(login, pwd);
}

void SessionManager::endSession(const User& user){ user.disconnect(); }

UserPtr User::exist(const string& login, const string& pwd){
    using query = odb::query<User>;
    return  db->query_one<User>( query::login == login  && query::password == pwd);
}

User User::newUser(PersonPtr employee, const string& login, const string& pwd){
    auto user = User(employee, login, pwd);
    user.persist();
    return user;
}

void User::persist(){
    if( ! employee_->id ) db->persist( employee_);
    db->persist( *this);
}

}
}

