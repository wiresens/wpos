#include "user.h"
#include "user-odb.hxx"

namespace wpos{
namespace model{

void SessionManager::authenticate(const User& user){
    if ( logIn(user)){
        user.updatelastLogin();
        startSession(user);
    }
    else notifyAuthtenticationError();
}

bool SessionManager::logIn(const User& user){
    return ( User::exist(user.login(), user.passWord()).get() != nullptr ) ;
}

UserPtr User::exist(const string& login, const string& pwd){
    using query = odb::query<User>;
    return  db->query_one<User>( query::login == login  && query::pwd_hash == pwd);
}

User User::newUser(PersonPtr employee, const string& login, const string& pwd){
    auto user = User(employee, login, pwd);
    user.persist();
    return user;
}

void User::persist(){
    db->persist( employee_);
    db->persist( *this);
}

}
}

