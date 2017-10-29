// Yama
// Copyright (c) 2016 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "common.hpp"

using namespace yama;
using doctest::Approx;

TEST_SUITE("matrix4x4");

TEST_CASE("construction")
{
    double d0[] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
    };

    auto m0 = matrix4x4_t<double>::zero();
    CHECK(m0.m00 == 0);
    CHECK(m0.m10 == 0);
    CHECK(m0.m20 == 0);
    CHECK(m0.m30 == 0);
    CHECK(m0.m01 == 0);
    CHECK(m0.m11 == 0);
    CHECK(m0.m21 == 0);
    CHECK(m0.m31 == 0);
    CHECK(m0.m02 == 0);
    CHECK(m0.m12 == 0);
    CHECK(m0.m22 == 0);
    CHECK(m0.m32 == 0);
    CHECK(m0.m03 == 0);
    CHECK(m0.m13 == 0);
    CHECK(m0.m23 == 0);
    CHECK(m0.m33 == 0);
    CHECK(memcmp(d0, &m0, 16 * sizeof(double)) == 0);

    float f1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    auto m1 = matrix::columns(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    CHECK(m1.m00 == 1);
    CHECK(m1.m10 == 2);
    CHECK(m1.m20 == 3);
    CHECK(m1.m30 == 4);
    CHECK(m1.m01 == 5);
    CHECK(m1.m11 == 6);
    CHECK(m1.m21 == 7);
    CHECK(m1.m31 == 8);
    CHECK(m1.m02 == 9);
    CHECK(m1.m12 == 10);
    CHECK(m1.m22 == 11);
    CHECK(m1.m32 == 12);
    CHECK(m1.m03 == 13);
    CHECK(m1.m13 == 14);
    CHECK(m1.m23 == 15);
    CHECK(m1.m33 == 16);
    CHECK(memcmp(f1, &m1, 16 * sizeof(float)) == 0);

    auto m2 = matrix::rows(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16);
    CHECK(m2.m00 == 1);
    CHECK(m2.m10 == 2);
    CHECK(m2.m20 == 3);
    CHECK(m2.m30 == 4);
    CHECK(m2.m01 == 5);
    CHECK(m2.m11 == 6);
    CHECK(m2.m21 == 7);
    CHECK(m2.m31 == 8);
    CHECK(m2.m02 == 9);
    CHECK(m2.m12 == 10);
    CHECK(m2.m22 == 11);
    CHECK(m2.m32 == 12);
    CHECK(m2.m03 == 13);
    CHECK(m2.m13 == 14);
    CHECK(m2.m23 == 15);
    CHECK(m2.m33 == 16);
    CHECK(memcmp(f1, &m1, 16 * sizeof(float)) == 0);

    auto m3 = matrix4x4_t<float>::uniform(3);
    CHECK(m3.m00 == 3);
    CHECK(m3.m10 == 3);
    CHECK(m3.m20 == 3);
    CHECK(m3.m30 == 3);
    CHECK(m3.m01 == 3);
    CHECK(m3.m11 == 3);
    CHECK(m3.m21 == 3);
    CHECK(m3.m31 == 3);
    CHECK(m3.m02 == 3);
    CHECK(m3.m12 == 3);
    CHECK(m3.m22 == 3);
    CHECK(m3.m32 == 3);
    CHECK(m3.m03 == 3);
    CHECK(m3.m13 == 3);
    CHECK(m3.m23 == 3);
    CHECK(m3.m33 == 3);

    auto m4 = matrix::identity();
    CHECK(m4.m00 == 1);
    CHECK(m4.m10 == 0);
    CHECK(m4.m20 == 0);
    CHECK(m4.m30 == 0);
    CHECK(m4.m01 == 0);
    CHECK(m4.m11 == 1);
    CHECK(m4.m21 == 0);
    CHECK(m4.m31 == 0);
    CHECK(m4.m02 == 0);
    CHECK(m4.m12 == 0);
    CHECK(m4.m22 == 1);
    CHECK(m4.m32 == 0);
    CHECK(m4.m03 == 0);
    CHECK(m4.m13 == 0);
    CHECK(m4.m23 == 0);
    CHECK(m4.m33 == 1);

    const float f[] = {
        16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
        36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21
    };
    auto m5 = matrix::from_ptr(f);
    CHECK(memcmp(&m5, f, 16 * sizeof(float)) == 0);

    // attach
    auto& m6 = matrix::attach_to_ptr(f);
    CHECK(m6.m00 == 16);
    CHECK(m6.m10 == 15);
    CHECK(m6.m20 == 14);
    CHECK(m6.m30 == 13);
    CHECK(m6.m01 == 12);
    CHECK(m6.m11 == 11);
    CHECK(m6.m21 == 10);
    CHECK(m6.m31 == 9);
    CHECK(m6.m02 == 8);
    CHECK(m6.m12 == 7);
    CHECK(m6.m22 == 6);
    CHECK(m6.m32 == 5);
    CHECK(m6.m03 == 4);
    CHECK(m6.m13 == 3);
    CHECK(m6.m23 == 2);
    CHECK(m6.m33 == 1);
    CHECK(reinterpret_cast<const float*>(&m6) == f);

    auto m7 = matrix::attach_to_array(f);
    CHECK(m7[0].m00 == 16);
    CHECK(m7[0].m10 == 15);
    CHECK(m7[0].m20 == 14);
    CHECK(m7[0].m30 == 13);
    CHECK(m7[0].m01 == 12);
    CHECK(m7[0].m11 == 11);
    CHECK(m7[0].m21 == 10);
    CHECK(m7[0].m31 == 9);
    CHECK(m7[0].m02 == 8);
    CHECK(m7[0].m12 == 7);
    CHECK(m7[0].m22 == 6);
    CHECK(m7[0].m32 == 5);
    CHECK(m7[0].m03 == 4);
    CHECK(m7[0].m13 == 3);
    CHECK(m7[0].m23 == 2);
    CHECK(m7[0].m33 == 1);
    CHECK(m7 == &m6);
    //CHECK(m7[0] == m6);
    CHECK(reinterpret_cast<const float*>(m7) == f);

    float ff[] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
        21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
    };
    auto& m8 = matrix::attach_to_ptr(ff);
    m8.m11 = 34;
    m8.m03 = 77;
    CHECK(ff[5] == 34);
    CHECK(ff[12] == 77);

    auto m9 = matrix::attach_to_array(ff);
    m9[0].m22 = 21;
    m9[1].m00 = 30;
    CHECK(ff[10] == 21);
    CHECK(ff[16] == 30);
}


TEST_CASE("compare")
{
    auto m0 = matrix::zero();
    CHECK(m0 == matrix::columns(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0));
    CHECK(m0 != matrix::columns(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));

    matrix m1;
    m1.m00 = 11; m1.m10 = 12; m1.m20 = 13; m1.m30 = 14;
    m1.m01 = 15; m1.m11 = 16; m1.m21 = 17; m1.m31 = 18;
    m1.m02 = 19; m1.m12 = 20; m1.m22 = 21; m1.m32 = 22;
    m1.m03 = 23; m1.m13 = 24; m1.m23 = 25; m1.m33 = 26;
    CHECK(m1 == matrix::columns(11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26));
    CHECK(m1 != m0);

    m0.m00 = 11; m0.m10 = 12; m0.m20 = 13; m0.m30 = 14;
    m0.m01 = 15; m0.m11 = 16; m0.m21 = 17; m0.m31 = 18;
    m0.m02 = 19; m0.m12 = 20; m0.m22 = 21; m0.m32 = 22;
    m0.m03 = 23; m0.m13 = 24; m0.m23 = 25; m0.m33 = 26;
    CHECK(m1 == m0);

    CHECK(close(m1, m0));
    CHECK(close(m0, m1));
    m0.m21 += 1;
    CHECK(!close(m0, m1));
    CHECK(close(m0, m1, 2.f));
    m0.m00 = 11.000001f; m0.m10 = 12.000001f; m0.m20 = 13.000001f; m0.m30 = 14.000001f;
    m0.m01 = 15.000001f; m0.m11 = 16.000001f; m0.m21 = 17.000001f; m0.m31 = 18.000001f;
    m0.m02 = 19.000001f; m0.m12 = 20.000001f; m0.m22 = 21.000001f; m0.m32 = 22.000001f;
    m0.m03 = 23.000001f; m0.m13 = 24.000001f; m0.m23 = 25.000001f; m0.m33 = 26.000001f;
    CHECK(close(m0, m1));
}

TEST_CASE("special_construction")
{
    auto m0 = matrix::translation(0, 0, 0);
    CHECK(m0 == matrix::identity());

    m0.m03 = 1; m0.m13 = 2; m0.m23 = 3;
    CHECK(m0 == matrix::translation(v(1, 2, 3)));
    CHECK(m0 == matrix::translation(1, 2, 3));

    m0 = matrix::scaling_uniform(1);
    CHECK(m0 == matrix::identity());
    CHECK(m0 == matrix::scaling(1, 1, 1));

    m0.m00 = 5; m0.m11 = 5; m0.m22 = 5;
    CHECK(m0 == matrix::scaling_uniform(5));
    CHECK(m0 == matrix::scaling(5, 5, 5));
    CHECK(m0 == matrix::scaling(v(5, 5, 5)));

    m0.m00 = 2; m0.m11 = 4; m0.m22 = 6;
    CHECK(m0 == matrix::scaling(2, 4, 6));
    CHECK(m0 == matrix::scaling(v(2, 4, 6)));

	m0 = matrix::basis_transform(vector3::zero(), vector3::unit_x(), vector3::unit_y(), vector3::unit_z());
	CHECK(m0 == matrix::identity());
}

TEST_CASE("access")
{
    auto m0 = matrix::columns(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    CHECK(m0.data() == reinterpret_cast<const float*>(&m0));
    CHECK(m0.data()[0] == 1);
    CHECK(m0.data()[1] == 2);
    CHECK(m0.data()[2] == 3);
    CHECK(m0.data()[3] == 4);
    CHECK(m0[4] == 5);
    CHECK(m0[5] == 6);
    CHECK(m0[6] == 7);
    CHECK(m0[7] == 8);
    CHECK(m0.at(8) == 9);
    CHECK(m0.at(9) == 10);
    CHECK(m0.at(10) == 11);
    CHECK(m0.at(11) == 12);
    CHECK(m0.data() == m0.as_ptr());
    CHECK(m0.data() == m0.column(0));
    CHECK(m0.data() + 4 == m0.column(1));
    CHECK(m0.data() + 8 == m0.column(2));
    CHECK(m0.data() + 12 == m0.column(3));
    CHECK(m0(0, 3) == 13);
    CHECK(m0(1, 3) == 14);
    CHECK(m0.m(2, 3) == 15);
    CHECK(m0.m(3, 3) == 16);

    CHECK(m0.as_matrix4x4_t<int>() == matrix4x4_t<int>::columns(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));

    m0.column_vector(0) = v(20, 30, 40, 50);
    CHECK(m0.m00 == 20);
    CHECK(m0.m30 == 50);
    CHECK(m0.column_vector<2>(0) == v(20, 30));
    m0.column_vector<4>(1) = v(-1, -2, -3, -4);
    CHECK(m0.m01 == -1);
    CHECK(m0.m31 == -4);
    CHECK(m0.column_vector<3>(1, 1) == v(-2, -3, -4));

    CHECK(m0.row_vector(0) == v(20, -1, 9, 13));
    CHECK(m0.row_vector<2>(2, 2) == v(11, 15));

    CHECK(m0.main_diagonal() == v(20, -2, 11, 16));
    CHECK(m0.main_diagonal<3>(1) == v(-2, 11, 16));

    const auto m1 = matrix::columns(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    CHECK(m1.data() == reinterpret_cast<const float*>(&m1));
    CHECK(m1.data()[0] == 1);
    CHECK(m1.data()[1] == 2);
    CHECK(m1.data()[2] == 3);
    CHECK(m1.data()[3] == 4);
    CHECK(m1[4] == 5);
    CHECK(m1[5] == 6);
    CHECK(m1[6] == 7);
    CHECK(m1[7] == 8);
    CHECK(m1.at(8) == 9);
    CHECK(m1.at(9) == 10);
    CHECK(m1.at(10) == 11);
    CHECK(m1.at(11) == 12);
    CHECK(m1.data() == m1.as_ptr());
    CHECK(m1.data() == m1.column(0));
    CHECK(m1.data() + 4 == m1.column(1));
    CHECK(m1.data() + 8 == m1.column(2));
    CHECK(m1.data() + 12 == m1.column(3));
    CHECK(m1(0, 3) == 13);
    CHECK(m1(1, 3) == 14);
    CHECK(m1.m(2, 3) == 15);
    CHECK(m1.m(3, 3) == 16);

    CHECK(m1.as_matrix4x4_t<int>() == matrix4x4_t<int>::columns(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));

    CHECK(m1.column_vector<2>(0) == v(1, 2));
    CHECK(m1.column_vector<3>(1, 1) == v(6, 7, 8));

    CHECK(m1.row_vector(0) == v(1, 5, 9, 13));
    CHECK(m1.row_vector<2>(2, 2) == v(11, 15));

    CHECK(m1.main_diagonal() == v(1, 6, 11, 16));
    CHECK(m1.main_diagonal<3>(1) == v(6, 11, 16));
}


TEST_CASE("std")
{
    auto m0 = matrix::columns(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    CHECK(m0.front() == 1);
    CHECK(m0.back() == 16);
    CHECK(m0.begin() == m0.data());
    CHECK(m0.begin() + 16 == m0.end());
    CHECK(m0.rbegin() + 16 == m0.rend());
    auto i = m0.rbegin();
    CHECK(*i == 16);
    ++i;
    CHECK(*i == 15);

    for (auto& e : m0)
    {
        e += 10;
    }

    const auto m1 = m0;

    CHECK(m1.front() == 11);
    CHECK(m1.back() == 26);
    CHECK(m1.begin() == m1.data());
    CHECK(m1.begin() + 16 == m1.end());
    CHECK(m1.rbegin() + 16 == m1.rend());
    auto ci = m1.rbegin();
    CHECK(*ci == 26);
    ++ci;
    CHECK(*ci == 25);
}


TEST_CASE("members")
{
    auto m0 = matrix::columns(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    CHECK(m0 == +m0);
    auto m1 = matrix::columns(-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16);
    CHECK(-m0 == m1);
    CHECK(-m1 == m0);

    m0 += m1;
    CHECK(m0 == matrix::zero());

    m0 -= m1;
    CHECK(m0 == -m1);

    m1 = m0;
    m0 *= 2;
    CHECK(m0 == matrix::columns(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32));

    m0 /= 2;
    CHECK(m0 == m1);

    m0.div(m1);
    CHECK(m0 == matrix::uniform(1));

    m0.mul(m1);
    CHECK(m0 == m1);

    m0 *= matrix::identity();
    CHECK(m0 == m1);
    CHECK(matrix::identity().determinant() == 1);

    auto m2 = matrix::identity();
    m2.inverse();
    CHECK(m2 == matrix::identity());
    m2.transpose();
    CHECK(m2 == matrix::identity());

    m0.transpose();
    CHECK(m0 == matrix::rows(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));

    m0 = m1 = matrix::rows(
        1, 2, 3, 4,
        5, 3, 2, 2,
        2, 1, 1, 1,
        5, 6, 10, 2
    );

    auto det = m1.inverse();
    CHECK(det == 43);
    m0 *= m1;
    CHECK(YamaApprox(m0) == matrix::identity());

    m0 = m2 = matrix::rows(3, 22, 12, 5, 17, 8, 24, 6, 19, 27, 3, 7, 10, 7, 11, 8);
    det = m2.inverse();
    CHECK(det == 47634);
    m0 *= m2;
    CHECK(YamaApprox(m0) == matrix::identity());
}

TEST_CASE("ops")
{
    const auto m0 = matrix::columns(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    auto m1 = matrix::columns(-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16);
    auto m3 = matrix::columns(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32);

    CHECK(m0 + m1 == matrix::zero());
    CHECK(m0 - m1 == m3);

    CHECK(abs(m1) == m0);
    CHECK(abs(m0) == m0);

    m1 = m0 * 2.f;
    CHECK(m1 == m3);

    m1 = 2.f * m0;
    CHECK(m1 == m3);

    m1 = m3 / 2.f;
    CHECK(m1 == m0);

    m1 = 16.f / m0;
    auto m2 = matrix::columns(
        16, 8, 5.333333f, 4, 3.2f,
        2.666667f, 2.285714f, 2, 1.777778f,
        1.6f, 1.454545f, 1.333333f,
        1.230769f, 1.142857f, 1.066667f, 1);
    CHECK(YamaApprox(m1) == m2);

    m1 = div(m3, m0);
    CHECK(m1 == matrix::uniform(2));
    CHECK(mul(m0, m1) == m3);

    CHECK(isfinite(m0));
    CHECK(isfinite(m1));
    CHECK(isfinite(m2));
    CHECK(isfinite(m3));

    m1.m11 = std::numeric_limits<float>::infinity();
    CHECK(!isfinite(m1));

    m2.m30 = std::numeric_limits<float>::quiet_NaN();
    CHECK(!isfinite(m2));

    CHECK(!isfinite(m1 + m2));

    m1 = matrix::rows(
        1, 2, 3, 4,
        5, 3, 2, 2,
        2, 1, 1, 1,
        5, 6, 10, 2
    );

    CHECK(m1 * matrix::identity() == m1);
    CHECK(matrix::identity() * m1 == m1);

    float det;
    m2 = inverse(m1, det);
    CHECK(det == 43);
    CHECK(YamaApprox(m1 * m2) == matrix::identity());
    CHECK(YamaApprox(m2 * m1) == matrix::identity());
}

TEST_CASE("transform")
{
    const auto i = matrix::identity();
    vector3 vec = v(1, 2, 3);
    CHECK(transform_coord(vec, i) == vec);

    auto t = matrix::translation(3, 1, -2);
    CHECK(transform_coord(vec, t) == v(4, 3, 1));

    t = matrix::translation(v(3, 1, -2));
    CHECK(transform_coord(vec, t) == v(4, 3, 1));

    auto s = matrix::scaling_uniform(2);
    CHECK(transform_coord(vec, s) == v(2, 4, 6));

    s = matrix::scaling(2, 0.5f, 1);
    CHECK(transform_coord(vec, s) == v(2, 1, 3));

    s = matrix::scaling(v(1, 0.5f, 0.33333333f));
    CHECK(YamaApprox(transform_coord(vec, s)) == v(1, 1, 1));

    const auto ux = vector3::unit_x();
    const auto uy = vector3::unit_y();
    const auto uz = vector3::unit_z();

    CHECK(YamaApprox(matrix::rotation_axis(ux, 2.11f)) == matrix::rotation_x(2.11f));
    CHECK(YamaApprox(matrix::rotation_axis(uy, 0.13f)) == matrix::rotation_y(0.13f));
    CHECK(YamaApprox(matrix::rotation_axis(uz, 1.22f)) == matrix::rotation_z(1.22f));

    auto q0 = quaternion::rotation_x(constants::PI_HALF());
    auto m1 = matrix::rotation_quaternion(q0);

    auto m0 = matrix::rotation_x(constants::PI_HALF());
    CHECK(YamaApprox(m1) == m0);
    CHECK(YamaApprox(transform_coord(ux, m0)) == ux);
    CHECK(YamaApprox(transform_coord(uy, m0)) == uz);
    CHECK(YamaApprox(transform_coord(uz, m0)) == -uy);

    q0 = quaternion::rotation_y(constants::PI_HALF());
    m1 = matrix::rotation_quaternion(q0);
    m0 = matrix::rotation_y(constants::PI_HALF());
    CHECK(YamaApprox(m1) == m0);
    CHECK(YamaApprox(transform_coord(ux, m0)) == -uz);
    CHECK(YamaApprox(transform_coord(uy, m0)) == uy);
    CHECK(YamaApprox(transform_coord(uz, m0)) == ux);

    q0 = quaternion::rotation_z(constants::PI_HALF());
    m1 = matrix::rotation_quaternion(q0);
    m0 = matrix::rotation_z(constants::PI_HALF());
    CHECK(YamaApprox(m1) == m0);
    CHECK(YamaApprox(transform_coord(ux, m0)) == uy);
    CHECK(YamaApprox(transform_coord(uy, m0)) == -ux);
    CHECK(YamaApprox(transform_coord(uz, m0)) == uz);

    auto axis = v(1, 2, 3);
    auto angle = 2.66f;

    m0 = matrix::rotation_axis(axis, angle);

    q0 = quaternion::rotation_axis(axis, angle);
    m1 = matrix::rotation_quaternion(q0);
    CHECK(YamaApprox(m1) == m0);

    auto v0 = normalize(v(1, 2, 3));

    CHECK(matrix::rotation_vectors(v0, v0) == matrix::identity());

    auto v1 = normalize(v(-3, 5, 11));

    m0 = matrix::rotation_vectors(v0, v1);
    CHECK(YamaApprox(transform_coord(v0, m0)) == v1);

    v1 = -v0;
    m0 = matrix::rotation_vectors(v0, v1);
    CHECK(YamaApprox(transform_coord(v(1, 2, 3), m0)) == v(-1, -2, -3));
}

TEST_CASE("camera")
{
    auto p = matrix::ortho_lh(2, 2, 0, 1);
    CHECK(p == matrix::identity());
    p = matrix::ortho_rh(2, 2, 0, 1);
    CHECK(abs(p) == matrix::identity());

    p = matrix::ortho_lh_cube(2, 2, -1, 1);
    CHECK(p == matrix::identity());
    p = matrix::ortho_rh_cube(2, 2, -1, 1);
    CHECK(abs(p) == matrix::identity());
    CHECK(p == matrix::ortho_rh_cube(-1, 1, -1, 1, -1, 1));

    auto vec0 = v(0.5f, 0.5f, 0.5f);
    p = matrix::ortho_lh(1, 1, 0.5f, 3);
    CHECK(transform_coord(vec0, p) == v(1, 1, 0));

    p = matrix::ortho_lh_cube(1, 1, 0, 1);
    CHECK(transform_coord(vec0, p) == v(1, 1, 0));

    p = matrix::ortho_rh(1, 1, 0, 1);
    CHECK(transform_coord(vec0, p) == v(1, 1, -0.5f));

    p = matrix::ortho_rh_cube(1, 1, 0, 1);
    CHECK(transform_coord(vec0, p) == v(1, 1, -2));
    CHECK(p == matrix::ortho_rh_cube(-0.5f, 0.5f, -0.5f, 0.5f, 0, 1));

	p = matrix::ortho_lh(-1, 1, -1, 1, 0, 1);
	CHECK(p == matrix::identity());

    p = matrix::ortho_lh_cube(-1, 1, -1, 1, -1, 1);
    CHECK(p == matrix::identity());

    auto v0 = matrix::look_towards_lh(vector3::zero(), 2.f * vector3::unit_z(), 0.2f * vector3::unit_y());
    CHECK(YamaApprox(v0) == matrix::identity());

    v0 = matrix::look_at_lh(vector3::zero(), 2.f * vector3::unit_z(), 0.2f * vector3::unit_y());
    CHECK(YamaApprox(v0) == matrix::identity());

    p = matrix::ortho_rh(4, 4, 0, 1);
    v0 = matrix::look_towards_rh(v(1, 1, 1), v(1, 0, 0), v(0, -1, 0));
    vec0 = v(1.5f, 2, 3);

    auto pv = p * v0;

    CHECK(transform_coord(vec0, pv) == v(-1, -0.5f, 0.5f));

    p = matrix::ortho_rh_cube(4, 4, 0, 1);
    v0 = matrix::look_at_rh(v(1, 1, 1), v(5, 1, 1), v(0, 1, 0));
    pv = p * v0;
    CHECK(transform_coord(vec0, pv) == v(1, 0.5f, 0));

    p = matrix::perspective_lh(8, 6, 3, 10);
    CHECK(p == matrix::perspective_lh(-4, 4, -3, 3, 3, 10));
    CHECK(YamaApprox(p) == matrix::perspective_fov_lh(constants::PI_HALF(), 4.f / 3.f, 3, 10));

    vec0 = v(8, 3, 6);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(1, 0.5f, 0.71428571428f));
    vec0 = v(0, 0, 3);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(0, 0, 0));
    vec0 = v(0, 0, 10);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(0, 0, 1));

    p = matrix::perspective_lh_cube(8, 6, 3, 10);
    CHECK(p == matrix::perspective_lh_cube(-4, 4, -3, 3, 3, 10));
    CHECK(YamaApprox(p) == matrix::perspective_fov_lh_cube(constants::PI_HALF(), 4.f / 3.f, 3, 10));

    vec0 = v(8, 3, 6);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(1, 0.5f, 0.4285714285f));
    vec0 = v(0, 0, 3);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(0, 0, -1));
    vec0 = v(0, 0, 10);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(0, 0, 1));


    p = matrix::perspective_rh(8, 6, 3, 10);
    CHECK(p == matrix::perspective_rh(-4, 4, -3, 3, 3, 10));
    CHECK(YamaApprox(p) == matrix::perspective_fov_rh(constants::PI_HALF(), 4.f / 3.f, 3, 10));
    p *= matrix::look_towards_rh(v(0, 0, 0), v(0, 0, -1), v(0, 1, 0));

    vec0 = v(8, 3, -6);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(1, 0.5f, 0.71428571428f));
    vec0 = v(0, 0, -3);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(0, 0, 0));
    vec0 = v(0, 0, -10);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(0, 0, 1));

    p = matrix::perspective_rh_cube(8, 6, 3, 10);
    CHECK(p == matrix::perspective_rh_cube(-4, 4, -3, 3, 3, 10));
    CHECK(YamaApprox(p) == matrix::perspective_fov_rh_cube(constants::PI_HALF(), 4.f / 3.f, 3, 10));
    p *= matrix::look_towards_rh(v(0, 0, 0), v(0, 0, -1), v(0, 1, 0));

    vec0 = v(8, 3, -6);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(1, 0.5f, 0.4285714285f));
    vec0 = v(0, 0, -3);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(0, 0, -1));
    vec0 = v(0, 0, -10);
    CHECK(YamaApprox(transform_coord(vec0, p)) == v(0, 0, 1));
}