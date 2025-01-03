# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

include(CMakePackageConfigHelpers)

# copy header files to CMAKE_INSTALL_INCLUDEDIR
install(
    DIRECTORY
    "${PROJECT_SOURCE_DIR}/include/"  # our header files
    "${PROJECT_BINARY_DIR}/include/"  # generated header files
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT ${package_name}-development
    FILES_MATCHING PATTERN "*.h"
    PATTERN "CMake*" EXCLUDE
)

# copy target build output artifacts to OS dependent locations
# (except includes, that just sets a compiler flag with the path)
install(
    TARGETS ${target_name}
    EXPORT ${package_name}-targets
    RUNTIME #
    COMPONENT ${package_name}-runtime
    LIBRARY #
    COMPONENT ${package_name}-runtime
    NAMELINK_COMPONENT ${package_name}-development
    ARCHIVE #
    COMPONENT ${package_name}-development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# create config file that points to targets file
configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/in/patomic-config.cmake.in"
    "${PROJECT_BINARY_DIR}/cmake/${package_name}-config.cmake"
    @ONLY
)

# copy config file for find_package to find
install(
    FILES "${PROJECT_BINARY_DIR}/cmake/${package_name}-config.cmake"
    DESTINATION "${CMAKE_INSTALL_CMAKEDIR}"
    COMPONENT ${package_name}-development
)

# create version file for consumer to check version in CMake
write_basic_package_version_file(
    "${package_name}-config-version.cmake"
    COMPATIBILITY SameMajorVersion  # a.k.a. SemVer
)

# copy version file for find_package to find for version check
install(
    FILES "${PROJECT_BINARY_DIR}/${package_name}-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_CMAKEDIR}"
    COMPONENT ${package_name}-development
)

# create targets file included by config file with targets for consumers
install(
    EXPORT ${package_name}-targets
    NAMESPACE patomic::
    DESTINATION "${CMAKE_INSTALL_CMAKEDIR}"
    COMPONENT ${package_name}-development
)

# support packaging library
if(PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif()
