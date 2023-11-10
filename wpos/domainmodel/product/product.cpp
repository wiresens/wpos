#include "product.h"
#include "pricelog.h"

#include "product-odb.hxx"

namespace ws{
namespace model{

Product::Product(
    ProductTemplatePtr tpl, const string& name,
    double price, const string& code)
    : tpl_{tpl}, name_{name}, price_{price}, code_{code}
{
    if(name.empty()) throw EmptyNameException{};
    if( ! ( price_ > 0.0 || price_ < 0.0) ) throw ZeroPriceException{};
    db->persist(shared_from_this());
    savePrice();
}

void Product::setPrice(double newPrice, PriceType){
    if( price_ != newPrice && ( newPrice > 0.0 || newPrice < 0.0) ){
//        logPrice();
        price_ = newPrice;
    }
}

void Product::logPrice(){
    PriceLog::logPrice(shared_from_this());
}

double Product::priceAsOf(const TimeStamp& stamp, PriceType) const
{
    auto current_pricelog = PriceLog::priceAsOf(shared_from_this(), stamp);
    if(current_pricelog) return current_pricelog->price_;
    throw NotFoundException{};
}

}
}
