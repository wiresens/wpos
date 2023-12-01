// file      :	order.cpp
// birth     :  11/23/2023
// copiright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "order.h"
#include "order-odb.hxx"
#include "orderline-odb.hxx"

//template<typename T, typename... Args>
//std::shared_ptr<T>
//make_shared(Args&&... args);

template<typename T, typename... Args>
std::shared_ptr<T>
make_shared(Args&&... args)
{
    using type =  typename std::remove_cv<T>::type;
    return std::allocate_shared<T>(std::allocator<type>(),
        std::forward<Args>(args)...);
}

//template<typename... Args>
//std::shared_ptr<OrderLine> make_shared(Args&&... args){
//    using type =  typename std::remove_cv<OrderLine>::type;
//    return std::allocate_shared<OrderLine>(std::allocator<type>(),
//        std::forward<Args>(args)...);
//}

namespace wpos{
namespace model{

void Order::addItem(ConstProductPtr product, uint32_t quantity){

    bool found{false};
    for( auto& item : items_){
        if( item->product().id == product->id ){
            item->setQuantity( item->quantity() + quantity);
            found = true;
            break;
        }
    }

    if(!found){
        OrderLinePtr orderLine = std::make_shared<OrderLine>(shared_from_this(), product, quantity);
//        OrderLinePtr orderLine( new OrderLine{shared_from_this(), product, quantity});
        items_.emplace_back(orderLine);
    }

}

void Order::removeItem(ConstProductPtr product){
    for( auto it = items_.begin(); it != items_.end(); ++it)
    if( (*it)->product().id == product->id ){
        items_.erase(it);
        break;
    }
}

void Order::removeItems(vector<ConstProductPtr> products){}

double Order::total() const{
    double tmp{};
    for(auto& item : items_)
        tmp += item->total();
    return tmp;
}

double Order::totalRebate() const{
    double tmp{};
    for(auto& item : items_)
        tmp += item->totalRebate();
    return tmp;
}

double Order::totalTax() const{
    double tmp{};
    for(auto& item : items_)
        tmp += item->totalTax();
    return tmp;
}

void Order::persist(){
    db->persist(*this);
    for(auto& item : items_)
        db->persist(*item);
}

}}
