# ---- Create Test ----

# Creates a test executable and registers it with CTest.
#
# create_test(
#     BT|UT <name>
#     [INCLUDE <item>...]
#     [SOURCE <item>...]
# )
function(create_test)
    cmake_parse_arguments(
        "ARG"
        ""
        "BT;UT"
        "INCLUDE;SOURCE"
        ${ARGN}
    )


    # validate arguments

    set(args_valid TRUE)
    set(func_name "create_test")

    if(NOT ARG_BT AND NOT ARG_UT)
        message(WARNING "<BT|UT> option needs to be specified when invoking '${func_name}'")
        set(args_valid FALSE)
    elseif(ARG_BT AND ARG_UT)
        message(WARNING "Only one of <BT|UT> options may be specified when invoking '${func_name}'")
        set(args_valid FALSE)
    endif()

    if(DEFINED ARG_UNPARSED_ARGUMENTS)
        message(WARNING "The following arguments were not recognised when invoking '${func_name}': ${ARG_UNPARSED_ARGUMENTS}")
        set(args_valid FALSE)
    endif()

    if(NOT ${args_valid})
        message(FATAL_ERROR "Aborting '${func_name}' due to invalid arguments")
    endif()


    # coalesce arguments

    set(name "${ARG_BT}")
    set(kind "bt")
    if(ARG_UT)
        set(name "${ARG_UT}")
        set(kind "ut")
    endif()


    # setup target

    set(target patomic_${kind}_${name})

    add_executable(
        ${target}
        ${ARG_INCLUDE}
        ${ARG_SOURCE}
    )

    target_link_libraries(
        ${target}
        PRIVATE
        patomic::patomic
        GTest::gtest_main
    )

    target_compile_features(${target} PRIVATE cxx_std_14)


    # setup tests with CTest

    # must be run in same directory scope as target
    gtest_add_tests(
        TARGET ${target}
        TEST_LIST added_tests
    )

    # get these values from root target so we don't recompute every time
    get_target_property(deps_path patomic_test WIN_DEPS_PATH)
    get_target_property(win_and_shared patomic_test WIN_AND_SHARED)

    # set environment variable for each test so that CTest works automatically
    if(win_and_shared AND patomic_test_SET_CTEST_PATH_ENV_WINDOWS)
        foreach(test IN LISTS added_tests)
            set_property(
                TEST "${test}"
                PROPERTY ENVIRONMENT "PATH=${deps_path}"
            )
        endforeach()
    endif()


    # setup install of target

    set(component patomic_${kind})

    if(NOT CMAKE_SKIP_INSTALL_RULES)
        install(
            TARGETS ${target}
            RUNTIME #
            COMPONENT ${component}
            DESTINATION "${CMAKE_INSTALL_TESTDIR}/patomic/${kind}"
            EXCLUDE_FROM_ALL
        )
    endif()


    # attach to root target

    add_dependencies(patomic_test ${target})

endfunction()
