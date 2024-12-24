# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

# add all subdirectories
add_subdirectory(internal)
add_subdirectory(macros)
add_subdirectory(stdlib)
add_subdirectory(wrapped)

# add directory files to target
target_sources(${target_name} PRIVATE
    config.h
)
