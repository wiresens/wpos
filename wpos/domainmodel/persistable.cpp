#include "persistable.h"
#include "persistable-odb.hxx"
#include <odb/database.hxx>

namespace ws{
namespace model{
    Database Persistable::db{ nullptr };

}
}
