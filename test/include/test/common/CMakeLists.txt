# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

# add directory files to target
target_sources(${test_target_name}-include INTERFACE
    align.hpp
    compare.hpp
    death.hpp
    generic_int.hpp
    make_ops.hpp
    math.hpp
    name.hpp
    params.hpp
    skip.hpp
    support.hpp
    utility.hpp
)
