/***************************************************************************
                          pricelog.cpp  -  description
                             -------------------
    begin                : 10/3/2023
    copyright            : (C) 2023 by Gilles BENE
    email                : gilles.bene@wiresens.com
 ***************************************************************************/

/***************************************************************************
                            MIT License Version                                               *
            Copyright (c) 2016-2023  Gilles BENE. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************************/


#include "pricelog.h"
#include "pricelog-odb.hxx"
#include "uom-odb.hxx"

namespace wpos{
namespace model{

inline PriceLog::PriceLog(Product& product, double price)
    : price_{price}
{
    auto id   = db->persist(product);
    product_  = db->load<Product>(id);
}

using query  = odb::query<PriceLog>;
PriceLogPtr PriceLog::priceAsOf(
    ConstProductPtr product_ptr,
    const TimeStamp& stamp)
{
    PriceLogPtr loggedPrice = db->query_one<PriceLog>(
        query::product->id == product_ptr->id
        && query::effectivity.start_date <= stamp
        && stamp < query::effectivity.end_date
    );
    return loggedPrice;
}

void PriceLog::logPrice(ProductPtr productPtr)
{
    auto prev_pricelog = db->query_one<PriceLog>(
        query::product->id == productPtr->id &&
        query::effectivity.end_date >= maxDateTime()
    );

    auto current_price = productPtr->price();
    if (prev_pricelog){
        if( prev_pricelog->price_ > current_price || prev_pricelog->price_ < current_price ) // pricelog->price_ != price
        {
            prev_pricelog->close();
            DateRange newrange(prev_pricelog->effectivity_.endDate(), maxDateTime());
            auto new_pricelog = PriceLog(prev_pricelog->product_, newrange, current_price);
            db->persist(new_pricelog);
            db->update(prev_pricelog);
        }
    }
    else{
        auto new_pricelog = PriceLog(productPtr, current_price);
        db->persist(new_pricelog);
    }
}

void PriceLog::savePrice(Product& product)
{
    auto new_pricelog = PriceLog(product, product.price());
    db->persist(new_pricelog);
}

}}
