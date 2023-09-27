#ifndef PRODUCT_H
#define PRODUCT_H

#include "persistable.h"

namespace ws{
namespace model{

class Product:  public Persistable{
    friend class odb::access;

protected:
    Product() = default;

    ulong id_{0};
    string code_;
    string name_;
    string imageFile_;
    bool active_{false};

public:
    Product(const string& code, const string& name);
    ulong getId() const;

    void setCode(const string& code);
    const string& getCode() const;

    void setName(const string& name);
    const string& getName() const;

    void setImageFile(const string& imageFile);
    const string& getImageFile() const;

    double getPrice() const;

};

struct ProductView{
    std::shared_ptr<Product> products;
};

}
}


#endif // PRODUCT_H
