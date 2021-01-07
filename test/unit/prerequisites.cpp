// Yama
// Copyright (c) 2016-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "yama/yama.hpp"
#include "doctest/doctest.h"

using namespace yama;

static_assert(constants_t<int>::PI == 3, "yama constants must be constexpr");
static_assert(constants_t<double>::SQRT_2 == 1.4142135623730950488016887242097, "yama constants must be constexpr");

static_assert(!is_yama<int>::value, "int is not a yama type");
static_assert(!is_vector<float>::value, "float is not a vector type");
static_assert(!is_matrix<double>::value, "double is not a matrix type");

// funcs
static_assert(sq(11) == 121, "yama::sq must be constexpr");

// vec2
static_assert(sizeof(vector2_t<float>) == 2 * sizeof(float), "yama types must be identical to ntuples of T in size");
static_assert(sizeof(vector2_t<double>) == 2 * sizeof(double), "yama types must be identical to ntuples of T in size");
static_assert(is_yama<vector2_t<int>>::value, "yama::vector2_t must be a yama type");
static_assert(is_vector<vector2_t<double>>::value, "yama::vector2_t must be a vector type");
static_assert(is_vector<vector2>::value, "yama::vector2 must be a vector type");
static_assert(!is_matrix<vector2>::value, "yama::vector2 is not a matrix type");
static_assert(vector2_t<int>::value_count == 2, "yama::vector2 has two elements");
static_assert(vector2_t<int>::coord(1, 2).x == 1, "yama constructors must be constexpr");
static_assert(vector2_t<int>::uniform(2).y == 2, "yama constructors must be constexpr");
static_assert(vector2_t<int>::zero().y == 0, "yama constructors must be constexpr");
static_assert(v(1, 2).y == 2.f, "yama constructors must be constexpr");
static_assert(vt(1, 2).y == 2, "yama constructors must be constexpr");
static_assert(std::is_same<vector2, point2>::value, "vector2 must be the same as point2");
static_assert(std::is_same<vector2_t<double>, dim<2>::vector_t<double>>::value, "vector2_t must be the same as dim<2>::vector2_t");
static_assert(std::is_same<vector2, dim<2>::vector>::value, "vector2 must be the same as dim<2>::vector2");

// vec3
static_assert(sizeof(vector3_t<float>) == 3 * sizeof(float), "yama types must be identical to ntuples of T in size");
static_assert(sizeof(vector3_t<double>) == 3 * sizeof(double), "yama types must be identical to ntuples of T in size");
static_assert(is_yama<vector3_t<int>>::value, "yama::vector3_t must be a yama type");
static_assert(is_vector<vector3_t<double>>::value, "yama::vector3_t must be a vector type");
static_assert(is_vector<vector3>::value, "yama::vector3 must be a vector type");
static_assert(!is_matrix<vector3>::value, "yama::vector3 is not a matrix type");
static_assert(vector3_t<int>::value_count == 3, "yama::vector3 has three elements");
static_assert(vector3_t<int>::coord(1, 2, 3).y == 2, "yama constructors must be constexpr");
static_assert(vector3_t<int>::uniform(2).z == 2, "yama constructors must be constexpr");
static_assert(vector3_t<int>::zero().z == 0, "yama constructors must be constexpr");
static_assert(v(1, 2, 3).z == 3.f, "yama constructors must be constexpr");
static_assert(vt(1, 2, 3).z == 3, "yama constructors must be constexpr");
static_assert(std::is_same<vector3_t<double>, dim<3>::vector_t<double>>::value, "vector3_t must be the same as dim<3>::vector3_t");
static_assert(std::is_same<vector3, point3>::value, "vector2 must be the same as point2");
static_assert(std::is_same<vector3, dim<3>::vector>::value, "vector3 must be the same as dim<3>::vector");

// vec4
static_assert(sizeof(vector4_t<float>) == 4 * sizeof(float), "yama types must be identical to ntuples of T in size");
static_assert(sizeof(vector4_t<double>) == 4 * sizeof(double), "yama types must be identical to ntuples of T in size");
static_assert(is_yama<vector4_t<int>>::value, "yama::vector4_t must be a yama type");
static_assert(is_vector<vector4_t<double>>::value, "yama::vector4_t must be a vector type");
static_assert(is_vector<vector4>::value, "yama::vector4 must be a vector type");
static_assert(!is_matrix<vector4>::value, "yama::vector4 is not a matrix type");
static_assert(vector4_t<int>::value_count == 4, "yama::vector4 has four elements");
static_assert(vector4_t<int>::coord(1, 2, 3, 5).w == 5, "yama constructors must be constexpr");
static_assert(vector4_t<int>::uniform(2).w == 2, "yama constructors must be constexpr");
static_assert(vector4_t<int>::zero().z == 0, "yama constructors must be constexpr");
static_assert(v(1, 2, 3, 4).w == 4.f, "yama constructors must be constexpr");
static_assert(vt(1, 2, 3, 4).w == 4, "yama constructors must be constexpr");
static_assert(std::is_same<vector4_t<double>, dim<4>::vector_t<double>>::value, "vector4_t must be the same as dim<3>::vector4_t");
static_assert(std::is_same<vector4, point4>::value, "vector2 must be the same as point2");
static_assert(std::is_same<vector4, dim<4>::vector>::value, "vector4 must be the same as dim<3>::vector");

// quat
static_assert(sizeof(quaternion_t<float>) == 4 * sizeof(float), "yama types must be identical to ntuples of T in size");
static_assert(sizeof(quaternion_t<double>) == 4 * sizeof(double), "yama types must be identical to ntuples of T in size");
static_assert(is_yama<quaternion_t<int>>::value, "yama::quaternion_t must be a yama type");
static_assert(!is_vector<quaternion_t<double>>::value, "yama::quaternion_t is not a vector type");
static_assert(!is_matrix<quaternion>::value, "yama::quaternion is not a matrix type");
static_assert(quaternion_t<int>::value_count == 4, "yama::quaternion has four elements");
static_assert(quaternion_t<int>::xyzw(1, 2, 3, 5).w == 5, "yama constructors must be constexpr");
static_assert(quaternion_t<int>::uniform(2).w == 2, "yama constructors must be constexpr");
static_assert(quaternion_t<int>::zero().z == 0, "yama constructors must be constexpr");
static_assert(v(1, 2, 3, 4).w == 4.f, "yama constructors must be constexpr");
static_assert(vt(1, 2, 3, 4).w == 4, "yama constructors must be constexpr");

// mat4x4
static_assert(sizeof(matrix4x4_t<float>) == 16 * sizeof(float), "yama types must be identical to ntuples of T in size");
static_assert(sizeof(matrix4x4_t<double>) == 16 * sizeof(double), "yama types must be identical to ntuples of T in size");
static_assert(is_yama<matrix4x4_t<int>>::value, "yama::quaternion_t must be a yama type");
static_assert(!is_vector<matrix4x4_t<double>>::value, "yama::matrix4x4_t is not a vector type");
static_assert(is_matrix<matrix>::value, "yama::matrix4x4_t must be a matrix type");
static_assert(matrix4x4_t<int>::value_count == 16, "yama::matrix4x4_t has sixteen elements");
static_assert(matrix4x4_t<int>::columns(1, 2, 3, 5, 10, 8, 7, 6, 22, 23, 24, 25, -1, -2, -3, -4).m30 == 5, "yama constructors must be constexpr");
static_assert(matrix4x4_t<int>::uniform(2).m22 == 2, "yama constructors must be constexpr");
static_assert(matrix4x4_t<int>::zero().m13 == 0, "yama constructors must be constexpr");

// mat4x4
static_assert(sizeof(matrix3x4_t<float>) == 12 * sizeof(float), "yama types must be identical to ntuples of T in size");
static_assert(sizeof(matrix3x4_t<double>) == 12 * sizeof(double), "yama types must be identical to ntuples of T in size");
static_assert(is_yama<matrix3x4_t<int>>::value, "yama::quaternion_t must be a yama type");
static_assert(!is_vector<matrix3x4_t<double>>::value, "yama::matrix3x4_t is not a vector type");
static_assert(is_matrix<matrix>::value, "yama::matrix3x4_t must be a matrix type");
static_assert(matrix3x4_t<int>::value_count == 12, "yama::matrix3x4_t has sixteen elements");
static_assert(matrix3x4_t<int>::columns(1, 2, 3, 5, 10, 8, 7, 6, 22, 23, 24, 25).m01 == 5, "yama constructors must be constexpr");
static_assert(matrix3x4_t<int>::uniform(2).m22 == 2, "yama constructors must be constexpr");
static_assert(matrix3x4_t<int>::zero().m13 == 0, "yama constructors must be constexpr");

// unions must compile
union foo
{
    matrix m4x4;
    matrix3x4 m3x4;
    vector2 v2;
    vector3 v3;
    vector4 v4;
    quaternion q;
};

TEST_SUITE_BEGIN("prerequisites");

class nrvo_test
{
public:
    static size_t copies;

    nrvo_test(int d)
        : dummy(d)
    {}

    nrvo_test(const nrvo_test& b)
        : dummy(b.dummy)
    {
        ++copies;
    }

    int dummy;

    static nrvo_test seven()
    {
        return nrvo_test(7);
    }
};

size_t nrvo_test::copies = 0;

TEST_CASE("nrvo")
{
    nrvo_test t = nrvo_test::seven();

    CHECK(t.dummy == 7);

    // IF THIS FAILS: This build configuration (or your compiler) doesn't support named return
    // value optimizations. MathGP will suffer big performance penalties because  of that
    WARN(nrvo_test::copies == 0);
}
