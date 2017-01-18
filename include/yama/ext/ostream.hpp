// Yama
// Copyright (c) 2016 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <ostream>
#include "../vector2.hpp"
#include "../vector3.hpp"
#include "../vector4.hpp"
#include "../quaternion.hpp"
#include "../matrix4x4.hpp"

namespace yama
{

template <typename T>
::std::ostream& operator<<(::std::ostream& o, const vector2_t<T>& v)
{
    o << '(' << v.x << ", " << v.y << ')';
    return o;
}

template <typename T>
::std::ostream& operator<<(::std::ostream& o, const vector3_t<T>& v)
{
    o << '(' << v.x << ", " << v.y << ", " << v.z << ')';
    return o;
}

template <typename T>
::std::ostream& operator<<(::std::ostream& o, const vector4_t<T>& v)
{
    o << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
    return o;
}

template <typename T>
::std::ostream& operator<<(::std::ostream& o, const quaternion_t<T>& v)
{
    o << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
    return o;
}

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
