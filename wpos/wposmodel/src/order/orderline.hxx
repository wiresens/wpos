// file      :	orderline.hxx
// birth     :  11/23/2023
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef ORDERLINE_HXX
#define ORDERLINE_HXX

#include "orderline.h"
#include "version.hxx"

using namespace wpos::model;

#pragma db object(OrderLine) definition
#pragma db member(OrderLine::id) id auto
#pragma db member(OrderLine::quantity_) not_null options("CHECK(quantity > 0)")
#pragma db member(OrderLine::unit_price_) not_null options("CHECK(unit_price > 0)")
#pragma db member(OrderLine::rebate_)  options("CHECK(rebate >= 0 AND rebate < 100)")
#pragma db member(OrderLine::vat_)  options("CHECK(vat >= 0 AND vat < 100)")

#pragma db member(OrderLine::product_) on_delete(cascade ) not_null
#pragma db member(OrderLine::order_) on_delete( cascade ) not_null

#pragma db index(OrderLine::"product_order_i") unique members ( product_ , order_ )

#ifdef ODB_COMPILER
    #include "order.hxx"
    #include "product.hxx"
#endif

#endif // ORDERLINE_HXX
