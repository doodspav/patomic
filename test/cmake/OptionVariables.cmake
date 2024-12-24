# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

include(GNUInstallDirs)

# ---- Options Summary ----

# ----------------------------------------------------------------------------------------------
# | Option                                | Availability   | Default                           |
# |=======================================|================|===================================|
# | CMAKE_INSTALL_TESTDIR (unofficial)    | Always         | ${CMAKE_INSTALL_DATAROOTDIR}/test |
# |---------------------------------------|----------------|-----------------------------------|
# | PATOMIC_CREATE_TEST_TARGETS_MATCHING  | Always         | ^(.*)$                            |
# | PATOMIC_WINDOWS_MODIFY_CTEST_PATH_ENV | Always (3.22+) | ON                                |
# ----------------------------------------------------------------------------------------------


# ---- Test Install Directory ----

# Normally tests would be install in CMAKE_INSTALL_BINDIR by default since
# they're executables.
# This is undesirable, so this variable exists to override the install location
# of test binaries separately.
# It's not project-specific because it's ok for it to be shared and
# overridden by parent projects.
# The variable type is STRING rather than PATH, because otherwise passing
# -DCMAKE_INSTALL_TESTDIR=share/test on the command line would expand to an
# absolute path with the base being the current CMake directory, leading to
# unexpected errors.
#
# Note: this is not an official CMake variable
set(
    CMAKE_INSTALL_TESTDIR "${CMAKE_INSTALL_DATAROOTDIR}/test"
    CACHE STRING "(unofficial) CMake top level test location relative to the install prefix"
)
# set it back to a PATH type for GUI assistance
set_property(CACHE CMAKE_INSTALL_TESTDIR PROPERTY TYPE PATH)
# depends on CMAKE_INSTALL_DATAROOTDIR which is marked as advanced in GNUInstallDirs
mark_as_advanced(CMAKE_INSTALL_TESTDIR)
# generate absolute path version
set(dir "DATAROOTDIR")  # use DATAROOTDIR since that forms the root of CMAKE_INSTALL_TESTDIR
set(dir_param )
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.20)
    set(dir_param "${dir}")
endif()
GNUInstallDirs_get_absolute_install_dir(
    CMAKE_INSTALL_FULL_TESTDIR
    CMAKE_INSTALL_TESTDIR
    ${dir_param}
)
unset(dir_param)
unset(dir)


# ---- Test Build Selection ----

# This option provides a way to selectively disable/enable building tests based
# on target name (not suite/case name, or name passed to create_test).
# Regex must be written according to CMake Regex Specification.
# By default all test targets are enabled.
set(
    PATOMIC_CREATE_TEST_TARGETS_MATCHING "^(.*)$"
    CACHE STRING "Only test targets matching regex are created and registered with CTest"
)
mark_as_advanced(PATOMIC_CREATE_TEST_TARGETS_MATCHING)


# ---- Windows Tests Paths ----

# By default we prepend the PATH environment variable for tests run with CTest
# on Windows with the directory paths of all its shared library dependencies.
# This is done to prevent linker errors (because Windows doesn't support rpath).
# This option has no effect when not running on Windows.
# This option requires CMake 3.22 or later.
if("${CMAKE_VERSION}" VERSION_GREATER_EQUAL "3.22.0")
    option(
        PATOMIC_WINDOWS_MODIFY_CTEST_PATH_ENV
        "Modify PATH environment variable for tests when run with CTest on Windows"
        ON
    )
    mark_as_advanced(PATOMIC_WINDOWS_MODIFY_CTEST_PATH_ENV)
elseif(PATOMIC_WINDOWS_MODIFY_CTEST_PATH_ENV)
    message(
        WARNING
        "Option 'PATOMIC_WINDOWS_MODIFY_CTEST_PATH_ENV' for 'patomic_test' requires CMake 3.22+, currently running ${CMAKE_VERSION}, option is disabled"
    )
    unset(CACHE PATOMIC_WINDOWS_MODIFY_CTEST_PATH_ENV)
    unset(PATOMIC_WINDOWS_MODIFY_CTEST_PATH_ENV)
endif()
