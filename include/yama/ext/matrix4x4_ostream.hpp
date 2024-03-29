// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

#include <ostream>
#include "../matrix4x4.hpp"

namespace yama
{

template <typename T>
::std::ostream& operator<<(::std::ostream& o, const matrix4x4_t<T>& m)
{
    o << "(("
        << m.m00 << ", " << m.m01 << ", " << m.m02 << ", " << m.m03 << "), ("
        << m.m10 << ", " << m.m11 << ", " << m.m12 << ", " << m.m13 << "), ("
        << m.m20 << ", " << m.m21 << ", " << m.m22 << ", " << m.m23 << "), ("
        << m.m30 << ", " << m.m31 << ", " << m.m32 << ", " << m.m33 << "))";
    return o;
}

}
