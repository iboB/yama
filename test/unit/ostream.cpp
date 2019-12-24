// Yama
// Copyright (c) 2016-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "common.hpp"
#include "yama/ext/ostream.hpp"
#include <sstream>

using namespace yama;

TEST_SUITE("ext_ostream");

static void clr(std::ostringstream& o)
{
    o.str(std::string());
    o.clear();
}

TEST_CASE("test")
{
    std::ostringstream sout;

    auto v2 = v(11, 12);
    sout << v2;
    CHECK(sout.str() == "(11, 12)");

    clr(sout);

    auto v3 = v(25, 88, 11);
    sout << v3;
    CHECK(sout.str() == "(25, 88, 11)");

    clr(sout);

    auto v4 = v(-2, 3, 5, 11);
    sout << v4;
    CHECK(sout.str() == "(-2, 3, 5, 11)");

    clr(sout);

    auto m4 = matrix::columns(
        1, 5, 9, 13,
        2, 6, 10, 14,
        3, 7, 11, 15,
        4, 8, 12, 16
    );
    sout << m4;
    CHECK(sout.str() == "((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))");
}
