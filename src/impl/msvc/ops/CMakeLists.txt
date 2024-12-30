# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

# add all subdirectories
add_subdirectory(arithmetic)
add_subdirectory(base)
add_subdirectory(binary)
add_subdirectory(bitwise)
add_subdirectory(xchg)

# add directory files to target
target_sources(${target_name} PRIVATE
    base.h
)
