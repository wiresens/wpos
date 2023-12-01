// file      :	order.h
// birth     :  11/23/2023
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef ORDER_H
#define ORDER_H

#include "core/party.h"
#include "order/orderline.h"

namespace wpos{
namespace model{

    class Order;
    using OrderPtr      = std::shared_ptr<Order>;
    using ConstOrderPtr = std::shared_ptr<const Order>;

    using OrderLinePtr  = std::shared_ptr<OrderLine>;
    using ConstOrderLinePtr  = std::shared_ptr<const OrderLine>;

    using std::vector;
    using Items = vector<OrderLinePtr>;

class Order:
    public  Persistable,
    public std::enable_shared_from_this<Order>

{
    friend class odb::access;

public:
    Order(ConstPersonPtr saler): saler_{saler}{}

    const Person& saler() const{ return *saler_;}

    void addItem(ConstProductPtr product, uint32_t quantity = 1);
    void removeItem(ConstProductPtr product);
    void removeItems(vector<ConstProductPtr> products);

    double total() const;
    double totalRebate() const;
    double totalTax() const;

    const Items& items() const { return items_;}
    TimeStamp date() const { return date_; }
    int size() const { return items_.size(); }

    void persist() override;

private:
    Order() = default;

public:
    const ulong id{0};

private:
    ConstPersonPtr saler_;
    vector<OrderLinePtr> items_;

    TimeStamp date_{nowLocal()};
    double total_{};
    double total_rebate{};
};

}}

#endif // ORDER_H
