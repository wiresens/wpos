#include "persistable.h"
#include <odb/core.hxx>
#include <odb/database.hxx>
#include <database.h>

namespace wpos{
namespace model{
    DatabasePtr Persistable::db{nullptr};
    Persistable::Persistable(){
        db = &wpos::database::instance();
    }

    const string Persistable::type() const {
        return string("Persistable") ;
    }

    // void Persistable::persist(){ db->persist(this) ; }
    void Persistable::persist(){}
}
}
