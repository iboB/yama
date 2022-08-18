// Yama
// Copyright (c) 2016-2021 Borislav Stanimirov
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
class vector2_t;

template <typename T>
class vector4_t;

template <typename T>
class vector3_t
{
public:
    T x, y, z;

    using value_type = T;
    using size_type = size_t;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = typename std::reverse_iterator<iterator>;
    using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

    static constexpr size_type value_count = 3;

    constexpr size_type max_size() const { return value_count; }
    constexpr size_type size() const { return max_size(); }

    ///////////////////////////////////////////////////////////////////////////
    // named constructors
    static constexpr vector3_t coord(const value_type& x, const value_type& y, const value_type& z)
    {
        return {x, y, z};
    }

    static constexpr vector3_t uniform(const value_type& s)
    {
        return coord(s, s, s);
    }

    static constexpr vector3_t zero()
    {
        return uniform(value_type(0));
    }

    static vector3_t from_ptr(const value_type* ptr)
    {
        YAMA_ASSERT_CRIT(ptr, "Constructing yama::vector3_t from nullptr");
        return coord(ptr[0], ptr[1], ptr[2]);
    }

    static constexpr vector3_t unit_x()
    {
        return coord(1, 0, 0);
    }

    static constexpr vector3_t unit_y()
    {
        return coord(0, 1, 0);
    }

    static constexpr vector3_t unit_z()
    {
        return coord(0, 0, 1);
    }

    ///////////////////////////
    // attach
    static vector3_t& attach_to_ptr(value_type* ptr)
    {
        YAMA_ASSERT_BAD(ptr, "Attaching yama::vector3_t to nullptr");
        return *reinterpret_cast<vector3_t*>(ptr);
    }

    static const vector3_t& attach_to_ptr(const value_type* ptr)
    {
        YAMA_ASSERT_BAD(ptr, "Attaching yama::vector3_t to nullptr");
        return *reinterpret_cast<const vector3_t*>(ptr);
    }

    static vector3_t* attach_to_array(value_type* ptr)
    {
        YAMA_ASSERT_WARN(ptr, "Attaching yama::vector3_t to nullptr");
        return reinterpret_cast<vector3_t*>(ptr);
    }

    static const vector3_t* attach_to_array(const value_type* ptr)
    {
        YAMA_ASSERT_WARN(ptr, "Attaching yama::vector3_t to nullptr");
        return reinterpret_cast<const vector3_t*>(ptr);
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
        YAMA_ASSERT_CRIT(i < value_count, "yama::vector3_t index overflow");
        return data()[i];
    }

    constexpr const value_type& at(size_type i) const
    {
        YAMA_ASSERT_CRIT(i < value_count, "yama::vector3_t index overflow");
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
    vector3_t<S> as_vector3_t() const
    {
        return vector3_t<S>::coord(S(x), S(y), S(z));
    }


    vector2_t<T>& xy();
    const vector2_t<T>& xy() const;
    vector2_t<T> xz() const;
    vector3_t<T>& xyz() { return *this; }
    const vector3_t<T>& xyz() const { return *this; }
    vector3_t<T> zyx() const { return coord(z, y, x); }
    vector4_t<T> xyzw(const value_type& w = 0) const;

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

    constexpr const vector3_t& operator+() const
    {
        return *this;
    }

    constexpr vector3_t operator-() const
    {
        return coord(-x, -y, -z);
    }

    vector3_t& operator+=(const vector3_t& b)
    {
        x += b.x;
        y += b.y;
        z += b.z;
        return *this;
    }

    vector3_t& operator-=(const vector3_t& b)
    {
        x -= b.x;
        y -= b.y;
        z -= b.z;
        return *this;
    }

    vector3_t& operator*=(const value_type& s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    vector3_t& operator/=(const value_type& s)
    {
        YAMA_ASSERT_WARN(s != 0, "yama::vector3_t division by zero");
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    vector3_t& mul(const vector3_t& b)
    {
        x *= b.x;
        y *= b.y;
        z *= b.z;
        return *this;
    }

    vector3_t& div(const vector3_t& b)
    {
        x /= b.x;
        y /= b.y;
        z /= b.z;
        return *this;
    }

    constexpr value_type length_sq() const
    {
        return sq(x) + sq(y) + sq(z);
    }

    value_type length() const
    {
        return std::sqrt(length_sq());
    }

    constexpr value_type manhattan_length() const
    {
        return std::abs(x) + std::abs(y) + std::abs(z);
    }

    value_type normalize()
    {
        auto l = length();
        YAMA_ASSERT_WARN(l, "Normalizing zero-length yama::vector3_t");
        x /= l;
        y /= l;
        z /= l;
        return l;
    }

    bool is_normalized() const
    {
        return close(length(), value_type(1));
    }

    void homogenous_normalize()
    {
        YAMA_ASSERT_WARN(z != 0, "Homogenous normalization of yama::vector3_t with zero y");
        x /= z;
        y /= z;
        z = 1;
    }

    vector3_t reflection(const vector3_t& normal) const
    {
        YAMA_ASSERT_WARN(normal.is_normalized(), "Reflecting with a non-normalized normal yama::vector3_t");
        auto dd = 2 * dot(*this, normal);
        return coord(x - dd * normal.x, y - dd * normal.y, z - dd * normal.z);
    }

    vector3_t get_orthogonal() const
    {
        size_t non_zeros = 0;
        size_t non_zero_index = 0;

        for (size_t i = 0; i<3; ++i)
        {
            if (std::abs(at(i)) > constants_t<T>::EPSILON)
            {
                ++non_zeros;
                non_zero_index = i;
            }
        }

        YAMA_ASSERT_WARN(non_zeros, "finding an orthogonal of a zero vector3");

        if (non_zeros >= 2)
        {
            return vector3_t::coord(y * z / 2, x * z / 2, -x * y);
        }
        else
        {
            auto ret = vector3_t::zero();

            ret.at((non_zero_index + 1) % 3) = this->at(non_zero_index);
            return ret;
        }
    }

    constexpr value_type product() const
    {
        return x * y * z;
    }

    constexpr value_type sum() const
    {
        return x + y + z;
    }
};

template <typename T>
vector3_t<T> operator+(const vector3_t<T>& a, const vector3_t<T>& b)
{
    return vector3_t<T>::coord(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T>
vector3_t<T> operator-(const vector3_t<T>& a, const vector3_t<T>& b)
{
    return vector3_t<T>::coord(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename T>
vector3_t<T> operator*(const vector3_t<T>& a, const T& s)
{
    return vector3_t<T>::coord(a.x * s, a.y * s, a.z * s);
}

template <typename T>
vector3_t<T> operator*(const T& s, const vector3_t<T>& b)
{
    return vector3_t<T>::coord(s * b.x, s * b.y, s * b.z);
}

template <typename T>
vector3_t<T> operator/(const vector3_t<T>& a, const T& s)
{
    YAMA_ASSERT_WARN(s != 0, "yama::vector3_t division by zero");
    return vector3_t<T>::coord(a.x / s, a.y / s, a.z / s);
}

template <typename T>
vector3_t<T> operator/(const T& s, const vector3_t<T>& b)
{
    return vector3_t<T>::coord(s / b.x, s / b.y, s / b.z);
}

template <typename T>
bool operator==(const vector3_t<T>& a, const vector3_t<T>& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <typename T>
bool operator!=(const vector3_t<T>& a, const vector3_t<T>& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

template <typename T>
bool close(const vector3_t<T>& a, const vector3_t<T>& b, const T& epsilon = constants_t<T>::EPSILON)
{
    return close(a.x, b.x, epsilon) && close(a.y, b.y, epsilon) && close(a.z, b.z, epsilon);
}

template <typename T>
vector3_t<T> abs(const vector3_t<T>& a)
{
    return vector3_t<T>::coord(std::abs(a.x), std::abs(a.y), std::abs(a.z));
}

template <typename T>
vector3_t<T> mul(const vector3_t<T>& a, const vector3_t<T>& b)
{
    return vector3_t<T>::coord(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <typename T>
vector3_t<T> div(const vector3_t<T>& a, const vector3_t<T>& b)
{
    YAMA_ASSERT_WARN(b.x != 0, "yama::vector3_t division by zero");
    YAMA_ASSERT_WARN(b.y != 0, "yama::vector3_t division by zero");
    YAMA_ASSERT_WARN(b.z != 0, "yama::vector3_t division by zero");
    return vector3_t<T>::coord(a.x / b.x, a.y / b.y, a.z / b.z);
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value,
    vector3_t<T>>::type mod(const vector3_t<T>& n, const vector3_t<T>& d)
{
    YAMA_ASSERT_WARN(d.x != 0, "yama::vector3_t division by zero");
    YAMA_ASSERT_WARN(d.y != 0, "yama::vector3_t division by zero");
    YAMA_ASSERT_WARN(d.z != 0, "yama::vector3_t division by zero");
    return vector3_t<T>::coord(fmod(n.x, d.x), fmod(n.y, d.y), fmod(n.z, d.z));
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value,
    vector3_t<T>>::type mod(const vector3_t<T>& n, const vector3_t<T>& d)
{
    YAMA_ASSERT_WARN(d.x != 0, "yama::vector3_t division by zero");
    YAMA_ASSERT_WARN(d.y != 0, "yama::vector3_t division by zero");
    YAMA_ASSERT_WARN(d.z != 0, "yama::vector3_t division by zero");
    return vector3_t<T>::coord(n.x % d.x, n.y % d.y, n.z % d.z);
}

template <typename T>
vector3_t<T> floor(const vector3_t<T>& a)
{
    return vector3_t<T>::coord(::std::floor(a.x), ::std::floor(a.y), ::std::floor(a.z));
}

template <typename T>
vector3_t<T> ceil(const vector3_t<T>& a)
{
    return vector3_t<T>::coord(::std::ceil(a.x), ::std::ceil(a.y), ::std::ceil(a.z));
}

template <typename T>
vector3_t<T> round(const vector3_t<T>& a)
{
    return vector3_t<T>::coord(::std::round(a.x), ::std::round(a.y), ::std::round(a.z));
}


template <typename T>
vector3_t<T> frac(const vector3_t<T>& a)
{
    auto x = ::std::abs(a.x);
    auto y = ::std::abs(a.y);
    auto z = ::std::abs(a.z);
    return vector3_t<T>::coord(x - ::std::floor(x), y - ::std::floor(y), z - ::std::floor(z));
}

template <typename T>
bool isfinite(const vector3_t<T>& a)
{
    return std::isfinite(a.x) && std::isfinite(a.y) && std::isfinite(a.z);
}

template <typename T>
vector3_t<T> sign(const vector3_t<T>& a)
{
    return vector3_t<T>::coord(sign(a.x), sign(a.y), sign(a.z));
}

template <typename T>
vector3_t<T> clamp(const vector3_t<T>& v, const vector3_t<T>& min, const vector3_t<T>& max)
{
    return vector3_t<T>::coord(clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y), clamp(v.z, min.z, max.z));
}

#if !defined(min)
template <typename T>
vector3_t<T> min(const vector3_t<T>& a, const vector3_t<T>& b)
{
    return vector3_t<T>::coord(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}
#endif

#if !defined(max)
template <typename T>
vector3_t<T> max(const vector3_t<T>& a, const vector3_t<T>& b)
{
    return vector3_t<T>::coord(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}
#endif

template <typename T>
T dot(const vector3_t<T>& a, const vector3_t<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
vector3_t<T> cross(const vector3_t<T>& a, const vector3_t<T>& b)
{
    YAMA_ASSERT_WARN(!close(a, vector3_t<T>::zero()), "Cross product with a zero vector3_t");
    YAMA_ASSERT_WARN(!close(b, vector3_t<T>::zero()), "Cross product with a zero vector3_t");
    return vector3_t<T>::coord(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

template <typename T>
T distance_sq(const vector3_t<T>& a, const vector3_t<T>& b)
{
    return sq(a.x - b.x) + sq(a.y - b.y) + sq(a.z - b.z);
}

template <typename T>
T distance(const vector3_t<T>& a, const vector3_t<T>& b)
{
    return std::sqrt(distance_sq(a, b));
}

template <typename T>
vector3_t<T> normalize(const vector3_t<T>& a)
{
    auto l = a.length();
    YAMA_ASSERT_WARN(l, "Normalizing zero-length yama::vector3_t");
    return vector3_t<T>::coord(a.x / l, a.y / l, a.z / l);
}

template <typename T>
bool orthogonal(const vector3_t<T>& a, const vector3_t<T>& b)
{
    return close(dot(a, b), T(0));
}

template <typename T>
bool collinear(const vector3_t<T>& a, const vector3_t<T>& b)
{
    T div[3];
    int non_zeroes = 0;

    for (int i = 0; i < 3; ++i)
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
struct is_yama<vector3_t<T>> : public std::true_type {};

template <typename T>
struct is_vector<vector3_t<T>> : public std::true_type {};

// casts
template <typename V3_U, typename T>
V3_U vector_cast(const vector3_t<T>& v)
{
    using U = typename V3_U::value_type;
    return {U(v.x), U(v.y), U(v.z)};
}

// shorthand
#if !defined(YAMA_NO_SHORTHAND)

using vector3 = vector3_t<preferred_type>;
using point3 = vector3;

constexpr vector3 v(preferred_type x, preferred_type y, preferred_type z)
{
    return vector3::coord(x, y, z);
}

template <typename T>
constexpr vector3_t<T> vt(const T& x, const T& y, const T& z)
{
    return vector3_t<T>::coord(x, y, z);
}

#endif

}
