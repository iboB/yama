// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
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
