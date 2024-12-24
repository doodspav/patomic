# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

# add sources to test target
target_sources(dummy_target_bt_logic PRIVATE
    add.cpp
    sub.cpp
    inc.cpp
    dec.cpp
    neg.cpp
)
