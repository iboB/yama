// Yama
// Copyright (c) 2016-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "yama/vector4.hpp"
#include "common.hpp"
#include "yama/ext/vector4_ostream.hpp"

using namespace yama;
using doctest::Approx;

TEST_SUITE("vector4");

TEST_CASE("construction")
{
    double d0[] = { 0, 0, 0, 0 };
    auto v0 = vector4_t<double>::zero();
    CHECK(v0.x == 0);
    CHECK(v0.y == 0);
    CHECK(v0.z == 0);
    CHECK(v0.w == 0);
    CHECK(memcmp(d0, &v0, 4 * sizeof(double)) == 0);

    float f1[] = { 1,2,3,4 };
    auto v1 = vector4::coord(1, 2, 3, 4);
    CHECK(v1.x == 1);
    CHECK(v1.y == 2);
    CHECK(v1.z == 3);
    CHECK(v1.w == 4);
    CHECK(memcmp(f1, &v1, 4 * sizeof(float)) == 0);

    auto v2 = vector4_t<float>::uniform(3);
    CHECK(v2.x == 3);
    CHECK(v2.x == v2.y);
    CHECK(v2.x == v2.z);
    CHECK(v2.x == v2.w);

    auto v3 = v(3, 1, 5, 2);
    CHECK(v3.x == 3);
    CHECK(v3.y == 1);
    CHECK(v3.z == 5);
    CHECK(v3.w == 2);

    const char* c4 = "1234";
    auto v4 = vt('1', '2', '3', '4');
    CHECK(v4.x == '1');
    CHECK(v4.y == '2');
    CHECK(v4.z == '3');
    CHECK(v4.w == '4');
    CHECK(sizeof(v4) == 4);
    CHECK(memcmp(c4, &v4, sizeof(v4)) == 0);

    const float f[] = { 10,9,8,7,6,5,4,3 };
    auto v5 = vector4::from_ptr(f);
    CHECK(memcmp(&v5, f, 4 * sizeof(float)) == 0);
    CHECK(v5.x == 10);
    CHECK(v5.y == 9);
    CHECK(v5.z == 8);
    CHECK(v5.w == 7);

    // attach
    auto& v6 = vector4::attach_to_ptr(f);
    CHECK(v6.x == 10);
    CHECK(v6.y == 9);
    CHECK(v6.z == 8);
    CHECK(v6.w == 7);
    CHECK(reinterpret_cast<const float*>(&v6) == f);

    auto v7 = vector4::attach_to_array(f);
    CHECK(v7[0].x == 10);
    CHECK(v7[0].y == 9);
    CHECK(v7[0].z == 8);
    CHECK(v7[0].w == 7);
    CHECK(v7[1].x == 6);
    CHECK(v7[1].y == 5);
    CHECK(v7[1].z == 4);
    CHECK(v7[1].w == 3);
    CHECK(v7 == &v6);
    CHECK(v7[0] == v6);
    CHECK(reinterpret_cast<const float*>(v7) == f);

    float ff[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    auto& v8 = vector4::attach_to_ptr(ff);
    v8.x = 10;
    v8.w = 20;
    CHECK(ff[0] == 10);
    CHECK(ff[3] == 20);

    auto v9 = vector4::attach_to_array(ff);
    v9[0].y = 21;
    v9[1].z = 30;
    CHECK(ff[1] == 21);
    CHECK(ff[6] == 30);
}

TEST_CASE("compare")
{
    auto v0 = vector4::zero();
    CHECK(v0 == v(0, 0, 0, 0));
    CHECK(v0 != v(1, 0, 0, 0));
    CHECK(v0 != v(0, 1, 0, 0));
    CHECK(v0 != v(0, 0, 1, 0));
    CHECK(v0 != v(0, 0, 0, 1));

    vector4 v1;
    v1.x = 10;
    v1.y = 20;
    v1.z = 30;
    v1.w = 40;
    CHECK(v1 == v(10, 20, 30, 40));
    CHECK(v1 != v(1, 20, 30, 40));
    CHECK(v1 != v(10, 1, 30, 40));
    CHECK(v1 != v(10, 20, 1, 40));
    CHECK(v1 != v(10, 20, 30, 1));

    CHECK(v0 != v1);

    v0.x = 10;
    v0.y = 20;
    v0.z = 30;
    v0.w = 40;

    CHECK(v0 == v1);

    CHECK(close(v0, v1));
    v1.x = 11;
    CHECK(!close(v0, v1));
    v1.y = 21;
    CHECK(!close(v0, v1));
    v1.z = 31;
    CHECK(!close(v0, v1));
    v1.w = 41;
    CHECK(close(v0, v1, 2.f));
    v1 = v(10.000001f, 20.000001f, 30.000001f, 40.000001f);
    CHECK(close(v0, v1));

    CHECK(vector4::unit_x() == v(1, 0, 0, 0));
    CHECK(vector4::unit_y() == v(0, 1, 0, 0));
    CHECK(vector4::unit_z() == v(0, 0, 1, 0));
    CHECK(vector4::unit_w() == v(0, 0, 0, 1));
}

TEST_CASE("special_construction")
{
}

TEST_CASE("access")
{
    auto v0 = v(1, 2, 3, 4);
    CHECK(v0.data() == reinterpret_cast<const float*>(&v0));
    CHECK(v0.data()[0] == 1);
    CHECK(v0.data()[1] == 2);
    CHECK(v0.data()[2] == 3);
    CHECK(v0.data()[3] == 4);
    CHECK(v0[0] == 1);
    CHECK(v0[1] == 2);
    CHECK(v0[2] == 3);
    CHECK(v0[3] == 4);
    CHECK(v0.at(0) == 1);
    CHECK(v0.at(1) == 2);
    CHECK(v0.at(2) == 3);
    CHECK(v0.at(3) == 4);
    CHECK(v0.data() == v0.as_ptr());

    v0[0] = 10;
    v0.at(1) = 20;
    v0.z = 30;
    v0.data()[3] = 40;

    const auto v1 = v0;
    CHECK(v1.data() == reinterpret_cast<const float*>(&v1));
    CHECK(v1.data()[0] == 10);
    CHECK(v1.data()[1] == 20);
    CHECK(v1.data()[2] == 30);
    CHECK(v1.data()[3] == 40);
    CHECK(v1[0] == 10);
    CHECK(v1[1] == 20);
    CHECK(v1[2] == 30);
    CHECK(v1[3] == 40);
    CHECK(v1.at(0) == 10);
    CHECK(v1.at(1) == 20);
    CHECK(v1.at(2) == 30);
    CHECK(v1.at(3) == 40);
    CHECK(v1.data() == v1.as_ptr());

    v0.x = 0.3f;
    v0.y = 2.91f;
    v0.z = -11.123f;
    v0.w = -1.99f;

    CHECK(v0.as_vector4_t<int>() == vt(0, 2, -11, -1));
}

TEST_CASE("std")
{
    auto v0 = v(11, 12, 13, 14);
    CHECK(v0.front() == 11);
    CHECK(v0.back() == 14);
    CHECK(v0.begin() == v0.data());
    CHECK(v0.begin() + 4 == v0.end());
    CHECK(v0.rbegin() + 4 == v0.rend());
    auto i = v0.rbegin();
    CHECK(*i == 14);
    ++i;
    CHECK(*i == 13);

    for (auto& e : v0)
    {
        e += 10;
    }

    const auto v1 = v0;

    CHECK(v1.front() == 21);
    CHECK(v1.back() == 24);
    CHECK(v1.begin() == v1.data());
    CHECK(v1.begin() + 4 == v1.end());
    CHECK(v1.rbegin() + 4 == v1.rend());
    auto ci = v1.rbegin();
    CHECK(*ci == 24);
    ++ci;
    CHECK(*ci == 23);
}


TEST_CASE("members")
{
    auto v0 = v(1, 2, 3, 4);
    auto v1 = +v0;
    CHECK(v0 == v1);
    v1 = -v0;
    CHECK(v1.x == -v0.x);
    CHECK(v1.y == -v0.y);
    CHECK(v1.z == -v0.z);
    CHECK(v1.w == -v0.w);

    v0 += v1;
    CHECK(v0 == vector4::zero());

    v0 -= v1;
    CHECK(v0 == -v1);

    v0 *= 2;
    CHECK(v0 == v(2, 4, 6, 8));

    v0 /= 8;
    CHECK(YamaApprox(v0) == v(0.25f, 0.5f, 0.75f, 1));

    v1 *= -2;
    CHECK(v1 == v(2, 4, 6, 8));
    v0.mul(v1);
    CHECK(YamaApprox(v0) == v(0.5f, 2, 4.5f, 8));

    v1.div(v0);
    CHECK(YamaApprox(v1) == v(4, 2, 1.3333333f, 1));

    CHECK(Approx(v0.length_sq()) == 88.5f);
    v1 = v(1, 2, 8, 10);
    CHECK(Approx(v1.length()) == 13);

    CHECK(v1.manhattan_length() == 21);

    v1.normalize();
    CHECK(Approx(v1.length()) == 1);
    CHECK(Approx(v1.w) == 0.76923076923);
    CHECK(v1.is_normalized());

    v0 = v(1, 2, 4, 5);
    v0.homogenous_normalize();
    CHECK(YamaApprox(v0) == v(0.2f, 0.4f, 0.8f, 1));

    v0 = v(1, 1, 1, 1);
    CHECK(v0.reflection(v(0, 1, 0, 0)) == v(1, -1, 1, 1));
    CHECK(v0.reflection(v(0, 0, -1, 0)) == v(1, 1, -1, 1));

    v0 = v(1, -2, 5, 3);
    CHECK(v0.reflection(v(0, 1, 0, 0)) == v(1, 2, 5, 3));
    v0.normalize();
    CHECK(YamaApprox(v0.reflection(v0)) == -v0);
}

TEST_CASE("ops")
{
    auto v0 = v(1, 2, 3, 4);
    auto v1 = v(3, 4, 5, 6);

    auto v2 = v0 + v1;
    CHECK(v2 == v(4, 6, 8, 10));

    v1 = v2 - v0;
    CHECK(v1 == v(3, 4, 5, 6));

    v1 = 2.f * v0;
    CHECK(v1 == v(2, 4, 6, 8));

    v0 = v1 * 0.5f;
    CHECK(v0 == v(1, 2, 3, 4));

    v0 = 4.f / v1;
    CHECK(YamaApprox(v0) == v(2, 1, 0.6666666f, 0.5f));

    v1 = v0 / 2.f;
    CHECK(YamaApprox(v1) == v(1, 0.5f, 0.3333333f, 0.25f));

    v0 = abs(v1);
    CHECK(YamaApprox(v1) == v(1, 0.5f, 0.3333333f, 0.25f));

    v0 = abs(-v1);
    CHECK(YamaApprox(v1) == v(1, 0.5f, 0.3333333f, 0.25f));

    v0 = v(0.25f, 0.5f, 1.5f, 1.75f);
    v1 = v(8, 10, 12, 16);
    v2 = mul(v0, v1);
    CHECK(v2 == v(2, 5, 18, 28));

    v2 = div(v1, v(4, 2, 12, 8));
    CHECK(v2 == v(2, 5, 1, 2));

    auto iv0 = mod(vt(5, 6, 11, 8), vt(2, 4, 7, 4));
    CHECK(iv0 == vt(1, 2, 4, 0));

    CHECK(YamaApprox(mod(v(5.3f, 18.5f, 11, 8), v(2, 4.2f, 7, 4))) == v(1.3f, 1.7f, 4, 0));

    v1 = sign(v2);
    CHECK(v1 == v(1, 1, 1, 1));
    v1 = sign(-v2);
    CHECK(v1 == v(-1, -1, -1, -1));
    v1 = sign(v(-1, 2, -4, 0));
    CHECK(v1 == v(-1, 1, -1, 1));

    v1 = v(-1, 10, 6, 2);
    v0 = max(v1, v2);
    CHECK(v0 == v(2, 10, 6, 2));
    v0 = min(v1, v2);
    CHECK(v0 == v(-1, 5, 1, 2));

    CHECK(dot(v(0, 1, 0, 0), v(0, 0, 0, 1)) == 0);
    CHECK(dot(v1, v1) == v1.length_sq());
    CHECK(dot(v1, v2) == 58);

    const auto v3 = v(3, 4, 12, 0);
    v0 = normalize(v3);
    CHECK(YamaApprox(v0) == v(0.23076923076f, 0.30769230769f, 0.92307692307f, 0));

    CHECK(orthogonal(v(0, 1, 0, 0), v(1, 0, 0, 0)));
    CHECK(orthogonal(v(0, 1, 0, 0), v(0, 0, 0, 1)));
    CHECK(orthogonal(v(1, 0, 0, 2), v(-2, 0, 0, 1)));

    CHECK(collinear(v(1, 2, 3, 4), v(-1, -2, -3, -4)));
    CHECK(collinear(v(1, 0, 0, 0), v(5, 0, 0, 0)));
    CHECK(collinear(v(0, 2, 0, 0), v(0, 11, 0, 0)));
    CHECK(collinear(v(0, 0, 6, 0), v(0, 0, 10, 0)));
    CHECK(collinear(v(0, 0, 0, 7), v(0, 0, 0, 3)));
    CHECK(collinear(v(1, 2, 0, 0), v(5, 10, 0, 0)));
    CHECK(collinear(v(3, 0, 6, 0), v(5, 0, 10, 0)));
    CHECK(collinear(v(1, 2, 0, 3), v(5, 10, 0, 15)));
    CHECK(collinear(v(3, 0, 6, 6), v(5, 0, 10, 10)));
    CHECK(collinear(v(4, 2, 10, 8), v(1, 0.5f, 2.5f, 2)));

    v0 = v(-1.723f, 5.23f, 2.522f, -2.222f);
    CHECK(floor(v0) == v(-2, 5, 2, -3));
    CHECK(ceil(v0) == v(-1, 6, 3, -2));
    CHECK(round(v0) == v(-2, 5, 3, -2));
    CHECK(YamaApprox(frac(v0)) == v(0.723f, 0.23f, 0.522f, 0.222f));

    CHECK(isfinite(v0));
    CHECK(isfinite(v1));
    CHECK(isfinite(v2));

    v0.x = std::numeric_limits<float>::quiet_NaN();
    v1.y = std::numeric_limits<float>::infinity();
    v2 = v0 + v1;
    CHECK(!isfinite(v0));
    CHECK(!isfinite(v1));
    CHECK(!isfinite(v2));

    v1 = v(1, 2, 3, 4);
    v2 = v(5, 10, 15, 18);

    CHECK(distance_sq(v1, v2) == 420);
    CHECK(distance(vector4::zero(), v2) == v2.length());
    CHECK(distance(v2, vector4::zero()) == v2.length());

    CHECK(clamp(v(3, 1, 20, 4), v1, v2) == v(3, 2, 15, 4));
    CHECK(clamp(v(8, 6, 0, 18), v1, v2) == v(5, 6, 3, 18));
    CHECK(clamp(v(3, 4, 5, 6), v1, v2) == v(3, 4, 5, 6));
    CHECK(clamp(v(10, 20, 30, 40), v1, v2) == v2);
    CHECK(clamp(v(0, 1, 2, 3), v1, v2) == v1);

    CHECK(YamaApprox(lerp(v1, v2, 0.3f)) == v(2.2f, 4.4f, 6.6f, 8.2f));
    CHECK(YamaApprox(lerp(v1, v2, 0.f)) == v1);
    CHECK(YamaApprox(lerp(v1, v2, 1.f)) == v2);
}
