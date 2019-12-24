// Yama
// Copyright (c) 2016-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <ostream>
#include "../matrix3x4.hpp"

namespace yama
{

template <typename T>
::std::ostream& operator<<(::std::ostream& o, const matrix3x4_t<T>& m)
{
    o << "(("
        << m.m00 << ", " << m.m01 << ", " << m.m02 << ", " << m.m03 << "), ("
        << m.m10 << ", " << m.m11 << ", " << m.m12 << ", " << m.m13 << "), ("
        << m.m20 << ", " << m.m21 << ", " << m.m22 << ", " << m.m23 << "))";
    return o;
}

}
