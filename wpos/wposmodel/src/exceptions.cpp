// file      :	exceptions.cpp
// birth     :  11/16/2023
// copiright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "exceptions.h"

namespace wpos {

    const char* EmptyNameException::
    what() const noexcept{
        return "Fatal : Empty Name Supplied";
    }

    EmptyNameException* EmptyNameException::
    clone () const{
        return new EmptyNameException (*this);
    }

    const char* BadGenderException::
    what() const noexcept{
        return "Error : Invalid Gender Supplied";
    }

    BadGenderException* BadGenderException::
    clone () const{
        return new BadGenderException(*this);
    }

    const char* DuplicateAuthTokenException::
    what() const noexcept{
        return "Error : Duplicate Authentication Token Supplied";
    }

    DuplicateAuthTokenException* DuplicateAuthTokenException::
    clone () const{
        return new DuplicateAuthTokenException(*this);
    }

    const char* ZeroPriceException::
        what() const noexcept{
        return "Error : Zero Price Supplied";
    }

    ZeroPriceException* ZeroPriceException::
        clone () const{
        return new ZeroPriceException(*this);
    }

    const char* NotFoundException::
        what() const noexcept{
        return "Error : Item not Found";
    }

    NotFoundException* NotFoundException::
        clone () const{
        return new NotFoundException(*this);
    }

    const char* unknown_driver_exception::
        what() const noexcept{
        return "Fatal : Unkown driver; Know drivers : PGSQL|MYSQL|SQLITE";
    }

    unknown_driver_exception* unknown_driver_exception::
        clone () const{
        return new unknown_driver_exception(*this);
    }
}

