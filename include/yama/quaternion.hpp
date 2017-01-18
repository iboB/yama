// Yama
// Copyright (c) 2016 Borislav Stanimirov, Zahary Karadjov
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

#include "vector3.hpp"
#include "vector4.hpp"

namespace yama
{

template <typename T>
class quaternion_t
{
public:
    T x, y, z, w;

    typedef T value_type;
    typedef size_t size_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef typename std::reverse_iterator<iterator> reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;

    static constexpr size_type value_count = 4;

    constexpr size_type max_size() const { return value_count; }
    constexpr size_type size() const { return max_size(); }

    ///////////////////////////////////////////////////////////////////////////
    // named constructors
    static constexpr quaternion_t xyzw(const value_type& x, const value_type& y, const value_type& z, const value_type& w)
    {
        return{ x, y, z, w};
    }

    static constexpr quaternion_t identity()
    {
        return xyzw(0, 0, 0, 1);
    }

    static constexpr quaternion_t uniform(const value_type& s)
    {
        return xyzw(s, s, s, s);
    }

    static constexpr quaternion_t zero()
    {
        return uniform(value_type(0));
    }

    static quaternion_t from_ptr(const value_type* ptr)
    {
        YAMA_ASSERT_CRIT(ptr, "Constructing yama::quaternion_t from nullptr");
        return xyzw(ptr[0], ptr[1], ptr[2], ptr[3]);
    }

    static constexpr quaternion_t from_vector4(const vector4_t<value_type>& v)
    {
        return xyzw(v.x, v.y, v.z, v.w);
    }

    // for when you're sure that the axis is normalized
    static quaternion_t rotation_naxis(const vector3_t<value_type>& axis, value_type radians)
    {
        YAMA_ASSERT_BAD(axis.is_normalized(), "rotation axis should be normalized");
        const value_type s = std::sin(radians / 2);
        return xyzw(
            axis.x * s,
            axis.y * s,
            axis.z * s,
            std::cos(radians / 2)
        );
    }

    static quaternion_t rotation_axis(const vector3_t<value_type>& axis, value_type radians)
    {
        auto naxis = yama::normalize(axis);
        return rotation_naxis(naxis, radians);
    }

    static quaternion_t rotation_x(value_type radians)
    {
        return xyzw(
            std::sin(radians / 2),
            0,
            0,
            std::cos(radians / 2)
        );
    }

    static quaternion_t rotation_y(value_type radians)
    {
        return xyzw(
            0,
            std::sin(radians / 2),
            0,
            std::cos(radians / 2)
        );
    }

    static quaternion_t rotation_z(value_type radians)
    {
        return xyzw(
            0,
            0,
            std::sin(radians / 2),
            std::cos(radians / 2)
        );
    }

    static quaternion_t rotation_vectors(const vector3_t<value_type>& src, const vector3_t<value_type>& target)
    {
        YAMA_ASSERT_BAD(src.is_normalized(), "source vector should be normalized");
        YAMA_ASSERT_BAD(target.is_normalized(), "target vector should be normalized");
        YAMA_ASSERT_WARN(!close(src, vector3_t<value_type>::zero()), "source vector shouldn't be zero");
        YAMA_ASSERT_WARN(!close(target, vector3_t<value_type>::zero()), "target vector shouldn't be zero");

        auto axis = cross(src, target);
        auto axis_length = axis.normalize();

        if (axis_length > constants_t<value_type>::EPSILON) // not collinear
        {
            auto half_cos = dot(src, target);
            auto cos_half = std::sqrt(value_type(0.5) + half_cos);
            auto sin_half = std::sqrt(value_type(0.5) - half_cos);
            return xyzw(axis.x * sin_half, axis.y * sin_half, axis.z * sin_half, cos_half);
        }
        else
        {
            // collinear
            return identity();
        }
    }

    ///////////////////////////
    // attach
    static quaternion_t& attach_to_ptr(value_type* ptr)
    {
        YAMA_ASSERT_BAD(ptr, "Attaching yama::quaternion_t to nullptr");
        return *reinterpret_cast<quaternion_t*>(ptr);
    }

    static const quaternion_t& attach_to_ptr(const value_type* ptr)
    {
        YAMA_ASSERT_BAD(ptr, "Attaching yama::quaternion_t to nullptr");
        return *reinterpret_cast<const quaternion_t*>(ptr);
    }

    static quaternion_t* attach_to_array(value_type* ptr)
    {
        YAMA_ASSERT_WARN(ptr, "Attaching yama::quaternion_t to nullptr");
        return reinterpret_cast<quaternion_t*>(ptr);
    }

    static const quaternion_t* attach_to_array(const value_type* ptr)
    {
        YAMA_ASSERT_WARN(ptr, "Attaching yama::quaternion_t to nullptr");
        return reinterpret_cast<const quaternion_t*>(ptr);
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
        YAMA_ASSERT_CRIT(i < value_count, "yama::quaternion_t index overflow");
        return data()[i];
    }

    constexpr const value_type& at(size_type i) const
    {
        YAMA_ASSERT_CRIT(i < value_count, "yama::quaternion_t index overflow");
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
    quaternion_t<S> as_quaternion_t() const
    {
        return quaternion_t<S>::xyzw(S(x), S(y), S(z), S(w));
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

    constexpr const quaternion_t& operator+() const
    {
        return *this;
    }

    constexpr quaternion_t operator-() const
    {
        return xyzw(-x, -y, -z, -w);
    }

    quaternion_t& operator+=(const quaternion_t& b)
    {
        x += b.x;
        y += b.y;
        z += b.z;
        w += b.w;
        return *this;
    }

    quaternion_t& operator-=(const quaternion_t& b)
    {
        x -= b.x;
        y -= b.y;
        z -= b.z;
        w -= b.w;
        return *this;
    }

    quaternion_t& operator*=(const value_type& s)
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }

    quaternion_t& operator/=(const value_type& s)
    {
        YAMA_ASSERT_WARN(s != 0, "yama::quaternion_t division by zero");
        x /= s;
        y /= s;
        z /= s;
        w /= s;
        return *this;
    }

    quaternion_t& operator*=(const quaternion_t& b)
    {
        auto rx = w*b.x + x*b.w + y*b.z - z*b.y;
        auto ry = w*b.y - x*b.z + y*b.w + z*b.x;
        auto rz = w*b.z + x*b.y - y*b.x + z*b.w;
        auto rw = w*b.w - x*b.x - y*b.y - z*b.z;
        x = rx;
        y = ry;
        z = rz;
        w = rw;
        return *this;
    }

    quaternion_t& operator/=(const quaternion_t& b)
    {
        auto ls = b.length_sq();
        YAMA_ASSERT_WARN(!close(ls, T(0)), "Dividing by a zero-length yama::quaternion_t");
        auto rx = (-w*b.x + x*b.w - y*b.z + z*b.y) / ls;
        auto ry = (-w*b.y + x*b.z + y*b.w - z*b.x) / ls;
        auto rz = (-w*b.z - x*b.y + y*b.x + z*b.w) / ls;
        auto rw = ( w*b.w + x*b.x + y*b.y + z*b.z) / ls;
        x = rx;
        y = ry;
        z = rz;
        w = rw;
        return *this;
    }

    quaternion_t& mul(const quaternion_t& b)
    {
        x *= b.x;
        y *= b.y;
        z *= b.z;
        w *= b.w;
        return *this;
    }

    quaternion_t& div(const quaternion_t& b)
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

    quaternion_t& conjugate()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    quaternion_t& inverse()
    {
        YAMA_ASSERT_WARN(!close(length_sq(), value_type(0)), "Invering a zero-length yama::quaternion_t");
        auto ls = length_sq();
        x /= -ls;
        y /= -ls;
        z /= -ls;
        w /= ls;
        return *this;
    }

    value_type normalize()
    {
        auto l = length();
        YAMA_ASSERT_WARN(l, "Normalizing zero-length yama::quaternion_t");
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

    void to_axis_angle(vector3_t<value_type>& out_axis, value_type& out_angle) const
    {
        out_angle = 2 * std::acos(w);

        value_type scale = value_type(1) - sq(w);

        if (scale < constants_t<value_type>::EPSILON)
        {
            out_axis = vector3_t<value_type>::coord(0, 0, 1);
        }
        else
        {
            scale = 1 / std::sqrt(scale);
            out_axis = vector3_t<value_type>::coord(x*scale, y*scale, z*scale);
        }
    }
};

template <typename T>
quaternion_t<T> operator+(const quaternion_t<T>& a, const quaternion_t<T>& b)
{
    return quaternion_t<T>::xyzw(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <typename T>
quaternion_t<T> operator-(const quaternion_t<T>& a, const quaternion_t<T>& b)
{
    return quaternion_t<T>::xyzw(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <typename T>
quaternion_t<T> operator*(const quaternion_t<T>& a, const T& s)
{
    return quaternion_t<T>::xyzw(a.x * s, a.y * s, a.z * s, a.w * s);
}

template <typename T>
quaternion_t<T> operator*(const T& s, const quaternion_t<T>& b)
{
    return quaternion_t<T>::xyzw(s * b.x, s * b.y, s * b.z, s * b.w);
}

template <typename T>
quaternion_t<T> operator/(const quaternion_t<T>& a, const T& s)
{
    YAMA_ASSERT_WARN(s != 0, "yama::quaternion_t division by zero");
    return quaternion_t<T>::xyzw(a.x / s, a.y / s, a.z / s, a.w / s);
}

template <typename T>
quaternion_t<T> operator/(const T& s, const quaternion_t<T>& b)
{
    return quaternion_t<T>::xyzw(s / b.x, s / b.y, s / b.z, s / b.w);
}

template <typename T>
quaternion_t<T> operator*(const quaternion_t<T>& a, const quaternion_t<T>& b)
{
    return quaternion_t<T>::xyzw(
        a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
        a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
        a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w,
        a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z
    );
}

template <typename T>
quaternion_t<T> operator/(const quaternion_t<T>& a, const quaternion_t<T>& b)
{
    auto ls = b.length_sq();
    YAMA_ASSERT_WARN(!close(ls, T(0)), "Dividing by a zero-length yama::quaternion_t");
    return quaternion_t<T>::xyzw(
        (-a.w*b.x + a.x*b.w - a.y*b.z + a.z*b.y) / ls,
        (-a.w*b.y + a.x*b.z + a.y*b.w - a.z*b.x) / ls,
        (-a.w*b.z - a.x*b.y + a.y*b.x + a.z*b.w) / ls,
        ( a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z) / ls
    );
}

template <typename T>
bool operator==(const quaternion_t<T>& a, const quaternion_t<T>& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template <typename T>
bool operator!=(const quaternion_t<T>& a, const quaternion_t<T>& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

template <typename T>
bool close(const quaternion_t<T>& a, const quaternion_t<T>& b, const T& epsilon = constants_t<T>::EPSILON)
{
    return close(a.x, b.x, epsilon) && close(a.y, b.y, epsilon) && close(a.z, b.z, epsilon) && close(a.w, b.w, epsilon);
}

template <typename T>
quaternion_t<T> abs(const quaternion_t<T>& a)
{
    return quaternion_t<T>::xyzw(std::abs(a.x), std::abs(a.y), std::abs(a.z), std::abs(a.w));
}

template <typename T>
quaternion_t<T> mul(const quaternion_t<T>& a, const quaternion_t<T>& b)
{
    return quaternion_t<T>::xyzw(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template <typename T>
quaternion_t<T> div(const quaternion_t<T>& a, const quaternion_t<T>& b)
{
    YAMA_ASSERT_WARN(b.x != 0, "yama::quaternion_t division by zero");
    YAMA_ASSERT_WARN(b.y != 0, "yama::quaternion_t division by zero");
    YAMA_ASSERT_WARN(b.z != 0, "yama::quaternion_t division by zero");
    YAMA_ASSERT_WARN(b.w != 0, "yama::quaternion_t division by zero");
    return quaternion_t<T>::xyzw(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value,
    quaternion_t<T>>::type mod(const quaternion_t<T>& n, const quaternion_t<T>& d)
{
    YAMA_ASSERT_WARN(d.x != 0, "yama::quaternion_t division by zero");
    YAMA_ASSERT_WARN(d.y != 0, "yama::quaternion_t division by zero");
    YAMA_ASSERT_WARN(d.z != 0, "yama::quaternion_t division by zero");
    YAMA_ASSERT_WARN(d.w != 0, "yama::quaternion_t division by zero");
    return quaternion_t<T>::xyzw(fmod(n.x, d.x), fmod(n.y, d.y), fmod(n.z, d.z), fmod(n.w, d.w));
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value,
    quaternion_t<T>>::type mod(const quaternion_t<T>& n, const quaternion_t<T>& d)
{
    YAMA_ASSERT_WARN(d.x != 0, "yama::quaternion_t division by zero");
    YAMA_ASSERT_WARN(d.y != 0, "yama::quaternion_t division by zero");
    YAMA_ASSERT_WARN(d.z != 0, "yama::quaternion_t division by zero");
    YAMA_ASSERT_WARN(d.w != 0, "yama::quaternion_t division by zero");

    return quaternion_t<T>::xyzw(n.x % d.x, n.y % d.y, n.z % d.z, n.w % d.w);
}

template <typename T>
quaternion_t<T> floor(const quaternion_t<T>& a)
{
    return quaternion_t<T>::xyzw(::std::floor(a.x), ::std::floor(a.y), ::std::floor(a.z), ::std::floor(a.w));
}

template <typename T>
quaternion_t<T> ceil(const quaternion_t<T>& a)
{
    return quaternion_t<T>::xyzw(::std::ceil(a.x), ::std::ceil(a.y), ::std::ceil(a.z), ::std::ceil(a.w));
}

template <typename T>
quaternion_t<T> round(const quaternion_t<T>& a)
{
    return quaternion_t<T>::xyzw(::std::round(a.x), ::std::round(a.y), ::std::round(a.z), ::std::round(a.w));
}


template <typename T>
quaternion_t<T> frac(const quaternion_t<T>& a)
{
    auto x = ::std::abs(a.x);
    auto y = ::std::abs(a.y);
    auto z = ::std::abs(a.z);
    auto w = ::std::abs(a.w);
    return quaternion_t<T>::xyzw(x - ::std::floor(x), y - ::std::floor(y), z - ::std::floor(z), w - ::std::floor(w));
}

template <typename T>
bool isfinite(const quaternion_t<T>& a)
{
    return std::isfinite(a.x) && std::isfinite(a.y) && std::isfinite(a.z) && std::isfinite(a.w);
}

template <typename T>
quaternion_t<T> sign(const quaternion_t<T>& a)
{
    return quaternion_t<T>::xyzw(sign(a.x), sign(a.y), sign(a.z), sign(a.w));
}

template <typename T>
quaternion_t<T> clamp(const quaternion_t<T>& v, const quaternion_t<T>& min, const quaternion_t<T>& max)
{
    return quaternion_t<T>::xyzw(clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y), clamp(v.z, min.z, max.z), clamp(v.w, min.w, max.w));
}

#if !defined(min)
template <typename T>
quaternion_t<T> min(const quaternion_t<T>& a, const quaternion_t<T>& b)
{
    return quaternion_t<T>::xyzw(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
}
#endif

#if !defined(max)
template <typename T>
quaternion_t<T> max(const quaternion_t<T>& a, const quaternion_t<T>& b)
{
    return quaternion_t<T>::xyzw(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
}
#endif

template <typename T>
T dot(const quaternion_t<T>& a, const quaternion_t<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <typename T>
quaternion_t<T> normalize(const quaternion_t<T>& a)
{
    auto l = a.length();
    YAMA_ASSERT_WARN(l, "Normalizing zero-length yama::quaternion_t");
    return quaternion_t<T>::xyzw(a.x / l, a.y / l, a.z / l, a.w / l);
}

template <typename T>
quaternion_t<T> lerp(const quaternion_t<T>& from, const quaternion_t<T>& to, const T& ratio)
{
    YAMA_ASSERT_WARN(ratio >= 0, "yama::quaternion_t lerp is defined between 0 and 1 ");
    YAMA_ASSERT_WARN(ratio <= 1, "yama::quaternion_t lerp is defined between 0 and 1 ");
    auto q = from + ratio * (to - from);
    return normalize(q);
}

template <typename T>
quaternion_t<T> slerp(const quaternion_t<T>& from, const quaternion_t<T>& to, T ratio)
{
    T cos_angle = dot(from, to);

    // if the angle is small (ie cos_angle is close to -1 or 1),
    // could use lerp, instead of slerp:
    // if(cos_angle < -0.99 || cos_angle > 0.99) return lerp(from, to, ratio)
    // need to test how this works

    T angle = std::acos(cos_angle);

    return (from*std::sin((1 - ratio)*angle) + to*std::sin(angle*ratio)) / std::sin(angle);
}

template <typename T>
quaternion_t<T> conjugate(const quaternion_t<T>& a)
{
    return quaternion_t<T>::xyzw(-a.x, -a.y, -a.z, a.w);
}

template <typename T>
quaternion_t<T> inverse(const quaternion_t<T>& a)
{
    YAMA_ASSERT_WARN(!close(a.length_sq(), T(0)), "Invering a zero-length yama::quaternion_t");
    auto ls = a.length_sq();
    return quaternion_t<T>::xyzw(-a.x/ls, -a.y/ls, -a.z/ls, a.w/ls);
}

template <typename T>
vector3_t<T> rotate(const vector3_t<T>& v, const quaternion_t<T>& q)
{
    // c1 = q x v
    auto c1 = vector3_t<T>::coord(
        q.y*v.z - q.z*v.y,
        q.z*v.x - q.x*v.z,
        q.x*v.y - q.y*v.x
    );
    // c2 = q x (q x v)
    auto c2 = vector3_t<T>::coord(
       -sq(q.y)*v.x - sq(q.z)*v.x + q.x*q.y*v.y + q.x*q.z*v.z,
        q.x*q.y*v.x - sq(q.x)*v.y - sq(q.z)*v.y + q.y*q.z*v.z,
        q.x*q.z*v.x + q.y*q.z*v.y - sq(q.x)*v.z - sq(q.y)*v.z
    );

    return v + ((c1 * q.w) + c2) * T(2);
}

// type traits
template <typename T>
struct is_yama<quaternion_t<T>> : public std::true_type {};

// shorthand
#if !defined(YAMA_NO_SHORTHAND)

typedef quaternion_t<preferred_type> quaternion;

#endif

}
