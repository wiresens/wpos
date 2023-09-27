#ifndef USER_H
#define USER_H

#include "persistable.h"

namespace ws {
namespace model{

using namespace odb::core;

class User : public Persistable {
public:

protected:
    User() = default;
};


}
}



#endif // USER_H
