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

# need to enable testing ourselves if we're the root project
if(PROJECT_IS_TOP_LEVEL)
    enable_testing()
endif()

# patomic is not a required dependency
if(NOT TARGET patomic::patomic)
    find_package(patomic QUIET)
endif()

# get GTest
find_package(GTest REQUIRED)
include(GoogleTest)

# sometimes this variable is not correctly set by GTest's find_package
if(NOT GTEST_INCLUDE_DIRS)
    if("${CMAKE_VERSION}" VERSION_GREATER_EQUAL "3.20.0")
        get_target_property(
            GTEST_INCLUDE_DIRS
            GTest::gtest INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
        )
    else()
        get_target_property(
            GTEST_INCLUDE_DIRS
            GTest::GTest INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
        )
    endif()
endif()

# get libatomic if it exists (.so.1 in case of no namelink, e.g. FC19 and Ubuntu)
# courtesy of HP's foedus_code's FindGccAtomic.cmake (the .so.1 part)
find_library(libatomic NAMES atomic atomic.so.1 libatomic.so.1)

# link GTest to libatomic if it exists
# necessary for using GTest on platforms without atomic operations where
# libatomic is not automatically linked (e.g. clang-15 for arm-linux-gnueabi)
if(libatomic)
    set_property(
        TARGET GTest::gtest
        APPEND PROPERTY INTERFACE_LINK_LIBRARIES "${libatomic}"
    )
endif()


# ---- Declare Tests ----

add_custom_target(patomic-test)

# always add include and src; used by and linked to all test targets
# must be added first to create targets that we use when creating test targets
add_subdirectory(include)
add_subdirectory(src)

# only add BTs if patomic target is available
if(TARGET patomic::patomic)
    add_subdirectory(bt)
    message(STATUS "Enabled binary tests")
else()
    message(STATUS "Skipping binary tests; patomic target not available")
endif()

# only add UTs if patomic files are available
# these are currently set by patomic before including this project
if(PATOMIC_BINARY_DIR AND PATOMIC_SOURCE_DIR)
    add_subdirectory(ut)
    message(STATUS "Enabled unit tests")
else()
    message(STATUS "Skipping unit tests; not building as sub-project of patomic")
endif()

# only add CIs if running in CI
if($ENV{CI})
    add_subdirectory(ci)
    message(STATUS "Enabled continuous integration tests")
else()
    message(STATUS "Skipping continuous integration tests; not building as part of pipeline run")
endif()


# ---- Support Packaging Library ----

if(PROJECT_IS_TOP_LEVEL AND NOT CMAKE_SKIP_INSTALL_RULES)
    include(CPack)
endif()
