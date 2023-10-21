include(../cmake/CreateTest.cmake)
include(../cmake/WindowsDependenciesPath.cmake)


# ---- Setup Tests ----

add_custom_target(patomic-test-ut)
add_dependencies(patomic-test patomic-test-ut)

create_test(UT example_add SOURCE example_add.cpp)
create_test(UT example_sub SOURCE example_sub.cpp)


# ---- Windows Path Issues ----

create_test_win_deps_paths_file(UT)
