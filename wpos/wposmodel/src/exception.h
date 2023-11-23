// file      :	exception.h
// birth     :  11/16/2023
// copiright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

namespace wpos
{
    struct exception : std::exception{
        virtual const char*
        what () const noexcept  = 0;

        virtual exception*
        clone () const = 0;
    };

    namespace common
    {
        using wpos::exception;
    }

}

#endif // EXCEPTION_H
