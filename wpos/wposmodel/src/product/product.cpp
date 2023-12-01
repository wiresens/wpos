#include "product.h"
#include "pricelog.h"
#include "database.h"

#include "product-odb.hxx"

namespace wpos{
namespace model{

Product::Product(ProductTemplatePtr tpl,
    const string& name,
    double price):
    tpl_{tpl},
    name_{name},
    price_{price}
{
    if(name.empty()) throw EmptyValueException{};
    if( ! ( price_ > 0.0 || price_ < 0.0) ) throw ZeroPriceException{};
    savePrice();
}

void Product::setPrice(double newPrice, PriceType){
    if( price_ != newPrice && ( newPrice > 0.0 || newPrice < 0.0) ){       
        price_ = newPrice;
        logPrice();
    }
}

void Product::logPrice(){
    PriceLog::logPrice(shared_from_this());
}

void Product::savePrice(){
    PriceLog::savePrice(*this);
}

double Product::priceAsOf(const TimeStamp& stamp, PriceType) const
{
    auto current_pricelog = PriceLog::priceAsOf(shared_from_this(), stamp);
    if(current_pricelog) return current_pricelog->price_;
    throw NotFoundException{};
}

}
}
