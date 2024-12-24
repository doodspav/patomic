# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

# add all subdirectories
add_subdirectory(ops)

# add directory files to target
target_sources(${target_name} PRIVATE
    align.h
    combine.h
    core.h
    feature_check.h
    ids.h
    memory_order.h
    ops.h
    options.h
    transaction.h
)
