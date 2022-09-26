// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

#include <ostream>
#include "../vector4.hpp"

namespace yama
{

template <typename T>
::std::ostream& operator<<(::std::ostream& o, const vector4_t<T>& v)
{
    o << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
    return o;
}

}
