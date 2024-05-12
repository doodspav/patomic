include(../../cmake/CreateTest.cmake)


# ---- Setup Tests ----

add_custom_target(patomic-test-st)
add_dependencies(patomic-test patomic-test-st)

create_st(NAME StAsan SOURCE check_asan.cpp)
create_st(NAME StUbsan SOURCE check_ubsan.cpp)
