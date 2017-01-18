// Yama
// Copyright (c) 2016 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

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
#   define YAMA_ASSERT_CRIT(condition, text) _YAMA_NOOP
#   define YAMA_ASSERT_BAD(condition, text) _YAMA_NOOP
#   define YAMA_ASSERT_WARN(condition, text) _YAMA_NOOP
#elif YAMA_ASSERT_LEVEL == YAMA_ASSERT_LEVEL_CRITICAL
#   define YAMA_ASSERT_CRIT(condition, text) YAMA_ASSERT(condition, text)
#   define YAMA_ASSERT_BAD(condition, text) _YAMA_NOOP
#   define YAMA_ASSERT_WARN(condition, text) _YAMA_NOOP
#elif YAMA_ASSERT_LEVEL == YAMA_ASSERT_LEVEL_BAD
#   define YAMA_ASSERT_CRIT(condition, text) YAMA_ASSERT(condition, text)
#   define YAMA_ASSERT_BAD(condition, text) YAMA_ASSERT(condition, text)
#   define YAMA_ASSERT_WARN(condition, text) _YAMA_NOOP
#elif YAMA_ASSERT_LEVEL == YAMA_ASSERT_LEVEL_ALL
#   define YAMA_ASSERT_CRIT(condition, text) YAMA_ASSERT(condition, text)
#   define YAMA_ASSERT_BAD(condition, text) YAMA_ASSERT(condition, text)
#   define YAMA_ASSERT_WARN(condition, text) YAMA_ASSERT(condition, text)
#else
#   error "Yama: Invalid assertion level."
#endif

#define YAMA_ASSERT(cond, msg) assert((cond) && msg)
