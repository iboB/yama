cmake_minimum_required(VERSION 3.2.2)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # if picobench is built as root, activate folders
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # demo means that the project is built from the root folder and not from
    # add_subdirectory
    # this means that subrojects (examples, tests) will be activated by default
    set(yama_is_demo ON)
else()
    set(yama_is_demo OFF)
endif()

project(yama)

option(YAMA_BUILD_UNIT_TESTS "yama: build tests" ${yama_is_demo})
mark_as_advanced(YAMA_BUILD_UNIT_TESTS)

set(inc ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(src ${inc}/yama)

add_library(yama INTERFACE)
target_sources(yama INTERFACE
    ${src}/assert.hpp
    ${src}/config.hpp
    ${src}/dim.hpp
    ${src}/matrix3x4.hpp
    ${src}/matrix4x4.hpp
    ${src}/quaternion.hpp
    ${src}/shorthand.hpp
    ${src}/type_traits.hpp
    ${src}/util.hpp
    ${src}/vector_xyzw.hpp
    ${src}/vector2.hpp
    ${src}/vector3.hpp
    ${src}/vector4.hpp
    ${src}/yama.hpp
    ${src}/ext/matrix4x4_ostream.hpp
    ${src}/ext/matrix3x4_ostream.hpp
    ${src}/ext/quaternion_ostream.hpp
    ${src}/ext/vector2_ostream.hpp
    ${src}/ext/vector3_ostream.hpp
    ${src}/ext/vector4_ostream.hpp
    ${src}/ext/ostream.hpp
)
target_include_directories(yama INTERFACE ${inc})
target_compile_features(yama INTERFACE
    cxx_auto_type
    cxx_constexpr
    cxx_delegating_constructors
    cxx_range_for
    cxx_right_angle_brackets
)

if(${YAMA_BUILD_UNIT_TESTS})
    enable_testing()
    add_subdirectory(test/unit)
endif()
