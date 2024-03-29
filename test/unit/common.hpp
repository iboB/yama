// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "yama/util.hpp"
#include <iostream>
#include "doctest/doctest.h"
#include <cstring> // for memcmp

template <typename Y>
class YApprox
{
public:
    using value_type = typename Y::value_type;

    explicit YApprox(const Y& value)
        : e(yama::constants_t<value_type>::EPSILON)
        , v(value)
    {

    }

    YApprox& epsilon(value_type newEpsilon)
    {
        e = newEpsilon;
        return *this;
    }

    value_type e;
    const Y& v;
};

template <typename Y>
inline bool operator==(const Y& lhs, const YApprox<Y>& rhs)
{
    return yama::close(lhs, rhs.v, rhs.e);
}

template <typename Y>
inline bool operator==(const YApprox<Y>& lhs, const Y& rhs) { return operator==(rhs, lhs); }
template <typename Y>
inline bool operator!=(const Y& lhs, const YApprox<Y>& rhs) { return !operator==(lhs, rhs); }
template <typename Y>
inline bool operator!=(const YApprox<Y>& lhs, const Y& rhs) { return !operator==(rhs, lhs); }

template <typename Y>
YApprox<Y> YamaApprox(const Y& y)
{
    return YApprox<Y>(y);
}

template <typename Y>
std::ostream& operator<<(std::ostream& o, const YApprox<Y>& a)
{
    o << a.v;
    return o;
}
