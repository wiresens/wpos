#include "persistable.h"
#include "persistable-odb.hxx"
#include <odb/database.hxx>
#include <database.h>

namespace wpos{
namespace model{
    DatabasePtr Persistable::db{nullptr};
    Persistable::Persistable(){
        db = &database::instance();
    }
}
}
