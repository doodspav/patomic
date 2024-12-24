# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

# ---- Setup Tests ----

add_custom_target(${test_target_name}-bt)
add_dependencies(${test_target_name} ${test_target_name}-bt)

# add all subdirectories
add_subdirectory(api)
add_subdirectory(death)
add_subdirectory(logic)
