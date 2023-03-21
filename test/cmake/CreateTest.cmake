# ---- Create Test ----

# Creates a target to build a test executable and registers it with CTest.
# Expects a target named patomic_${kind} to exist.
# E.g. if you call it as create_test(BT ...) then patomic_bt must exist.
#
# create_test(
#     BT|UT <name>
#     [INCLUDE <item>...]
#     [SOURCE <item>...]
#     [LINK <item>...]
# )
function(create_test)

    # setup what arguments we expect

    set(all_kinds "BT;UT")  # list we can iterate over
    set(all_kinds_option "BT|UT")  # string to use in debug message

    cmake_parse_arguments(
        "ARG"
        ""
        "${all_kinds}"
        "INCLUDE;SOURCE;LINK"
        ${ARGN}
    )


    # check what test kinds are passed

    set(kind "")  # -> "bt"
    set(name "")  # -> "${ARG_BT}"
    foreach(ak IN LISTS all_kinds)
        # if(ARG_BT)
        if (ARG_${ak})
            string(TOLOWER ${ak} ak_lower)
            # kind.append(bt)
            # name = ${ARG_BT}
            list(APPEND kind "${ak_lower}")
            set(name "${ARG_${ak}}")
        endif()
    endforeach()


    # validate arguments

    set(args_valid TRUE)
    set(func_name "create_test")
    set(LENGTH kind kinds_count)

    if(kinds_count EQUAL 0)
        message(WARNING "${all_kinds_option} option needs to be specified when invoking '${func_name}'")
        set(args_valid FALSE)
    elseif(kinds_count GREATER 1)
        message(WARNING "Only a single ${all_kinds_option} option may be specified when invoking '${func_name}'")
        set(args_valid FALSE)
    elseif(TARGET ${name})
        message(WARNING "Test name must not be an existing target when invoking '${func_name}', was passed: ${name}")
        set(args_valid FALSE)
    elseif("${name}" STREQUAL "")
        message(WARNING "Test name must not be empty when invoking '${func_name}'")
    endif()

    if(DEFINED ARG_UNPARSED_ARGUMENTS)
        message(WARNING "The following arguments were not recognised when invoking '${func_name}': ${ARG_UNPARSED_ARGUMENTS}")
        set(args_valid FALSE)
    endif()

    if (NOT args_valid)
        message(FATAL_ERROR "Aborting '${func_name}' due to invalid arguments")
    endif()


    # setup target

    set(parent_target patomic_${kind})
    set(target ${name})
    set(target_deps patomic::patomic GTest::gtest_main ${ARG_LINK})

    add_executable(
        ${target}
        ${ARG_INCLUDE}
        ${ARG_SOURCE}
    )

    target_link_libraries(
        ${target}
        PRIVATE
        ${target_deps}
    )

    target_compile_features(${target} PRIVATE cxx_std_14)


    # setup tests with CTest

    # must be run in same directory scope as target
    gtest_add_tests(
        TARGET ${target}
        TEST_LIST added_tests
    )


    # deal with Windows runtime linker issues for tests (with and without CTest)

    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")

        # get paths to all shared library dependencies (DLLs)
        windows_deps_path(
            deps_path
            ${target_deps}
        )

        # set environment variable for the each test so that CTest works automatically
        if (deps_path AND patomic_test_SET_CTEST_PATH_ENV_WINDOWS)
            foreach(test IN LISTS added_tests)
                set_property(
                    TEST "${test}"
                    PROPERTY ENVIRONMENT "PATH=${deps_path}"
                )
            endforeach()
        endif()

        # make dependencies accessible from parent target
        set_property(
            TARGET ${parent_target}
            APPEND PROPERTY WIN_DEP_TARGETS ${target_deps}
        )
    endif()


    # setup install of target

    if(NOT CMAKE_SKIP_INSTALL_RULES)
        install(
            TARGETS ${target}
            RUNTIME #
            COMPONENT "${parent_target}"
            DESTINATION "${CMAKE_INSTALL_TESTDIR}/patomic/${kind}"
            EXCLUDE_FROM_ALL
        )
    endif()


    # attach to parent target

    add_dependencies(${parent_target} ${target})

endfunction()


# ---- Create Test Dependency File ----

# Creates a file containing the output of windows_deps_path for all tests of
# the given kind registered so far.
# Expects a target named patomic_${kind} to exist
# E.g. if you call it as create_test_win_deps_path_file(BT) then patomic_bt must
# exist.
#
# create_test_win_deps_path_file(
#     BT|UT
# )
function(create_test_win_deps_path_file ARG_KIND)

    # check KIND is valid

    set(all_kinds_option "BT|UT")
    set(func_name "create_test_win_deps_path_file")

    if (NOT ARG_KIND MATCHES "^(${all_kinds_option})$")
        message(WARNING "${all_kinds_option} option needs to be specified when invoking '${func_name}'")
        message(FATAL_ERROR "Aborting '${func_name}' due to invalid arguments")
    endif()

    string(TOLOWER ${ARG_KIND} kind)


    # create and install file with dependencies path

    if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")

        # get dependencies set by create_test from target
        get_target_property(dep_targets patomic_${kind} WIN_DEP_TARGETS)
        list(REMOVE_DUPLICATES dep_targets)

        # get paths to all shared library dependencies (DLLs)
        windows_deps_path(
            deps_path
            ${dep_targets}
        )

        # create file
        file(GENERATE
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/windows_dependencies_path.txt"
            CONTENT "${deps_path}"
        )

        # copy file to install location
        if(NOT CMAKE_SKIP_INSTALL_RULES)
            install(
                FILES "${CMAKE_CURRENT_BINARY_DIR}/windows_dependencies_path.txt"
                COMPONENT patomic_${kind}
                DESTINATION "${CMAKE_INSTALL_TESTDIR}/patomic/${kind}"
                EXCLUDE_FROM_ALL
            )
        endif()
    endif()

endfunction()
