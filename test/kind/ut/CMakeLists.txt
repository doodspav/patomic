# ---- Setup Tests ----

add_custom_target(${test_target_name}-ut)
add_dependencies(${test_target_name} ${test_target_name}-ut)

create_ut(NAME example_add SOURCE example_add.cpp "${PATOMIC_SOURCE_DIR}/src/patomic.c")
create_ut(NAME example_sub SOURCE example_sub.cpp "${PATOMIC_SOURCE_DIR}/src/patomic.c")
