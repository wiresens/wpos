// file      :	orderline.h
// birth     :  11/23/2023
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef ORDERLINE_H
#define ORDERLINE_H

#include "product.h"
#include <odb/lazy-ptr.hxx>
#include <memory>

// template<typename T, typename... Args>
// std::shared_ptr<T>
// std::make_shared(Args&&... args);

namespace wpos{
namespace model{

class Order;
class OrderLine;

using OrderLWPtr        = odb::lazy_weak_ptr<Order>;
using OrderPtr          = std::shared_ptr<Order>;
using ConstOrderLinePtr = std::shared_ptr<const OrderLine>;

class OrderLine : public Persistable{
    friend class odb::access;
//    friend class Order;

    //This doesn't work, still have to figure how
//    template<typename T, typename... Args>
//    friend std::shared_ptr<T>
//    std::make_shared(Args&&... args);


private:
    OrderLine() = default;

public:
    OrderLine(
        const OrderPtr& order,
        const ConstProductPtr& product,
        uint32_t quantity = 1)
        :order_{order},
        product_{product},
        quantity_{quantity},
        unit_price_{product->price()}
    {}

public:

    double total() const { return  unit_price_*quantity_ ;}
    double totalRebate() const{ return  ( 1.0 - rebate_/100.0)*total() ;}
    double totalTax() const{ return  ( 1.0 + vat_/100.0)*totalRebate() ;}

    uint32_t quantity() const{ return quantity_;}
    void setQuantity(uint32_t quantity){ quantity_ = quantity;}

    double unitPrice() const{ return unit_price_;}

    float rebate() const{ return rebate_;}
    void setRebate(float rebate) { rebate_ = rebate; }

    const Product& product() const{ return *product_;}
    Order& order() const;

public:
    const ulong id{0};

private:
    OrderLWPtr order_;
    ConstProductPtr product_;
    uint32_t  quantity_{};
    double unit_price_{};
    float  rebate_{0.0};
    float vat_{0.0};
};

}}

#endif // ORDERLINE_H
