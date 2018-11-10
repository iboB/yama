
file(GLOB tests ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
source_group("tests" FILES ${tests})
file(GLOB doctest ${CMAKE_CURRENT_SOURCE_DIR}/doctest/*.h)
source_group("third_party" FILES ${doctest})

add_executable(yama-unit-test
    ${tests}
    ${yama}
    ${doctest}
)

target_link_libraries(yama-unit-test yama)

add_test(yama-unit-test yama-unit-test)
