// file      :	exceptions.h
// birth     :  11/16/2023
// copiright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "exception.h"

namespace wpos {
struct EmptyValueException : wpos::exception{
    virtual const char*  what() const noexcept;
    virtual EmptyValueException*  clone () const;
};

struct BadGenderException : wpos::exception{
    virtual const char* what() const noexcept;
    virtual BadGenderException*  clone () const;
};

struct DuplicateAuthTokenException : wpos::exception{
    virtual const char*  what() const noexcept;
    virtual DuplicateAuthTokenException*  clone () const;
};

struct ZeroPriceException : wpos::exception{
    virtual const char*  what() const noexcept;
    virtual ZeroPriceException*  clone () const;
};

struct NotFoundException : wpos::exception{
    virtual const char*  what() const noexcept;
    virtual NotFoundException*  clone () const;
};

struct unknown_driver_exception : wpos::exception{
    virtual const char*  what() const noexcept;
    virtual unknown_driver_exception*  clone () const;
};

}


#endif // EXCEPTIONS_H
