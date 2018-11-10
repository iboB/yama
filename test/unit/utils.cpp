// Yama
// Copyright (c) 2016-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#define _USE_MATH_DEFINES
#include "yama/yama.hpp"
#include "doctest/doctest.h"

using namespace yama;
using doctest::Approx;

TEST_SUITE("utils");

TEST_CASE("constants")
{
    CHECK(Approx(constants_t<double>::PI()) == M_PI);
    CHECK(Approx(constants_t<double>::PI()) == constants_t<float>::PI());
    CHECK(Approx(constants_t<double>::PI_HALF()) == M_PI / 2);
    CHECK(Approx(constants_t<double>::PI_D4()) == M_PI / 4);
    CHECK(Approx(constants_t<double>::PI_DBL()) == M_PI * 2);
    CHECK(Approx(constants_t<double>::OVER_PI()) == 1 / M_PI);
    CHECK(Approx(constants_t<double>::E()) == M_E);
    CHECK(Approx(constants_t<double>::SQRT_2()) == std::sqrt(2.0));
    CHECK(constants_t<double>::EPSILON ()> 0);
    CHECK(constants_t<double>::EPSILON_HIGH ()> 0);
    CHECK(constants_t<double>::EPSILON_LOW ()> 0);
    CHECK(constants_t<double>::EPSILON_LOW ()< 1);
    CHECK(constants_t<double>::EPSILON ()< constants_t<double>::EPSILON_LOW());
    CHECK(constants_t<double>::EPSILON_HIGH ()< constants_t<double>::EPSILON());
}

TEST_CASE("functions")
{
    CHECK(Approx(sq(5.1)) == 26.01);
    CHECK(sign(-123) == -1);
    CHECK(sign(52.11) == 1.0);
    CHECK(sign(-25.1f) == -1.f);
    CHECK(sign(unsigned(-1)) == 1);

    int i = -123;
    flip_sign(i);
    CHECK(i == 123);
    double d = 52.11;
    flip_sign(d);
    CHECK(d == -52.11);
    float f = -25.1f;
    flip_sign(f);
    CHECK(f == 25.1f);

    CHECK(Approx(lerp(4., 7., 0.3333333)) == 5.);
    CHECK(Approx(lerp(1.f, 5.f, 0.25f)) == 2.f);

    CHECK(Approx(rad_to_deg(constants_t<double>::PI_HALF())) == 90);
    CHECK(Approx(rad_to_deg(constants_t<double>::PI() / 3)) == 60);

    CHECK(Approx(deg_to_rad(60.f)) == constants::PI() / 3);
    CHECK(Approx(deg_to_rad(90.f)) == constants::PI_HALF());

    CHECK(close(1, 3, 3));
    CHECK(close(1, 1));
    CHECK(!close(1.0, 1.1));
    CHECK(close(1.0, 1.000001));

    CHECK(clamp(1.3, 1.0, 2.0) == 1.3);
    CHECK(clamp(1.3, 2.0, 1.0) == 1.3);
    CHECK(clamp(2.3, 1.0, 2.0) == 2.0);
    CHECK(clamp(0.3, 1.0, 2.0) == 1.0);
}

TEST_CASE("ordering")
{
    strict_weak_ordering o;

    auto v21 = v(1, 2);
    auto v22 = v(2, 3);
    auto v23 = v(1, 3);
    auto v24 = v(1, 2);

    CHECK(o(v21, v22));
    CHECK(!o(v22, v21));
    CHECK(o(v21, v23));
    CHECK(!o(v23, v21));
    CHECK(o(v23, v22));
    CHECK(!o(v22, v23));

    CHECK(!o(v21, v24));
    CHECK(!o(v24, v21));

    auto v31 = v(1, 2, 3);
    auto v32 = v(2, 3, 4);
    auto v33 = v(1, 2, 4);
    auto v34 = v(1, 2, 3);

    CHECK(o(v31, v32));
    CHECK(!o(v32, v31));
    CHECK(o(v31, v33));
    CHECK(!o(v33, v31));
    CHECK(o(v33, v32));
    CHECK(!o(v32, v33));

    CHECK(!o(v31, v34));
    CHECK(!o(v34, v31));

    auto v41 = v(1, 2, 3, 4);
    auto v42 = v(2, 3, 4, 5);
    auto v43 = v(1, 2, 3, 5);
    auto v44 = v(1, 2, 3, 4);

    CHECK(o(v41, v42));
    CHECK(!o(v42, v41));
    CHECK(o(v41, v43));
    CHECK(!o(v43, v41));
    CHECK(o(v43, v42));
    CHECK(!o(v42, v43));

    CHECK(!o(v41, v44));
    CHECK(!o(v44, v41));

    auto q1 = quaternion::xyzw(1, 2, 3, 4);
    auto q2 = quaternion::xyzw(2, 3, 4, 5);
    auto q3 = quaternion::xyzw(1, 2, 3, 5);
    auto q4 = quaternion::xyzw(1, 2, 3, 4);

    CHECK(o(q1, q2));
    CHECK(!o(q2, q1));
    CHECK(o(q1, q3));
    CHECK(!o(q3, q1));
    CHECK(o(q3, q2));
    CHECK(!o(q2, q3));

    CHECK(!o(q1, q4));
    CHECK(!o(q4, q1));
}
