// Yama
// Copyright (c) 2016 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <type_traits>

namespace yama
{

template <typename T>
struct is_yama : public std::false_type {};

template <typename T>
struct is_vector : public std::false_type {};

template <typename T>
struct is_matrix : public std::false_type {};

}
