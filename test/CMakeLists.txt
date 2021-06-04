cmake_minimum_required(VERSION 3.14)

project(patomic_test LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/windows-set-path.cmake)

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
    include/patomic_test/curry_op.hpp
    # logic
    logic/test_buffer_ops.cpp
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
gtest_discover_tests(patomic_test)

windows_set_path(patomic_test patomic::patomic gtest_main)