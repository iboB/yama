// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <cmath>
#include <cstddef>
#include <iterator>
#include <cstdlib>
#include <algorithm>

#include "util.hpp"
#include "shorthand.hpp"
#include "type_traits.hpp"

namespace yama
{

template <typename T>
class vector2_t;

template <typename T>
class vector3_t;

template <typename T>
class vector4_t
{
public:
    T x, y, z, w;

    using value_type = T;
    using size_type = size_t;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = typename std::reverse_iterator<iterator>;
    using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

    static constexpr size_type value_count = 4;

    constexpr size_type max_size() const { return value_count; }
    constexpr size_type size() const { return max_size(); }

    ///////////////////////////////////////////////////////////////////////////
    // named constructors
    static constexpr vector4_t coord(const value_type& x, const value_type& y, const value_type& z, const value_type& w)
    {
        return{ x, y, z, w};
    }

    static constexpr vector4_t uniform(const value_type& s)
    {
        return coord(s, s, s, s);
    }

    static constexpr vector4_t zero()
    {
        return uniform(value_type(0));
    }

    static vector4_t from_ptr(const value_type* ptr)
    {
        YAMA_ASSERT_CRIT(ptr, "Constructing yama::vector4_t from nullptr");
        return coord(ptr[0], ptr[1], ptr[2], ptr[3]);
    }

    static constexpr vector4_t unit_x()
    {
        return coord(1, 0, 0, 0);
    }

    static constexpr vector4_t unit_y()
    {
        return coord(0, 1, 0, 0);
    }

    static constexpr vector4_t unit_z()
    {
        return coord(0, 0, 1, 0);
    }

    static constexpr vector4_t unit_w()
    {
        return coord(0, 0, 0, 1);
    }

    ///////////////////////////
    // attach
    static vector4_t& attach_to_ptr(value_type* ptr)
    {
        YAMA_ASSERT_BAD(ptr, "Attaching yama::vector4_t to nullptr");
        return *reinterpret_cast<vector4_t*>(ptr);
    }

    static const vector4_t& attach_to_ptr(const value_type* ptr)
    {
        YAMA_ASSERT_BAD(ptr, "Attaching yama::vector4_t to nullptr");
        return *reinterpret_cast<const vector4_t*>(ptr);
    }

    static vector4_t* attach_to_array(value_type* ptr)
    {
        YAMA_ASSERT_WARN(ptr, "Attaching yama::vector4_t to nullptr");
        return reinterpret_cast<vector4_t*>(ptr);
    }

    static const vector4_t* attach_to_array(const value_type* ptr)
    {
        YAMA_ASSERT_WARN(ptr, "Attaching yama::vector4_t to nullptr");
        return reinterpret_cast<const vector4_t*>(ptr);
    }

    ///////////////////////////////////////////////////////////////////////////
    // access
    value_type* data()
    {
        return reinterpret_cast<value_type*>(this);
    }

    constexpr const value_type* data() const
    {
        return reinterpret_cast<const value_type*>(this);
    }

    value_type& at(size_type i)
    {
        YAMA_ASSERT_CRIT(i < value_count, "yama::vector4_t index overflow");
        return data()[i];
    }

    constexpr const value_type& at(size_type i) const
    {
        YAMA_ASSERT_CRIT(i < value_count, "yama::vector4_t index overflow");
        return data()[i];
    }

    value_type& operator[](size_type i)
    {
        return at(i);
    }

    constexpr const value_type& operator[](size_type i) const
    {
        return at(i);
    }

    ///////////////////////////
    // cast

    value_type* as_ptr()
    {
        return data();
    }

    const value_type* as_ptr() const
    {
        return data();
    }

    template <typename S>
    vector4_t<S> as_vector4_t() const
    {
        return vector4_t<S>::coord(S(x), S(y), S(z), S(w));
    }


    vector2_t<T>& xy();
    const vector2_t<T>& xy() const;
    vector2_t<T> xz() const;
    vector2_t<T> zw() const;
    vector3_t<T>& xyz();
    const vector3_t<T>& xyz() const;
    vector3_t<T> zyx() const;
    vector4_t<T>& xyzw() { return *this; }
    const vector4_t<T>& xyzw() const { return *this; }
    vector4_t<T> zyxw() const { return coord(z, y, x, w); }
    vector4_t<T> wzyx() const { return coord(w, z, y, x); }

    constexpr vector4_t swizzle(int sx, int sy, int sz, int sw) const {
        YAMA_ASSERT_CRIT(sx >= 0 && sx < value_count, "yama::vector4_t swizzle index out of range");
        YAMA_ASSERT_CRIT(sy >= 0 && sy < value_count, "yama::vector4_t swizzle index out of range");
        YAMA_ASSERT_CRIT(sz >= 0 && sz < value_count, "yama::vector4_t swizzle index out of range");
        YAMA_ASSERT_CRIT(sw >= 0 && sw < value_count, "yama::vector4_t swizzle index out of range");
        return coord(at(sx), at(sy), at(sz), at(sw));
    }

    constexpr vector4_t swizzle(const vector4_t<int>& s) const {
        return swizzle(s.x, s.y, s.z, s.w);
    }

    ///////////////////////////
    // std

    iterator begin()
    {
        return data();
    }

    iterator end()
    {
        return data() + value_count;
    }

    const_iterator begin() const
    {
        return data();
    }

    const_iterator end() const
    {
        return data() + value_count;
    }

    value_type& front()
    {
        return at(0);
    }

    value_type& back()
    {
        return at(value_count - 1);
    }

    constexpr const value_type& front() const
    {
        return at(0);
    }

    constexpr const value_type& back() const
    {
        return at(value_count - 1);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend() const { return rend(); }

    ///////////////////////////////////////////////////////////////////////////
    // arithmetic

    constexpr const vector4_t& operator+() const
    {
        return *this;
    }

    constexpr vector4_t operator-() const
    {
        return coord(-x, -y, -z, -w);
    }

    vector4_t& operator+=(const vector4_t& b)
    {
        x += b.x;
        y += b.y;
        z += b.z;
        w += b.w;
        return *this;
    }

    vector4_t& operator-=(const vector4_t& b)
    {
        x -= b.x;
        y -= b.y;
        z -= b.z;
        w -= b.w;
        return *this;
    }

    vector4_t& operator*=(const value_type& s)
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }

    vector4_t& operator/=(const value_type& s)
    {
        YAMA_ASSERT_WARN(s != 0, "yama::vector4_t division by zero");
        x /= s;
        y /= s;
        z /= s;
        w /= s;
        return *this;
    }

    vector4_t& mul(const vector4_t& b)
    {
        x *= b.x;
        y *= b.y;
        z *= b.z;
        w *= b.w;
        return *this;
    }

    vector4_t& div(const vector4_t& b)
    {
        x /= b.x;
        y /= b.y;
        z /= b.z;
        w /= b.w;
        return *this;
    }

    constexpr value_type length_sq() const
    {
        return sq(x) + sq(y) + sq(z) + sq(w);
    }

    value_type length() const
    {
        return std::sqrt(length_sq());
    }

    constexpr value_type manhattan_length() const
    {
        return std::abs(x) + std::abs(y) + std::abs(z) + std::abs(w);
    }

    value_type normalize()
    {
        auto l = length();
        YAMA_ASSERT_WARN(l, "Normalizing zero-length yama::vector4_t");
        x /= l;
        y /= l;
        z /= l;
        w /= l;
        return l;
    }

    bool is_normalized() const
    {
        return close(length(), value_type(1));
    }

    void homogenous_normalize()
    {
        YAMA_ASSERT_WARN(w != 0, "Homogenous normalization of yama::vector4_t with zero y");
        x /= w;
        y /= w;
        z /= w;
        w = 1;
    }

    vector4_t reflection(const vector4_t& normal) const
    {
        YAMA_ASSERT_WARN(normal.is_normalized(), "Reflecting with a non-normalized normal yama::vector4_t");
        auto dd = 2 * dot(*this, normal);
        return coord(x - dd * normal.x, y - dd * normal.y, z - dd * normal.z, w - dd * normal.w);
    }

    constexpr value_type product() const
    {
        return x * y * z * w;
    }

    constexpr value_type sum() const
    {
        return x + y + z + w;
    }
};

template <typename T>
vector4_t<T> operator+(const vector4_t<T>& a, const vector4_t<T>& b)
{
    return vector4_t<T>::coord(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <typename T>
vector4_t<T> operator-(const vector4_t<T>& a, const vector4_t<T>& b)
{
    return vector4_t<T>::coord(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <typename T>
vector4_t<T> operator*(const vector4_t<T>& a, const T& s)
{
    return vector4_t<T>::coord(a.x * s, a.y * s, a.z * s, a.w * s);
}

template <typename T>
vector4_t<T> operator*(const T& s, const vector4_t<T>& b)
{
    return vector4_t<T>::coord(s * b.x, s * b.y, s * b.z, s * b.w);
}

template <typename T>
vector4_t<T> operator/(const vector4_t<T>& a, const T& s)
{
    YAMA_ASSERT_WARN(s != 0, "yama::vector4_t division by zero");
    return vector4_t<T>::coord(a.x / s, a.y / s, a.z / s, a.w / (s));
}

template <typename T>
vector4_t<T> operator/(const T& s, const vector4_t<T>& b)
{
    return vector4_t<T>::coord(s / b.x, s / b.y, s / b.z, s / b.w);
}

template <typename T>
bool operator==(const vector4_t<T>& a, const vector4_t<T>& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template <typename T>
bool operator!=(const vector4_t<T>& a, const vector4_t<T>& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

template <typename T>
bool close(const vector4_t<T>& a, const vector4_t<T>& b, const T& epsilon = constants_t<T>::EPSILON)
{
    return close(a.x, b.x, epsilon) && close(a.y, b.y, epsilon) && close(a.z, b.z, epsilon) && close(a.w, b.w, epsilon);
}

template <typename T>
vector4_t<T> abs(const vector4_t<T>& a)
{
    return vector4_t<T>::coord(std::abs(a.x), std::abs(a.y), std::abs(a.z), std::abs(a.w));
}

template <typename T>
vector4_t<T> mul(const vector4_t<T>& a, const vector4_t<T>& b)
{
    return vector4_t<T>::coord(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template <typename T>
vector4_t<T> div(const vector4_t<T>& a, const vector4_t<T>& b)
{
    YAMA_ASSERT_WARN(b.x != 0, "yama::vector4_t division by zero");
    YAMA_ASSERT_WARN(b.y != 0, "yama::vector4_t division by zero");
    YAMA_ASSERT_WARN(b.z != 0, "yama::vector4_t division by zero");
    YAMA_ASSERT_WARN(b.w != 0, "yama::vector4_t division by zero");
    return vector4_t<T>::coord(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value,
    vector4_t<T>>::type mod(const vector4_t<T>& n, const vector4_t<T>& d)
{
    YAMA_ASSERT_WARN(d.x != 0, "yama::vector4_t division by zero");
    YAMA_ASSERT_WARN(d.y != 0, "yama::vector4_t division by zero");
    YAMA_ASSERT_WARN(d.z != 0, "yama::vector4_t division by zero");
    YAMA_ASSERT_WARN(d.w != 0, "yama::vector4_t division by zero");
    return vector4_t<T>::coord(fmod(n.x, d.x), fmod(n.y, d.y), fmod(n.z, d.z), fmod(n.w, d.w));
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value,
    vector4_t<T>>::type mod(const vector4_t<T>& n, const vector4_t<T>& d)
{
    YAMA_ASSERT_WARN(d.x != 0, "yama::vector4_t division by zero");
    YAMA_ASSERT_WARN(d.y != 0, "yama::vector4_t division by zero");
    YAMA_ASSERT_WARN(d.z != 0, "yama::vector4_t division by zero");
    YAMA_ASSERT_WARN(d.w != 0, "yama::vector4_t division by zero");

    return vector4_t<T>::coord(n.x % d.x, n.y % d.y, n.z % d.z, n.w % d.w);
}

template <typename T>
vector4_t<T> floor(const vector4_t<T>& a)
{
    return vector4_t<T>::coord(::std::floor(a.x), ::std::floor(a.y), ::std::floor(a.z), ::std::floor(a.w));
}

template <typename T>
vector4_t<T> ceil(const vector4_t<T>& a)
{
    return vector4_t<T>::coord(::std::ceil(a.x), ::std::ceil(a.y), ::std::ceil(a.z), ::std::ceil(a.w));
}

template <typename T>
vector4_t<T> round(const vector4_t<T>& a)
{
    return vector4_t<T>::coord(::std::round(a.x), ::std::round(a.y), ::std::round(a.z), ::std::round(a.w));
}


template <typename T>
vector4_t<T> frac(const vector4_t<T>& a)
{
    auto x = ::std::abs(a.x);
    auto y = ::std::abs(a.y);
    auto z = ::std::abs(a.z);
    auto w = ::std::abs(a.w);
    return vector4_t<T>::coord(x - ::std::floor(x), y - ::std::floor(y), z - ::std::floor(z), w - ::std::floor(w));
}

template <typename T>
bool isfinite(const vector4_t<T>& a)
{
    return std::isfinite(a.x) && std::isfinite(a.y) && std::isfinite(a.z) && std::isfinite(a.w);
}

template <typename T>
vector4_t<T> sign(const vector4_t<T>& a)
{
    return vector4_t<T>::coord(sign(a.x), sign(a.y), sign(a.z), sign(a.w));
}

template <typename T>
vector4_t<T> clamp(const vector4_t<T>& v, const vector4_t<T>& min, const vector4_t<T>& max)
{
    return vector4_t<T>::coord(clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y), clamp(v.z, min.z, max.z), clamp(v.w, min.w, max.w));
}

#if !defined(min)
template <typename T>
vector4_t<T> min(const vector4_t<T>& a, const vector4_t<T>& b)
{
    return vector4_t<T>::coord(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
}
#endif

#if !defined(max)
template <typename T>
vector4_t<T> max(const vector4_t<T>& a, const vector4_t<T>& b)
{
    return vector4_t<T>::coord(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
}
#endif

template <typename T>
T dot(const vector4_t<T>& a, const vector4_t<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <typename T>
T distance_sq(const vector4_t<T>& a, const vector4_t<T>& b)
{
    return sq(a.x - b.x) + sq(a.y - b.y) + sq(a.z - b.z) + sq(a.w - b.w);
}

template <typename T>
T distance(const vector4_t<T>& a, const vector4_t<T>& b)
{
    return std::sqrt(distance_sq(a, b));
}

template <typename T>
vector4_t<T> normalize(const vector4_t<T>& a)
{
    auto l = a.length();
    YAMA_ASSERT_WARN(l, "Normalizing zero-length yama::vector4_t");
    return vector4_t<T>::coord(a.x / l, a.y / l, a.z / l, a.w / l);
}

template <typename T>
bool orthogonal(const vector4_t<T>& a, const vector4_t<T>& b)
{
    return close(dot(a, b), T(0));
}

template <typename T>
bool collinear(const vector4_t<T>& a, const vector4_t<T>& b)
{
    T div[4];
    int non_zeroes = 0;

    for (int i = 0; i < 4; ++i)
    {
        if (close(a[i], 0.f))
        {
            if (close(b[i], 0.f))
            {
                continue;
            }
            else
            {
                return false;
            }
        }

        div[non_zeroes++] = a[i] / b[i];
    }

    for (int i = 1; i < non_zeroes; ++i)
    {
        if (!close(div[0], div[i]))
        {
            return false;
        }
    }

    return true;
}

// type traits
template <typename T>
struct is_yama<vector4_t<T>> : public std::true_type {};

template <typename T>
struct is_vector<vector4_t<T>> : public std::true_type {};

// casts
template <typename V4_U, typename T>
V4_U vector_cast(const vector4_t<T>& v)
{
    using U = typename V4_U::value_type;
    return {U(v.x), U(v.y), U(v.z), U(v.w)};
}

// shorthand
#if !defined(YAMA_NO_SHORTHAND)

using vector4 = vector4_t<preferred_type>;
using point4 = vector4;

constexpr vector4 v(preferred_type x, preferred_type y, preferred_type z, preferred_type w)
{
    return vector4::coord(x, y, z, w);
}

template <typename T>
constexpr vector4_t<T> vt(const T& x, const T& y, const T& z, const T& w)
{
    return vector4_t<T>::coord(x, y, z, w);
}

#endif

}
