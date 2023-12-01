#ifndef PRODUCT_H
#define PRODUCT_H

#include "persistable.h"
#include "daterange.h"
#include "producttemplate.h"

#include <limits>

namespace wpos{
namespace model{

class Product;

using ProductPtr = std::shared_ptr<Product>;
using ConstProductPtr = std::shared_ptr<const Product>;

//class Product:  public Persistable<Product>{
class Product:
    public Persistable,
    public std::enable_shared_from_this<Product>
{
    friend class odb::access;    

public:
    enum PriceType{
        CostPrice = 0,
        ListPrice = 1
    };

    using ProductTemplatePtr = std::shared_ptr<ProductTemplate>;
    Product(const Product&) = delete;
    Product& operator=(const Product&) = delete;
    virtual ~Product() = default;

    Product(
        ProductTemplatePtr tpl,
        const string& name,
        double price = std::numeric_limits<double>::max()
    );

    virtual void setCode(const string& code){ code_ = code ;}
    virtual const string& getCode() const { return code_ ; }

    void setName(const string& name){ if( !name.empty()) name_ = name ;}
    const string& name() const { return name_ ; }
    const string& code() const { return code_ ; }

    ProductTemplate& tmplate() const { return *tpl_; }
    Uom& uom() const { return tpl_->uom(); }

    void setImageFile(const string& imageFile){  imageFile_ = imageFile ;}
    const string& getImageFile() const { return imageFile_ ; }

    void enable(){ setActive(true); }
    void disable(){ setActive(false); }
    bool isEnabled() const { return getActive() ; }

    double price(PriceType = ListPrice) const{ return price_ ; }

    double priceAsOf(const TimeStamp& stamp, PriceType = ListPrice) const;
    void setPrice(double newPrice, PriceType = ListPrice );

protected:
    virtual void logPrice();
    void savePrice();

    Product() = default;
    void setActive(bool active){ active_ = active ; }
    bool getActive() const { return active_ ;}

public:
    const ulong id{0};

private:
    ProductTemplatePtr tpl_;

    string name_;
    string code_;
    mutable double price_{std::numeric_limits<double>::max()};
    string imageFile_;
    bool active_{true};
};

struct ProductView{
    ProductPtr products;
};

}
}

#endif // PRODUCT_H
