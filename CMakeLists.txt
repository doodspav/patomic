cmake_minimum_required(VERSION 3.14)

include(cmake/InSourceGuard.cmake)


project(
    patomic
    VERSION 0.3.0
    DESCRIPTION "Portable C90 Atomics Library"
    HOMEPAGE_URL "https://github.com/doodspav/patomic"
    LANGUAGES C
)

include(cmake/ProjectIsTopLevel.cmake)
include(cmake/Variables.cmake)

# ---- Declare library ----

add_library(
    patomic_patomic
    # include
    include/patomic/patomic.h
    # include/types
    include/patomic/types/align.h
    include/patomic/types/feature_check.h
    include/patomic/types/memory_order.h
    include/patomic/types/options.h
    include/patomic/types/ops.h
    include/patomic/types/ids.h
    include/patomic/types/transaction.h
    # src
    src/patomic.c
    # src/include/patomic
    src/include/patomic/patomic_config.h
    # src/include/patomic/macros
    src/include/patomic/macros/force_inline.h
    src/include/patomic/macros/func_name.h
    src/include/patomic/macros/ignore_unused.h
    src/include/patomic/macros/noinline.h
    src/include/patomic/macros/noreturn.h
    src/include/patomic/macros/restrict.h
    src/include/patomic/macros/static_assert.h
    src/include/patomic/macros/unreachable.h
    # src/include/patomic/stdlib
    src/include/patomic/stdlib/assert.h
    src/include/patomic/stdlib/string.h
    # src/include/patomic/types
    src/include/patomic/types/intptr.h
    # src/patomic
    src/patomic/patomic_version.c
    # src/patomic/stdlib
    src/patomic/stdlib/assert.c
    src/patomic/stdlib/string.c
    # src/patomic/types
    src/patomic/types/align.c
    src/patomic/types/feature_check.c
    src/patomic/types/memory_order.c
    src/patomic/types/transaction.c
    # src/impl
    src/impl/register.h
    # src/impl/null
    src/impl/null/null.h
    src/impl/null/null.c
    # src/impl/std
    src/impl/std/std.h
    src/impl/std/std.c
    # src/impl/msvc
    src/impl/msvc/msvc.h
    src/impl/msvc/msvc.c
    src/impl/msvc/msvc_x86.h
    src/impl/msvc/msvc_arm.h
    src/impl/msvc/msvc_ext.h
    src/impl/msvc/generic_uint.h
    # src/impl/tsx
    src/impl/tsx/tsx.h
    src/impl/tsx/tsx.c
    # src/impl/gnu
    src/impl/gnu/gnu.h
    src/impl/gnu/gnu.c
)
add_library(patomic::patomic ALIAS patomic_patomic)

# ---- Export library ----

include(GenerateExportHeader)
generate_export_header(
    patomic_patomic
    BASE_NAME patomic
    EXPORT_FILE_NAME include/patomic/patomic_export.h
)

configure_file(
    cmake/in/patomic_version.h.in
    include/patomic/patomic_version.h
    @ONLY
)

include(cmake/CompilerFeatureChecks.cmake)
configure_file(
    cmake/in/patomic_config.h.in
    src/include/patomic/_patomic_config.h
    @ONLY
)

# ---- Library properties ----

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(patomic_patomic PUBLIC PATOMIC_STATIC_DEFINE)
endif()

set_target_properties(
    patomic_patomic PROPERTIES
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME patomic
    OUTPUT_NAME patomic
)

target_include_directories(
    patomic_patomic
    ${patomic_warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>"
)

# macro includes (private)
target_include_directories(
    patomic_patomic
    ${patomic_warning_guard}
    PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/include>"
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src/include>"
)

# ---- Enable IPO when not Debug ----

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_result OUTPUT ipo_output)
if(ipo_result)
    set_target_properties(
        patomic_patomic
        PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION ON
        INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF
    )
endif()

# ---- Link libatomic ----

# .so.1 in case of no .so symlink (e.g. FC19 and Ubuntu)
# courtesy of HP's foedus_code's FindGccAtomic.cmake#L24-L25 @f72fe1905a
find_library(ATOMIC NAMES atomic atomic.so.1 libatomic.so.1)
if(ATOMIC)
    target_link_libraries(
        patomic_patomic
        PRIVATE
        ${ATOMIC}
    )
endif()

# ---- Install rules ----

include(cmake/InstallRules.cmake)

# ---- Developer mode ----

if(NOT patomic_DEVELOPER_MODE)
    return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
    message(
        AUTHOR_WARNING
        "Developer mode is intended for developers of patomic"
    )
endif()

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(test)
endif()
