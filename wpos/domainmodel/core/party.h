#ifndef PARTY_H
#define PARTY_H

#include <persistable.h>

namespace ws {
namespace model{

class Party : public Persistable, std::enable_shared_from_this<Party>{
    friend class odb::access;

public:

    virtual ~Party() = default;

protected:
    Party() = default;

private:
    ulong id_{0};
    string code_;
    string name_;
    string imageFile_;
    bool active_{false};

    std::shared_ptr<Party> replaced_by_{nullptr};
};

}
}

#endif // PARTY_H
