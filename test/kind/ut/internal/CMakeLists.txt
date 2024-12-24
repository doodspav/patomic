# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

# ---- Create Tests ----

create_ut(
    NAME UtInternalAlign
    SOURCE
        align.cpp
        "${PATOMIC_SOURCE_DIR}/src/api/align.c"
)
