include(CheckCSourceCompiles)


# ---- Check Source Compiles ----
#
# Checks that a string containing complete C source code compiles, and sets an
# output variable to 1 if it does, and 0 if it does not.
#
# If all the conditions passed to WILL_SUCCEED_IF are true, then all checks are
# skipped and the output variable is set to 1.
#
# This function is necessary because check_c_source_compiles will set the
# output variable to an empty string, which cannot directly be used in a C
# preprocessing directive in the desired manner.
#
# check_c_source_compiles_or_zero(
#     SOURCE <source>
#     OUTPUT_VARIABLE <outputVar>
#     [WILL_SUCCEED_IF <condition>...]
# )
function(check_c_source_compiles_or_zero)

    # setup what arguments we expect

    cmake_parse_arguments(
        "ARG"
        ""
        "OUTPUT_VARIABLE"
        "SOURCE;WILL_SUCCEED_IF"
        ${ARGN}
    )


    # validate arguments

    # setup
    set(args_valid TRUE)
    set(func_name "check_c_source_compiles_or_zero")

    # check that SOURCE is set
    if(NOT ARG_SOURCE)
        message(WARNING "Option 'SOURCE' must be be specified with a value when invoking '${func_name}'")
        set(args_valid FALSE)
    endif()

    # check that OUTPUT_VARIABLE is set
    if(NOT ARG_OUTPUT_VARIABLE)
        message(WARNING "Option 'OUTPUT_VARIABLE' must be specified with a value when invoking '${func_name}'")
        set(args_valid FALSE)
    endif()

    # check that there are no leftover arguments
    if(ARG_UNPARSED_ARGUMENTS)
        message(WARNING "The following arguments were not recognised when invoking '${func_name}': ${ARG_UNPARSED_ARGUMENTS}")
        set(args_valid FALSE)
    endif()

    # abort if validation failed
    if(NOT args_valid)
        message(FATAL_ERROR "Aborting '${func_name}' due to invalid arguments")
    endif()


    # see if we can skip check due to implicit success

    # setup
    set(skip 0)
    if(ARG_WILL_SUCCEED_IF)
        set(skip 1)
    endif()

    # check the conditions provided
    foreach(cond IN LISTS ARG_WILL_SUCCEED_IF)
        if(cond)
            message("Condition ${cond}: 1")
            set(cond_ok 1)
        else()
            message("Condition ${cond}: 0")
            set(cond_ok 0)
        endif()
        math(EXPR skip "${skip} * ${cond_ok}" OUTPUT_FORMAT DECIMAL)
    endforeach()

    # skip if we can
    if(skip)
        message(STATUS "Performing Test ${ARG_OUTPUT_VARIABLE}")
        message(STATUS "Performing Test ${ARG_OUTPUT_VARIABLE} - Success (implicit)")
        set(${ARG_OUTPUT_VARIABLE} 1 PARENT_SCOPE)
        return()
    endif()


    # defer check to CMake module

    # perform check
    # shadow parent scope variable so that output message uses it
    check_c_source_compiles(
        "${ARG_SOURCE}"
        ${ARG_OUTPUT_VARIABLE}
    )
    set(result "${${ARG_OUTPUT_VARIABLE}}")

    # propagate result
    if(result)
        set(${ARG_OUTPUT_VARIABLE} 1 PARENT_SCOPE)
    else()
        set(${ARG_OUTPUT_VARIABLE} 0 PARENT_SCOPE)
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
