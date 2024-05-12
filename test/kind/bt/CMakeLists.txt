include(../../cmake/CreateTest.cmake)


# ---- Setup Tests ----

add_custom_target(patomic-test-bt)
add_dependencies(patomic-test patomic-test-bt)

create_bt(NAME types_transaction SOURCE types_transaction.cpp)
