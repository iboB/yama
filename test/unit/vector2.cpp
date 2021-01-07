// Yama
// Copyright (c) 2016-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "yama/vector2.hpp"
#include "common.hpp"
#include "yama/ext/vector2_ostream.hpp"

using namespace yama;
using doctest::Approx;

TEST_SUITE_BEGIN("vector2");

TEST_CASE("construction")
{
    double d0[] = { 0, 0, 0, 0 };
    auto v0 = vector2_t<double>::zero();
    CHECK(v0.x == 0);
    CHECK(v0.y == 0);
    CHECK(memcmp(d0, &v0, 2 * sizeof(double)) == 0);

    float f1[] = { 1,2,3,4 };
    auto v1 = vector2::coord(1, 2);
    CHECK(v1.x == 1);
    CHECK(v1.y == 2);
    CHECK(memcmp(f1, &v1, 2 * sizeof(float)) == 0);

    auto v2 = vector2_t<float>::uniform(3);
    CHECK(v2.x == 3);
    CHECK(v2.x == v2.y);

    auto v3 = v(3, 1);
    CHECK(v3.x == 3);
    CHECK(v3.y == 1);

    const char* c4 = "1234";
    auto v4 = vt('1', '2');
    CHECK(v4.x == '1');
    CHECK(v4.y == '2');
    CHECK(sizeof(v4) == 2);
    CHECK(memcmp(c4, &v4, sizeof(v4)) == 0);

    const float f[] = { 10,9,8,7 };
    auto v5 = vector2::from_ptr(f);
    CHECK(memcmp(&v5, f, 2 * sizeof(float)) == 0);
    CHECK(v5.x == 10);
    CHECK(v5.y == 9);

    // attach
    auto& v6 = vector2::attach_to_ptr(f);
    CHECK(v6.x == 10);
    CHECK(v6.y == 9);
    CHECK(reinterpret_cast<const float*>(&v6) == f);

    auto v7 = vector2::attach_to_array(f);
    CHECK(v7[0].x == 10);
    CHECK(v7[0].y == 9);
    CHECK(v7[1].x == 8);
    CHECK(v7[1].y == 7);
    CHECK(v7 == &v6);
    CHECK(v7[0] == v6);
    CHECK(reinterpret_cast<const float*>(v7) == f);

    float ff[] = { 1, 2, 3, 4 };
    auto& v8 = vector2::attach_to_ptr(ff);
    v8.x = 10;
    v8.y = 20;
    CHECK(ff[0] == 10);
    CHECK(ff[1] == 20);

    auto v9 = vector2::attach_to_array(ff);
    v9[0].y = 21;
    v9[1].x = 30;
    CHECK(ff[1] == 21);
    CHECK(ff[2] == 30);
}

TEST_CASE("compare")
{
    auto v0 = vector2::zero();
    CHECK(v0 == v(0, 0));
    CHECK(v0 != v(1, 0));
    CHECK(v0 != v(0, 1));

    vector2 v1;
    v1.x = 10;
    v1.y = 20;
    CHECK(v1 == v(10, 20));
    CHECK(v1 != v(1, 20));
    CHECK(v1 != v(10, 1));

    CHECK(v0 != v1);

    v0.x = 10;
    v0.y = 20;

    CHECK(v0 == v1);

    CHECK(close(v0, v1));
    v1.x = 11;
    CHECK(!close(v0, v1));
    v1.y = 21;
    CHECK(!close(v0, v1));
    CHECK(close(v0, v1, 2.f));
    v1 = v(10.000001f, 20.000001f);
    CHECK(close(v0, v1));

    CHECK(vector2::unit_x() == v(1, 0));
    CHECK(vector2::unit_y() == v(0, 1));
}

TEST_CASE("access")
{
    auto v0 = v(1, 2);
    CHECK(v0.data() == reinterpret_cast<const float*>(&v0));
    CHECK(v0.data()[0] == 1);
    CHECK(v0.data()[1] == 2);
    CHECK(v0[0] == 1);
    CHECK(v0[1] == 2);
    CHECK(v0.at(0) == 1);
    CHECK(v0.at(1) == 2);
    CHECK(v0.data() == v0.as_ptr());

    v0[0] = 10;
    v0.at(1) = 20;

    const auto v1 = v0;
    CHECK(v1.data() == reinterpret_cast<const float*>(&v1));
    CHECK(v1.data()[0] == 10);
    CHECK(v1.data()[1] == 20);
    CHECK(v1[0] == 10);
    CHECK(v1[1] == 20);
    CHECK(v1.at(0) == 10);
    CHECK(v1.at(1) == 20);
    CHECK(v1.data() == v1.as_ptr());

    v0.x = 0.3f;
    v0.y = 2.91f;

    CHECK(v0.as_vector2_t<int>() == vt(0, 2));
}

TEST_CASE("std")
{
    auto v0 = v(11, 12);
    CHECK(v0.front() == 11);
    CHECK(v0.back() == 12);
    CHECK(v0.begin() == v0.data());
    CHECK(v0.begin() + 2 == v0.end());
    CHECK(v0.rbegin() + 2 == v0.rend());
    auto i = v0.rbegin();
    CHECK(*i == 12);
    ++i;
    CHECK(*i == 11);

    for (auto& e : v0)
    {
        e += 10;
    }

    const auto v1 = v0;

    CHECK(v1.front() == 21);
    CHECK(v1.back() == 22);
    CHECK(v1.begin() == v1.data());
    CHECK(v1.begin() + 2 == v1.end());
    CHECK(v1.rbegin() + 2 == v1.rend());
    auto ci = v1.rbegin();
    CHECK(*ci == 22);
    ++ci;
    CHECK(*ci == 21);
}


TEST_CASE("members")
{
    auto v0 = v(1, 2);
    auto v1 = +v0;
    CHECK(v0 == v1);
    v1 = -v0;
    CHECK(v1.x == -v0.x);
    CHECK(v1.y == -v0.y);

    v0 += v1;
    CHECK(v0 == vector2::zero());

    v0 -= v1;
    CHECK(v0 == -v1);

    v0 *= 2;
    CHECK(v0 == v(2, 4));

    v0 /= 8;
    CHECK(YamaApprox(v0) == v(0.25f, 0.5f));

    v1 *= -2;
    v0.mul(v1);
    CHECK(YamaApprox(v0) == v(0.5f, 2));

    v1.div(v0);
    CHECK(YamaApprox(v1) == v(4, 2));

    CHECK(Approx(v0.length_sq()) == 4.25);
    v1.y = 3;
    CHECK(Approx(v1.length()) == 5);

    CHECK(v1.manhattan_length() == 7);

    v1.normalize();
    CHECK(Approx(v1.length()) == 1);
    CHECK(Approx(v1.x) == 0.8);
    CHECK(v1.is_normalized());

    v0 = v(1, 2);
    v0.homogenous_normalize();
    CHECK(YamaApprox(v0) == v(0.5f, 1));

    v0 = v(1, 1);
    CHECK(v0.reflection(v(0, 1)) == v(1, -1));
    CHECK(v0.reflection(v(0, -1)) == v(1, -1));

    v0 = v(1, -2);
    CHECK(v0.reflection(v(0, 1)) == v(1, 2));
    v0.normalize();
    CHECK(YamaApprox(v0.reflection(v0)) == -v0);
}

TEST_CASE("ops")
{
    auto v0 = v(1, 2);
    auto v1 = v(3, 4);

    auto v2 = v0 + v1;
    CHECK(v2 == v(4, 6));

    v1 = v2 - v0;
    CHECK(v1 == v(3, 4));

    v1 = 2.f * v0;
    CHECK(v1 == v(2, 4));

    v0 = v1 * 0.5f;
    CHECK(v0 == v(1, 2));

    v0 = 4.f / v1;
    CHECK(v0 == v(2, 1));

    v1 = v0 / 2.f;
    CHECK(v1 == v(1, 0.5f));

    v0 = abs(v1);
    CHECK(v1 == v(1, 0.5f));

    v0 = abs(-v1);
    CHECK(v1 == v(1, 0.5f));

    v0 = v(0.25f, 0.5f);
    v1 = v(8, 10);
    v2 = mul(v0, v1);
    CHECK(v2 == v(2, 5));

    v2 = div(v1, v(4, 2));
    CHECK(v2 == v(2, 5));

    auto iv0 = mod(vt(5, 6), vt(2, 4));
    CHECK(iv0 == vt(1, 2));

    CHECK(YamaApprox(mod(v(5.3f, 18.5f), v(2, 4.2f))) == v(1.3f, 1.7f));

    v1 = sign(v2);
    CHECK(v1 == v(1, 1));
    v1 = sign(-v2);
    CHECK(v1 == v(-1, -1));
    v1 = sign(v(-1, 2));
    CHECK(v1 == v(-1, 1));

    v1 = v(-1, 10);
    v0 = max(v1, v2);
    CHECK(v0 == v(2, 10));
    v0 = min(v1, v2);
    CHECK(v0 == v(-1, 5));

    CHECK(dot(v(0, 1), v(1, 0)) == 0);
    CHECK(dot(v1, v1) == v1.length_sq());
    CHECK(dot(v1, v2) == 48);

    CHECK(cross_magnitude(v(0, 1), v(1, 0)) == -1);
    CHECK(cross_magnitude(v(2, 1), v(3, 4)) == 5);

    auto vo = v1.get_orthogonal();
    CHECK(orthogonal(vo, v1));

    vo = v2.get_orthogonal();
    CHECK(orthogonal(v2, vo));

    const auto v3 = v(3, 4);
    v0 = normalize(v3);
    CHECK(YamaApprox(v0) == v(0.6f, 0.8f));

    CHECK(orthogonal(v(0, 1), v(1, 0)));
    CHECK(orthogonal(v(1, 2), v(-2, 1)));

    CHECK(collinear(v(1, 2), v(-1, -2)));
    CHECK(collinear(v(1, 0), v(5, 0)));
    CHECK(collinear(v(0, 2), v(0, 11)));
    CHECK(collinear(v(4, 2), v(1, 0.5)));

    v0 = v(-1.723f, 5.23f);
    CHECK(floor(v0) == v(-2, 5));
    CHECK(ceil(v0) == v(-1, 6));
    CHECK(round(v0) == v(-2, 5));
    CHECK(YamaApprox(frac(v0)) == v(0.723f, 0.23f));

    CHECK(isfinite(v0));
    CHECK(isfinite(v1));
    CHECK(isfinite(v2));

    v0.x = std::numeric_limits<float>::quiet_NaN();
    v1.y = std::numeric_limits<float>::infinity();
    v2 = v0 + v1;
    CHECK(!isfinite(v0));
    CHECK(!isfinite(v1));
    CHECK(!isfinite(v2));

    v1 = v(1, 2);
    v2 = v(5, 10);

    CHECK(distance_sq(v1, v2) == 80);
    CHECK(distance(vector2::zero(), v2) == v2.length());
    CHECK(distance(v2, vector2::zero()) == v2.length());

    CHECK(clamp(v(3, 1), v1, v2) == v(3, 2));
    CHECK(clamp(v(8, 6), v1, v2) == v(5, 6));
    CHECK(clamp(v(3, 4), v1, v2) == v(3, 4));
    CHECK(clamp(v(10, 20), v1, v2) == v(5, 10));
    CHECK(clamp(v(0, 1), v1, v2) == v(1, 2));

    CHECK(YamaApprox(lerp(v1, v2, 0.3f)) == v(2.2f, 4.4f));
    CHECK(YamaApprox(lerp(v1, v2, 0.f)) == v1);
    CHECK(YamaApprox(lerp(v1, v2, 1.f)) == v2);
}
