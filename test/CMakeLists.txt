cmake_minimum_required(VERSION 3.14)

include(cmake/PreventInSourceBuilds.cmake)

project(
    patomic_test
    DESCRIPTION "Standalone tests for patomic library"
    HOMEPAGE_URL "https://github.com/doodspav/patomic"
    LANGUAGES CXX
)

include(cmake/OptionVariables.cmake)


# ---- Dependencies ----

include(cmake/ProjectIsTopLevel.cmake)

if(PROJECT_IS_TOP_LEVEL)
    find_package(patomic REQUIRED)
    enable_testing()
endif()

# for Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
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


# ---- Windows Path Issues ----

include(cmake/WindowsDependenciesPath.cmake)
windows_deps_path(
    deps_path
    patomic::patomic
    GTest::gtest_main
)

# check we're on Windows, and we have SHARED_LIBRARY dependencies
if(NOT deps_path STREQUAL "" AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")

    # set environment variable for each test so that ctest works automatically
    if(patomic_test_SET_CTEST_PATH_ENV_WINDOWS)
        foreach(test IN LISTS patomic_test_TESTS)
            set_property(
                TEST "${test}"
                PROPERTY ENVIRONMENT "PATH=${deps_path}"
            )
        endforeach()
    endif()

    # create file with PATH contents for when not running tests through CMake
    file(GENERATE
        OUTPUT "${PROJECT_BINARY_DIR}/windows_dependencies_path.txt"
        CONTENT "${deps_path}"
    )
endif()


# ---- Install Rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/InstallRules.cmake)
endif()
