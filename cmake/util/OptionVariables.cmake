# ---- Build Shared ----

# Sometimes it's useful to be able to single out a dependency to be built as
# static or shared, even if obtained from source.
if(PROJECT_IS_TOP_LEVEL)
    option(BUILD_SHARED_LIBS "Build shared libs" OFF)
endif()
option(
    PATOMIC_BUILD_SHARED
    "Override BUILD_SHARED_LIBS for patomic library"
    ${BUILD_SHARED_LIBS}
)
mark_as_advanced(PATOMIC_BUILD_SHARED)
set(patomic_BUILD_TYPE STATIC)
if(PATOMIC_BUILD_SHARED)
    set(patomic_BUILD_TYPE SHARED)
endif()


# ---- Warning Guard ----

# target_include_directories with SYSTEM modifier will request the compiler to
# omit warnings from the provided paths, if the compiler supports that.
# This is to provide a user experience similar to find_package when
# add_subdirectory or FetchContent is used to consume this project.
set(patomic_WARNING_GUARD "")
if(NOT PROJECT_IS_TOP_LEVEL)
    option(
        PATOMIC_INCLUDES_WITH_SYSTEM
        "Use SYSTEM modifier for patomic's includes, disabling warnings"
        ON
    )
    mark_as_advanced(PATOMIC_INCLUDES_WITH_SYSTEM)
    if(PATOMIC_INCLUDES_WITH_SYSTEM)
        set(patomic_WARNING_GUARD SYSTEM)
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
option(
    PATOMIC_BUILD_TESTING
    "Override BUILD_TESTING for patomic library"
    ${BUILD_TESTING}
)
mark_as_advanced(PATOMIC_BUILD_TESTING)
