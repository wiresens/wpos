// file      :	orderline.cpp
// birth     :  11/23/2023
// copiright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "orderline.h"
#include "orderline-odb.hxx"

namespace wpos{
namespace model{

Order& OrderLine::order() const {
    auto res = order_.load();
    return *res;
}

}}
