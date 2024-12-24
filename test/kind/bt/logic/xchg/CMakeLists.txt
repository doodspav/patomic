# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

# add sources to test target
target_sources(dummy_target_bt_logic PRIVATE
    exchange.cpp
    cmpxchg_weak.cpp
    cmpxchg_strong.cpp
)
