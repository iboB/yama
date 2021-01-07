// Yama
// Copyright (c) 2016-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "yama/matrix3x3.hpp"
#include "common.hpp"
#include "yama/ext/ostream.hpp"

using namespace yama;
using doctest::Approx;

TEST_SUITE_BEGIN("matrix3x3");

TEST_CASE("construction")
{
    double d0[] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
    };

    auto m0 = matrix3x3_t<double>::zero();
    CHECK(m0.m00 == 0);
    CHECK(m0.m10 == 0);
    CHECK(m0.m20 == 0);
    CHECK(m0.m01 == 0);
    CHECK(m0.m11 == 0);
    CHECK(m0.m21 == 0);
    CHECK(m0.m02 == 0);
    CHECK(m0.m12 == 0);
    CHECK(m0.m22 == 0);
    CHECK(memcmp(d0, &m0, 9 * sizeof(double)) == 0);

    float f1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    auto m1 = matrix3::columns(1, 2, 3, 4, 5, 6, 7, 8, 9);
    CHECK(m1.m00 == 1);
    CHECK(m1.m10 == 2);
    CHECK(m1.m20 == 3);
    CHECK(m1.m01 == 4);
    CHECK(m1.m11 == 5);
    CHECK(m1.m21 == 6);
    CHECK(m1.m02 == 7);
    CHECK(m1.m12 == 8);
    CHECK(m1.m22 == 9);
    CHECK(memcmp(f1, &m1, 9 * sizeof(float)) == 0);

    auto m2 = matrix3::rows(1, 4, 7, 2, 5, 8, 3, 6, 9);
    CHECK(m2.m00 == 1);
    CHECK(m2.m10 == 2);
    CHECK(m2.m20 == 3);
    CHECK(m2.m01 == 4);
    CHECK(m2.m11 == 5);
    CHECK(m2.m21 == 6);
    CHECK(m2.m02 == 7);
    CHECK(m2.m12 == 8);
    CHECK(m2.m22 == 9);
    CHECK(memcmp(f1, &m2, 9 * sizeof(float)) == 0);

    auto m3 = matrix3x3_t<float>::uniform(3);
    CHECK(m3.m00 == 3);
    CHECK(m3.m10 == 3);
    CHECK(m3.m20 == 3);
    CHECK(m3.m01 == 3);
    CHECK(m3.m11 == 3);
    CHECK(m3.m21 == 3);
    CHECK(m3.m02 == 3);
    CHECK(m3.m12 == 3);
    CHECK(m3.m22 == 3);

    auto m4 = matrix3::identity();
    CHECK(m4.m00 == 1);
    CHECK(m4.m10 == 0);
    CHECK(m4.m20 == 0);
    CHECK(m4.m01 == 0);
    CHECK(m4.m11 == 1);
    CHECK(m4.m21 == 0);
    CHECK(m4.m02 == 0);
    CHECK(m4.m12 == 0);
    CHECK(m4.m22 == 1);

    const float f[] = {
        9, 8, 7, 6, 5, 4, 3, 2, 1,
        18, 17, 16, 15, 14, 13, 12, 11, 10,
    };
    auto m5 = matrix3::from_ptr(f);
    CHECK(memcmp(&m5, f, 9 * sizeof(float)) == 0);

    // attach
    auto& m6 = matrix3::attach_to_ptr(f);
    CHECK(m6.m00 == 9);
    CHECK(m6.m10 == 8);
    CHECK(m6.m20 == 7);
    CHECK(m6.m01 == 6);
    CHECK(m6.m11 == 5);
    CHECK(m6.m21 == 4);
    CHECK(m6.m02 == 3);
    CHECK(m6.m12 == 2);
    CHECK(m6.m22 == 1);
    CHECK(reinterpret_cast<const float*>(&m6) == f);

    auto m7 = matrix3::attach_to_array(f);
    CHECK(m7[0].m00 == 9);
    CHECK(m7[0].m10 == 8);
    CHECK(m7[0].m20 == 7);
    CHECK(m7[0].m01 == 6);
    CHECK(m7[0].m11 == 5);
    CHECK(m7[0].m21 == 4);
    CHECK(m7[0].m02 == 3);
    CHECK(m7[0].m12 == 2);
    CHECK(m7[0].m22 == 1);
    CHECK(m7 == &m6);
    //CHECK(m7[0] == m6);
    CHECK(reinterpret_cast<const float*>(m7) == f);
}


TEST_CASE("compare")
{
    auto m0 = matrix3::zero();
    CHECK(m0 == matrix3::columns(0, 0, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix3::columns(1, 0, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix3::columns(0, 1, 0, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix3::columns(0, 0, 1, 0, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix3::columns(0, 0, 0, 1, 0, 0, 0, 0, 0));
    CHECK(m0 != matrix3::columns(0, 0, 0, 0, 1, 0, 0, 0, 0));
    CHECK(m0 != matrix3::columns(0, 0, 0, 0, 0, 1, 0, 0, 0));
    CHECK(m0 != matrix3::columns(0, 0, 0, 0, 0, 0, 1, 0, 0));
    CHECK(m0 != matrix3::columns(0, 0, 0, 0, 0, 0, 0, 1, 0));
    CHECK(m0 != matrix3::columns(0, 0, 0, 0, 0, 0, 0, 0, 1));

    matrix3 m1;
    m1.m00 = 11; m1.m10 = 12; m1.m20 = 13;
    m1.m01 = 14; m1.m11 = 15; m1.m21 = 16;
    m1.m02 = 17; m1.m12 = 18; m1.m22 = 19;

    CHECK(m1 == matrix3::columns(11, 12, 13, 14, 15, 16, 17, 18, 19));
    CHECK(m1 != m0);

    m0.m00 = 11; m0.m10 = 12; m0.m20 = 13;
    m0.m01 = 14; m0.m11 = 15; m0.m21 = 16;
    m0.m02 = 17; m0.m12 = 18; m0.m22 = 19;
    CHECK(m1 == m0);

    CHECK(close(m1, m0));
    CHECK(close(m0, m1));
    m0.m21 += 1;
    CHECK(!close(m0, m1));
    CHECK(close(m0, m1, 2.f));
    m0.m00 = 11.000001f; m0.m10 = 12.000001f; m0.m20 = 13.000001f;
    m0.m01 = 14.000001f; m0.m11 = 15.000001f; m0.m21 = 16.000001f;
    m0.m02 = 17.000001f; m0.m12 = 18.000001f; m0.m22 = 19.000001f;
    CHECK(close(m0, m1));
}

TEST_CASE("special_construction")
{
    auto m0 = matrix3::scaling_uniform(1);
    CHECK(m0 == matrix3::identity());
    CHECK(m0 == matrix3::scaling(1, 1, 1));

    m0.m00 = 5; m0.m11 = 5; m0.m22 = 5;
    CHECK(m0 == matrix3::scaling_uniform(5));
    CHECK(m0 == matrix3::scaling(5, 5, 5));
    CHECK(m0 == matrix3::scaling(v(5, 5, 5)));

    m0.m00 = 2; m0.m11 = 4; m0.m22 = 6;
    CHECK(m0 == matrix3::scaling(2, 4, 6));
    CHECK(m0 == matrix3::scaling(v(2, 4, 6)));
}

TEST_CASE("access")
{
    auto m0 = matrix3::columns(1, 2, 3, 4, 5, 6, 7, 8, 9);
    CHECK(m0.data() == reinterpret_cast<const float*>(&m0));
    CHECK(m0.data()[0] == 1);
    CHECK(m0.data()[1] == 2);
    CHECK(m0.data()[2] == 3);
    CHECK(m0[3] == 4);
    CHECK(m0[4] == 5);
    CHECK(m0[5] == 6);
    CHECK(m0.at(6) == 7);
    CHECK(m0.at(7) == 8);
    CHECK(m0.at(8) == 9);
    CHECK(m0.data() == m0.as_ptr());
    CHECK(m0.data() == m0.column(0));
    CHECK(m0.data() + 3 == m0.column(1));
    CHECK(m0.data() + 6 == m0.column(2));
    CHECK(m0(0, 2) == 7);
    CHECK(m0(1, 2) == 8);
    CHECK(m0.m(2, 1) == 6);
    CHECK(m0.m(2, 2) == 9);

    CHECK(m0.as_matrix3x3_t<int>() == matrix3x3_t<int>::columns(1, 2, 3, 4, 5, 6, 7, 8, 9));

    m0.column_vector(0) = v(20, 30, 40);
    CHECK(m0.m00 == 20);
    CHECK(m0.m20 == 40);
    CHECK(m0.column_vector<2>(0) == v(20, 30));
    m0.column_vector<3>(1) = v(-1, -2, -3);
    CHECK(m0.m01 == -1);
    CHECK(m0.m21 == -3);
    CHECK(m0.column_vector<2>(1, 1) == v(-2, -3));

    CHECK(m0.row_vector(0) == v(20, -1, 7));
    CHECK(m0.row_vector<2>(1, 1) == v(-2, 8));

    CHECK(m0.main_diagonal() == v(20, -2, 9));
    CHECK(m0.main_diagonal<2>(1) == v(-2, 9));

    const auto m1 = matrix3::columns(1, 2, 3, 4, 5, 6, 7, 8, 9);
    CHECK(m1.data() == reinterpret_cast<const float*>(&m1));
    CHECK(m1.data()[0] == 1);
    CHECK(m1.data()[1] == 2);
    CHECK(m1.data()[2] == 3);
    CHECK(m1[3] == 4);
    CHECK(m1[4] == 5);
    CHECK(m1[5] == 6);
    CHECK(m1.at(6) == 7);
    CHECK(m1.at(7) == 8);
    CHECK(m1.at(8) == 9);
    CHECK(m1.data() == m1.as_ptr());
    CHECK(m1.data() == m1.column(0));
    CHECK(m1.data() + 3 == m1.column(1));
    CHECK(m1.data() + 6 == m1.column(2));
    CHECK(m1(0, 2) == 7);
    CHECK(m1(1, 2) == 8);
    CHECK(m1.m(2, 1) == 6);
    CHECK(m1.m(2, 2) == 9);

    CHECK(m1.as_matrix3x3_t<int>() == matrix3x3_t<int>::columns(1, 2, 3, 4, 5, 6, 7, 8, 9));

    CHECK(m1.column_vector<2>(0) == v(1, 2));
    CHECK(m1.column_vector<2>(1, 1) == v(5, 6));

    CHECK(m1.row_vector(0) == v(1, 4, 7));
    CHECK(m1.row_vector<2>(1, 1) == v(5, 8));

    CHECK(m1.main_diagonal() == v(1, 5, 9));
    CHECK(m1.main_diagonal<2>(1) == v(5, 9));
}


TEST_CASE("std")
{
    auto m0 = matrix3::columns(1, 2, 3, 4, 5, 6, 7, 8, 9);
    CHECK(m0.front() == 1);
    CHECK(m0.back() == 9);
    CHECK(m0.begin() == m0.data());
    CHECK(m0.begin() + 9 == m0.end());
    CHECK(m0.rbegin() + 9 == m0.rend());
    auto i = m0.rbegin();
    CHECK(*i == 9);
    ++i;
    CHECK(*i == 8);

    for (auto& e : m0)
    {
        e += 10;
    }

    const auto m1 = m0;

    CHECK(m1.front() == 11);
    CHECK(m1.back() == 19);
    CHECK(m1.begin() == m1.data());
    CHECK(m1.begin() + 9 == m1.end());
    CHECK(m1.rbegin() + 9 == m1.rend());
    auto ci = m1.rbegin();
    CHECK(*ci == 19);
    ++ci;
    CHECK(*ci == 18);
}


TEST_CASE("members")
{
    auto m0 = matrix3::columns(1, 2, 3, 4, 5, 6, 7, 8, 9);
    CHECK(m0 == +m0);
    auto m1 = matrix3::columns(-1, -2, -3, -4, -5, -6, -7, -8, -9);
    CHECK(-m0 == m1);
    CHECK(-m1 == m0);

    m0 += m1;
    CHECK(m0 == matrix3::zero());

    m0 -= m1;
    CHECK(m0 == -m1);

    m1 = m0;
    m0 *= 2;
    CHECK(m0 == matrix3::columns(2, 4, 6, 8, 10, 12, 14, 16, 18));

    m0 /= 2;
    CHECK(m0 == m1);

    m0.div(m1);
    CHECK(m0 == matrix3::uniform(1));

    m0.mul(m1);
    CHECK(m0 == m1);

    m0 *= matrix3::identity();
    CHECK(m0 == m1);
    CHECK(matrix3::identity().determinant() == 1);

    auto m2 = matrix3::identity();
    m2.inverse();
    CHECK(m2 == matrix3::identity());
    m2.transpose();
    CHECK(m2 == matrix3::identity());

    m0.transpose();
    CHECK(m0 == matrix3::rows(1, 2, 3, 4, 5, 6, 7, 8, 9));

    m0 = m1 = matrix3::rows(
        1, 2, 3,
        5, 3, 2,
        2, 1, 1
    );

    auto det = m1.inverse();
    CHECK(det == -4);
    m0 *= m1;
    CHECK(YamaApprox(m0) == matrix3::identity());

    m0 = m2 = matrix3::rows(3, 22, 12, 5, 17, 8, 24, 6, 19);
    det = m2.inverse();
    CHECK(det == -1577);
    m0 *= m2;
    CHECK(YamaApprox(m0) == matrix3::identity());
}

TEST_CASE("ops")
{
    const auto m0 = matrix3::columns(1, 2, 3, 4, 5, 6, 7, 8, 9);
    auto m1 = matrix3::columns(-1, -2, -3, -4, -5, -6, -7, -8, -9);
    auto m3 = matrix3::columns(2, 4, 6, 8, 10, 12, 14, 16, 18);

    CHECK(m0 + m1 == matrix3::zero());
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
    auto m2 = matrix3::columns(
        16, 8, 5.333333f,
        4, 3.2f, 2.666667f,
        2.285714f, 2, 1.777778f);
    CHECK(YamaApprox(m1) == m2);

    m1 = div(m3, m0);
    CHECK(m1 == matrix3::uniform(2));
    CHECK(mul(m0, m1) == m3);

    CHECK(isfinite(m0));
    CHECK(isfinite(m1));
    CHECK(isfinite(m2));
    CHECK(isfinite(m3));

    m1.m11 = std::numeric_limits<float>::infinity();
    CHECK(!isfinite(m1));

    m2.m20 = std::numeric_limits<float>::quiet_NaN();
    CHECK(!isfinite(m2));

    CHECK(!isfinite(m1 + m2));

    m1 = matrix3::rows(
        1, 2, 3,
        5, 3, 2,
        2, 1, 1
    );

    CHECK(m1 * matrix3::identity() == m1);
    CHECK(matrix3::identity() * m1 == m1);

    float det;
    m2 = inverse(m1, det);
    CHECK(det == -4);
    CHECK(YamaApprox(m1 * m2) == matrix3::identity());
    CHECK(YamaApprox(m2 * m1) == matrix3::identity());
}
