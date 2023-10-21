# ---- Create Test ----

# Creates a target to build a test executable and registers it with CTest.
# Expects a target named patomic-test-${kind} to exist.
# E.g. if you call it as create_test(BT ...) then patomic-test-bt must exist.
#
# Naming:
# - created target -> patomic-test-${kind}-${name} (e.g. patomic-test-bt-SomeExample)
# - executable name -> ${name} (e.g. SomeExample on Unix or SomeExample.exe on Windows)
# - install directory -> ${CMAKE_INSTALL_TESTDIR}/${kind} (e.g. share/test/bt)
#
# Hierarchy:
# - patomic-test -> base custom target & component (build/install) for all tests
# - patomic-test-${kind} -> custom target & component (build install) for all tests of a specific kind
# - patomic-test-${kind}-${name} -> executable target for a single test
#
# _create_test(
#     BT|UT <name>
#     [INCLUDE <item>...]
#     [SOURCE <item>...]
#     [LINK <item>...]
# )
function(_create_test)

    # setup what arguments we expect

    set(all_kinds "BT;UT")  # list of all kinds we can iterate over
    set(all_kinds_opt_msg "BT|UT")  # string to use in debug message

    cmake_parse_arguments(
        "ARG"
        ""
        "${all_kinds}"
        "INCLUDE;SOURCE;LINK"
        ${ARGN}
    )


    # check what test kinds are passed

    set(kind "")  # -> "bt"
    set(name "")  # -> value of "${ARG_BT}"
    # we turn 'kind' into a list so that we can check its length (which should be 1)
    # 'name' is just the last name we process (there should only be 1 name)
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

    # setup
    set(args_valid TRUE)
    set(func_name "create_test")  # CMAKE_CURRENT_FUNCTION is CMake 3.17+
    set(LENGTH kind kinds_count)  # expected value is 1

    # go through all possible issues with arguments
    if(kinds_count EQUAL 0)
        message(WARNING "'${all_kinds_opt_msg}' option must be specified invoking '${func_name}'")
        set(args_valid FALSE)
    elseif(kinds_count GREATER 1)
        message(WARNING "Only a single '${all_kinds_opt_msg}' option may be specified when invoking '${func_name}'")
        set(args_valid FALSE)
    elseif(TARGET ${name})
        message(WARNING "Test name must not be an existing target when invoking '${func_name}', was passed: '${name}'")
        set(args_valid FALSE)
    elseif("${name}" STREQUAL "")
        message(WARNING "Test name must not be empty when invoking '${func_name}'")
        set(args_valid FALSE)
    endif()

    # check there are no leftover arguments
    if(DEFINED ARG_UNPARSED_ARGUMENTS)
        message(WARNING "The following arguments were not recognised when invoking '${func_name}': ${ARG_UNPARSED_ARGUMENTS}")
        set(args_valid FALSE)
    endif()

    # abort if validation failed
    if(NOT args_valid)
        message(FATAL_ERROR "Aborting '${func_name}' due to invalid arguments")
    endif()


    # create test target

    # setup
    set(base_target patomic-test)              # patomic-test
    set(parent_target ${base_target}-${kind})  # patomic-test-bt
    set(target ${parent_target}-${name})       # patomic-test-bt-SomeExample

    # check target name matches pattern
    if(NOT "${target}" MATCHES "${PATOMIC_CREATE_TEST_TARGETS_MATCHING}")
        message(VERBOSE "Skipping creation of test target '${target}' (does not match ${PATOMIC_CREATE_TEST_TARGETS_MATCHING})")
    endif()

    # create target with sources
    add_executable(
        ${target}
        ${ARG_SOURCE}
    )

    # add include directories
    target_include_directories(
        ${target} PRIVATE
        ${ARG_INCLUDE}
    )

    # link dependencies (all tests use GTest framework)
    # update list directly because we use it in Windows PATH stuff later
    list(APPEND ARG_LINK GTest::gtest_main GTest::gmock_main)
    target_link_libraries(
        ${target} PRIVATE
        ${ARG_LINK}
    )

    # require C++14 as minimum
    target_compile_features(
        ${target} PRIVATE
        cxx_std_14
    )

    # set macro to know which test kind code is part of
    string(TOUPPER "${kind}" kind_upper)
    target_compile_definitions(
        ${target} PRIVATE
        "PATOMIC_${kind_upper}"
    )

    # set binary name instead of using default
    set_target_properties(
        ${target} PROPERTIES
        OUTPUT_NAME "${name}"
    )


    # register test with GTest/CTest and parent target

    # must be run in same directory scope as target
    gtest_add_tests(
        TARGET ${target}
        TEST_LIST added_tests
    )

    add_dependencies(${parent_target} ${target})


    # deal with Windows runtime linker issues

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")

        # check we actually care about Windows PATH stuff
        if(NOT PATOMIC_WINDOWS_SET_CTEST_PATH_ENV AND
           NOT PATOMIC_WINDOWS_CREATE_PATH_ENV_FILE)
            return()
        endif()

        # get paths to all shared library dependencies (DLLs)
        windows_deps_paths(
            deps_paths
            ${ARG_LINK}
        )

        # set environment variable for each test so that CTest works
        if(deps_paths AND PATOMIC_WINDOWS_SET_CTEST_PATH_ENV)
            foreach(test IN LISTS added_tests)
                set_property(
                    TEST "${test}"
                    PROPERTY ENVIRONMENT "PATH=${deps_paths}"
                )
            endforeach()
        endif()

        # make dependencies accessible from parent target
        # so that we can create single file for all tests in kind
        if(PATOMIC_WINDOWS_CREATE_PATH_ENV_FILE)
            set_property(
                TARGET ${parent_target}
                APPEND PROPERTY WIN_DEPS_TARGETS "${ARG_LINK}"
            )
        endif()

    endif()


    # setup install of target

    if(NOT CMAKE_SKIP_INSTALL_RULES)

        # install as part of patomic-test-${kind} component
        install(
            TARGETS ${target}
            RUNTIME #
            COMPONENT ${parent_target}
            DESTINATION "${CMAKE_INSTALL_TESTDIR}/patomic/${kind}/"
            EXCLUDE_FROM_ALL
        )

        # install as part of patomic-test component
        install(
            TARGETS ${target}
            RUNTIME #
            COMPONENT ${base_target}
            DESTINATION "${CMAKE_INSTALL_TESTDIR}/patomic/${kind}/"
            EXCLUDE_FROM_ALL
        )

    endif()

endfunction()


# Creates target patomic-test-bt-${name} corresponding to BT test executable.
#
# create_bt(
#     NAME <name>
#     [INCLUDE <item>...]
#     [SOURCE <item>...]
#     [LINK <item>...]
# )
function(create_bt)

    cmake_parse_arguments(
        "ARG"
        ""
        "NAME"
        "INCLUDE;SOURCE;LINK"
        ${ARGN}
    )

    _create_test(
        ${ARG_UNPARSED_ARGUMENTS}
        BT      ${ARG_NAME}
        INCLUDE ${ARG_INCLUDE}
        SOURCE  ${ARG_SOURCE}
        LINK
            patomic::patomic
            ${ARG_LINK}
    )

endfunction()


# Creates target patomic-test-ut-${name} corresponding to UT test executable.
#
# create_ut(
#     NAME <name>
#     [INCLUDE <item>...]
#     [SOURCE <item>...]
# )
function(create_ut)

    cmake_parse_arguments(
        "ARG"
        ""
        "NAME"
        "INCLUDE;SOURCE"
        ${ARGN}
    )

    _create_test(
        ${ARG_UNPARSED_ARGUMENTS}
        UT      ${ARG_NAME}
        SOURCE  ${ARG_SOURCE}
        INCLUDE
            "$<BUILD_INTERFACE:${PATOMIC_BINARY_DIR}/include>"
            "$<BUILD_INTERFACE:${PATOMIC_SOURCE_DIR}/include>"
            "$<BUILD_INTERFACE:${PATOMIC_SOURCE_DIR}/src/include>"
            ${ARG_INCLUDE}
    )

endfunction()


# ---- Create Test Dependency File ----

# Creates a file containing the output of windows_deps_paths for all tests of
# the given kind registered so far.
# The file path will be "${CMAKE_CURRENT_BINARY_DIR}/windows_dependencies_path.txt".
# Expects a target named patomic-test-${kind} to exist
# E.g. if you call it as create_test_win_deps_paths_file(BT) then patomic-bt must
# exist.
# This function has no effect when not running on Windows.
#
# create_test_win_deps_paths_file(
#     BT|UT
# )
function(create_test_win_deps_paths_file ARG_KIND)

    # check we actually want to generate file

    if(NOT PATOMIC_WINDOWS_CREATE_PATH_ENV_FILE)
        return()
    endif()


    # check KIND is valid

    set(all_kinds_opt_msg "BT|UT")
    set(func_name "create_test_win_deps_paths_file")

    if(NOT ARG_KIND MATCHES "^(${all_kinds_option})$")
        message(WARNING "${all_kinds_option} option needs to be specified when invoking '${func_name}'")
        message(FATAL_ERROR "Aborting '${func_name}' due to invalid arguments")
    endif()

    string(TOLOWER ${ARG_KIND} kind)


    # create and install file with dependencies path

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")

        # get dependencies set by create_test from target
        get_target_property(dep_targets patomic-test-${kind} WIN_DEP_TARGETS)
        if("${dep_targets}" STREQUAL "dep_targets-NOTFOUND")
            message(VERBOSE "Skipping creation of Windows dependencies PATH file for ${ARG_KIND}; no relevant test targets created")
            return()
        endif()
        list(REMOVE_DUPLICATES dep_targets)

        # get paths to all shared library dependencies (DLLs)
        windows_deps_paths(
            deps_paths
            ${dep_targets}
        )

        # create file
        set(file_path "${CMAKE_CURRENT_BINARY_DIR}/windows_dependencies_path.txt")
        file(GENERATE
            OUTPUT ${file_path}
            CONTENT "${deps_paths}"
        )

        # copy file to install location
        if(NOT CMAKE_SKIP_INSTALL_RULES)

            # install as part of patomic-test-${kind} component
            install(
                FILES ${file_path}
                COMPONENT patomic-test-${kind}
                DESTINATION "${CMAKE_INSTALL_TESTDIR}/patomic/${kind}"
                EXCLUDE_FROM_ALL
            )

            # install as part of patomic-test component
            install(
                FILES ${file_path}
                COMPONENT patomic-test
                DESTINATION "${CMAKE_INSTALL_TESTDIR}/patomic/${kind}"
                EXCLUDE_FROM_ALL
            )

        endif()

    endif()

endfunction()
