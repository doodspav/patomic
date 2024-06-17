# ---- Setup Tests ----

add_custom_target(${test_target_name}-bt)
add_dependencies(${test_target_name} ${test_target_name}-bt)

add_subdirectory(api)
