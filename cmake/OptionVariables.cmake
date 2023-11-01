# included further down to avoid interfering with our cache variables
# include(GNUInstallDirs)


# ---- Options Summary ----

# ------------------------------------------------------------------------------------------------
# | Option                       | Availability  | Default                                       |
# |==============================|===============|===============================================|
# | BUILD_SHARED_LIBS            | Top-Level     | OFF                                           |
# | BUILD_TESTING                | Top-Level     | OFF                                           |
# | CMAKE_INSTALL_INCLUDEDIR     | Top-Level     | include/${package_name}-${PROJECT_VERSION}    |
# |------------------------------|---------------|-----------------------------------------------|
# | PATOMIC_BUILD_SHARED         | Always        | ${BUILD_SHARED_LIBS}                          |
# | PATOMIC_BUILD_TESTING        | Always        | ${BUILD_TESTING}                              |
# | PATOMIC_INCLUDES_WITH_SYSTEM | Not Top-Level | ON                                            |
# | PATOMIC_INSTALL_CMAKEDIR     | Always        | ${CMAKE_INSTALL_LIBDIR}/cmake/${package_name} |
# ------------------------------------------------------------------------------------------------


# ---- Build Shared ----

# Sometimes it's useful to be able to single out a dependency to be built as
# static or shared, even if obtained from source.
if(PROJECT_IS_TOP_LEVEL)
    option(BUILD_SHARED_LIBS "Build shared libs" OFF)
endif()
option(
    PATOMIC_BUILD_SHARED
    "Override BUILD_SHARED_LIBS for ${package_name} library"
    ${BUILD_SHARED_LIBS}
)
mark_as_advanced(PATOMIC_BUILD_SHARED)
set(build_type STATIC)
if(PATOMIC_BUILD_SHARED)
    set(build_type SHARED)
endif()


# ---- Warning Guard ----

# target_include_directories with SYSTEM modifier will request the compiler to
# omit warnings from the provided paths, if the compiler supports that.
# This is to provide a user experience similar to find_package when
# add_subdirectory or FetchContent is used to consume this project.
set(warning_guard "")
if(NOT PROJECT_IS_TOP_LEVEL)
    option(
        PATOMIC_INCLUDES_WITH_SYSTEM
        "Use SYSTEM modifier for ${package_name}'s includes, disabling warnings"
        ON
    )
    mark_as_advanced(PATOMIC_INCLUDES_WITH_SYSTEM)
    if(PATOMIC_INCLUDES_WITH_SYSTEM)
        set(warning_guard SYSTEM)
    endif()
endif()


# ---- Enable Testing ----

# By default tests aren't enabled even with BUILD_TESTING unless the library is
# built as a top level project.
# This is in order to cut down on unnecessary compile times, since it's unlikely
# for users to want to run the tests of their dependencies.
if(PROJECT_IS_TOP_LEVEL)
    option(BUILD_TESTING "Build tests" OFF)
endif()
if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    set(build_testing ON)
endif()
option(
    PATOMIC_BUILD_TESTING
    "Override BUILD_TESTING for ${package_name} library"
    ${build_testing}
)
set(build_testing )
mark_as_advanced(PATOMIC_BUILD_TESTING)


# ---- Install Include Directory ----

# Adds an extra directory to the include path by default, so that when you link
# against the target, you get `<prefix>/include/<package>-X.Y.Z` added to your
# include paths rather than `<prefix/include`.
# This doesn't affect include paths used by consumers of this project, but helps
# prevent headers from multiple versions affecting each other.
# The variable type is STRING rather than PATH, because otherwise passing
# -DCMAKE_INSTALL_INCLUDEDIR=include on the command line would expand to an
# absolute path with the base being the current CMake directory, leading to
# unexpected errors.
if(PROJECT_IS_TOP_LEVEL)
    set(
        CMAKE_INSTALL_INCLUDEDIR "include/${package_name}-${PROJECT_VERSION}"
        CACHE STRING ""
    )
    # marked as advanced in GNUInstallDirs version, so we follow their lead
    mark_as_advanced(CMAKE_INSTALL_INCLUDEDIR)
endif()


# do not include earlier or we can't set CMAKE_INSTALL_INCLUDEDIR above
# include required for CMAKE_INSTALL_LIBDIR below
include(GNUInstallDirs)


# ---- Install CMake Directory ----

# This allows package maintainers to freely override the installation path for
# the CMake configs.
# This doesn't affect include paths used by consumers of this project.
# The variable type is STRING rather than PATH, because otherwise passing
# -DPATOMIC_INSTALL_CMAKEDIR=lib/cmake on the command line would expand to an
# absolute path with the base being the current CMake directory, leading to
# unexpected errors.
set(
    PATOMIC_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package_name}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
mark_as_advanced(PATOMIC_INSTALL_CMAKEDIR)
