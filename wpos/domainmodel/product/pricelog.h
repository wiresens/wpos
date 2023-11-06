/***************************************************************************
                          pricelog.h  -  description
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

#ifndef PRICELOG_H
#define PRICELOG_H

#include "product.h"
#include "../daterange.h"

#include <limits>

namespace ws{
namespace model{

using ws::DateRange;
class PriceLog;
using PriceLogPtr = std::shared_ptr<PriceLog>;

//class PriceLog : public Persistable<PriceLog>{
class PriceLog : public Persistable{
   friend class odb::access;

public:
    PriceLog(const PriceLog&) = delete;
    PriceLog& operator=(const PriceLog&) = delete;

    ulong id() const{ return id_ ;}
    const DateRange& effectivity() const{
        return effectivity_;
    }

    static void newPrice(ProductPtr product_ptr, double price);
    static PriceLogPtr checkPrice(ConstProductPtr product_ptr, const TimeStamp& stamp  = nowLocal());
//    static double price(const Product& product, const TimeStamp& stamp = nowLocal());

private:
    PriceLog() = default;

    explicit PriceLog(ConstProductPtr product, double price = std::numeric_limits<double>::max())
        : product_{ product}, price_{price}{}

    PriceLog(ConstProductPtr product_ptr, const DateRange& range, double price )
        : PriceLog{product_ptr, price}{ effectivity_ = range;}

    void close(const TimeStamp& stamp){
        effectivity_= DateRange(effectivity_.startDate(), stamp);
    }

private:
    ulong id_{0};
    DateRange effectivity_;

public:
    ConstProductPtr product_;
    const double price_{std::numeric_limits<double>::max()};
    Product::PriceType type_;
};

}
}

#endif // PRICELOG_H
