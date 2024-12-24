# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

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
