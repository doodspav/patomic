include(../../cmake/CreateTest.cmake)


# ---- Setup Tests ----

add_custom_target(patomic-test-st)
add_dependencies(patomic-test patomic-test-st)

create_st(NAME check_asan SOURCE check_asan.cpp)
create_st(NAME check_ubsan SOURCE check_ubsan.cpp)
