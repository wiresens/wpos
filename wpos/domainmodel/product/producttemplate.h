#ifndef PRODUCTTEMPLATE_H
#define PRODUCTTEMPLATE_H

#include "persistable.h"

namespace ws{
namespace model{

class ProductTemplate:  public Persistable{
    friend class odb::access;

public:

protected:
    ProductTemplate() = default;

private:
    ulong id_{0};
    string code_;
    string name_;
    string imageFile_;
    bool active_{false};
    bool consumable_{false};
};

}
}

#endif // PRODUCTTEMPLATE_H
