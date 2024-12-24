# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

# add directory files to target
target_sources(${target_name} PRIVATE
    abort.h
    assert.h
    math.h
    sort.h
    stdalign.h
    stdint.h
)
