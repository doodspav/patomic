cmake_minimum_required(VERSION 3.14)

project(patomic_test LANGUAGES CXX)

include(../cmake/ProjectIsTopLevel.cmake)
include(../cmake/WindowsSetPath.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(patomic REQUIRED)
  enable_testing()
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.10.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    patomic_test
    # include
    include/patomic_test/patomic_test.hpp
    include/patomic_test/aligned_buffer.hpp
    include/patomic_test/generic_int.hpp
    include/patomic_test/curry_op.hpp
    # src
    src/test_logic_ops_buffer.cpp
    src/test_logic_ops_arithmetic.cpp
    src/test_death_ops_valid_order.cpp
)
target_include_directories(
    patomic_test
    SYSTEM
    PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>"
)
target_link_libraries(
    patomic_test
    PRIVATE patomic::patomic
    PRIVATE gtest_main
)
target_compile_features(
    patomic_test
    PRIVATE
    cxx_generic_lambdas
    cxx_variadic_templates
)

include(GoogleTest)
gtest_add_tests(
    TARGET patomic_test
    TEST_LIST patomic_test_TESTS
)

foreach(test IN LISTS patomic_test_TESTS)
    windows_set_path("${test}" patomic::patomic gtest)
endforeach()