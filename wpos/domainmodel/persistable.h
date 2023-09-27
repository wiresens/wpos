#ifndef PERSISTABLE_H
#define PERSISTABLE_H

#include <odb/vector.hxx>
//#include <odb/qt/lazy-ptr.hxx>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <boost/date_time.hpp>

#include <string>
#include <chrono>

namespace ws{
namespace model{

namespace time  = std::chrono;
namespace greg  = boost::gregorian;
namespace pt    = boost::posix_time;

using namespace odb::core;
using std::string;

class Persistable{
    friend class odb::access;

public :
    Persistable& operator=(const Persistable&) = delete;
    Persistable(const Persistable&) = delete;

    virtual const string& type() const = 0;

protected:
    Persistable();

private:
    time::nanoseconds create_date;
    time::nanoseconds write_date;
    uint32_t create_uid;
    uint32_t write_uid;
};

}
}

#endif // PERSISTABLE_H
