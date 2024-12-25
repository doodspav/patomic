# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

# add sources to test target
target_sources(dummy_target_bt_logic PRIVATE
    double_cmpxchg.cpp
    multi_cmpxchg.cpp
    generic.cpp
    generic_wfb.cpp
)
