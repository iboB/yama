include(./get_cpm.cmake)

CPMAddPackage(
    NAME doctest-lib
    GITHUB_REPOSITORY iboB/doctest-lib
    GIT_TAG 2.4.3b
)

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
