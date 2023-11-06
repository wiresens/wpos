/***************************************************************************
                          daterange.hxx  -  description
                             -------------------
    begin                : 10/4/2023
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


#ifndef DATERANGE_HXX
#define DATERANGE_HXX

#include "daterange.h"
using namespace ws;

#pragma db value(DateRange) definition
#pragma db member(DateRange::start_date_) not_null
//#pragma db value(DateRange) options ("CHECK DateRange::end_date_ > DateRange::start_date_")

//#pragma db value(Money) definition
//#pragma db member(Money::currency_) not_null

//#pragma db object(Currency) definition
//#pragma db member(Currency::id_) id auto
//#pragma db member(Currency::Name) unique not_null
//#pragma db member(Currency::Symbol) not_null
//#pragma db member(Currency::Digits) not_null

#endif // DATERANGE_HXX
