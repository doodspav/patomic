# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

# add directory files to target
target_sources(${test_target_name}-src PRIVATE
    align.cpp
    compare.cpp
    death.cpp
    generic_int.cpp
    make_ops.cpp
    name.cpp
    params.cpp
    support.cpp
    transaction.cpp
)
