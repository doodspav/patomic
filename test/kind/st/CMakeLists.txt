include(../../cmake/CreateTest.cmake)


# ---- Setup Tests ----

add_custom_target(patomic-test-st)
add_dependencies(patomic-test patomic-test-st)
