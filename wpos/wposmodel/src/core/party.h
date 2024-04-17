#ifndef PARTY_H
#define PARTY_H

#include "persistable.h"
#include "exceptions.h"

namespace wpos {
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
    const string& code() const { return code_;}

    void setName(string name){ name_ = name ;}
    virtual string name() const { return name_ ;}

    void setPicture(string pictureFile){ pictureFile_ = pictureFile ;}
    const string& picture() const { return pictureFile_;}

    void activate(){ active_ = true ; }
    void deActivate(){ active_ = false ; }

    void setAddress(AddressPtr address){ address_ = address;}
    const Address& address() const { return *address_; }

protected:
    Party() = default;
    Party(AddressPtr address, string code, string name, string pictureFile = "")
        :address_{address},code_{code}, name_{name}, pictureFile_{pictureFile}
    {
        if( name.empty() || code.empty() ) throw EmptyValueException{};
    }

protected:
    AddressPtr address_;
    string code_;
    string name_;
    string pictureFile_;    
    bool active_{false};   
};

class  Person : public Party{
    friend class odb::access;

public:
    enum Gender : char{
        Female = 'F',
        Male   = 'M'
    };

    Person(AddressPtr address, Gender gender, string code, string name, string pictureFile="")
        :Party{address, code, name, pictureFile}, gender_{gender}
    {
        if ( gender_ != Male && gender_ != Female ) throw BadGenderException{};
    }

    void setFirstName(const string& name){ first_name_ = name ;}
    const string& firstName() const { return first_name_ ;}

    void setFamilyName(const string& name){ name_ = name ;}
    const string& familyName() const { return name_;}

    virtual  string name() const override { return name_ + " " + first_name_;}

    char gender() const{
        return gender_;
    }

private:
    Person() = default;

public:
    const ulong id{0};

private:

    string first_name_;
    char gender_;
};

class Address {
    friend class odb::access;

public:
    Address(string phone, string country, string city)
        :phone1_{phone}, country_{country}, city_{city}{}

    void setPrincipalPhone(const string& phone){ setPhone1(phone) ;}
    const string& principalPhone() const { return phone1()  ;}

    void setSecondaryPhone(const string& phone){ setPhone2(phone);}
    const string& secondaryPhone() const { return phone2()  ;}

    void setEmail(const string& email) { email_ = email ; }
    const string& email() const { return email_ ; }

    void setCountry(const string& country){ country_ = country ;}
    const string& country() const { return country_ ; }

    void setCity(const string& city){ city_ = city ;}
    const string& city() const { return city_  ;}

    void setStreet(const string& street){ street_ = street ;}
    const string& street() const { return street_  ;}

private:
    Address() = default;
    void setPhone1( const string& phone){ phone1_ = phone ; }
    const string& phone1() const { return phone1_  ; }

    void setPhone2( const string& phone){   phone2_ = phone ; }
    const string& phone2() const { return phone2_ ; }

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
