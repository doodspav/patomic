include(CheckCSourceCompiles)


# ---- Check Source Compiles ----
#
# Checks that a string containing complete C source code compiles, and sets an
# output variable to 1 if it does, and 0 if it does not.
#
# If all the conditions passed to WILL_SUCCEED_IF_ALL are true, then the check
# is skipped, and the output variable is set to 1. If no conditions are passed
# then this does nothing.
#
# If any of the conditions passed to WILL_FAIL_IF_ANY_NOT are false, then the
# check is skipped, and the output variable is set to 0. If no conditions are
# passed then this does nothing.
#
# This function is necessary because check_c_source_compiles will set the
# output variable to an empty string, which cannot directly be used in a C
# preprocessing directive in the desired manner.
# It has the added benefit of skipping costly compilation checks if it can
# be predetermined that the check will succeed or fail without needing to
# run it.
#
# check_c_source_compiles_or_zero(
#     SOURCE <source>
#     OUTPUT_VARIABLE <outputVar>
#     [WILL_SUCCEED_IF_ALL <condition>...]
#     [WILL_FAIL_IF_ANY_NOT <condition>...]
# )
function(check_c_source_compiles_or_zero)

    # setup what arguments we expect

    cmake_parse_arguments(
        "ARG"
        ""
        "OUTPUT_VARIABLE"
        "SOURCE;WILL_SUCCEED_IF_ALL;WILL_FAIL_IF_ANY_NOT"
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
    set(success_count_true 0)

    # check the conditions provided
    foreach(cond IN LISTS ARG_WILL_SUCCEED_IF_ALL)
        if(cond)
            math(EXPR success_count_true "${success_count_true} + 1")
        endif()
    endforeach()

    # skip if we can (all conditions are true or all conditions are false)
    if(ARG_WILL_SUCCEED_IF_ALL)
        list(LENGTH ARG_WILL_SUCCEED_IF_ALL success_count)
        if(success_count_true EQUAL success_count)
            message(STATUS "Performing Test ${ARG_OUTPUT_VARIABLE}")
            message(STATUS "Performing Test ${ARG_OUTPUT_VARIABLE} - Success (implicit)")
            set(${ARG_OUTPUT_VARIABLE} 1 PARENT_SCOPE)
            return()
        endif()
    endif()


    # see if we can skip check due to implicit failure

    # setup
    set(failure_count_false 0)

    # check the conditions provided
    foreach(cond IN LISTS ARG_WILL_FAIL_IF_ANY_NOT)
        if(NOT cond)
            math(EXPR failure_count_false "${failure_count_false} + 1")
        endif()
    endforeach()

    # skip if we can (any conditions are false)
    if(ARG_WILL_FAIL_IF_ANY_NOT AND NOT failure_count_false EQUAL 0)
        message(STATUS "Performing Test ${ARG_OUTPUT_VARIABLE}")
        message(STATUS "Performing Test ${ARG_OUTPUT_VARIABLE} - Failed (implicit)")
        set(${ARG_OUTPUT_VARIABLE} 0 PARENT_SCOPE)
        return()
    endif()


    # defer check to CMake module

    # perform check
    # shadow parent scope variable so that output message uses its name
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
include(cmake/check/HasIdentifiers.cmake)
