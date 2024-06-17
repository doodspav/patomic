# ---- Setup Tests ----

add_custom_target(${test_target_name}-ut)
add_dependencies(${test_target_name} ${test_target_name}-ut)
