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

add_custom_target(patomic_test)

# add all test subdirectories
add_subdirectory(bt)
add_subdirectory(ut)


# ---- Support Packaging Library ----

if(PROJECT_IS_TOP_LEVEL AND NOT CMAKE_SKIP_INSTALL_RULES)
    include(CPack)
endif()
