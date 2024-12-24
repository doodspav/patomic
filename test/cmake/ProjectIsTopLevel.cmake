# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

# this variable is set by project() in CMake 3.21+
string(
    COMPARE EQUAL
    "${CMAKE_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}"
    PROJECT_IS_TOP_LEVEL
)
