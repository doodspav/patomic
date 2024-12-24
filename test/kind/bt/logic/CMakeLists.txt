# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

# ---- Gather Sources ----

# create dummy target so that sources have absolute paths
add_custom_target(dummy_target_bt_logic)

# add all subdirectories
add_subdirectory(ldst)
add_subdirectory(xchg)
add_subdirectory(bitwise)
add_subdirectory(binary)
add_subdirectory(arithmetic)
add_subdirectory(special)
add_subdirectory(flag)
add_subdirectory(raw)

# extract sources from dummy
get_target_property(
    bt_logic_sources
    dummy_target_bt_logic SOURCES
)

# ---- Create Tests ----

create_bt(
    NAME BtLogic
    SOURCE
        logic.cpp
        "${bt_logic_sources}"
)
