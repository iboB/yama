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

TEST_SUITE("vector3");

TEST_CASE("construction")
{
    double d0[] = { 0, 0, 0, 0 };
    auto v0 = vector3_t<double>::zero();
    CHECK(v0.x == 0);
    CHECK(v0.y == 0);
    CHECK(v0.z == 0);
    CHECK(memcmp(d0, &v0, 3 * sizeof(double)) == 0);

    float f1[] = { 1,2,3,4 };
    auto v1 = vector3::coord(1, 2, 3);
    CHECK(v1.x == 1);
    CHECK(v1.y == 2);
    CHECK(v1.z == 3);
    CHECK(memcmp(f1, &v1, 3 * sizeof(float)) == 0);

    auto v2 = vector3_t<float>::uniform(3);
    CHECK(v2.x == 3);
    CHECK(v2.x == v2.y);
    CHECK(v2.x == v2.z);

    auto v3 = v(3, 1, 5);
    CHECK(v3.x == 3);
    CHECK(v3.y == 1);
    CHECK(v3.z == 5);

    const char* c4 = "1234";
    auto v4 = vt('1', '2', '3');
    CHECK(v4.x == '1');
    CHECK(v4.y == '2');
    CHECK(v4.z == '3');
    CHECK(sizeof(v4) == 3);
    CHECK(memcmp(c4, &v4, sizeof(v4)) == 0);

    const float f[] = { 10,9,8,7,6,5 };
    auto v5 = vector3::from_ptr(f);
    CHECK(memcmp(&v5, f, 3 * sizeof(float)) == 0);
    CHECK(v5.x == 10);
    CHECK(v5.y == 9);
    CHECK(v5.z == 8);

    // attach
    auto& v6 = vector3::attach_to_ptr(f);
    CHECK(v6.x == 10);
    CHECK(v6.y == 9);
    CHECK(v6.z == 8);
    CHECK(reinterpret_cast<const float*>(&v6) == f);

    auto v7 = vector3::attach_to_array(f);
    CHECK(v7[0].x == 10);
    CHECK(v7[0].y == 9);
    CHECK(v7[0].z == 8);
    CHECK(v7[1].x == 7);
    CHECK(v7[1].y == 6);
    CHECK(v7[1].z == 5);
    CHECK(v7 == &v6);
    CHECK(v7[0] == v6);
    CHECK(reinterpret_cast<const float*>(v7) == f);

    float ff[] = { 1, 2, 3, 4, 5, 6 };
    auto& v8 = vector3::attach_to_ptr(ff);
    v8.x = 10;
    v8.y = 20;
    CHECK(ff[0] == 10);
    CHECK(ff[1] == 20);

    auto v9 = vector3::attach_to_array(ff);
    v9[0].y = 21;
    v9[1].z = 30;
    CHECK(ff[1] == 21);
    CHECK(ff[5] == 30);
}

TEST_CASE("compare")
{
    auto v0 = vector3::zero();
    CHECK(v0 == v(0, 0, 0));
    CHECK(v0 != v(1, 0, 0));
    CHECK(v0 != v(0, 1, 0));
    CHECK(v0 != v(0, 0, 1));

    vector3 v1;
    v1.x = 10;
    v1.y = 20;
    v1.z = 30;
    CHECK(v1 == v(10, 20, 30));
    CHECK(v1 != v(1, 20, 30));
    CHECK(v1 != v(10, 1, 30));
    CHECK(v1 != v(10, 20, 1));

    CHECK(v0 != v1);

    v0.x = 10;
    v0.y = 20;
    v0.z = 30;

    CHECK(v0 == v1);

    CHECK(close(v0, v1));
    v1.x = 11;
    CHECK(!close(v0, v1));
    v1.y = 21;
    CHECK(!close(v0, v1));
    v1.z = 31;
    CHECK(!close(v0, v1));
    CHECK(close(v0, v1, 2.f));
    v1 = v(10.000001f, 20.000001f, 30.000001f);
    CHECK(close(v0, v1));

    CHECK(vector3::unit_x() == v(1, 0, 0));
    CHECK(vector3::unit_y() == v(0, 1, 0));
    CHECK(vector3::unit_z() == v(0, 0, 1));
}

TEST_CASE("special_construction")
{
}

TEST_CASE("access")
{
    auto v0 = v(1, 2, 3);
    CHECK(v0.data() == reinterpret_cast<const float*>(&v0));
    CHECK(v0.data()[0] == 1);
    CHECK(v0.data()[1] == 2);
    CHECK(v0.data()[2] == 3);
    CHECK(v0[0] == 1);
    CHECK(v0[1] == 2);
    CHECK(v0[2] == 3);
    CHECK(v0.at(0) == 1);
    CHECK(v0.at(1) == 2);
    CHECK(v0.at(2) == 3);
    CHECK(v0.data() == v0.as_ptr());

    v0[0] = 10;
    v0.at(1) = 20;
    v0.z = 30;

    const auto v1 = v0;
    CHECK(v1.data() == reinterpret_cast<const float*>(&v1));
    CHECK(v1.data()[0] == 10);
    CHECK(v1.data()[1] == 20);
    CHECK(v1.data()[2] == 30);
    CHECK(v1[0] == 10);
    CHECK(v1[1] == 20);
    CHECK(v1[2] == 30);
    CHECK(v1.at(0) == 10);
    CHECK(v1.at(1) == 20);
    CHECK(v1.at(2) == 30);
    CHECK(v1.data() == v1.as_ptr());

    v0.x = 0.3f;
    v0.y = 2.91f;
    v0.z = -11.123f;

    CHECK(v0.as_vector3_t<int>() == vt(0, 2, -11));
}

TEST_CASE("std")
{
    auto v0 = v(11, 12, 13);
    CHECK(v0.front() == 11);
    CHECK(v0.back() == 13);
    CHECK(v0.begin() == v0.data());
    CHECK(v0.begin() + 3 == v0.end());
    CHECK(v0.rbegin() + 3 == v0.rend());
    auto i = v0.rbegin();
    CHECK(*i == 13);
    ++i;
    CHECK(*i == 12);

    for (auto& e : v0)
    {
        e += 10;
    }

    const auto v1 = v0;

    CHECK(v1.front() == 21);
    CHECK(v1.back() == 23);
    CHECK(v1.begin() == v1.data());
    CHECK(v1.begin() + 3 == v1.end());
    CHECK(v1.rbegin() + 3 == v1.rend());
    auto ci = v1.rbegin();
    CHECK(*ci == 23);
    ++ci;
    CHECK(*ci == 22);
}


TEST_CASE("members")
{
    auto v0 = v(1, 2, 3);
    auto v1 = +v0;
    CHECK(v0 == v1);
    v1 = -v0;
    CHECK(v1.x == -v0.x);
    CHECK(v1.y == -v0.y);
    CHECK(v1.z == -v0.z);

    v0 += v1;
    CHECK(v0 == vector3::zero());

    v0 -= v1;
    CHECK(v0 == -v1);

    v0 *= 2;
    CHECK(v0 == v(2, 4, 6));

    v0 /= 8;
    CHECK(YamaApprox(v0) == v(0.25f, 0.5f, 0.75f));

    v1 *= -2;
    CHECK(v1 == v(2, 4, 6));
    v0.mul(v1);
    CHECK(YamaApprox(v0) == v(0.5f, 2, 4.5f));

    v1.div(v0);
    CHECK(YamaApprox(v1) == v(4, 2, 1.3333333f));

    CHECK(Approx(v0.length_sq()) == 24.5f);
    v1 = v(3, 4, 12);
    CHECK(Approx(v1.length()) == 13);

    CHECK(v1.manhattan_length() == 19);

    v1.normalize();
    CHECK(Approx(v1.length()) == 1);
    CHECK(Approx(v1.x) == 0.23076923076);
    CHECK(v1.is_normalized());

    v0 = v(1, 2, 4);
    v0.homogenous_normalize();
    CHECK(YamaApprox(v0) == v(0.25f, 0.5f, 1));

    v0 = v(1, 1, 1);
    CHECK(v0.reflection(v(0, 1, 0)) == v(1, -1, 1));
    CHECK(v0.reflection(v(0, -1, 0)) == v(1, -1, 1));

    v0 = v(1, -2, 5);
    CHECK(v0.reflection(v(0, 1, 0)) == v(1, 2, 5));
    v0.normalize();
    CHECK(YamaApprox(v0.reflection(v0)) == -v0);
}

TEST_CASE("ops")
{
    auto v0 = v(1, 2, 3);
    auto v1 = v(3, 4, 5);

    auto v2 = v0 + v1;
    CHECK(v2 == v(4, 6, 8));

    v1 = v2 - v0;
    CHECK(v1 == v(3, 4, 5));

    v1 = 2.f * v0;
    CHECK(v1 == v(2, 4, 6));

    v0 = v1 * 0.5f;
    CHECK(v0 == v(1, 2, 3));

    v0 = 4.f / v1;
    CHECK(YamaApprox(v0) == v(2, 1, 0.6666666f));

    v1 = v0 / 2.f;
    CHECK(YamaApprox(v1) == v(1, 0.5f, 0.3333333f));

    v0 = abs(v1);
    CHECK(YamaApprox(v1) == v(1, 0.5f, 0.3333333f));

    v0 = abs(-v1);
    CHECK(YamaApprox(v1) == v(1, 0.5f, 0.3333333f));

    v0 = v(0.25f, 0.5f, 1.5f);
    v1 = v(8, 10, 12);
    v2 = mul(v0, v1);
    CHECK(v2 == v(2, 5, 18));

    v2 = div(v1, v(4, 2, 12));
    CHECK(v2 == v(2, 5, 1));

    auto iv0 = mod(vt(5, 6, 11), vt(2, 4, 7));
    CHECK(iv0 == vt(1, 2, 4));

    CHECK(YamaApprox(mod(v(5.3f, 18.5f, 11), v(2, 4.2f, 7))) == v(1.3f, 1.7f, 4));

    v1 = sign(v2);
    CHECK(v1 == v(1, 1, 1));
    v1 = sign(-v2);
    CHECK(v1 == v(-1, -1, -1));
    v1 = sign(v(-1, 2, -4));
    CHECK(v1 == v(-1, 1, -1));

    v1 = v(-1, 10, 6);
    v0 = max(v1, v2);
    CHECK(v0 == v(2, 10, 6));
    v0 = min(v1, v2);
    CHECK(v0 == v(-1, 5, 1));

    CHECK(dot(v(0, 1, 0), v(0, 0, 1)) == 0);
    CHECK(dot(v1, v1) == v1.length_sq());
    CHECK(dot(v1, v2) == 54);

    CHECK(cross(v(1, 0, 0), v(0, 1, 0)) == v(0, 0, 1));
    CHECK(cross(v(0, 1, 0), v(0, 0, 1)) == v(1, 0, 0));
    CHECK(cross(v(1, 0, 0), v(0, 0, 1)) == v(0, -1, 0));
    CHECK(cross(v1, v2) == v(-20, 13, -25));

    auto vo = v1.get_orthogonal();
    CHECK(orthogonal(vo, v1));

    vo = v2.get_orthogonal();
    CHECK(orthogonal(v2, vo));

    const auto v3 = v(3, 4, 12);
    v0 = normalize(v3);
    CHECK(YamaApprox(v0) == v(0.23076923076f, 0.30769230769f, 0.92307692307f));

    CHECK(orthogonal(v(0, 1, 0), v(1, 0, 0)));
    CHECK(orthogonal(v(0, 1, 0), v(0, 0, 1)));
    CHECK(orthogonal(v(1, 0, 2), v(-2, 0, 1)));
    CHECK(orthogonal(v(1, 2, 3), v(-2, -0.5f, 1)));

    CHECK(collinear(v(1, 2, 3), v(-1, -2, -3)));
    CHECK(collinear(v(1, 0, 0), v(5, 0, 0)));
    CHECK(collinear(v(0, 2, 0), v(0, 11, 0)));
    CHECK(collinear(v(0, 0, 6), v(0, 0, 10)));
    CHECK(collinear(v(1, 2, 0), v(5, 10, 0)));
    CHECK(collinear(v(3, 0, 6), v(5, 0, 10)));
    CHECK(collinear(v(0, 2, 1), v(0, -4, -2)));
    CHECK(collinear(v(4, 2, 10), v(1, 0.5f, 2.5f)));

    v0 = v(-1.723f, 5.23f, 2.522f);
    CHECK(floor(v0) == v(-2, 5, 2));
    CHECK(ceil(v0) == v(-1, 6, 3));
    CHECK(round(v0) == v(-2, 5, 3));
    CHECK(YamaApprox(frac(v0)) == v(0.723f, 0.23f, 0.522f));

    CHECK(isfinite(v0));
    CHECK(isfinite(v1));
    CHECK(isfinite(v2));

    v0.x = std::numeric_limits<float>::quiet_NaN();
    v1.y = std::numeric_limits<float>::infinity();
    v2.z = -std::numeric_limits<float>::infinity();
    v2 = v0 + v1;
    CHECK(!isfinite(v0));
    CHECK(!isfinite(v1));
    CHECK(!isfinite(v2));

    v1 = v(1, 2, 3);
    v2 = v(5, 10, 15);

    CHECK(distance_sq(v1, v2) == 224);
    CHECK(distance(vector3::zero(), v2) == v2.length());
    CHECK(distance(v2, vector3::zero()) == v2.length());

    CHECK(clamp(v(3, 1, 20), v1, v2) == v(3, 2, 15));
    CHECK(clamp(v(8, 6, 0), v1, v2) == v(5, 6, 3));
    CHECK(clamp(v(3, 4, 5), v1, v2) == v(3, 4, 5));
    CHECK(clamp(v(10, 20, 30), v1, v2) == v2);
    CHECK(clamp(v(0, 1, 2), v1, v2) == v1);

    CHECK(YamaApprox(lerp(v1, v2, 0.3f)) == v(2.2f, 4.4f, 6.6f));
    CHECK(YamaApprox(lerp(v1, v2, 0.f)) == v1);
    CHECK(YamaApprox(lerp(v1, v2, 1.f)) == v2);
}
