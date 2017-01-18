#pragma once

#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

namespace yama
{

template <typename T>
vector3_t<T> vector2_t<T>::xyz(const T& z) const
{
    return vector3_t<T>::coord(x, y, z);
}

template <typename T>
vector4_t<T> vector2_t<T>::xyzw(const T& z, const T& w) const
{
    return vector4_t<T>::coord(x, y, z, w);
}

template <typename T>
vector2_t<T>& vector3_t<T>::xy()
{
    return vector2_t<T>::attach_to_ptr(as_ptr());
}

template <typename T>
const vector2_t<T>& vector3_t<T>::xy() const
{
    return vector2_t<T>::attach_to_ptr(as_ptr());
}

template <typename T>
vector2_t<T> vector3_t<T>::xz() const
{
    return vector2_t<T>::coord(x, z);
}

template <typename T>
vector4_t<T> vector3_t<T>::xyzw(const value_type& w) const
{
    return vector4_t<T>::coord(x, y, z, w);
}

template <typename T>
vector2_t<T>& vector4_t<T>::xy()
{
    return vector2_t<T>::attach_to_ptr(as_ptr());
}

template <typename T>
const vector2_t<T>& vector4_t<T>::xy() const
{
    return vector2_t<T>::attach_to_ptr(as_ptr());
}

template <typename T>
vector2_t<T> vector4_t<T>::xz() const
{
    return vector2_t<T>::coord(x, z);
}

template <typename T>
vector2_t<T> vector4_t<T>::zw() const
{
    return vector2_t<T>::coord(z, w);
}

template <typename T>
vector3_t<T>& vector4_t<T>::xyz()
{
    return vector3_t<T>::attach_to_ptr(as_ptr());
}

template <typename T>
const vector3_t<T>& vector4_t<T>::xyz() const
{
    return vector3_t<T>::attach_to_ptr(as_ptr());
}

template <typename T>
vector3_t<T> vector4_t<T>::zyx() const
{
    return vector3_t<T>::coord(z, y, x);
}

}
