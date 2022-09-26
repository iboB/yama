// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
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
