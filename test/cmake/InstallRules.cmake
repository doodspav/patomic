# copy file with windows dependencies path
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    install(
        FILES "${PROJECT_BINARY_DIR}/patomic/windows_dependencies_path.txt"
        COMPONENT patomic_test
        DESTINATION "${CMAKE_INSTALL_TESTDIR}"
        EXCLUDE_FROM_ALL
    )
endif()

# support packaging library
if(PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif()
