# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

# add directory files to target
target_sources(${target_name} PRIVATE
    config.h
    macros.h
    msvc128.h
)
