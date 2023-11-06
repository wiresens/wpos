#ifndef PARTY_H
#define PARTY_H

#include "../persistable.h"

namespace ws {
namespace model{

class Address;
class Person;
class Party;

using AddressPtr = std::shared_ptr<Address>;
using PersonPtr  = std::shared_ptr<Person>;
using PartyPtr   = std::shared_ptr<Party>;

using ConstAddressPtr = std::shared_ptr<const Address>;
using ConstPersonPtr  = std::shared_ptr<const Person>;
using ConstPartyPtr   = std::shared_ptr<const Party>;

//class Party : std::enable_shared_from_this<Party>,
//              public Persistable<Party>{
class Party : public Persistable{
    friend class odb::access;

public:
    virtual ~Party() = default;

    void setCode(string code){ code_ = code ;}
    const string& getCode() const { return code_;}

    void setName(string name){ name_ = name ;}
    virtual string getName() const { return name_ ;}

    void setPicture(string pictureFile){ pictureFile_ = pictureFile ;}
    const string& getPicture() const { return pictureFile_;}

    void activate(){ active_ = true ; }
    void deActivate(){ active_ = false ; }

    void setAddress(Address* address){ address_.reset(address);}
    const Address& getAddress() const { return *address_.get(); }

protected:
    Party() = default;
    Party(string code, string name, string pictureFile = "")
        :code_{code}, name_{name}, pictureFile_{pictureFile}
    {
         if( name.empty() || code.empty() ) throw EmptyNameException{};
    }

protected:
    string code_;
    string name_;
    string pictureFile_;
    bool active_{false};
    AddressPtr address_;
};

class  Person : public Party{
    friend class odb::access;

public:
    Person(char gender, string code, string name, string pictureFile="")
        :Party{code, name, pictureFile}, gender_{gender}
    {
        if ( gender_ != 'm' && gender_ != 'f' ) throw BadGenderException{};
    }

    void setFirstName(string name){ first_name_ = name ;}
    const string& getFirstName() const { return first_name_ ;}

    void setFamilyName(string name){ name_ = name ;}
    const string& getFamilyName() const { return name_;}

    virtual  string getName() const override { return name_ + " " + first_name_;}

private:
    Person() = default;

private:
    ulong id_{0};
    string first_name_;
    char gender_;
};

class Address {
    friend class odb::access;

public:
    Address(string phone, string country, string city)
        :phone1_{phone}, country_{country}, city_{city}{}

    void setPrincipalPhone(const string& phone){ phone1_ = phone ;}
    const string& principalPhone() const { return phone1_  ;}

    void setSecondaryPhone(const string& phone){ phone2_ = phone ;}
    const string& secondaryPhone() const { return phone2_  ;}

    void setEmail(const string& email) { email_ = email ; }
    const string& getEmail() const { return email_ ; }

    void setCountry(const string& country){ country_ = country ;}
    const string& getCountry() const { return country_ ; }

    void setCity(const string& city){ city_ = city ;}
    const string& getCity() const { return city_  ;}

    void setStreet(const string& street){ street_ = street ;}
    const string& getStreet() const { return street_  ;}

private:
    Address() = default;
    void setPhone1( const string& phone){  setPrincipalPhone(phone) ; }
    const string& getPhone1() const { return principalPhone()  ; }

    void setPhone2( const string& phone){   setSecondaryPhone(phone) ; }
    const string& getPhone2() const { return secondaryPhone() ; }

private:
    ulong id_{0};
    string phone1_;
    string phone2_;
    string email_;
    string country_;
    string city_;
    string street_;
};

}
}

#endif // PARTY_H
