# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
include(./get_cpm.cmake)
CPMAddPackage(gh:iboB/doctest-lib@2.4.9a)

file(GLOB tests ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
source_group("tests" FILES ${tests})

add_executable(yama-unit-test
    ${tests}
)

target_link_libraries(yama-unit-test
    yama
    doctest-main
)

add_test(yama-unit-test yama-unit-test)
