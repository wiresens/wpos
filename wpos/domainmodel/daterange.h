/***************************************************************************
                          daterange.h  -  description
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


#ifndef DATERANGE_H
#define DATERANGE_H

#include <chrono>
#include <memory>

#include <odb/database.hxx>
#include <boost/date_time.hpp>

namespace ws{
using std::string;

namespace time  = std::chrono;
namespace greg  = boost::gregorian;
namespace pt    = boost::posix_time;

using TimeStamp = pt::ptime;
using Date      = greg::date;
using DateTime  = greg::date;

TimeStamp nowLocal();
TimeStamp nowUTC();

class DateRange{
public:
    friend class odb::access;
    struct DateRangeException : std::exception{};

    DateRange() = default;

    DateRange( const pt::ptime& start_date, const pt::ptime& end_date = pt::pos_infin)
        : start_date_{start_date}, end_date_{end_date}
    {
        if( start_date_ >= end_date_ ) throw DateRangeException{};
    }

    void setRange(const DateRange& range){
        start_date_ = range.start_date_;
        end_date_   = range.end_date_;
    }

    const TimeStamp& startDate() const { return start_date_ ;}
    const TimeStamp& endDate() const { return end_date_ ;}

    bool contains( const pt::ptime& timestamp) const{
        return start_date_ <= timestamp && timestamp < end_date_ ;
    }

    bool isCurrent() const {
        return end_date_.is_not_a_date_time();
    }

private:
    TimeStamp start_date_{ nowLocal()};
    TimeStamp end_date_{pt::pos_infin};

    friend bool operator == ( const DateRange& l, const DateRange& r );
    friend bool operator <= ( const DateRange& l, const DateRange& r );
};

//struct Currency{
//    friend class odb::access;
//    using CurrencyPtr = std::shared_ptr<const Currency>;

//    static const Currency& USD;
//    static const Currency& EURO;
//    static const Currency& NAIRA;
//    static const Currency& POUND;
//    static const Currency& XAF;
//    static const Currency& XOF;

//    struct BadCurrency : std::exception{};
//    struct CurrencyMismacth : std::exception{};

//    Currency(string name, char symbol, uint digits = 4)
//        : Name{name}, Digits{digits}, Symbol{symbol}{
//        if( Name.empty() || isspace(Symbol) > 0   ) throw BadCurrency{};
//    }

//    uint         id_{0};
//    const string Name;
//    const uint   Digits;
//    const char   Symbol;

//private:
//    Currency() : Currency{"USD",'$'}{}
//};

//bool operator==( const Currency& l, const Currency&r);
//bool operator!=( const Currency& l, const Currency&r);

//class Money{
//public:
//    friend class odb::access;
//    Money(const Currency& currency)
//    {
//        currency_.reset(&currency, [](const Currency*){});
//    }

//    Money( double amount, const Currency& currency):
//        Money{currency}
//    {
//        amount_ = ceil( amount * centFactor());
//    }

//    Money( long amount, Currency& currency):
//        Money{currency}
//    {
//        amount_ = amount * centFactor();
//    }

//    double amount() const {
//        return amount_ / currency_->Digits;
//    }

//    const Currency& currency() const{
//        return *currency_;
//    }

//private:
//    uint centFactor(){
//        return cents[currency_->Digits];
//    }

//    bool sameUnit(const Money& other) const{
//        return *currency_ == *other.currency_;
//    }

//private:
//    static const uint cents[];
//    long amount_{0};
//    Currency::CurrencyPtr currency_;

//    friend bool operator==( const Money& l, const Money&r);
//    friend bool operator!=( const Money& l, const Money&r);
//    friend bool operator>=( const Money& l, const Money&r);
//    friend bool operator<=( const Money& l, const Money&r);
//    friend bool operator>( const Money& l, const Money&r);
//    friend bool operator<( const Money& l, const Money&r);

//    friend Money operator+( const Money& l, const Money&r);
//    friend Money operator-( const Money& l, const Money&r);
//    friend Money operator*( const Money& m, float v);
//    friend Money operator*( float v, const Money& m );
//    friend Money operator/( const Money& m, float v);
//};

}

#endif // DATERANGE_H
