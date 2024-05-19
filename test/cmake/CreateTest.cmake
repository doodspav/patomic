# ---- Create Test ----

# Creates a target to build a test executable and registers it with CTest.
# Expects a target named patomic-test-${kind} to exist.
# E.g. if you call it as create_test(BT ...) then patomic-test-bt must exist.
#
# Kinds:
# - BT (binary test), intended to test public API of compiled library
# - ST (system test), intended to test host system and frameworks/sanitizers used
# - UT (unit test), intended to test specific function in library source file
#
# Naming:
# - created target -> patomic-test-${kind}-${name} (e.g. patomic-test-bt-SomeExample)
# - executable name -> ${name} (e.g. SomeExample on Unix or SomeExample.exe on Windows)
# - install directory -> ${CMAKE_INSTALL_TESTDIR}/${kind} (e.g. share/test/bt)
#
# Labels:
# - each test (not target) will have ${kind} appended to its LABELS property
# - main use case is for code coverage to be able to only run unit tests
#
# Working Directory (CTest):
# - each test target has its working directory set to ${PROJECT_BINARY_DIR}/working/${kind}/${name}
# - if a multi-config generator is used, 'working' is replaced with 'working/$<CONFIG>'
# - reasoning:
#   - coverage files generated by executables may overwrite existing coverage files (e.g. with clang)
#   - this is an issue if multiple test executables exist in the same directory
#   - this solution solves that by running each executable in its own directory
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

    set(all_kinds "BT;ST;UT")  # list of all kinds we can iterate over
    set(all_kinds_opt_msg "BT|ST|UT")  # string to use in debug message

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
    if(ARG_UNPARSED_ARGUMENTS)
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
    add_executable(${target})

    # add sources to target
    target_sources(
        ${target} PRIVATE
        ${ARG_SOURCE}
    )

    # add include directories
    target_include_directories(
        ${target} PRIVATE
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
        ${ARG_INCLUDE}
    )

    # add /src and /include sources that apply to all test targets
    target_link_libraries(
        ${target} PRIVATE
        patomic-test-include
        patomic-test-src
    )

    # update dependencies list directly because we use it in Windows PATH stuff later
    if("${CMAKE_VERSION}" VERSION_GREATER_EQUAL "3.20.0")
        list(APPEND ARG_LINK GTest::gtest_main)
        # TODO: this prevents test lookup on Windows; fix once pipeline exists (GHI #30)
        # TODO: see https://github.com/google/googletest/issues/2157 (GHI #30)
        # if ("${CMAKE_VERSION}" VERSION_GREATER_EQUAL "3.23.0")
        #     list(APPEND ARG_LINK GTest::gmock)
        # endif()
    else()
        list(APPEND ARG_LINK GTest::Main GTest::GTest)
    endif()

    # link dependencies (all tests use GTest framework)
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
        "PATOMIC_TEST_KIND_${kind_upper}=1"
    )

    # set binary name instead of using default
    set_target_properties(
        ${target} PROPERTIES
        OUTPUT_NAME "${name}"
    )


    # register test with GTest/CTest and parent target

    # setup path differently depending on generator
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    set(test_working_dir "${PROJECT_BINARY_DIR}/working")
    if(is_multi_config)
        set(test_working_dir "${test_working_dir}/$<CONFIG>/${kind}/${name}")
    else()
        set(test_working_dir "${test_working_dir}/${kind}/${name}")
    endif()

    # must be run in same directory scope as target
    gtest_add_tests(
        TARGET ${target}
        TEST_LIST added_tests
        WORKING_DIRECTORY "${test_working_dir}"
    )

    # add label to tests so ctest can run them by kind
    foreach(test IN LISTS added_tests)
        set_property(
            TEST "${test}"
            APPEND PROPERTY LABELS "${kind}"
        )
    endforeach()

    # custom target to make sure the working directory exists for the test
    add_custom_target(
        ${target}-create-working-dir
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${test_working_dir}"
    )

    # set build dependencies
    add_dependencies(${target} ${target}-create-working-dir)
    add_dependencies(${parent_target} ${target})


    # deal with Windows runtime linker lookup issues

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND
        PATOMIC_WINDOWS_MODIFY_CTEST_PATH_ENV)

        # get paths to all shared library dependencies (DLLs)
        # this should just be patomic and gtest
        set(deps_paths )
        foreach(dep_target IN LISTS ARG_LINK)
            # This will fail if passed a link option that isn't a target
            # This is intentional; don't do that.
            # Instead, create an IMPORTED library, and set its target properties
            # such as IMPORTED_LOCATION for the library path and set
            # INTERFACE_INCLUDE_DIRECTORIES to the directories containing any
            # necessary header files.
            if(NOT TARGET "${dep_target}")
                message(FATAL_ERROR "Encountered non-target dependency '${dep_target}' for target ${target} (check source comment)")
            endif()
            get_target_property(type "${dep_target}" TYPE)
            if(type STREQUAL "SHARED_LIBRARY")
                list(APPEND deps_paths "$<TARGET_FILE_DIR:${dep_target}>")
            endif()
        endforeach()

        # tidy up the paths
        list(REMOVE_DUPLICATES deps_paths)
        string(REPLACE ";" "\;" deps_paths "${deps_paths}")

        # modify environment variable for each test so that CTest can find DLLs
        foreach(test IN LISTS added_tests)
            set_tests_properties(
                "${test}" PROPERTIES
                ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${deps_paths}"
            )
        endforeach()

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


# Creates target patomic-test-st-${name} corresponding to ST test executable.
#
# create_st(
#     NAME <name>
# )
function(create_st)

    cmake_parse_arguments(
        "ARG"
        ""
        "NAME"
        "SOURCE"
        ${ARGN}
    )

    _create_test(
        ${ARG_UNPARSED_ARGUMENTS}
        ST     ${ARG_NAME}
        SOURCE ${ARG_SOURCE}
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

    # visibility macros will break UTs on Windows
    set(target_name patomic-test-ut-${ARG_NAME})
    target_compile_definitions(${target_name} PRIVATE PATOMIC_STATIC_DEFINE)

endfunction()
