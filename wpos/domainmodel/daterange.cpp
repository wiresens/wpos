/***************************************************************************
                          daterange.cpp  -  description
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


#include "daterange.h"
#include "daterange-odb.hxx"

namespace ws {
bool operator == ( const DateRange& l, const DateRange& r ){
    return (l.start_date_ == r.start_date_ && l.end_date_ == r.end_date_);
}

bool operator <= ( const DateRange& l, const DateRange& r ){
    return (l.start_date_ <= r.start_date_ && l.end_date_ >= r.end_date_);
}

TimeStamp nowLocal(){
    return  pt::microsec_clock::local_time();
}

TimeStamp nowUTC(){
    return  pt::microsec_clock::universal_time();
}

//const Currency& Currency::USD = Currency("USD", '$', 2);
//const Currency& Currency::EURO = Currency("EURO", '€', 2);
//const Currency& Currency::NAIRA = Currency("NAIRA", '₦', 2);
//const Currency& Currency::POUND = Currency("POUND", '£', 2);
//const Currency& Currency::XAF = Currency("XAF", 'F', 2);
//const Currency& Currency::XOF = Currency("XOF", 'F', 2);

//const uint Money::cents[] = {1, 10, 100, 1000, 10000, 100000, 1000000};

//bool operator==( const Currency& l, const Currency&r){
//   return  l.Digits == r.Digits &&
//           l.Symbol == r.Symbol &&
//           l.Name   == r.Name ;
//}

//bool operator!=( const Currency& l, const Currency&r){
//   return !(l==r);
//}

//bool operator==( const Money& l, const Money&r){
//   if (!l.sameUnit(r)) throw Currency::CurrencyMismacth{};
//   return l.amount_ == r.amount_;
//}

//bool operator!=( const Money& l, const Money&r){
//   return !( l == r);
//}

//bool operator>( const Money& l, const Money&r){
//   if (!l.sameUnit(r)) throw Currency::CurrencyMismacth{};
//   return l.amount_   > r.amount_;
//}

//bool operator<( const Money& l, const Money&r){
//   if (!l.sameUnit(r)) throw Currency::CurrencyMismacth{};
//   return l.amount_   < r.amount_;
//}

//bool operator>=( const Money& l, const Money&r){
//   return l > r || l == r;
//}

//bool operator<=( const Money& l, const Money&r){
//   return l < r || l == r;
//}

//Money operator+( const Money& l, const Money&r){
//    if (!l.sameUnit(r)) throw Currency::CurrencyMismacth{};
//    Money tmp(l);
//    tmp.amount_ += r.amount_;
//    return tmp;
//}

//Money operator-( const Money& l, const Money&r){
//    if (!l.sameUnit(r)) throw Currency::CurrencyMismacth{};
//    Money tmp(l);
//    tmp.amount_ -= r.amount_;
//    return tmp;
//}

//Money operator*( const Money& m, float v){
//    Money tmp(m);
//    tmp.amount_ *= v;
//    return tmp;
//}

//Money operator*( float v, const Money& m ){
//    return m*v;
//}

//Money operator/( const Money& m, float v){
//    Money tmp(m);
//    tmp.amount_ /= v;
//    return tmp;
//}

}
