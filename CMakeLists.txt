cmake_minimum_required(VERSION 3.14)

include(cmake/in-source-guard.cmake)

project(
    patomic
    VERSION 0.1.1
    DESCRIPTION "Portable C90 Atomics Library"
    HOMEPAGE_URL "https://github.com/doodspav/patomic"
    LANGUAGES C
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    patomic_patomic
    # include/types
    include/patomic/types/align.h
    include/patomic/types/memory_order.h
    include/patomic/types/options.h
    include/patomic/types/ops.h
    include/patomic/types/ids.h
    # src
    src/patomic.c
    # src/include/patomic/fake
    src/include/patomic/fake/stdatomic.h
    # src/include/patomic/macros
    src/include/patomic/macros/force_inline.h
    src/include/patomic/macros/ignore_unused.h
    src/include/patomic/macros/have_long_long.h
    src/include/patomic/macros/have_std_atomic.h
    src/include/patomic/macros/have_std_alignof.h
    # src/impl
    src/impl/register.h
    # src/impl/null
    src/impl/null/null.h
    src/impl/null/null.c
    # src/impl/std
    src/impl/std/std.h
    src/impl/std/std.c
)
add_library(patomic::patomic ALIAS patomic_patomic)

include(GenerateExportHeader)
generate_export_header(
    patomic_patomic
    BASE_NAME patomic
    EXPORT_FILE_NAME include/patomic/patomic_export.h
)

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

target_compile_features(patomic_patomic PUBLIC c_std_90)

# ---- Install rules ----

include(cmake/install-rules.cmake)

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
  message("Building stuff")
  add_subdirectory(test)
endif()
