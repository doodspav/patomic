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

# for Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
find_package(GTest REQUIRED)
include(GoogleTest)


# ---- Declare Tests ----

add_custom_target(patomic-test)

# only add BTs if patomic target is available
if(TARGET patomic::patomic)
    add_subdirectory(bt)
else()
    message(STATUS "Skipping binary tests; patomic target not available")
endif()

# only add UTs if patomic files are available
# these are currently set by patomic before including this project
if(PATOMIC_BINARY_DIR AND PATOMIC_SOURCE_DIR)
   add_subdirectory(ut)
else()
    message(STATUS "Skipping unit tests; not building as sub-project of patomic")
endif()


# ---- Support Packaging Library ----

if(PROJECT_IS_TOP_LEVEL AND NOT CMAKE_SKIP_INSTALL_RULES)
    include(CPack)
endif()
