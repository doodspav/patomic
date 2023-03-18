cmake_minimum_required(VERSION 3.14)

project(
    patomic_test
    DESCRIPTION "Standalone tests for patomic library"
    HOMEPAGE_URL "https://github.com/doodspav/patomic"
    LANGUAGES CXX
)


# ---- Dependencies ----

include(cmake/ProjectIsTopLevel.cmake)

if(PROJECT_IS_TOP_LEVEL)
    find_package(patomic REQUIRED)
    enable_testing()
endif()

find_package(GTest REQUIRED)
include(GoogleTest)


# ---- Declare Tests ----

add_executable(
    patomic_test
    example_add.cpp
)
add_executable(patomic::test ALIAS patomic_test)

target_link_libraries(
    patomic_test
    PRIVATE
    patomic::patomic
    GTest::gtest_main
)

target_compile_features(patomic_test PRIVATE cxx_std_14)


# ---- Discover Tests ----

gtest_add_tests(
    TARGET patomic_test
    TEST_LIST patomic_test_TESTS
)
