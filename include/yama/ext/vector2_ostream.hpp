// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

#include <ostream>
#include "../vector2.hpp"

namespace yama
{

template <typename T>
::std::ostream& operator<<(::std::ostream& o, const vector2_t<T>& v)
{
    o << '(' << v.x << ", " << v.y << ')';
    return o;
}

}
