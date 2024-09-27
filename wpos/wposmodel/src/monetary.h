/***************************************************************************
                          monetary.h  -  description
                             -------------------
    begin                : 10/28/2023
    copyright            : (C) 2023 by Gilles BENE POUGOUE
    email                : gilles.bene@wiresens.com
 ***************************************************************************/

/***************************************************************************
                            MIT License Version 2.0                                               *
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


#ifndef MONETARY_H
#define MONETARY_H

#include <cstdint>

#include <memory>
#include <cmath>
#include <odb/database.hxx>

namespace wpos{
using std::string;
using uint = unsigned int;

struct Currency{
    friend class odb::access;
    using CurrencyPtr = std::shared_ptr<const Currency>;

    static const Currency& USD;
    static const Currency& EURO;
    static const Currency& NAIRA;
    static const Currency& POUND;
    static const Currency& XAF;
    static const Currency& XOF;

    struct BadCurrency : std::exception{};
    struct CurrencyMismacth : std::exception{};

    Currency(string name, char symbol, uint digits = 4)
        : Name{name}, Digits{digits}, Symbol{symbol}{
        if( Name.empty() || isspace(Symbol) > 0   ) throw BadCurrency{};
    }

    uint         id_{0};
    const string Name;
    const uint   Digits;
    const char   Symbol;

private:
    Currency() : Currency{"USD",'$'}{}
};

bool operator==( const Currency& l, const Currency&r);
bool operator!=( const Currency& l, const Currency&r);

class Money{
public:
    friend class odb::access;
    Money(const Currency& currency)
    {
        currency_.reset(&currency, [](const Currency*){});
    }

    Money( double amount, const Currency& currency):
        Money{currency}
    {
        amount_ = ceil( amount * centFactor());
    }

    Money( long amount, Currency& currency):
        Money{currency}
    {
        amount_ = amount * centFactor();
    }

    double amount() const {
        return amount_ / currency_->Digits;
    }

    const Currency& currency() const{
        return *currency_;
    }

private:
    uint centFactor(){
        return cents[currency_->Digits];
    }

    bool sameUnit(const Money& other) const{
        return *currency_ == *other.currency_;
    }

private:
    static const uint cents[];
    long amount_{0};
    Currency::CurrencyPtr currency_;

    friend bool operator==( const Money& l, const Money& r);
    friend bool operator!=( const Money& l, const Money& r);
    friend bool operator>=( const Money& l, const Money& r);
    friend bool operator<=( const Money& l, const Money& r);
    friend bool operator >( const Money& l, const Money& r);
    friend bool operator <( const Money& l, const Money& r);

    friend Money operator+( const Money& l, const Money& r);
    friend Money operator-( const Money& l, const Money& r);
    friend Money operator*( const Money& m, float v);
    friend Money operator*( float v, const Money& m );
    friend Money operator/( const Money& m, float v);
};

}

#endif // MONETARY_H
