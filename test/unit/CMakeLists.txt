cmake_minimum_required(VERSION 2.8)

project(yama-test)

enable_testing()

set(INC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
include_directories(${INC})

file(GLOB_RECURSE yama "${INC}/yama/*.hpp" "${INC}/yama/*.inl")
source_group("yama" FILES ${yama})
file(GLOB tests ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
source_group("tests" FILES ${tests})
file(GLOB doctest ${CMAKE_CURRENT_SOURCE_DIR}/doctest/*.h)
source_group("third_party" FILES ${doctest})

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

add_executable(yama-test
    ${tests}
    ${yama}
    ${doctest}
)

add_test(yama-test yama-test)
