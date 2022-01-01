include(CheckTypeSize)

function(check_type_size_with_prelude prelude_contents type alias var)
    set(temp_var)
    # create temp header file
    set(temp_file_path "${PROJECT_BINARY_DIR}/__temp_patomicCheckTypeSizeWithPrelude.h")
    file(REMOVE ${temp_file_path})
    file(WRITE ${temp_file_path} "${prelude_contents}")
    # call check_type_size with our header
    set(CMAKE_EXTRA_INCLUDE_FILES ${temp_file_path})
    set(CMAKE_REQUIRED_QUIET "ON")
    check_type_size(
        ${type} temp_var
        LANGUAGE C
    )
    set(${var} ${temp_var} PARENT_SCOPE)
    # cleanup
    file(REMOVE ${temp_file_path})
    # print output
    message("-- Check size of ${alias}")
    if("${temp_var}" STREQUAL "")
        message("-- Check size of ${alias} - failed")
    elseif("${temp_var}" STREQUAL "0")
        message("-- Check size of ${alias} - architecture dependent")
    else()
        message("-- Check size of ${alias} - done")
    endif()
endfunction()

function(zero_if_blank var)
    if(${var} STREQUAL "")
        set(${var} 0 PARENT_SCOPE)
    endif()
endfunction()

#
# always available in C89/90
#

check_type_size(
    char COMPILER_SIZEOF_CHAR
    BUILTIN_TYPES_ONLY
    LANGUAGE C
)
zero_if_blank(COMPILER_SIZEOF_CHAR)

check_type_size(
    short COMPILER_SIZEOF_SHORT
    BUILTIN_TYPES_ONLY
    LANGUAGE C
)
zero_if_blank(COMPILER_SIZEOF_SHORT)

check_type_size(
    int COMPILER_SIZEOF_INT
    BUILTIN_TYPES_ONLY
    LANGUAGE C
)
zero_if_blank(COMPILER_SIZEOF_INT)

check_type_size(
    long COMPILER_SIZEOF_LONG
    BUILTIN_TYPES_ONLY
    LANGUAGE C
)
zero_if_blank(COMPILER_SIZEOF_LONG)

#
# not required to be available in C89/90
#

check_type_size(
    "long long" COMPILER_SIZEOF_LLONG
    BUILTIN_TYPES_ONLY
    LANGUAGE C
)
if("${COMPILER_SIZEOF_LLONG}" STREQUAL "")
    check_type_size_with_prelude(
        "__extension__ typedef long long patomic_llong_signed;\n\n"
        patomic_llong_signed "__extension__ long long" COMPILER_SIZEOF_LLONG
    )
endif()
zero_if_blank(COMPILER_SIZEOF_LLONG)

check_type_size(
    __int128 COMPILER_SIZEOF_I128
    BUILTIN_TYPES_ONLY
    LANGUAGE C
)
if("${COMPILER_SIZEOF_I128}" STREQUAL "")
    check_type_size_with_prelude(
        "\n\n__extension__ typedef __int128 patomic_ms_i128;\n\n"
        patomic_ms_i128 "__extension__ __int128" COMPILER_SIZEOF_I128
    )
endif()
zero_if_blank(COMPILER_SIZEOF_I128)
