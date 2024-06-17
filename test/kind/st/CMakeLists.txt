# ---- Setup Tests ----

add_custom_target(${test_target_name}-st)
add_dependencies(${test_target_name} ${test_target_name}-st)


# ---- Create Tests ----

create_st(
    NAME StAsan
    SOURCE
        asan.cpp
)

create_st(
    NAME StUbsan
    SOURCE
        ubsan.cpp
)
