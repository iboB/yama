// Yama
// Copyright (c) 2016-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
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
class vector3_t;

template <typename T>
class vector4_t;

template <typename T>
class vector2_t
{
public:
    T x, y;

    typedef T value_type;
    typedef size_t size_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef typename std::reverse_iterator<iterator> reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;

    static constexpr size_type value_count = 2;

    constexpr size_type max_size() const { return value_count; }
    constexpr size_type size() const { return max_size(); }

    ///////////////////////////////////////////////////////////////////////////
    // named constructors
    static constexpr vector2_t coord(const value_type& x, const value_type& y)
    {
        return{ x, y };
    }

    static constexpr vector2_t uniform(const value_type& s)
    {
        return coord(s, s);
    }

    static constexpr vector2_t zero()
    {
        return uniform(value_type(0));
    }

    static vector2_t from_ptr(const value_type* ptr)
    {
        YAMA_ASSERT_CRIT(ptr, "Constructing yama::vector2_t from nullptr");
        return coord(ptr[0], ptr[1]);
    }

    static constexpr vector2_t unit_x()
    {
        return coord(1, 0);
    }

    static constexpr vector2_t unit_y()
    {
        return coord(0, 1);
    }

    ///////////////////////////
    // attach
    static vector2_t& attach_to_ptr(value_type* ptr)
    {
        YAMA_ASSERT_BAD(ptr, "Attaching yama::vector2_t to nullptr");
        return *reinterpret_cast<vector2_t*>(ptr);
    }

    static const vector2_t& attach_to_ptr(const value_type* ptr)
    {
        YAMA_ASSERT_BAD(ptr, "Attaching yama::vector2_t to nullptr");
        return *reinterpret_cast<const vector2_t*>(ptr);
    }

    static vector2_t* attach_to_array(value_type* ptr)
    {
        YAMA_ASSERT_WARN(ptr, "Attaching yama::vector2_t to nullptr");
        return reinterpret_cast<vector2_t*>(ptr);
    }

    static const vector2_t* attach_to_array(const value_type* ptr)
    {
        YAMA_ASSERT_WARN(ptr, "Attaching yama::vector2_t to nullptr");
        return reinterpret_cast<const vector2_t*>(ptr);
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
        YAMA_ASSERT_CRIT(i < value_count, "yama::vector2_t index overflow");
        return data()[i];
    }

    constexpr const value_type& at(size_type i) const
    {
        YAMA_ASSERT_CRIT(i < value_count, "yama::vector2_t index overflow");
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
    vector2_t<S> as_vector2_t() const
    {
        return vector2_t<S>::coord(S(x), S(y));
    }


    vector2_t<T>& xy() { return *this; }
    const vector2_t<T>& xy() const { return *this; }
    vector2_t<T> yx() const { coord(y, x); }
    vector3_t<T> xyz(const value_type& z = 0) const;
    vector4_t<T> xyzw(const value_type& z = 0, const value_type& w = 0) const;

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

    constexpr const vector2_t& operator+() const
    {
        return *this;
    }

    constexpr vector2_t operator-() const
    {
        return coord(-x, -y);
    }

    vector2_t& operator+=(const vector2_t& b)
    {
        x += b.x;
        y += b.y;
        return *this;
    }

    vector2_t& operator-=(const vector2_t& b)
    {
        x -= b.x;
        y -= b.y;
        return *this;
    }

    vector2_t& operator*=(const value_type& s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    vector2_t& operator/=(const value_type& s)
    {
        YAMA_ASSERT_WARN(s != 0, "yama::vector2_t division by zero");
        x /= s;
        y /= s;
        return *this;
    }

    vector2_t& mul(const vector2_t& b)
    {
        x *= b.x;
        y *= b.y;
        return *this;
    }

    vector2_t& div(const vector2_t& b)
    {
        x /= b.x;
        y /= b.y;
        return *this;
    }

    constexpr value_type length_sq() const
    {
        return sq(x) + sq(y);
    }

    value_type length() const
    {
        return std::sqrt(length_sq());
    }

    constexpr value_type manhattan_length() const
    {
        return std::abs(x) + std::abs(y);
    }

    value_type normalize()
    {
        auto l = length();
        YAMA_ASSERT_WARN(l, "Normalizing zero-length yama::vector2_t");
        x /= l;
        y /= l;
        return l;
    }

    bool is_normalized() const
    {
        return close(length(), value_type(1));
    }

    void homogenous_normalize()
    {
        YAMA_ASSERT_WARN(y != 0, "Homogenous normalization of yama::vector2_t with zero y");
        x /= y;
        y = 1;
    }

    vector2_t reflection(const vector2_t& normal) const
    {
        YAMA_ASSERT_WARN(normal.is_normalized(), "Reflecting with a non-normalized normal yama::vector2_t");
        auto dd = 2 * dot(*this, normal);
        return coord(x - dd * normal.x, y - dd * normal.y);
    }

    vector2_t get_orthogonal() const
    {
        YAMA_ASSERT_WARN(!close(*this, zero()), "finding an orthogonal of a zero vector2_t");
        return coord(-y, x);
    }

    value_type product() const
    {
        return x * y;
    }
};

template <typename T>
vector2_t<T> operator+(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return vector2_t<T>::coord(a.x + b.x, a.y + b.y);
}

template <typename T>
vector2_t<T> operator-(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return vector2_t<T>::coord(a.x - b.x, a.y - b.y);
}

template <typename T>
vector2_t<T> operator*(const vector2_t<T>& a, const T& s)
{
    return vector2_t<T>::coord(a.x * s, a.y * s);
}

template <typename T>
vector2_t<T> operator*(const T& s, const vector2_t<T>& b)
{
    return vector2_t<T>::coord(s * b.x, s * b.y);
}

template <typename T>
vector2_t<T> operator/(const vector2_t<T>& a, const T& s)
{
    YAMA_ASSERT_WARN(s != 0, "yama::vector2_t division by zero");
    return vector2_t<T>::coord(a.x / s, a.y / s);
}

template <typename T>
vector2_t<T> operator/(const T& s, const vector2_t<T>& b)
{
    return vector2_t<T>::coord(s / b.x, s / b.y);
}

template <typename T>
bool operator==(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return a.x == b.x && a.y == b.y;
}

template <typename T>
bool operator!=(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return a.x != b.x || a.y != b.y;
}

template <typename T>
bool close(const vector2_t<T>& a, const vector2_t<T>& b, const T& epsilon = constants_t<T>::EPSILON())
{
    return close(a.x, b.x, epsilon) && close(a.y, b.y, epsilon);
}

template <typename T>
vector2_t<T> abs(const vector2_t<T>& a)
{
    return vector2_t<T>::coord(std::abs(a.x), std::abs(a.y));
}

template <typename T>
vector2_t<T> mul(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return vector2_t<T>::coord(a.x * b.x, a.y * b.y);
}

template <typename T>
vector2_t<T> div(const vector2_t<T>& a, const vector2_t<T>& b)
{
    YAMA_ASSERT_WARN(b.x != 0, "yama::vector2_t division by zero");
    YAMA_ASSERT_WARN(b.y != 0, "yama::vector2_t division by zero");
    return vector2_t<T>::coord(a.x / b.x, a.y / b.y);
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value,
    vector2_t<T>>::type mod(const vector2_t<T>& n, const vector2_t<T>& d)
{
    YAMA_ASSERT_WARN(d.x != 0, "yama::vector2_t division by zero");
    YAMA_ASSERT_WARN(d.y != 0, "yama::vector2_t division by zero");
    return vector2_t<T>::coord(fmod(n.x, d.x), fmod(n.y, d.y));
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value,
    vector2_t<T>>::type mod(const vector2_t<T>& n, const vector2_t<T>& d)
{
    YAMA_ASSERT_WARN(d.x != 0, "yama::vector2_t division by zero");
    YAMA_ASSERT_WARN(d.y != 0, "yama::vector2_t division by zero");
    return vector2_t<T>::coord(n.x % d.x, n.y % d.y);
}

template <typename T>
vector2_t<T> floor(const vector2_t<T>& a)
{
    return vector2_t<T>::coord(::std::floor(a.x), ::std::floor(a.y));
}

template <typename T>
vector2_t<T> ceil(const vector2_t<T>& a)
{
    return vector2_t<T>::coord(::std::ceil(a.x), ::std::ceil(a.y));
}

template <typename T>
vector2_t<T> round(const vector2_t<T>& a)
{
    return vector2_t<T>::coord(::std::round(a.x), ::std::round(a.y));
}


template <typename T>
vector2_t<T> frac(const vector2_t<T>& a)
{
    auto x = ::std::abs(a.x);
    auto y = ::std::abs(a.y);
    return vector2_t<T>::coord(x - ::std::floor(x), y - ::std::floor(y));
}

template <typename T>
bool isfinite(const vector2_t<T>& a)
{
    return std::isfinite(a.x) && std::isfinite(a.y);
}

template <typename T>
vector2_t<T> sign(const vector2_t<T>& a)
{
    return vector2_t<T>::coord(sign(a.x), sign(a.y));
}

template <typename T>
vector2_t<T> clamp(const vector2_t<T>& v, const vector2_t<T>& min, const vector2_t<T>& max)
{
    return vector2_t<T>::coord(clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y));
}

#if !defined(min)
template <typename T>
vector2_t<T> min(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return vector2_t<T>::coord(std::min(a.x, b.x), std::min(a.y, b.y));
}
#endif

#if !defined(max)
template <typename T>
vector2_t<T> max(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return vector2_t<T>::coord(std::max(a.x, b.x), std::max(a.y, b.y));
}
#endif

template <typename T>
T dot(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return a.x * b.x + a.y * b.y;
}

template <typename T>
T cross_magnitude(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return a.x * b.y - a.y * b.x;
}

template <typename T>
T distance_sq(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return sq(a.x - b.x) + sq(a.y - b.y);
}

template <typename T>
T distance(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return std::sqrt(distance_sq(a, b));
}

template <typename T>
vector2_t<T> normalize(const vector2_t<T>& a)
{
    auto l = a.length();
    YAMA_ASSERT_WARN(l, "Normalizing zero-length yama::vector2_t");
    return vector2_t<T>::coord(a.x / l, a.y / l);
}

template <typename T>
bool orthogonal(const vector2_t<T>& a, const vector2_t<T>& b)
{
    return close(dot(a, b), T(0));
}

template <typename T>
bool collinear(const vector2_t<T>& a, const vector2_t<T>& b)
{
    if (close(b.x, T(0)))
    {
        if (!close(a.x, T(0)))
        {
            return false;
        }

        if (close(b.y, T(0)))
        {
            return false;
        }

        return true;
    }
    else if (close(b.y, T(0)))
    {
        if (close(a.x, T(0)))
        {
            return false;
        }

        return close(a.y, T(0));
    }
    else
    {
        return close(a.x / b.x, a.y / b.y);
    }
}

// type traits
template <typename T>
struct is_yama<vector2_t<T>> : public std::true_type {};

template <typename T>
struct is_vector<vector2_t<T>> : public std::true_type {};

// casts
template <typename V2_U, typename T>
V2_U vector_cast(const vector2_t<T>& v)
{
    using U = typename V2_U::value_type;
    return {U(v.x), U(v.y)};
}

// shorthand
#if !defined(YAMA_NO_SHORTHAND)

typedef vector2_t<preferred_type> vector2;
typedef vector2 point2;

constexpr vector2 v(preferred_type x, preferred_type y)
{
    return vector2::coord(x, y);
}

template <typename T>
constexpr vector2_t<T> vt(const T& x, const T& y)
{
    return vector2_t<T>::coord(x, y);
}

#endif

}
