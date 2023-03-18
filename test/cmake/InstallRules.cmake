# we don't want the default install location to be in bin
# note: not currently an official CMake variable
set(
    CMAKE_INSTALL_TESTDIR "share/test"
    CACHE PATH "(unofficial) Default test install location"
)

# copy target build output artifacts to OS dependent locations
install(
    TARGETS patomic_test
    RUNTIME #
    COMPONENT patomic_test_Runtime
    DESTINATION "${CMAKE_INSTALL_TESTDIR}"
    EXCLUDE_FROM_ALL
)

# copy file with windows dependencies path
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    install(
        FILES "${PROJECT_BINARY_DIR}/windows_dependencies_path.txt"
        COMPONENT patomic_test_Runtime
        DESTINATION "${CMAKE_INSTALL_TESTDIR}"
        EXCLUDE_FROM_ALL
    )
endif()

# support packaging library
if(PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif()
