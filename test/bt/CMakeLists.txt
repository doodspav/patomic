include(../cmake/CreateTest.cmake)
include(../cmake/WindowsDependenciesPath.cmake)


# ---- Setup Tests ----

add_custom_target(patomic-test-bt)
add_dependencies(patomic-test patomic-test-bt)

create_bt(NAME example_add SOURCE example_add.cpp)
create_bt(NAME example_sub SOURCE example_sub.cpp)


# ---- Windows Path Issues ----

create_test_win_deps_paths_file(BT)
