#ifndef PERSISTABLE_H
#define PERSISTABLE_H

#include "daterange.h"
#include <boost/date_time.hpp>

#include <string>
#include <chrono>
#include <sys/types.h>

namespace odb {
class database;
}

namespace wpos{
namespace model{

namespace time  = std::chrono;
namespace greg  = boost::gregorian;
namespace pt    = boost::posix_time;

using std::string;
using DatabasePtr = odb::database*;

//template<typename T>
class Persistable{
    friend class odb::access;

public :
//    using object_type = T;
    Persistable& operator=(const Persistable&) = delete;
    Persistable(const Persistable&) = delete;

    Persistable& operator=(Persistable&&) = default;
    Persistable(Persistable&&) = default;

    virtual const string type() const ;
    virtual void persist();

protected:
    static DatabasePtr db;
    Persistable();

private:
//    time::nanoseconds create_date_;
//    time::nanoseconds write_date_;
    pt::ptime birth_date_{nowLocal()};
    pt::ptime change_date_{birth_date_};
//    uint32_t create_uid_;
//    uint32_t write_uid_;
};

}
}

#endif // PERSISTABLE_H
