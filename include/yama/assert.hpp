// Yama
// Copyright (c) 2016-2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include "config.hpp"
#include <cassert>

#define YAMA_ASSERT_LEVEL_NONE 0
#define YAMA_ASSERT_LEVEL_CRITICAL 1
#define YAMA_ASSERT_LEVEL_BAD 2
#define YAMA_ASSERT_LEVEL_ALL 3

#if !defined(YAMA_ASSERT_LEVEL)
#   define YAMA_ASSERT_LEVEL YAMA_ASSERT_LEVEL_ALL
#endif

#define _YAMA_NOOP(cond, msg)

#if YAMA_ASSERT_LEVEL == YAMA_ASSERT_LEVEL_NONE
#   define YAMA_ASSERT_CRIT _YAMA_NOOP
#   define YAMA_ASSERT_BAD _YAMA_NOOP
#   define YAMA_ASSERT_WARN _YAMA_NOOP
#elif YAMA_ASSERT_LEVEL == YAMA_ASSERT_LEVEL_CRITICAL
#   define YAMA_ASSERT_CRIT(condition, text) YAMA_ASSERT(condition, text)
#   define YAMA_ASSERT_BAD _YAMA_NOOP
#   define YAMA_ASSERT_WARN _YAMA_NOOP
#elif YAMA_ASSERT_LEVEL == YAMA_ASSERT_LEVEL_BAD
#   define YAMA_ASSERT_CRIT(condition, text) YAMA_ASSERT(condition, text)
#   define YAMA_ASSERT_BAD(condition, text) YAMA_ASSERT(condition, text)
#   define YAMA_ASSERT_WARN _YAMA_NOOP
#elif YAMA_ASSERT_LEVEL == YAMA_ASSERT_LEVEL_ALL
#   define YAMA_ASSERT_CRIT(condition, text) YAMA_ASSERT(condition, text)
#   define YAMA_ASSERT_BAD(condition, text) YAMA_ASSERT(condition, text)
#   define YAMA_ASSERT_WARN(condition, text) YAMA_ASSERT(condition, text)
#else
#   error "Yama: Invalid assertion level."
#endif

#if YAMA_HAS_CXX14
#   define YAMA_ASSERT_CRIT14(condition, text) YAMA_ASSERT_CRIT(condition, text)
#   define YAMA_ASSERT_BAD14(condition, text) YAMA_ASSERT_BAD(condition, text)
#   define YAMA_ASSERT_WARN14(condition, text) YAMA_ASSERT_WARN(condition, text)
#else
#   define YAMA_ASSERT_CRIT14 _YAMA_NOOP
#   define YAMA_ASSERT_BAD14 _YAMA_NOOP
#   define YAMA_ASSERT_WARN14 _YAMA_NOOP
#endif

#define YAMA_ASSERT(cond, msg) assert((cond) && msg)
