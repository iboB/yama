// Yama
// Copyright (c) 2016-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

namespace yama
{

template <size_t D>
struct dim {};

template <>
struct dim<2>
{
    template <typename T>
    using vector_t = vector2_t<T>;

#if !defined(YAMA_NO_SHORTHAND)
    using vector = vector2;
#endif
};

template <>
struct dim<3>
{
    template <typename T>
    using vector_t = vector3_t<T>;

#if !defined(YAMA_NO_SHORTHAND)
    using vector = vector3;
#endif
};

template <>
struct dim<4>
{
    template <typename T>
    using vector_t = vector4_t<T>;

#if !defined(YAMA_NO_SHORTHAND)
    using vector = vector4;
#endif
};

}
