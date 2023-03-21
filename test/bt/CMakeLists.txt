# create_test requires windows_deps_path
include(../cmake/CreateTest.cmake)
include(../cmake/WindowsDependenciesPath.cmake)


# ---- Setup Tests ----

add_custom_target(patomic_bt)
add_dependencies(patomic_test patomic_bt)

create_test(BT example_add SOURCE example_add.cpp)
create_test(BT example_sub SOURCE example_sub.cpp)


# ---- Windows Path Issues ----

create_test_win_deps_path_file(BT)
