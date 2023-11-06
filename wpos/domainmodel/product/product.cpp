#include "product.h"
#include "pricelog.h"

#include "product-odb.hxx"
#include "producttemplate-odb.hxx"

namespace ws{
namespace model{

void Product::setPrice(double price, PriceType){
    PriceLog::newPrice(shared_from_this(), price);
}

double Product::price(PriceType) const {
    if( price_ >= std::numeric_limits<double>::max() ){
        auto current_pricelog = PriceLog::checkPrice(shared_from_this());
        if(current_pricelog) price_ = current_pricelog->price_;
    }
    return price_;
}

}
}
