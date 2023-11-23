/***************************************************************************
                          party.hxx  -  description
                             -------------------
    begin                : 10/13/2023
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


#ifndef PARTY_HXX
#define PARTY_HXX

#include "party.h"
#include "../version.hxx"
using namespace wpos::model;

#pragma db object(Party) abstract definition
#pragma db member(Party::code_) unique not_null
#pragma db member(Party::name_) not_null
#pragma db member(Party::pictureFile_) null
#pragma db member(Party::address_) null

#pragma db object(Person) definition
#pragma db member(Person::id_) id auto
#pragma db member(Person::first_name_) not_null
#pragma db member(Person::gender_) not_null

#pragma db object(Address) definition
#pragma db member(Address::id_) id auto
#pragma db member(Address::phone1_) not_null
#pragma db member(Address::country_) not_null
#pragma db member(Address::city_) not_null

#ifdef ODB_COMPILER
    #include "../persistable.hxx"
#endif

#endif // PARTY_HXX
