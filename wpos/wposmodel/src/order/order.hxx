// file      :	order.hxx
// birth     :  11/23/2023
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef ORDER_HXX
#define ORDER_HXX

#include "order.h"
#include "version.hxx"

using namespace wpos::model;

#pragma db object(Order) definition
#pragma db member(Order::id) id auto
#pragma db member(Order::date_) not_null

#pragma db member(Order::saler_) on_delete( cascade) not_null
#pragma db member(Order::items_) inverse (order_) value_not_null

#pragma db index(Order::"saler_date_i") unique members ( saler_ , date_ )

#ifdef ODB_COMPILER
    #include "party.hxx"
    #include "orderline.hxx"
#endif

#endif // ORDER_HXX
