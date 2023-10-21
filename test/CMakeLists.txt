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

if(NOT TARGET patomic::patomic)
    find_package(patomic REQUIRED)
    enable_testing()
endif()

# for Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
find_package(GTest REQUIRED)
include(GoogleTest)


# ---- Declare Tests ----

add_custom_target(patomic-test)

# add BTs unconditionally
add_subdirectory(bt)

# only add UTs if patomic source files are available
if(PATOMIC_SOURCE_DIR)
   add_subdirectory(ut)
else()
    message(STATUS "Variable 'PATOMIC_SOURCE_DIR' empty or not set; skipping unit tests")
endif()


# ---- Support Packaging Library ----

if(PROJECT_IS_TOP_LEVEL AND NOT CMAKE_SKIP_INSTALL_RULES)
    include(CPack)
endif()
