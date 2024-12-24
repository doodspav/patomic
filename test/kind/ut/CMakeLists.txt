# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

# ---- Setup Tests ----

add_custom_target(${test_target_name}-ut)
add_dependencies(${test_target_name} ${test_target_name}-ut)

# add all subdirectories
add_subdirectory(internal)
