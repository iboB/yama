# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.14)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # dev_mode is used below to make life simpler for developers
    # it enables some configurations and the defaults for building tests and examples
    # which typically wouldn't be built if confy is a subdirectory of another project
    set(dev_mode ON)
else()
    set(dev_mode OFF)
endif()

project(yama)

option(YAMA_BUILD_UNIT_TESTS "yama: build tests" ${dev_mode})
option(YAMA_BUILD_SCRATCH "yama: build scratch project for testing and experiments" ${dev_mode})

mark_as_advanced(YAMA_BUILD_UNIT_TESTS YAMA_BUILD_SCRATCH)

if(dev_mode)
    include(./dev.cmake)
endif()

add_library(yama INTERFACE)
add_library(yama::yama ALIAS yama)
target_include_directories(yama INTERFACE include)

if(${YAMA_BUILD_UNIT_TESTS})
    enable_testing()
    add_subdirectory(test/unit)
endif()
