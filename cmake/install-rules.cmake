if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_INSTALL_INCLUDEDIR include/patomic CACHE PATH "")
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(
    DIRECTORY
    "${PROJECT_SOURCE_DIR}/include/patomic/"
    "${PROJECT_BINARY_DIR}/include/patomic/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT patomic_Development
)

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

write_basic_package_version_file(
    patomicConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

set(
    patomic_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/patomic"
    CACHE STRING "CMake package config location relative to the install prefix"
)

mark_as_advanced(patomic_INSTALL_CMAKEDIR)

install(
    FILES
    "${PROJECT_SOURCE_DIR}/cmake/patomicConfig.cmake"
    "${PROJECT_BINARY_DIR}/patomicConfigVersion.cmake"
    DESTINATION "${patomic_INSTALL_CMAKEDIR}"
    COMPONENT patomic_Development
)

install(
    EXPORT patomicTargets
    NAMESPACE patomic::
    DESTINATION "${patomic_INSTALL_CMAKEDIR}"
    COMPONENT patomic_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
