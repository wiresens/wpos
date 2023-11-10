#include "persistable.h"
#include "persistable-odb.hxx"
#include <odb/database.hxx>

namespace ws{
namespace model{
    DatabasePtr Persistable::db{ nullptr };

}
}
