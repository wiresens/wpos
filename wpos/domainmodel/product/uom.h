/***************************************************************************
                          uom.h  -  description
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

#ifndef UOM_H
#define UOM_H

#include "../persistable.h"

namespace ws{
namespace model{

//class UomType :  public Persistable<UomType>{
class UomType{
    friend class odb::access;

public:
    UomType(const UomType&) = delete;
    UomType& operator=(const UomType&) = delete;

    UomType(const string& name) : name_{name}{
        if (name.empty()) throw EmptyNameException{};
    }

    const string& name() { return name_ ;}

private:
    UomType() = default;

    uint id_{0};
    const string name_;

};

//class Uom : public Persistable<Uom>{
class Uom {
    friend class odb::access;

public:
    using Type = std::shared_ptr<UomType>;

    Uom(const Uom&) = delete;
    Uom& operator=(const Uom&) = delete;

    Uom(Type type, const string& name, const string& symbol)
        : type_{type}, name_{name}, symbol_{symbol}{
        if (name.empty() || symbol.empty()) throw EmptyNameException{};
    }

private:
    Uom() = default;

    uint    id_{0};
    Type    type_;
    string  name_;
    string  symbol_;
    string  symbolFile_;

    uint    digits_;
    double  factor_;
    double  rate_;
    double  rounding_;

    bool    active_{true};
};

}
}
#endif // UOM_H
