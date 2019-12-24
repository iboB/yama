// Yama
// Copyright (c) 2016-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "common.hpp"
#include "yama/vector_xyzw.hpp"

TEST_SUITE("vector_xyzw");

using namespace yama;

TEST_CASE("xyzw")
{
    auto v2 = v(1, 2);
    CHECK(v2.xy() == v(1, 2));
    v2.xy() = v(3, 4);
    CHECK(v2 == v(3, 4));
    CHECK(v2.xyz() == v(3, 4, 0));
    CHECK(v2.xyz(11) == v(3, 4, 11));
    CHECK(v2.xyzw() == v(3, 4, 0, 0));
    CHECK(v2.xyzw(5, 6) == v(3, 4, 5, 6));

    auto v3 = v(10, 50, 69);
    CHECK(v3.xyz() == v(10, 50, 69));
    v3.xyz() = v(6, 7, 8);
    CHECK(v3 == v(6, 7, 8));
    CHECK(v3.xy() == v(6, 7));
    CHECK(v3.xz() == v(6, 8));
    v3.xy() = v(1, 2);
    CHECK(v3 == v(1, 2, 8));
    CHECK(v3.xyzw() == v(1, 2, 8, 0));
    CHECK(v3.xyzw(3) == v(1, 2, 8, 3));
    CHECK(v3.zyx() == v(8, 2, 1));

    auto v4 = v(9, 8, 7, 6);
    CHECK(v4.xyzw() == v(9, 8, 7, 6));
    CHECK(v4.zyxw() == v(7, 8, 9, 6));
    CHECK(v4.wzyx() == v(6, 7, 8, 9));
    v4.xyzw() = v(1, 2, 3, 4);
    CHECK(v4 == v(1, 2, 3, 4));
    CHECK(v4.xy() == v(1, 2));
    CHECK(v4.xz() == v(1, 3));
    CHECK(v4.zw() == v(3, 4));
    v4.xy() = v(6, 7);
    CHECK(v4 == v(6, 7, 3, 4));
    CHECK(v4.xyz() == v(6, 7, 3));
    CHECK(v4.zyx() == v(3, 7, 6));
    v4.xyz() = v(1, 2, 3);
    CHECK(v4 == v(1, 2, 3, 4));
}
