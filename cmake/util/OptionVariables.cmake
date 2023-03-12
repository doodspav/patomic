# ---- Build Shared ----

# Sometimes it's useful to be able to single out a dependency to be built as
# static or shared, even if obtained from source.
if(PROJECT_IS_TOP_LEVEL)
    option(BUILD_SHARED_LIBS "Build shared libs" OFF)
endif()
option(
    patomic_BUILD_SHARED
    "Override BUILD_SHARED_LIBS for patomic library"
    ${BUILD_SHARED_LIBS}
)
mark_as_advanced(patomic_BUILD_SHARED)
set(patomic_BUILD_TYPE STATIC)
if(patomic_BUILD_SHARED)
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
        patomic_INCLUDES_WITH_SYSTEM
        "Use SYSTEM modifier for patomic's includes, disabling warnings"
        ON
    )
    mark_as_advanced(patomic_INCLUDES_WITH_SYSTEM)
    if(patomic_INCLUDES_WITH_SYSTEM)
        set(patomic_WARNING_GUARD SYSTEM)
    endif()
endif()
