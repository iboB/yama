// Yama
// Copyright (c) 2016-2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include "dim.hpp"
#include "quaternion.hpp"

namespace yama
{

template <typename T>
class matrix4x4_t
{
public:
    T m00, m10, m20, m30;
    T m01, m11, m21, m31;
    T m02, m12, m22, m32;
    T m03, m13, m23, m33;

    typedef T value_type;
    typedef size_t size_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef typename std::reverse_iterator<iterator> reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;

    static constexpr size_type rows_count = 4;
    static constexpr size_type columns_count = 4;
    static constexpr size_type value_count = 16;

    constexpr size_type max_size() const { return value_count; }
    constexpr size_type size() const { return max_size(); }

    ///////////////////////////////////////////////////////////////////////////
    // named constructors

    static constexpr matrix4x4_t columns(
        const T& cr00, const T& cr01, const T& cr02, const T& cr03, //column 0
        const T& cr10, const T& cr11, const T& cr12, const T& cr13, //column 1
        const T& cr20, const T& cr21, const T& cr22, const T& cr23, //column 2
        const T& cr30, const T& cr31, const T& cr32, const T& cr33  //column 3
    )
    {
        return{
            cr00, cr01, cr02, cr03,
            cr10, cr11, cr12, cr13,
            cr20, cr21, cr22, cr23,
            cr30, cr31, cr32, cr33
        };
    }

    static constexpr matrix4x4_t rows(
        const T& rc00, const T& rc01, const T& rc02, const T& rc03, //row 0
        const T& rc10, const T& rc11, const T& rc12, const T& rc13, //row 1
        const T& rc20, const T& rc21, const T& rc22, const T& rc23, //row 2
        const T& rc30, const T& rc31, const T& rc32, const T& rc33  //row 3
    )
    {
        return{
            rc00, rc10, rc20, rc30,
            rc01, rc11, rc21, rc31,
            rc02, rc12, rc22, rc32,
            rc03, rc13, rc23, rc33,
        };
    }

    static constexpr matrix4x4_t uniform(const value_type& s)
    {
        return columns(
            s, s, s, s,
            s, s, s, s,
            s, s, s, s,
            s, s, s, s
        );
    }

    static constexpr matrix4x4_t zero()
    {
        return uniform(value_type(0));
    }

    static matrix4x4_t from_ptr(const value_type* ptr)
    {
        YAMA_ASSERT_CRIT(ptr, "Constructing yama::matrix4x4_t from nullptr");
        return columns(
            ptr[0], ptr[1], ptr[2], ptr[3],
            ptr[4], ptr[5], ptr[6], ptr[7],
            ptr[8], ptr[9], ptr[10], ptr[11],
            ptr[12], ptr[13], ptr[14], ptr[15]
        );
    }

    static constexpr matrix4x4_t identity()
    {
        return columns(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
    }

    ////////////////////////////////////////////////////////
    // transforms

    static constexpr matrix4x4_t translation(const value_type& x, const value_type& y, const value_type& z)
    {
        return rows(
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1
        );
    }

    static constexpr matrix4x4_t translation(const vector3_t<T>& pos)
    {
        return translation(pos.x, pos.y, pos.z);
    }

    static constexpr matrix4x4_t scaling_uniform(const value_type& s)
    {
        YAMA_ASSERT_WARN14(!close(s, value_type(0)), "scale shouldn't be zero");
        return columns(
            s, 0, 0, 0,
            0, s, 0, 0,
            0, 0, s, 0,
            0, 0, 0, 1
        );
    }

    static constexpr matrix4x4_t scaling(const value_type& x, const value_type& y, const value_type& z)
    {
        YAMA_ASSERT_WARN14(!close(x, value_type(0)), "scale shouldn't be zero");
        YAMA_ASSERT_WARN14(!close(y, value_type(0)), "scale shouldn't be zero");
        YAMA_ASSERT_WARN14(!close(z, value_type(0)), "scale shouldn't be zero");
        return columns(
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1
        );
    }

    static constexpr matrix4x4_t scaling(const vector3_t<T>& s)
    {
        return scaling(s.x, s.y, s.z);
    }

    // for when you're sure that the axis is normalized
    static matrix4x4_t rotation_naxis(const vector3_t<value_type>& axis, value_type radians)
    {
        YAMA_ASSERT_BAD(axis.is_normalized(), "rotation axis should be normalized");

        const value_type c = std::cos(radians);
        const value_type s = std::sin(radians);
        const value_type c1 = 1 - c;
        const value_type& x = axis.x;
        const value_type& y = axis.y;
        const value_type& z = axis.z;

        return rows(
            c + c1*sq(x), c1*y*x - s*z, c1*z*x + s*y, 0,
            c1*x*y + s*z, c + c1*sq(y), c1*z*y - s*x, 0,
            c1*x*z - s*y, c1*y*z + s*x, c + c1*sq(z), 0,
            0, 0, 0, 1
        );
    }

    static matrix4x4_t rotation_axis(const vector3_t<value_type>& axis, value_type radians)
    {
        auto naxis = yama::normalize(axis);
        return rotation_naxis(naxis, radians);
    }

    static matrix4x4_t rotation_x(const value_type& radians)
    {
        const value_type c = std::cos(radians);
        const value_type s = std::sin(radians);

        return rows(
            1, 0,  0, 0,
            0, c, -s, 0,
            0, s,  c, 0,
            0, 0,  0, 1
        );
    }

    static matrix4x4_t rotation_y(const value_type& radians)
    {
        const value_type c = std::cos(radians);
        const value_type s = std::sin(radians);

        return rows(
            c, 0, s, 0,
            0, 1, 0, 0,
           -s, 0, c, 0,
            0, 0, 0, 1
        );
    }

    static matrix4x4_t rotation_z(const value_type& radians)
    {
        const value_type c = std::cos(radians);
        const value_type s = std::sin(radians);

        return rows(
            c, -s, 0, 0,
            s,  c, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        );
    }

    static matrix4x4_t rotation_vectors(const vector3_t<value_type>& src, const vector3_t<value_type>& target)
    {
        YAMA_ASSERT_BAD(src.is_normalized(), "source vector should be normalized");
        YAMA_ASSERT_BAD(target.is_normalized(), "target vector should be normalized");
        YAMA_ASSERT_WARN(!close(src, vector3_t<value_type>::zero()), "source vector shouldn't be zero");
        YAMA_ASSERT_WARN(!close(target, vector3_t<value_type>::zero()), "target vector shouldn't be zero");

        auto axis = cross(src, target);
        auto axis_length = axis.length();

        if (axis_length > constants_t<value_type>::EPSILON()) // not collinear
        {
            axis /= axis_length; // normalize
            auto angle = acos(dot(src, target));
            return rotation_naxis(axis, angle);
        }
        else
        {
            if (close(src, target))
            {
                // collinear
                return identity();
            }
            else
            {
                // opposite
                auto o = normalize(src.get_orthogonal());

                return rows(
                    2*sq(o.x) - 1, 2*o.y*o.x, 2*o.z*o.x, 0,
                    2*o.x*o.y, 2*sq(o.y) - 1, 2*o.z*o.y, 0,
                    2*o.x*o.z, 2*o.y*o.z, 2*sq(o.z) - 1, 0,
                    0, 0, 0, 1
                );
            }
        }
    }

    static matrix4x4_t rotation_quaternion(const quaternion_t<T>& q)
    {
        YAMA_ASSERT_BAD(q.is_normalized(), "rotation with a non-normalized quaternion");
        YAMA_ASSERT_WARN(!close(q.length_sq(), value_type(0)), "rotating with a broken quaternion");

        const value_type x2 = sq(q.x);
        const value_type y2 = sq(q.y);
        const value_type z2 = sq(q.z);
        const value_type w2 = sq(q.w);
        const value_type xy = 2 * q.x * q.y;
        const value_type xz = 2 * q.x * q.z;
        const value_type xw = 2 * q.x * q.w;
        const value_type yz = 2 * q.y * q.z;
        const value_type yw = 2 * q.y * q.w;
        const value_type zw = 2 * q.z * q.w;

        return rows(
            w2 + x2 - y2 - z2, xy - zw,           xz + yw,           0,
            xy + zw,           w2 - x2 + y2 - z2, yz - xw,           0,
            xz - yw,           yz + xw,           w2 - x2 - y2 + z2, 0,
            0, 0, 0, 1
        );
    }

    static matrix4x4_t basis_transform(const vector3_t<value_type>& o, const vector3_t<value_type>& e1, const vector3_t<value_type>& e2, const vector3_t<value_type>& e3)
    {
        auto shift = vector3_t<value_type>::coord(
            -dot(e1, o),
            -dot(e2, o),
            -dot(e3, o)
        );

        auto transform = matrix4x4_t::rows(
            e1.x, e1.y, e1.z, shift.x,
            e2.x, e2.y, e2.z, shift.y,
            e3.x, e3.y, e3.z, shift.z,
            0,    0,    0,    1
        );

        YAMA_ASSERT_BAD(!close(transform.determinant(), value_type(0)), "linear dependency in basis transform");

        return transform;
    }

    ////////////////////////////////////////////////////////
    // projection
    static matrix4x4_t ortho_lh(value_type width, value_type height, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        auto d = far_dist - near_dist;
        return rows(
            2/width, 0,        0,    0,
            0,       2/height, 0,    0,
            0,       0,        1/d,  -near_dist/d,
            0, 0, 0, 1
        );
    }

    static matrix4x4_t ortho_lh(value_type left, value_type right, value_type bottom, value_type top, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        auto d = far_dist - near_dist;
        auto w = right - left;
        auto h = top - bottom;
        return rows(
            2/w, 0,   0,   -(left + right)/w,
            0,   2/h, 0,   -(top + bottom)/h,
            0,   0,   1/d, -near_dist/d,
            0, 0, 0, 1
        );
    }

    static matrix4x4_t ortho_lh_cube(value_type width, value_type height, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        auto d = far_dist - near_dist;
        return rows(
            2/width, 0,        0,    0,
            0,       2/height, 0,    0,
            0,       0,        2/d,  -(near_dist + far_dist)/d,
            0, 0, 0, 1
        );
    }

    static matrix4x4_t ortho_lh_cube(value_type left, value_type right, value_type bottom, value_type top, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        auto d = far_dist - near_dist;
        auto w = right - left;
        auto h = top - bottom;
        return rows(
            2/w, 0,   0,   -(left + right)/w,
            0,   2/h, 0,   -(top + bottom)/h,
            0,   0,   2/d, -(near_dist + far_dist)/d,
            0, 0, 0, 1
        );
    }

    static matrix4x4_t ortho_rh(value_type width, value_type height, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        auto d = far_dist - near_dist;
        return rows(
            2/width,   0,        0,    0,
            0,         2/height, 0,    0,
            0,         0,        -1/d, -near_dist/d,
            0, 0, 0, 1
        );
    }

    static matrix4x4_t ortho_rh(value_type left, value_type right, value_type bottom, value_type top, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        auto d = far_dist - near_dist;
        auto w = right - left;
        auto h = top - bottom;
        return rows(
            2/w, 0,   0,    -(left + right)/w,
            0,   2/h, 0,    -(top + bottom)/h,
            0,   0,   -1/d, -near_dist/d,
            0, 0, 0, 1
        );
    }

    static matrix4x4_t ortho_rh_cube(value_type width, value_type height, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        auto d = far_dist - near_dist;
        return rows(
            2/width, 0,        0,    0,
            0,       2/height, 0,    0,
            0,       0,        -2/d, -(near_dist + far_dist)/d,
            0, 0, 0, 1
        );
    }

    static matrix4x4_t ortho_rh_cube(value_type left, value_type right, value_type bottom, value_type top, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        auto d = far_dist - near_dist;
        auto w = right - left;
        auto h = top - bottom;
        return rows(
            2/w, 0,   0,    -(left + right)/w,
            0,   2/h, 0,    -(top + bottom)/h,
            0,   0,   -2/d, -(near_dist + far_dist)/d,
            0, 0, 0, 1
        );
    }

    static matrix4x4_t perspective_lh(value_type width, value_type height, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        auto d = far_dist - near_dist;
        return rows(
            (2*near_dist)/width, 0,             0,     0,
            0,            (2*near_dist)/height, 0,     0,
            0,            0,               far_dist/d, -(far_dist*near_dist)/d,
            0,            0,               1,          0
        );
    }

    static matrix4x4_t perspective_lh_cube(value_type width, value_type height, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        auto d = far_dist - near_dist;
        return rows(
            (2*near_dist)/width, 0,             0,     0,
            0,            (2*near_dist)/height, 0,     0,
            0,            0,               (far_dist+near_dist)/d, -2 * ((far_dist*near_dist)/d),
            0,            0,               1,          0
        );
    }

    static matrix4x4_t perspective_lh(value_type left, value_type right, value_type bottom, value_type top, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        auto d = far_dist - near_dist;
        auto w = right - left;
        auto h = top - bottom;

        return rows(
            (2*near_dist)/w, 0,            0,          -(left + right)/w,
            0,            (2*near_dist)/h, 0,          -(top + bottom)/h,
            0,            0,               far_dist/d, -(far_dist*near_dist)/d,
            0,            0,               1,          0
        );
    }

    static matrix4x4_t perspective_lh_cube(value_type left, value_type right, value_type bottom, value_type top, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        auto d = far_dist - near_dist;
        auto w = right - left;
        auto h = top - bottom;

        return rows(
            (2*near_dist)/w, 0,            0,          -(left + right)/w,
            0,            (2*near_dist)/h, 0,          -(top + bottom)/h,
            0,            0,               (far_dist+near_dist)/d, -2 * ((far_dist*near_dist)/d),
            0,            0,               1,          0
        );
    }

    static matrix4x4_t perspective_rh(value_type width, value_type height, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        auto d = far_dist - near_dist;
        return rows(
            (2*near_dist)/width, 0,             0,     0,
            0,            (2*near_dist)/height, 0,     0,
            0,            0,               -far_dist/d, -(far_dist*near_dist)/d,
            0,            0,               -1,          0
        );
    }

    static matrix4x4_t perspective_rh_cube(value_type width, value_type height, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        auto d = far_dist - near_dist;
        return rows(
            (2*near_dist)/width, 0,             0,     0,
            0,            (2*near_dist)/height, 0,     0,
            0,            0,               -(far_dist+near_dist)/d, -2 * ((far_dist*near_dist)/d),
            0,            0,               -1,          0
        );
    }

    static matrix4x4_t perspective_rh(value_type left, value_type right, value_type bottom, value_type top, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        auto d = far_dist - near_dist;
        auto w = right - left;
        auto h = top - bottom;

        return rows(
            (2*near_dist)/w, 0,            0,          -(left + right)/w,
            0,            (2*near_dist)/h, 0,          -(top + bottom)/h,
            0,            0,               -far_dist/d,-(far_dist*near_dist)/d,
            0,            0,               -1,         0
        );
    }

    static matrix4x4_t perspective_rh_cube(value_type left, value_type right, value_type bottom, value_type top, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        auto d = far_dist - near_dist;
        auto w = right - left;
        auto h = top - bottom;

        return rows(
            (2*near_dist)/w, 0,            0,          -(left + right)/w,
            0,            (2*near_dist)/h, 0,          -(top + bottom)/h,
            0,            0,               -(far_dist+near_dist)/d, -2*((far_dist*near_dist)/d),
            0,            0,               -1,          0
        );
    }

    static matrix4x4_t perspective_fov_lh(value_type fovy, value_type aspect, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        value_type yscale = value_type(1)/std::tan(fovy/2); //cot(fovy/2)
        value_type xscale = yscale/aspect;
        value_type depth = far_dist - near_dist;

        return matrix4x4_t::rows(
            xscale, 0,      0,          0,
            0,      yscale, 0,          0,
            0,      0,      far_dist/depth, -(far_dist*near_dist)/depth,
            0,      0,      1,          0);
    }

    static matrix4x4_t perspective_fov_lh_cube(value_type fovy, value_type aspect, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        value_type yscale = value_type(1)/std::tan(fovy/2); //cot(fovy/2)
        value_type xscale = yscale/aspect;
        value_type depth = far_dist - near_dist;

        return matrix4x4_t::rows(
            xscale, 0,      0,          0,
            0,      yscale, 0,          0,
            0,      0,      (far_dist+near_dist)/depth, -2*((far_dist*near_dist)/depth),
            0,      0,      1,          0);
    }

    static matrix4x4_t perspective_fov_rh(value_type fovy, value_type aspect, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        value_type yscale = value_type(1)/std::tan(fovy/2); //cot(fovy/2)
        value_type xscale = yscale/aspect;
        value_type depth = far_dist - near_dist;

        return matrix4x4_t::rows(
            xscale, 0,      0,          0,
            0,      yscale, 0,          0,
            0,      0,      -far_dist/depth, -(far_dist*near_dist)/depth,
            0,      0,      -1,         0);
    }

    static matrix4x4_t perspective_fov_rh_cube(value_type fovy, value_type aspect, value_type near_dist, value_type far_dist)
    {
        YAMA_ASSERT_BAD(!close(far_dist, near_dist), "near distance shouldn't the same as far distance");
        YAMA_ASSERT_BAD(!close(near_dist, value_type(0)), "near distance shouldn't be zero");

        value_type yscale = value_type(1)/std::tan(fovy/2); //cot(fovy/2)
        value_type xscale = yscale/aspect;
        value_type depth = far_dist - near_dist;

        return matrix4x4_t::rows(
            xscale, 0,      0,          0,
            0,      yscale, 0,          0,
            0,      0,      -(far_dist+near_dist)/depth, -2*((far_dist*near_dist)/depth),
            0,      0,      -1,         0);
    }

    ////////////////////////////////////////////////////////
    // view

    static matrix4x4_t look_towards_lh(const vector3_t<value_type>& eye, const vector3_t<value_type>& dir, const vector3_t<value_type>& up)
    {
        YAMA_ASSERT_BAD(!close(dir, vector3_t<value_type>::zero()), "direction shouldn't be zero");
        YAMA_ASSERT_BAD(!close(up, vector3_t<value_type>::zero()), "up vector shouldn't be zero");

        vector3_t<value_type> front = normalize(dir);
        vector3_t<value_type> right = normalize(cross(up, front));
        vector3_t<value_type> up2 = cross(front, right); //since the original up is not to be trusted

        return basis_transform(eye, right, up2, front);
    }

    static matrix4x4_t look_towards_rh(const vector3_t<value_type>& eye, const vector3_t<value_type>& dir, const vector3_t<value_type>& up)
    {
        return look_towards_lh(eye, -dir, up);
    }

    static matrix4x4_t look_at_lh(const vector3_t<value_type>& eye, const vector3_t<value_type>& at, const vector3_t<value_type>& up)
    {
        return look_towards_lh(eye, at - eye, up);
    }

    static matrix4x4_t look_at_rh(const vector3_t<value_type>& eye, const vector3_t<value_type>& at, const vector3_t<value_type>& up)
    {
        return look_towards_lh(eye, eye - at, up);
    }

    ///////////////////////////
    // attach
    static matrix4x4_t& attach_to_ptr(value_type* ptr)
    {
        YAMA_ASSERT_BAD(ptr, "Attaching yama::matrix4x4_t to nullptr");
        return *reinterpret_cast<matrix4x4_t*>(ptr);
    }

    static const matrix4x4_t& attach_to_ptr(const value_type* ptr)
    {
        YAMA_ASSERT_BAD(ptr, "Attaching yama::matrix4x4_t to nullptr");
        return *reinterpret_cast<const matrix4x4_t*>(ptr);
    }

    static matrix4x4_t* attach_to_array(value_type* ptr)
    {
        YAMA_ASSERT_WARN(ptr, "Attaching yama::matrix4x4_t to nullptr");
        return reinterpret_cast<matrix4x4_t*>(ptr);
    }

    static const matrix4x4_t* attach_to_array(const value_type* ptr)
    {
        YAMA_ASSERT_WARN(ptr, "Attaching yama::matrix4x4_t to nullptr");
        return reinterpret_cast<const matrix4x4_t*>(ptr);
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
        YAMA_ASSERT_CRIT(i < value_count, "yama::matrix4x4_t index overflow");
        return data()[i];
    }

    constexpr const value_type& at(size_type i) const
    {
        YAMA_ASSERT_CRIT14(i < value_count, "yama::matrix4x4_t index overflow");
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

    value_type* column(size_t i)
    {
        YAMA_ASSERT_CRIT(i < columns_count, "yama::matrix4x4_t column index overflow");
        return data() + rows_count * i;
    }

    constexpr const value_type* column(size_t i) const
    {
        YAMA_ASSERT_CRIT14(i < columns_count, "yama::matrix4x4_t column index overflow");
        return data() + rows_count * i;
    }

    value_type& m(size_t row, size_t col)
    {
        return column(col)[row];
    }

    constexpr const value_type& m(size_t row, size_t col) const
    {
        return column(col)[row];
    }

    value_type& operator()(size_t row, size_t col)
    {
        return m(row, col);
    }

    constexpr const value_type& operator()(size_t row, size_t col) const
    {
        return m(row, col);
    }

    template <size_t D>
    typename dim<D>::template vector_t<value_type>& column_vector(size_t col, size_t offset = 0)
    {
        static_assert(D <= columns_count, "yama::matrix4x4_t column_vector out of range");
        YAMA_ASSERT_BAD(D + offset <= rows_count, "yama::matrix4x4_t column_vector reaching end of column");
        return dim<D>::template vector_t<value_type>::attach_to_ptr(column(col) + offset);
    }

    template <size_t D>
    const typename dim<D>::template vector_t<value_type>& column_vector(size_t col, size_t offset = 0) const
    {
        static_assert(D <= columns_count, "yama::matrix4x4_t column_vector out of range");
        YAMA_ASSERT_BAD(D + offset <= rows_count, "yama::matrix4x4_t column_vector reaching end of column");
        return dim<D>::template vector_t<value_type>::attach_to_ptr(column(col) + offset);
    }

    vector4_t<value_type>& column_vector(size_t col)
    {
        return vector4_t<value_type>::attach_to_ptr(column(col));
    }

    const vector4_t<value_type>& column_vector(size_t col) const
    {
        return vector4_t<value_type>::attach_to_ptr(column(col));
    }

    template <size_t D>
    typename dim<D>::template vector_t<value_type> row_vector(size_t row, size_t offset = 0) const
    {
        static_assert(D < rows_count, "yama::matrix4x4_t row_vector out of range");
        YAMA_ASSERT_BAD(D + offset <= columns_count, "yama::matrix4x4_t row_vector reaching end of column");
        typename dim<D>::template vector_t<value_type> ret;
        for (size_t i = 0; i < D; ++i)
        {
            ret.at(i) = column(offset + i)[row];
        }
        return ret;
    }

    vector4_t<value_type> row_vector(size_t row) const
    {
        return vector4_t<value_type>::coord(m(row, 0), m(row, 1), m(row, 2), m(row, 3));
    }

    template <size_t D>
    typename dim<D>::template vector_t<value_type> main_diagonal(size_t offset = 0) const
    {
        static_assert(D < rows_count, "yama::matrix4x4_t row_vector out of range");
        YAMA_ASSERT_BAD(D + offset <= columns_count, "yama::matrix4x4_t row_vector reaching end of column");
        typename dim<D>::template vector_t<value_type> ret;
        for (size_t i = 0; i < D; ++i)
        {
            ret.at(i) = m(offset+i, offset+i);
        }
        return ret;
    }

    vector4_t<value_type> main_diagonal() const
    {
        return vector4_t<value_type>::coord(m(0, 0), m(1, 1), m(2, 2), m(3, 3));
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
    matrix4x4_t<S> as_matrix4x4_t() const
    {
        return matrix4x4_t<S>::columns(
            S(m00), S(m10), S(m20), S(m30),
            S(m01), S(m11), S(m21), S(m31),
            S(m02), S(m12), S(m22), S(m32),
            S(m03), S(m13), S(m23), S(m33)
        );
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

    constexpr const matrix4x4_t& operator+() const
    {
        return *this;
    }

    constexpr matrix4x4_t operator-() const
    {
        return columns(
            -m00, -m10, -m20, -m30,
            -m01, -m11, -m21, -m31,
            -m02, -m12, -m22, -m32,
            -m03, -m13, -m23, -m33
        );
    }

    matrix4x4_t& operator+=(const matrix4x4_t& b)
    {
        m00 += b.m00; m10 += b.m10; m20 += b.m20; m30 += b.m30;
        m01 += b.m01; m11 += b.m11; m21 += b.m21; m31 += b.m31;
        m02 += b.m02; m12 += b.m12; m22 += b.m22; m32 += b.m32;
        m03 += b.m03; m13 += b.m13; m23 += b.m23; m33 += b.m33;
        return *this;
    }

    matrix4x4_t& operator-=(const matrix4x4_t& b)
    {
        m00 -= b.m00; m10 -= b.m10; m20 -= b.m20; m30 -= b.m30;
        m01 -= b.m01; m11 -= b.m11; m21 -= b.m21; m31 -= b.m31;
        m02 -= b.m02; m12 -= b.m12; m22 -= b.m22; m32 -= b.m32;
        m03 -= b.m03; m13 -= b.m13; m23 -= b.m23; m33 -= b.m33;
        return *this;
    }

    matrix4x4_t& operator*=(const value_type& s)
    {
        m00 *= s; m10 *= s; m20 *= s; m30 *= s;
        m01 *= s; m11 *= s; m21 *= s; m31 *= s;
        m02 *= s; m12 *= s; m22 *= s; m32 *= s;
        m03 *= s; m13 *= s; m23 *= s; m33 *= s;
        return *this;
    }

    matrix4x4_t& operator/=(const value_type& s)
    {
        YAMA_ASSERT_WARN(s != 0, "yama::matrix4x4_t division by zero");
        m00 /= s; m10 /= s; m20 /= s; m30 /= s;
        m01 /= s; m11 /= s; m21 /= s; m31 /= s;
        m02 /= s; m12 /= s; m22 /= s; m32 /= s;
        m03 /= s; m13 /= s; m23 /= s; m33 /= s;
        return *this;
    }

    matrix4x4_t& operator*=(const matrix4x4_t& b)
    {
        auto c00 = m00 * b.m00 + m01 * b.m10 + m02 * b.m20 + m03 * b.m30;
        auto c10 = m10 * b.m00 + m11 * b.m10 + m12 * b.m20 + m13 * b.m30;
        auto c20 = m20 * b.m00 + m21 * b.m10 + m22 * b.m20 + m23 * b.m30;
        auto c30 = m30 * b.m00 + m31 * b.m10 + m32 * b.m20 + m33 * b.m30;
        auto c01 = m00 * b.m01 + m01 * b.m11 + m02 * b.m21 + m03 * b.m31;
        auto c11 = m10 * b.m01 + m11 * b.m11 + m12 * b.m21 + m13 * b.m31;
        auto c21 = m20 * b.m01 + m21 * b.m11 + m22 * b.m21 + m23 * b.m31;
        auto c31 = m30 * b.m01 + m31 * b.m11 + m32 * b.m21 + m33 * b.m31;
        auto c02 = m00 * b.m02 + m01 * b.m12 + m02 * b.m22 + m03 * b.m32;
        auto c12 = m10 * b.m02 + m11 * b.m12 + m12 * b.m22 + m13 * b.m32;
        auto c22 = m20 * b.m02 + m21 * b.m12 + m22 * b.m22 + m23 * b.m32;
        auto c32 = m30 * b.m02 + m31 * b.m12 + m32 * b.m22 + m33 * b.m32;
             m03 = m00 * b.m03 + m01 * b.m13 + m02 * b.m23 + m03 * b.m33;
             m13 = m10 * b.m03 + m11 * b.m13 + m12 * b.m23 + m13 * b.m33;
             m23 = m20 * b.m03 + m21 * b.m13 + m22 * b.m23 + m23 * b.m33;
             m33 = m30 * b.m03 + m31 * b.m13 + m32 * b.m23 + m33 * b.m33;

        m00 = c00; m10 = c10; m20 = c20; m30 = c30;
        m01 = c01; m11 = c11; m21 = c21; m31 = c31;
        m02 = c02; m12 = c12; m22 = c22; m32 = c32;

        return *this;
    }

    matrix4x4_t& mul(const matrix4x4_t& b)
    {
        m00 *= b.m00; m10 *= b.m10; m20 *= b.m20; m30 *= b.m30;
        m01 *= b.m01; m11 *= b.m11; m21 *= b.m21; m31 *= b.m31;
        m02 *= b.m02; m12 *= b.m12; m22 *= b.m22; m32 *= b.m32;
        m03 *= b.m03; m13 *= b.m13; m23 *= b.m23; m33 *= b.m33;
        return *this;
    }

    matrix4x4_t& div(const matrix4x4_t& b)
    {
        m00 /= b.m00; m10 /= b.m10; m20 /= b.m20; m30 /= b.m30;
        m01 /= b.m01; m11 /= b.m11; m21 /= b.m21; m31 /= b.m31;
        m02 /= b.m02; m12 /= b.m12; m22 /= b.m22; m32 /= b.m32;
        m03 /= b.m03; m13 /= b.m13; m23 /= b.m23; m33 /= b.m33;
        return *this;
    }

    matrix4x4_t& transpose()
    {
        std::swap(m10, m01);
        std::swap(m20, m02);
        std::swap(m30, m03);
        std::swap(m21, m12);
        std::swap(m31, m13);
        std::swap(m32, m23);
        return *this;
    }

    value_type determinant() const
    {
        return
            m03*m12*m21*m30 - m02*m13*m21*m30 - m03*m11*m22*m30 +
            m01*m13*m22*m30 + m02*m11*m23*m30 - m01*m12*m23*m30 -
            m03*m12*m20*m31 + m02*m13*m20*m31 + m03*m10*m22*m31 -
            m00*m13*m22*m31 - m02*m10*m23*m31 + m00*m12*m23*m31 +
            m03*m11*m20*m32 - m01*m13*m20*m32 - m03*m10*m21*m32 +
            m00*m13*m21*m32 + m01*m10*m23*m32 - m00*m11*m23*m32 -
            m02*m11*m20*m33 + m01*m12*m20*m33 + m02*m10*m21*m33 -
            m00*m12*m21*m33 - m01*m10*m22*m33 + m00*m11*m22*m33;
    }

    // returns determinant
    value_type inverse()
    {
        auto det = determinant();

        auto c00 = -(m13*m22*m31) + m12*m23*m31 + m13*m21*m32 - m11*m23*m32 - m12*m21*m33 + m11*m22*m33;
        auto c10 = m13*m22*m30 - m12*m23*m30 - m13*m20*m32 + m10*m23*m32 + m12*m20*m33 - m10*m22*m33;
        auto c20 = -(m13*m21*m30) + m11*m23*m30 + m13*m20*m31 - m10*m23*m31 - m11*m20*m33 + m10*m21*m33;
        auto c30 = m12*m21*m30 - m11*m22*m30 - m12*m20*m31 + m10*m22*m31 + m11*m20*m32 - m10*m21*m32;
        auto c01 = m03*m22*m31 - m02*m23*m31 - m03*m21*m32 + m01*m23*m32 + m02*m21*m33 - m01*m22*m33;
        auto c11 = -(m03*m22*m30) + m02*m23*m30 + m03*m20*m32 - m00*m23*m32 - m02*m20*m33 + m00*m22*m33;
        auto c21 = m03*m21*m30 - m01*m23*m30 - m03*m20*m31 + m00*m23*m31 + m01*m20*m33 - m00*m21*m33;
        auto c31 = -(m02*m21*m30) + m01*m22*m30 + m02*m20*m31 - m00*m22*m31 - m01*m20*m32 + m00*m21*m32;
        auto c02 = -(m03*m12*m31) + m02*m13*m31 + m03*m11*m32 - m01*m13*m32 - m02*m11*m33 + m01*m12*m33;
        auto c12 = m03*m12*m30 - m02*m13*m30 - m03*m10*m32 + m00*m13*m32 + m02*m10*m33 - m00*m12*m33;
        auto c22 = -(m03*m11*m30) + m01*m13*m30 + m03*m10*m31 - m00*m13*m31 - m01*m10*m33 + m00*m11*m33;
        auto c32 = m02*m11*m30 - m01*m12*m30 - m02*m10*m31 + m00*m12*m31 + m01*m10*m32 - m00*m11*m32;
        auto c03 = m03*m12*m21 - m02*m13*m21 - m03*m11*m22 + m01*m13*m22 + m02*m11*m23 - m01*m12*m23;
        auto c13 = -(m03*m12*m20) + m02*m13*m20 + m03*m10*m22 - m00*m13*m22 - m02*m10*m23 + m00*m12*m23;
        auto c23 = m03*m11*m20 - m01*m13*m20 - m03*m10*m21 + m00*m13*m21 + m01*m10*m23 - m00*m11*m23;
        auto c33 = -(m02*m11*m20) + m01*m12*m20 + m02*m10*m21 - m00*m12*m21 - m01*m10*m22 + m00*m11*m22;

        m00 = c00 / det; m10 = c10 / det; m20 = c20 / det; m30 = c30 / det;
        m01 = c01 / det; m11 = c11 / det; m21 = c21 / det; m31 = c31 / det;
        m02 = c02 / det; m12 = c12 / det; m22 = c22 / det; m32 = c32 / det;
        m03 = c03 / det; m13 = c13 / det; m23 = c23 / det; m33 = c33 / det;

        return det;
    }
};

template <typename T>
bool operator==(const matrix4x4_t<T>& a, const matrix4x4_t<T>& b)
{
    return
        a.m00 == b.m00 && a.m10 == b.m10 && a.m20 == b.m20 && a.m30 == b.m30 &&
        a.m01 == b.m01 && a.m11 == b.m11 && a.m21 == b.m21 && a.m31 == b.m31 &&
        a.m02 == b.m02 && a.m12 == b.m12 && a.m22 == b.m22 && a.m32 == b.m32 &&
        a.m03 == b.m03 && a.m13 == b.m13 && a.m23 == b.m23 && a.m33 == b.m33;
}

template <typename T>
bool operator!=(const matrix4x4_t<T>& a, const matrix4x4_t<T>& b)
{
    return
        a.m00 != b.m00 || a.m10 != b.m10 || a.m20 != b.m20 || a.m30 != b.m30 ||
        a.m01 != b.m01 || a.m11 != b.m11 || a.m21 != b.m21 || a.m31 != b.m31 ||
        a.m02 != b.m02 || a.m12 != b.m12 || a.m22 != b.m22 || a.m32 != b.m32 ||
        a.m03 != b.m03 || a.m13 != b.m13 || a.m23 != b.m23 || a.m33 != b.m33;
}

template <typename T>
bool close(const matrix4x4_t<T>& a, const matrix4x4_t<T>& b, const T& epsilon = constants_t<T>::EPSILON())
{
    return
        close(a.m00, b.m00, epsilon) && close(a.m10, b.m10, epsilon) && close(a.m20, b.m20, epsilon) && close(a.m30, b.m30, epsilon) &&
        close(a.m01, b.m01, epsilon) && close(a.m11, b.m11, epsilon) && close(a.m21, b.m21, epsilon) && close(a.m31, b.m31, epsilon) &&
        close(a.m02, b.m02, epsilon) && close(a.m12, b.m12, epsilon) && close(a.m22, b.m22, epsilon) && close(a.m32, b.m32, epsilon) &&
        close(a.m03, b.m03, epsilon) && close(a.m13, b.m13, epsilon) && close(a.m23, b.m23, epsilon) && close(a.m33, b.m33, epsilon);
}

template <typename T>
matrix4x4_t<T> operator+(const matrix4x4_t<T>& a, const matrix4x4_t<T>& b)
{
    return matrix4x4_t<T>::columns(
        a.m00 + b.m00, a.m10 + b.m10, a.m20 + b.m20, a.m30 + b.m30,
        a.m01 + b.m01, a.m11 + b.m11, a.m21 + b.m21, a.m31 + b.m31,
        a.m02 + b.m02, a.m12 + b.m12, a.m22 + b.m22, a.m32 + b.m32,
        a.m03 + b.m03, a.m13 + b.m13, a.m23 + b.m23, a.m33 + b.m33
    );
}

template <typename T>
matrix4x4_t<T> operator-(const matrix4x4_t<T>& a, const matrix4x4_t<T>& b)
{
    return matrix4x4_t<T>::columns(
        a.m00 - b.m00, a.m10 - b.m10, a.m20 - b.m20, a.m30 - b.m30,
        a.m01 - b.m01, a.m11 - b.m11, a.m21 - b.m21, a.m31 - b.m31,
        a.m02 - b.m02, a.m12 - b.m12, a.m22 - b.m22, a.m32 - b.m32,
        a.m03 - b.m03, a.m13 - b.m13, a.m23 - b.m23, a.m33 - b.m33
    );
}

template <typename T>
matrix4x4_t<T> operator*(const matrix4x4_t<T>& a, const T& s)
{
    return matrix4x4_t<T>::columns(
        a.m00 * s, a.m10 * s, a.m20 * s, a.m30 * s,
        a.m01 * s, a.m11 * s, a.m21 * s, a.m31 * s,
        a.m02 * s, a.m12 * s, a.m22 * s, a.m32 * s,
        a.m03 * s, a.m13 * s, a.m23 * s, a.m33 * s
    );
}

template <typename T>
matrix4x4_t<T> operator*(const T& s, const matrix4x4_t<T>& b)
{
    return matrix4x4_t<T>::columns(
        s * b.m00, s * b.m10, s * b.m20, s * b.m30,
        s * b.m01, s * b.m11, s * b.m21, s * b.m31,
        s * b.m02, s * b.m12, s * b.m22, s * b.m32,
        s * b.m03, s * b.m13, s * b.m23, s * b.m33
    );
}

template <typename T>
matrix4x4_t<T> operator/(const matrix4x4_t<T>& a, const T& s)
{
    return matrix4x4_t<T>::columns(
        a.m00 / s, a.m10 / s, a.m20 / s, a.m30 / s,
        a.m01 / s, a.m11 / s, a.m21 / s, a.m31 / s,
        a.m02 / s, a.m12 / s, a.m22 / s, a.m32 / s,
        a.m03 / s, a.m13 / s, a.m23 / s, a.m33 / s
    );
}

template <typename T>
matrix4x4_t<T> operator/(const T& s, const matrix4x4_t<T>& b)
{
    return matrix4x4_t<T>::columns(
        s / b.m00, s / b.m10, s / b.m20, s / b.m30,
        s / b.m01, s / b.m11, s / b.m21, s / b.m31,
        s / b.m02, s / b.m12, s / b.m22, s / b.m32,
        s / b.m03, s / b.m13, s / b.m23, s / b.m33
    );
}

template <typename T>
matrix4x4_t<T> operator*(const matrix4x4_t<T>& a, const matrix4x4_t<T>& b)
{
    return matrix4x4_t<T>::columns(
        a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20 + a.m03 * b.m30,
        a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20 + a.m13 * b.m30,
        a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20 + a.m23 * b.m30,
        a.m30 * b.m00 + a.m31 * b.m10 + a.m32 * b.m20 + a.m33 * b.m30,
        a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21 + a.m03 * b.m31,
        a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31,
        a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31,
        a.m30 * b.m01 + a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31,
        a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22 + a.m03 * b.m32,
        a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32,
        a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32,
        a.m30 * b.m02 + a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32,
        a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03 * b.m33,
        a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33,
        a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33,
        a.m30 * b.m03 + a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33
    );
}

template <typename T>
matrix4x4_t<T> abs(const matrix4x4_t<T>& a)
{
    return matrix4x4_t<T>::columns(
        std::abs(a.m00), std::abs(a.m10), std::abs(a.m20), std::abs(a.m30),
        std::abs(a.m01), std::abs(a.m11), std::abs(a.m21), std::abs(a.m31),
        std::abs(a.m02), std::abs(a.m12), std::abs(a.m22), std::abs(a.m32),
        std::abs(a.m03), std::abs(a.m13), std::abs(a.m23), std::abs(a.m33)
    );
}

template <typename T>
matrix4x4_t<T> mul(const matrix4x4_t<T>& a, const matrix4x4_t<T>& b)
{
    return matrix4x4_t<T>::columns(
        a.m00 * b.m00, a.m10 * b.m10, a.m20 * b.m20, a.m30 * b.m30,
        a.m01 * b.m01, a.m11 * b.m11, a.m21 * b.m21, a.m31 * b.m31,
        a.m02 * b.m02, a.m12 * b.m12, a.m22 * b.m22, a.m32 * b.m32,
        a.m03 * b.m03, a.m13 * b.m13, a.m23 * b.m23, a.m33 * b.m33
    );
}

template <typename T>
matrix4x4_t<T> div(const matrix4x4_t<T>& a, const matrix4x4_t<T>& b)
{
    return matrix4x4_t<T>::columns(
        a.m00 / b.m00, a.m10 / b.m10, a.m20 / b.m20, a.m30 / b.m30,
        a.m01 / b.m01, a.m11 / b.m11, a.m21 / b.m21, a.m31 / b.m31,
        a.m02 / b.m02, a.m12 / b.m12, a.m22 / b.m22, a.m32 / b.m32,
        a.m03 / b.m03, a.m13 / b.m13, a.m23 / b.m23, a.m33 / b.m33
    );
}

template <typename T>
bool isfinite(const matrix4x4_t<T>& a)
{
    return std::isfinite(a.m00) && std::isfinite(a.m10) && std::isfinite(a.m20) && std::isfinite(a.m30) &&
           std::isfinite(a.m01) && std::isfinite(a.m11) && std::isfinite(a.m21) && std::isfinite(a.m31) &&
           std::isfinite(a.m02) && std::isfinite(a.m12) && std::isfinite(a.m22) && std::isfinite(a.m32) &&
           std::isfinite(a.m03) && std::isfinite(a.m13) && std::isfinite(a.m23) && std::isfinite(a.m33);
}

template <typename T>
matrix4x4_t<T> inverse(const matrix4x4_t<T>& a, T& out_determinant)
{
    out_determinant = a.determinant();

    return matrix4x4_t<T>::columns(
        (-(a.m13*a.m22*a.m31) + a.m12*a.m23*a.m31 + a.m13*a.m21*a.m32 - a.m11*a.m23*a.m32 - a.m12*a.m21*a.m33 + a.m11*a.m22*a.m33) / out_determinant,
        (a.m13*a.m22*a.m30 - a.m12*a.m23*a.m30 - a.m13*a.m20*a.m32 + a.m10*a.m23*a.m32 + a.m12*a.m20*a.m33 - a.m10*a.m22*a.m33) / out_determinant,
        (-(a.m13*a.m21*a.m30) + a.m11*a.m23*a.m30 + a.m13*a.m20*a.m31 - a.m10*a.m23*a.m31 - a.m11*a.m20*a.m33 + a.m10*a.m21*a.m33) / out_determinant,
        (a.m12*a.m21*a.m30 - a.m11*a.m22*a.m30 - a.m12*a.m20*a.m31 + a.m10*a.m22*a.m31 + a.m11*a.m20*a.m32 - a.m10*a.m21*a.m32) / out_determinant,
        (a.m03*a.m22*a.m31 - a.m02*a.m23*a.m31 - a.m03*a.m21*a.m32 + a.m01*a.m23*a.m32 + a.m02*a.m21*a.m33 - a.m01*a.m22*a.m33) / out_determinant,
        (-(a.m03*a.m22*a.m30) + a.m02*a.m23*a.m30 + a.m03*a.m20*a.m32 - a.m00*a.m23*a.m32 - a.m02*a.m20*a.m33 + a.m00*a.m22*a.m33) / out_determinant,
        (a.m03*a.m21*a.m30 - a.m01*a.m23*a.m30 - a.m03*a.m20*a.m31 + a.m00*a.m23*a.m31 + a.m01*a.m20*a.m33 - a.m00*a.m21*a.m33) / out_determinant,
        (-(a.m02*a.m21*a.m30) + a.m01*a.m22*a.m30 + a.m02*a.m20*a.m31 - a.m00*a.m22*a.m31 - a.m01*a.m20*a.m32 + a.m00*a.m21*a.m32) / out_determinant,
        (-(a.m03*a.m12*a.m31) + a.m02*a.m13*a.m31 + a.m03*a.m11*a.m32 - a.m01*a.m13*a.m32 - a.m02*a.m11*a.m33 + a.m01*a.m12*a.m33) / out_determinant,
        (a.m03*a.m12*a.m30 - a.m02*a.m13*a.m30 - a.m03*a.m10*a.m32 + a.m00*a.m13*a.m32 + a.m02*a.m10*a.m33 - a.m00*a.m12*a.m33) / out_determinant,
        (-(a.m03*a.m11*a.m30) + a.m01*a.m13*a.m30 + a.m03*a.m10*a.m31 - a.m00*a.m13*a.m31 - a.m01*a.m10*a.m33 + a.m00*a.m11*a.m33) / out_determinant,
        (a.m02*a.m11*a.m30 - a.m01*a.m12*a.m30 - a.m02*a.m10*a.m31 + a.m00*a.m12*a.m31 + a.m01*a.m10*a.m32 - a.m00*a.m11*a.m32) / out_determinant,
        (a.m03*a.m12*a.m21 - a.m02*a.m13*a.m21 - a.m03*a.m11*a.m22 + a.m01*a.m13*a.m22 + a.m02*a.m11*a.m23 - a.m01*a.m12*a.m23) / out_determinant,
        (-(a.m03*a.m12*a.m20) + a.m02*a.m13*a.m20 + a.m03*a.m10*a.m22 - a.m00*a.m13*a.m22 - a.m02*a.m10*a.m23 + a.m00*a.m12*a.m23) / out_determinant,
        (a.m03*a.m11*a.m20 - a.m01*a.m13*a.m20 - a.m03*a.m10*a.m21 + a.m00*a.m13*a.m21 + a.m01*a.m10*a.m23 - a.m00*a.m11*a.m23) / out_determinant,
        (-(a.m02*a.m11*a.m20) + a.m01*a.m12*a.m20 + a.m02*a.m10*a.m21 - a.m00*a.m12*a.m21 - a.m01*a.m10*a.m22 + a.m00*a.m11*a.m22) / out_determinant
    );
}

template <typename T>
matrix4x4_t<T> inverse(const matrix4x4_t<T>& a)
{
    T det;
    return inverse(a, det);
}

template <typename T>
vector3_t<T> transform_coord(const vector3_t<T>& v, const matrix4x4_t<T>& m)
{
    vector3_t<T> out;

    T w = m(3, 0) * v.x + m(3, 1) * v.y + m(3, 2) * v.z + m(3, 3);

    out.x = (m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z + m(0, 3)) / w;
    out.y = (m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z + m(1, 3)) / w;
    out.z = (m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z + m(2, 3)) / w;

    return out;
}


template <typename T>
vector3_t<T> transform_normal(const vector3_t<T>& v, const matrix4x4_t<T>& m)
{
    vector3_t<T> out;

    out.x = m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z;
    out.y = m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z;
    out.z = m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z;

    return out;
}

// type traits
template <typename T>
struct is_yama<matrix4x4_t<T>> : public std::true_type {};

template <typename T>
struct is_matrix<matrix4x4_t<T>> : public std::true_type {};

// shorthand
#if !defined(YAMA_NO_SHORTHAND)

typedef matrix4x4_t<preferred_type> matrix4x4;
typedef matrix4x4 matrix;

#endif

}
