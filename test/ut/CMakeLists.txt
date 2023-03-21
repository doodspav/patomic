# create_test requires windows_deps_path
include(../cmake/CreateTest.cmake)
include(../cmake/WindowsDependenciesPath.cmake)


# ---- Setup Tests ----

add_custom_target(patomic_ut)
add_dependencies(patomic_test patomic_ut)

create_test(UT ut_example_add SOURCE example_add.cpp)
create_test(UT ut_example_sub SOURCE example_sub.cpp)


# ---- Windows Path Issues ----

create_test_win_deps_path_file(UT)
