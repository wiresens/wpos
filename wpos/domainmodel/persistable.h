#ifndef PERSISTABLE_H
#define PERSISTABLE_H

#include <odb/core.hxx>
#include <boost/date_time.hpp>

#include <string>
#include <chrono>
#include <exception>

using Database = std::unique_ptr<odb::database>;

namespace ws{
namespace model{

namespace time  = std::chrono;
namespace greg  = boost::gregorian;
namespace pt    = boost::posix_time;

using std::string;

//template<typename T>
class Persistable{
    friend class odb::access;

public :
//    using object_type = T;
    Persistable& operator=(const Persistable&) = delete;
    Persistable(const Persistable&) = delete;

    Persistable& operator=(Persistable&&) = default;
    Persistable(Persistable&&) = default;

    virtual const string type() const { return string("Persistable") ;}
    virtual void persist(){}

protected:
    static Database db;

    Persistable() = default;

private:
//    time::nanoseconds create_date_;
//    time::nanoseconds write_date_;
    pt::ptime create_date_;
    pt::ptime write_date_;
    uint32_t create_uid_;
    uint32_t write_uid_;
};

//using Persist = Persistable<std::string>;

struct EmptyNameException : public std::exception{
    virtual const char*
    what() const noexcept { return "Fatal : Empty Name Supplied"; }
};

struct BadGenderException : public std::exception{
    virtual const char*
    what() const noexcept { return "Fatal : Invalid Gender Supplied"; }
};

struct DuplicateAuthTokenException : public std::exception{
    virtual const char*
    what() const noexcept { return "Error : Duplicate Authentication Token Supplied"; }
};

}
}

#endif // PERSISTABLE_H
