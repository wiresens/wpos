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

namespace wpos{
using std::string;

namespace time  = std::chrono;
namespace greg  = boost::gregorian;
namespace pt    = boost::posix_time;

using TimeStamp = pt::ptime;
using Date      = greg::date;
using DateTime  = greg::date;

TimeStamp nowLocal();
TimeStamp nowUTC();
TimeStamp maxDateTime();

class DateRange{
public:
    friend class odb::access;
    struct DateRangeException : std::exception{};



    DateRange( const pt::ptime& start_date, const pt::ptime& end_date)
        : start_date_{start_date}, end_date_{end_date}
    {
        if( start_date_ >= end_date_ ) throw DateRangeException{};
    }

    #if defined(DATABASE_SQLITE)
    DateRange( const pt::ptime& date)
        : start_date_{date}, end_date_{date}
    {}
    #endif

    void setRange(const DateRange& range){
        start_date_ = range.start_date_;
        end_date_   = range.end_date_;
    }

    const TimeStamp& startDate() const { return start_date_ ;}
    const TimeStamp& endDate() const { return end_date_ ;}

    bool contains( const pt::ptime& timestamp) const{
        return start_date_ <= timestamp && timestamp < end_date_ ;
    }

    bool isOngoing() const {
        return end_date_ > nowLocal();
    }

private:
    DateRange() = default;

    TimeStamp start_date_{};
    TimeStamp end_date_{};

    friend bool operator == ( const DateRange& l, const DateRange& r );
    friend bool operator <= ( const DateRange& l, const DateRange& r );
};
}

#endif // DATERANGE_H
