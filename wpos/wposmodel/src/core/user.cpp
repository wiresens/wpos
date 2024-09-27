#include "user.h"
#include "user-odb.hxx"
#include <odb/database.hxx>

namespace wpos{
namespace model{

UserPtr User::find(const string& login, const string& pwd){
    using query = odb::query<User>;
    return  db->query_one<User>( query::login == login  && query::password == pwd);
}

UserPtr User::connect(const string& login, const string& pwd){
    UserPtr user_ptr =  find(login, pwd);
    if( user_ptr && !user_ptr->isConnected()){
        user_ptr->updatelastLogin();
        db->update(user_ptr);
    }
    return user_ptr;
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

