# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

# create object target that is automatically linked to all tests
# excluded from compilation unless a test is built
add_library(${test_target_name}-src OBJECT EXCLUDE_FROM_ALL)

# add dependencies
# don't need to link against GTest or patomic:
# - the test libraries already link against GTest which should be enough
# - we shouldn't need anything from patomic here (or UTs would break)
target_link_libraries(${test_target_name}-src PRIVATE
    ${test_target_name}-include
)

# require C++14 as minimum
target_compile_features(${test_target_name}-src PRIVATE
    cxx_std_14
)

# add all subdirectories
add_subdirectory(common)
add_subdirectory(sanitizer)
