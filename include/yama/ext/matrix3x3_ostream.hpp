// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

#include <ostream>
#include "../matrix3x3.hpp"

namespace yama
{

template <typename T>
::std::ostream& operator<<(::std::ostream& o, const matrix3x3_t<T>& m)
{
    o << "(("
        << m.m00 << ", " << m.m01 << ", " << m.m02 << "), ("
        << m.m10 << ", " << m.m11 << ", " << m.m12 << "), ("
        << m.m20 << ", " << m.m21 << ", " << m.m22 << "))";
    return o;
}

}
