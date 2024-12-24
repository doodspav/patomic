# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

# in-source build guard
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(
        FATAL_ERROR
        "In-source builds are not supported. "
        "You may need to delete 'CMakeCache.txt' and 'CMakeFiles/' before rebuilding this project."
    )
endif()
