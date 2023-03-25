if(PROJECT_IS_TOP_LEVEL)
    set(
        CMAKE_INSTALL_INCLUDEDIR "include/patomic-${PROJECT_VERSION}"
        CACHE PATH ""
    )
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package patomic)

# copy header files to CMAKE_INSTALL_INCLUDEDIR
install(
    DIRECTORY
    "${PROJECT_SOURCE_DIR}/include/"
    "${PROJECT_BINARY_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT patomic_Development
)

# copy target build output artifacts to OS dependent locations
install(
    TARGETS patomic_patomic
    EXPORT patomicTargets
    RUNTIME #
    COMPONENT patomic_Runtime
    LIBRARY #
    COMPONENT patomic_Runtime
    NAMELINK_COMPONENT patomic_Development
    ARCHIVE #
    COMPONENT patomic_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# allow package maintainers to freely override the path for the configs
set(
    PATOMIC_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(PATOMIC_INSTALL_CMAKEDIR)

# copy config file for find_package to find
install(
    FILES "${PROJECT_SOURCE_DIR}/cmake/util/InstallConfig.cmake"
    DESTINATION "${PATOMIC_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT patomic_Development
)

# create version file for consumer to check version in CMake
write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# copy version file for find_package to find
install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${PATOMIC_INSTALL_CMAKEDIR}"
    COMPONENT patomic_Development
)

# create core configuration file detailing targets for consumer
install(
    EXPORT patomicTargets
    NAMESPACE patomic::
    DESTINATION "${PATOMIC_INSTALL_CMAKEDIR}"
    COMPONENT patomic_Development
)

# support packaging library
if(PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif()
