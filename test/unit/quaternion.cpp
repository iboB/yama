// Yama
// Copyright (c) 2016-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "yama/quaternion.hpp"
#include "common.hpp"
#include "yama/ext/quaternion_ostream.hpp"
#include "yama/ext/vector3_ostream.hpp"

using namespace yama;
using doctest::Approx;

quaternion q(float x, float y, float z, float w)
{
    return quaternion::xyzw(x, y, z, w);
}

TEST_SUITE("quaternion");

TEST_CASE("construction")
{
    double d0[] = { 0, 0, 0, 0 };
    auto q0 = quaternion_t<double>::zero();
    CHECK(q0.x == 0);
    CHECK(q0.y == 0);
    CHECK(q0.z == 0);
    CHECK(q0.w == 0);
    CHECK(memcmp(d0, &q0, 4 * sizeof(double)) == 0);

    float f1[] = { 1,2,3,4 };
    auto q1 = quaternion::xyzw(1, 2, 3, 4);
    CHECK(q1.x == 1);
    CHECK(q1.y == 2);
    CHECK(q1.z == 3);
    CHECK(q1.w == 4);
    CHECK(memcmp(f1, &q1, 4 * sizeof(float)) == 0);

    auto q2 = quaternion_t<float>::uniform(3);
    CHECK(q2.x == 3);
    CHECK(q2.x == q2.y);
    CHECK(q2.x == q2.z);
    CHECK(q2.x == q2.w);

    auto q3 = q(3, 1, 5, 2);
    CHECK(q3.x == 3);
    CHECK(q3.y == 1);
    CHECK(q3.z == 5);
    CHECK(q3.w == 2);

    const char* c4 = "1234";
    auto q4 = vt('1', '2', '3', '4');
    CHECK(q4.x == '1');
    CHECK(q4.y == '2');
    CHECK(q4.z == '3');
    CHECK(q4.w == '4');
    CHECK(sizeof(q4) == 4);
    CHECK(memcmp(c4, &q4, sizeof(q4)) == 0);

    const float f[] = { 10,9,8,7,6,5,4,3 };
    auto q5 = quaternion::from_ptr(f);
    CHECK(memcmp(&q5, f, 4 * sizeof(float)) == 0);
    CHECK(q5.x == 10);
    CHECK(q5.y == 9);
    CHECK(q5.z == 8);
    CHECK(q5.w == 7);

    // attach
    auto& q6 = quaternion::attach_to_ptr(f);
    CHECK(q6.x == 10);
    CHECK(q6.y == 9);
    CHECK(q6.z == 8);
    CHECK(q6.w == 7);
    CHECK(reinterpret_cast<const float*>(&q6) == f);

    auto q7 = quaternion::attach_to_array(f);
    CHECK(q7[0].x == 10);
    CHECK(q7[0].y == 9);
    CHECK(q7[0].z == 8);
    CHECK(q7[0].w == 7);
    CHECK(q7[1].x == 6);
    CHECK(q7[1].y == 5);
    CHECK(q7[1].z == 4);
    CHECK(q7[1].w == 3);
    CHECK(q7 == &q6);
    CHECK(q7[0] == q6);
    CHECK(reinterpret_cast<const float*>(q7) == f);

    float ff[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    auto& q8 = quaternion::attach_to_ptr(ff);
    q8.x = 10;
    q8.w = 20;
    CHECK(ff[0] == 10);
    CHECK(ff[3] == 20);

    auto q9 = quaternion::attach_to_array(ff);
    q9[0].y = 21;
    q9[1].z = 30;
    CHECK(ff[1] == 21);
    CHECK(ff[6] == 30);
}

TEST_CASE("compare")
{
    auto q0 = quaternion::zero();
    CHECK(q0 == q(0, 0, 0, 0));
    CHECK(q0 != q(1, 0, 0, 0));
    CHECK(q0 != q(0, 1, 0, 0));
    CHECK(q0 != q(0, 0, 1, 0));
    CHECK(q0 != q(0, 0, 0, 1));

    quaternion q1;
    q1.x = 10;
    q1.y = 20;
    q1.z = 30;
    q1.w = 40;
    CHECK(q1 == q(10, 20, 30, 40));
    CHECK(q1 != q(1, 20, 30, 40));
    CHECK(q1 != q(10, 1, 30, 40));
    CHECK(q1 != q(10, 20, 1, 40));
    CHECK(q1 != q(10, 20, 30, 1));

    CHECK(q0 != q1);

    q0.x = 10;
    q0.y = 20;
    q0.z = 30;
    q0.w = 40;

    CHECK(q0 == q1);

    CHECK(close(q0, q1));
    q1.x = 11;
    CHECK(!close(q0, q1));
    q1.y = 21;
    CHECK(!close(q0, q1));
    q1.z = 31;
    CHECK(!close(q0, q1));
    q1.w = 41;
    CHECK(close(q0, q1, 2.f));
    q1 = q(10.000001f, 20.000001f, 30.000001f, 40.000001f);
    CHECK(close(q0, q1));
}

TEST_CASE("special_construction")
{
    auto q0 = quaternion::identity();
    CHECK(q0 == q(0, 0, 0, 1));


}

TEST_CASE("access")
{
    auto q0 = q(1, 2, 3, 4);
    CHECK(q0.data() == reinterpret_cast<const float*>(&q0));
    CHECK(q0.data()[0] == 1);
    CHECK(q0.data()[1] == 2);
    CHECK(q0.data()[2] == 3);
    CHECK(q0.data()[3] == 4);
    CHECK(q0[0] == 1);
    CHECK(q0[1] == 2);
    CHECK(q0[2] == 3);
    CHECK(q0[3] == 4);
    CHECK(q0.at(0) == 1);
    CHECK(q0.at(1) == 2);
    CHECK(q0.at(2) == 3);
    CHECK(q0.at(3) == 4);
    CHECK(q0.data() == q0.as_ptr());

    q0[0] = 10;
    q0.at(1) = 20;
    q0.z = 30;
    q0.data()[3] = 40;

    const auto q1 = q0;
    CHECK(q1.data() == reinterpret_cast<const float*>(&q1));
    CHECK(q1.data()[0] == 10);
    CHECK(q1.data()[1] == 20);
    CHECK(q1.data()[2] == 30);
    CHECK(q1.data()[3] == 40);
    CHECK(q1[0] == 10);
    CHECK(q1[1] == 20);
    CHECK(q1[2] == 30);
    CHECK(q1[3] == 40);
    CHECK(q1.at(0) == 10);
    CHECK(q1.at(1) == 20);
    CHECK(q1.at(2) == 30);
    CHECK(q1.at(3) == 40);
    CHECK(q1.data() == q1.as_ptr());

    q0.x = 0.3f;
    q0.y = 2.91f;
    q0.z = -11.123f;
    q0.w = -1.99f;

    CHECK(q0.as_quaternion_t<int>() == quaternion_t<int>::xyzw(0, 2, -11, -1));
}

TEST_CASE("std")
{
    auto q0 = q(11, 12, 13, 14);
    CHECK(q0.front() == 11);
    CHECK(q0.back() == 14);
    CHECK(q0.begin() == q0.data());
    CHECK(q0.begin() + 4 == q0.end());
    CHECK(q0.rbegin() + 4 == q0.rend());
    auto i = q0.rbegin();
    CHECK(*i == 14);
    ++i;
    CHECK(*i == 13);

    for (auto& e : q0)
    {
        e += 10;
    }

    const auto q1 = q0;

    CHECK(q1.front() == 21);
    CHECK(q1.back() == 24);
    CHECK(q1.begin() == q1.data());
    CHECK(q1.begin() + 4 == q1.end());
    CHECK(q1.rbegin() + 4 == q1.rend());
    auto ci = q1.rbegin();
    CHECK(*ci == 24);
    ++ci;
    CHECK(*ci == 23);
}


TEST_CASE("members")
{
    auto q0 = q(1, 2, 3, 4);
    auto q1 = +q0;
    CHECK(q0 == q1);
    q1 = -q0;
    CHECK(q1.x == -q0.x);
    CHECK(q1.y == -q0.y);
    CHECK(q1.z == -q0.z);
    CHECK(q1.w == -q0.w);

    q0 += q1;
    CHECK(q0 == quaternion::zero());

    q0 -= q1;
    CHECK(q0 == -q1);

    q0 *= 2;
    CHECK(q0 == q(2, 4, 6, 8));

    q0 /= 8;
    CHECK(YamaApprox(q0) == q(0.25f, 0.5f, 0.75f, 1));

    q1 *= -2;
    CHECK(q1 == q(2, 4, 6, 8));
    q0.mul(q1);
    CHECK(YamaApprox(q0) == q(0.5f, 2, 4.5f, 8));

    q1.div(q0);
    CHECK(YamaApprox(q1) == q(4, 2, 1.3333333f, 1));

    CHECK(Approx(q0.length_sq()) == 88.5f);
    q1 = q(1, 2, 8, 10);
    CHECK(Approx(q1.length()) == 13);

    q1.normalize();
    CHECK(Approx(q1.length()) == 1);
    CHECK(Approx(q1.w) == 0.76923076923);
    CHECK(q1.is_normalized());

    q0.conjugate();
    CHECK(YamaApprox(q0) == q(-0.5f, -2, -4.5f, 8));

    q0.conjugate();
    q1 = q0;
    q0.inverse();
    CHECK(YamaApprox(q0 * q1) == quaternion::identity());
    q0.inverse();
    CHECK(YamaApprox(q0) == q1);
}

TEST_CASE("ops")
{
    auto q0 = q(1, 2, 3, 4);
    auto q1 = q(3, 4, 5, 6);

    auto q2 = q0 + q1;
    CHECK(q2 == q(4, 6, 8, 10));

    q1 = q2 - q0;
    CHECK(q1 == q(3, 4, 5, 6));

    q1 = 2.f * q0;
    CHECK(q1 == q(2, 4, 6, 8));

    q0 = q1 * 0.5f;
    CHECK(q0 == q(1, 2, 3, 4));

    q0 = 4.f / q1;
    CHECK(YamaApprox(q0) == q(2, 1, 0.6666666f, 0.5f));

    q1 = q0 / 2.f;
    CHECK(YamaApprox(q1) == q(1, 0.5f, 0.3333333f, 0.25f));

    q0 = abs(q1);
    CHECK(YamaApprox(q1) == q(1, 0.5f, 0.3333333f, 0.25f));

    q0 = abs(-q1);
    CHECK(YamaApprox(q1) == q(1, 0.5f, 0.3333333f, 0.25f));

    q0 = q(0.25f, 0.5f, 1.5f, 1.75f);
    q1 = q(8, 10, 12, 16);
    q2 = mul(q0, q1);
    CHECK(q2 == q(2, 5, 18, 28));

    q2 = div(q1, q(4, 2, 12, 8));
    CHECK(q2 == q(2, 5, 1, 2));

    auto iq0 = mod(vt(5, 6, 11, 8), vt(2, 4, 7, 4));
    CHECK(iq0 == vt(1, 2, 4, 0));

    CHECK(YamaApprox(mod(q(5.3f, 18.5f, 11, 8), q(2, 4.2f, 7, 4))) == q(1.3f, 1.7f, 4, 0));

    q1 = sign(q2);
    CHECK(q1 == q(1, 1, 1, 1));
    q1 = sign(-q2);
    CHECK(q1 == q(-1, -1, -1, -1));
    q1 = sign(q(-1, 2, -4, 0));
    CHECK(q1 == q(-1, 1, -1, 1));

    q1 = q(-1, 10, 6, 2);
    q0 = max(q1, q2);
    CHECK(q0 == q(2, 10, 6, 2));
    q0 = min(q1, q2);
    CHECK(q0 == q(-1, 5, 1, 2));

    CHECK(dot(q(0, 1, 0, 0), q(0, 0, 0, 1)) == 0);
    CHECK(dot(q1, q1) == q1.length_sq());
    CHECK(dot(q1, q2) == 58);

    const auto q3 = q(3, 4, 12, 0);
    q0 = normalize(q3);
    CHECK(YamaApprox(q0) == q(0.23076923076f, 0.30769230769f, 0.92307692307f, 0));

    q0 = q(-1.723f, 5.23f, 2.522f, -2.222f);
    CHECK(floor(q0) == q(-2, 5, 2, -3));
    CHECK(ceil(q0) == q(-1, 6, 3, -2));
    CHECK(round(q0) == q(-2, 5, 3, -2));
    CHECK(YamaApprox(frac(q0)) == q(0.723f, 0.23f, 0.522f, 0.222f));

    CHECK(isfinite(q0));
    CHECK(isfinite(q1));
    CHECK(isfinite(q2));

    q0.x = std::numeric_limits<float>::quiet_NaN();
    q1.y = std::numeric_limits<float>::infinity();
    q2 = q0 + q1;
    CHECK(!isfinite(q0));
    CHECK(!isfinite(q1));
    CHECK(!isfinite(q2));

    q1 = q(1, 2, 3, 4);
    q2 = q(5, 10, 15, 18);
    CHECK(YamaApprox(lerp(q1, q2, 0.3f)) == q(0.189346f, 0.378692f, 0.568038f, 0.705744f));

    CHECK(q1 * q2 == q(38, 76, 114, 2));
    q1 *= q2;
    CHECK(q1 == q(38, 76, 114, 2));
    CHECK(q1 / q2 == q(1, 2, 3, 4));
    q1 /= q2;
    CHECK(q1 == q(1, 2, 3, 4));

    q0 = conjugate(q2);
    CHECK(YamaApprox(q0) == q(-5, -10, -15, 18));

    q0 = inverse(q2);
    CHECK(q0 == quaternion::identity() / q2);
    CHECK(YamaApprox(q0 * q2) == quaternion::identity());

    CHECK(YamaApprox(inverse(q0)) == q2);

    q1 = quaternion::xyzw(0.5f, 0.7f, 0.212f, 0.8f);
    q2 = quaternion::xyzw(0.12f, 0.33f, 0.4f, 0.5f);

    q0 = slerp(q1, q2, 0.55f);

    CHECK(YamaApprox(q0) ==
        quaternion::xyzw(0.30942556881258626f, 0.527364923066248f, 0.33413672218578583f, 0.6741185090656765f));
}

TEST_CASE("rotate")
{
    const auto ux = vector3::unit_x();
    const auto uy = vector3::unit_y();
    const auto uz = vector3::unit_z();

    CHECK(YamaApprox(quaternion::rotation_axis(ux, 2.11f)) == quaternion::rotation_x(2.11f));
    CHECK(YamaApprox(quaternion::rotation_axis(uy, 0.13f)) == quaternion::rotation_y(0.13f));
    CHECK(YamaApprox(quaternion::rotation_axis(uz, 1.22f)) == quaternion::rotation_z(1.22f));

    auto q0 = quaternion::rotation_x(constants::PI_HALF());
    CHECK(YamaApprox(rotate(ux, q0)) == ux);
    CHECK(YamaApprox(rotate(uy, q0)) == uz);
    CHECK(YamaApprox(rotate(uz, q0)) == -uy);

    vector3 axis;
    float angle;
    q0.to_axis_angle(axis, angle);
    CHECK(YamaApprox(axis) == ux);
    CHECK(Approx(angle) == constants::PI_HALF());

    q0 = quaternion::rotation_y(constants::PI_HALF());
    CHECK(YamaApprox(rotate(ux, q0)) == -uz);
    CHECK(YamaApprox(rotate(uy, q0)) == uy);
    CHECK(YamaApprox(rotate(uz, q0)) == ux);

    q0.to_axis_angle(axis, angle);
    CHECK(YamaApprox(axis) == uy);
    CHECK(Approx(angle) == constants::PI_HALF());

    q0 = quaternion::rotation_z(constants::PI_HALF());
    CHECK(YamaApprox(rotate(ux, q0)) == uy);
    CHECK(YamaApprox(rotate(uy, q0)) == -ux);
    CHECK(YamaApprox(rotate(uz, q0)) == uz);

    q0.to_axis_angle(axis, angle);
    CHECK(YamaApprox(axis) == uz);
    CHECK(Approx(angle) == constants::PI_HALF());

    axis = v(1, 2, 3);
    angle = 2.66f;
    q0 = quaternion::rotation_axis(axis, angle);

    q0.to_axis_angle(axis, angle);
    auto q1 = quaternion::rotation_axis(axis, angle);
    CHECK(YamaApprox(q1) == q0);

    axis = v(1.2f, 2.1f, -2.45f);
    auto v1 = axis;
    angle = 1.44f;
    q0 = quaternion::rotation_axis(axis, angle);

    q0.to_axis_angle(axis, angle);
    q1 = quaternion::rotation_axis(axis, angle);
    CHECK(YamaApprox(q1) == q0);

    q1 *= q1;
    q0 = quaternion::rotation_axis(v1, 2.88f);
    CHECK(YamaApprox(q1) == q0);

    auto v0 = normalize(v(1, 2, 3));

    CHECK(quaternion::rotation_vectors(v0, v0) == quaternion::identity());

    v1 = normalize(v(-3, 5, 11));

    q0 = quaternion::rotation_vectors(v0, v1);
    CHECK(YamaApprox(rotate(v0, q0)) == v1);

    v1 = -v0;
    q0 = quaternion::rotation_vectors(v0, v1);
    CHECK(YamaApprox(rotate(v(1, 2, 3), q0)) == v(-1, -2, -3));
}
