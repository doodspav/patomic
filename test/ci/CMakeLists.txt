include(../cmake/CreateTest.cmake)


# ---- Setup Tests ----

add_custom_target(patomic-test-ci)
add_dependencies(patomic-test patomic-test-ci)

create_ci(NAME check-asan SOURCE check_asan.cpp)
create_ci(NAME check-ubsan SOURCE check_ubsan.cpp)
