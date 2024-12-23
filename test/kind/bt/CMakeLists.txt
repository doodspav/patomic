# ---- Setup Tests ----

add_custom_target(${test_target_name}-bt)
add_dependencies(${test_target_name} ${test_target_name}-bt)

# add all subdirectories
add_subdirectory(api)
add_subdirectory(death)
add_subdirectory(logic)
