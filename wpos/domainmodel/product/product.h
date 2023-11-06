#ifndef PRODUCT_H
#define PRODUCT_H

#include "../persistable.h"
#include "../daterange.h"
#include "producttemplate.h"

#include <limits>

namespace ws{
namespace model{

class Product;
using ProductPtr = std::shared_ptr<Product>;
using ConstProductPtr = std::shared_ptr<const Product>;

//class Product:  public Persistable<Product>{
class Product:  public Persistable, public std::enable_shared_from_this<Product>{
    friend class odb::access;

public:
    enum PriceType{
        CostPrice = 0,
        ListPrice = 1
    };

    using Template = std::shared_ptr<ProductTemplate>;
    Product(const Product&) = delete;
    Product& operator=(const Product&) = delete;
    virtual ~Product() = default;

    Product(Template tpl, const string& code, const string& name)
        : tpl_{tpl}, code_{code}, name_{name}
    {
        if(name.empty()) throw EmptyNameException{};
    }

    ulong getId() const{ return id_ ;}
    void setCode(const string& code){ code_ = code ;}
    const string& getCode() const { return code_ ; }

    void setName(const string& name){ if( !name.empty()) name_ = name ;}
    const string& getName() const { return name_ ; }

    void setImageFile(const string& imageFile){  imageFile_ = imageFile ;}
    const string& getImageFile() const { return imageFile_ ; }

    void enable(){ setActive(true); }
    void disable(){ setActive(false); }

    bool isEnabled() const { return getActive() ; }
    virtual double price(PriceType = ListPrice) const;
    virtual void setPrice(double price, PriceType = ListPrice );

protected:
    Product() = default;
    void setId(ulong id){  if(id) id_ = id ;}
    void setActive(bool active){ active_ = active ; }
    bool getActive() const { return active_ ;}

private:
    ulong id_{0};
    Template tpl_;

    string code_;
    string name_;
    string imageFile_;
    mutable double price_{ std::numeric_limits<double>::max()};
    bool active_{true};

};

struct ProductView{
    std::shared_ptr<Product> products;
};

}
}


#endif // PRODUCT_H
