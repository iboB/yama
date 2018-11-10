// Yama
// Copyright (c) 2016-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <ostream>
#include "../vector3.hpp"

namespace yama
{

template <typename T>
::std::ostream& operator<<(::std::ostream& o, const vector3_t<T>& v)
{
    o << '(' << v.x << ", " << v.y << ", " << v.z << ')';
    return o;
}

}
