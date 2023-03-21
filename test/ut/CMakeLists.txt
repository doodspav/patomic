# create_test requires windows_deps_path
include(../cmake/CreateTest.cmake)
include(../cmake/WindowsDependenciesPath.cmake)


# ---- Setup Tests ----

add_custom_target(patomic_ut)
add_dependencies(patomic_test patomic_ut)

# appended to in create_test
set_property(TARGET patomic_ut PROPERTY WIN_DEP_TARGETS "")

create_test(UT ut_example_add SOURCE example_add.cpp)
create_test(UT ut_example_sub SOURCE example_sub.cpp)


# ---- Windows Path Issues ----

# variable so this code can by copy-pasted to new test directories
set(kind "ut")

# create and copy file with dependencies
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
    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/windows_dependencies_path.txt"
        COMPONENT patomic_${kind}
        DESTINATION "${CMAKE_INSTALL_TESTDIR}/patomic/${kind}"
        EXCLUDE_FROM_ALL
    )
endif()
