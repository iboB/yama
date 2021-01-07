// Yama
// Copyright (c) 2016-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include <cstddef>
#include <cmath>
#include <type_traits>

#include "assert.hpp"
#include "shorthand.hpp"

namespace yama
{

template <typename T>
class constants_t
{
public:
    static inline constexpr T PI = T(3.1415926535897932384626433832795);
    static inline constexpr T PI_HALF = T(1.5707963267948966192313216916398);
    static inline constexpr T PI_D4 = T(0.78539816339744830961566084581988); // pi / 4
    static inline constexpr T PI_DBL = T(6.283185307179586476925286766559); // 2*pi
    static inline constexpr T OVER_PI = T(0.31830988618379067153776752674503); // 1/pi
    static inline constexpr T E = T(2.71828182845904523536028747135266249); //natural constant
    static inline constexpr T SQRT_2 = T(1.4142135623730950488016887242097); //sqrt(2)

    static inline constexpr T EPSILON = T(1e-5); //epsilon for floating point equalities
    static inline constexpr T EPSILON_LOW = T(1e-3); //low precision epsilon
    static inline constexpr T EPSILON_HIGH = T(1e-7); //high precision epsilon
};

// shorthand
#if !defined(YAMA_NO_SHORTHAND)
using constants = constants_t<preferred_type>;
#endif

template <typename T>
constexpr T sq(const T& a)
{
    return a*a;
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value,
    T>::type sign(const T& t)
{
    return std::signbit(t) ? T(-1) : T(1);
}

template <typename T>
typename std::enable_if<std::is_signed<T>::value && std::is_integral<T>::value,
    T>::type sign(const T& t)
{
    return t < 0 ? T(-1) : T(1);
}

template <typename T>
typename std::enable_if<std::is_unsigned<T>::value,
    T>::type sign(const T& )
{
    return 1;
}

template <typename T>
void flip_sign(T& a)
{
    a = -a;
}

template <typename T, typename S>
T lerp(const T& from, const T& to, const S& ratio)
{
    return from + ratio * (to - from);
}

template <typename T>
T rad_to_deg(const T& radians)
{
    return radians * (T(180) / constants_t<T>::PI);
}

template <typename T>
T deg_to_rad(const T& degrees)
{
    return degrees * (constants_t<T>::PI / T(180));
}

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value,
    bool>::type close(const T& a, const T& b, const T& epsilon = constants_t<T>::EPSILON)
{
    return !(std::abs(a - b) > epsilon);
}


template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value,
    const T&>::type clamp(const T& v, const T& min, const T& max)
{
    if (min < max)
    {
        if (v < min)
            return min;

        if (v > max)
            return max;
    }
    else
    {
        if (v < max)
            return max;

        if (v > min)
            return min;
    }

    return v;
}

// can be used to make a type with defined `U at(integral n)` be used as a key in a std::map or in a std::set
struct strict_weak_ordering
{
    template <typename T>
    bool operator()(const T& a, const T& b)
    {
        YAMA_ASSERT_CRIT(a.size() == b.size(), "yama::strict_weak_ordering of types with no matching sizes");
        for (size_t i = 0; i < a.size(); ++i)
        {
            if (a.at(i) < b.at(i))
                return true;
            else if (a.at(i) > b.at(i))
                return false;
        }

        return false;
    }
};

}
