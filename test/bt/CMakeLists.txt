include(../cmake/CreateTest.cmake)


# ---- Setup Tests ----

add_custom_target(patomic-test-bt)
add_dependencies(patomic-test patomic-test-bt)

create_bt(NAME example_add SOURCE example_add.cpp)
create_bt(NAME example_sub SOURCE example_sub.cpp)
