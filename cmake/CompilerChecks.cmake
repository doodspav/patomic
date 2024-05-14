include(CheckCSourceCompiles)


# ---- Check Source Compiles ----
#
# Checks that a string containing complete C source code compiles, and sets an
# output variable to 1 if it does, and 0 if it does not.
#
# This is necessary because check_c_source_compiles will set the output
# variable to an empty string, which cannot directly be used in a C
# preprocessing directive in the desired manner.
#
# check_c_source_compiles_or_zero(
#     <source>
#     <resultVar>
# )
function(check_c_source_compiles_or_zero source resultVar)

    # resultVar is intentionally used both as local and parent scope variable
    # this is so that the status message uses the parent scope variable name

    # defer check to CMake module
    check_c_source_compiles(
        "${source}"
        ${resultVar}
    )

    # propagate result
    if(${resultVar})
        set(${resultVar} 1 PARENT_SCOPE)
    else()
        set(${resultVar} 0 PARENT_SCOPE)
    endif()

endfunction()


# ---- Perform Checks ----

# include all compiler checks performed by CMake
# this relies on the previously defined functions above
#
# WARNING:
#   DO NOT change the include order, checks can depend on other checks
include(cmake/check/HasKeywords.cmake)
include(cmake/check/HasIntegerTypes.cmake)
