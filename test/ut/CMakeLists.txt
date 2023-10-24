include(../cmake/CreateTest.cmake)


# ---- Setup Tests ----

add_custom_target(patomic-test-ut)
add_dependencies(patomic-test patomic-test-ut)

create_ut(NAME example_add SOURCE example_add.cpp "${PATOMIC_SOURCE_DIR}/src/patomic.c")
create_ut(NAME example_sub SOURCE example_sub.cpp "${PATOMIC_SOURCE_DIR}/src/patomic.c")
