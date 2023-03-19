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


# ---- Windows Path Issues ----

include(cmake/WindowsDependenciesPath.cmake)
windows_deps_path(
    deps_path
    patomic::patomic
    GTest::gtest_main
)

# check we're on Windows, and we have SHARED_LIBRARY dependencies
set(windows_and_shared FALSE)
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows" AND NOT deps_path STREQUAL "")
    set(windows_and_shared TRUE)

    # create file with PATH contents for when running tests independent of CTest
    file(GENERATE
        OUTPUT "${PROJECT_BINARY_DIR}/windows_dependencies_path.txt"
        CONTENT "${deps_path}"
    )
endif()


# ---- Declare Tests ----

add_custom_target(patomic_test)
set_target_properties(
    patomic_test
    PROPERTIES
    WIN_DEPS_PATH  ${deps_path}
    WIN_AND_SHARED ${windows_and_shared}
)

# add all test subdirectories
add_subdirectory(bt)
add_subdirectory(ut)


# ---- Further Installation and Packaging ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/InstallRules.cmake)
endif()
