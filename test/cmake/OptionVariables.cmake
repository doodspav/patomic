# ---- Options Summary ----

# --------------------------------------------------------------------
# | Option                               | Availability | Default    |
# |======================================|==============|============|
# | CMAKE_INSTALL_TESTDIR (unofficial)   | Always       | share/test |
# |--------------------------------------|--------------|------------|
# | PATOMIC_SOURCE_DIR                   | Always       | ""         |
# | PATOMIC_CREATE_TEST_TARGETS_MATCHING | Always       | ^(.*)$     |
# | PATOMIC_WINDOWS_SET_CTEST_PATH_ENV   | Always       | ON         |
# | PATOMIC_WINDOWS_CREATE_PATH_ENV_FILE | Always       | OFF        |
# --------------------------------------------------------------------

# Note:
# PATOMIC_SOURCE_DIR is set by patomic and available through there if
# patomic-test is built as a subproject.


# ---- Test Install Directory ----

# Normally tests would be install in CMAKE_INSTALL_BINDIR by default since
# they're executables.
# This is undesirable, so this variable exists to override the install location
# of test binaries separately.
# It's not prefixed with PATOMIC_ because it's ok for it to be shared and
# overridden by parent projects.
# Note: this is not an official CMake variable
# The variable type is STRING rather than PATH, because otherwise passing
# -DCMAKE_INSTALL_TESTDIR=share/test on the command line would expand to an
# absolute path with the base being the current CMake directory, leading to
# unexpected errors.
set(
    CMAKE_INSTALL_TESTDIR "share/test"
    CACHE STRING "(unofficial) Default test install location"
)


# ---- Library Source Directory

# Unit tests need access to the private header files not available from linking
# against the patomic::patomic target.
# This variable can be set to let unit tests know where to find these files.
# When building this as a subproject of patomic (as you would by just building
# patomic normally with tests enabled), this variable is set automatically.
# If this variable is empty or not set, no unit tests will be built.
# The variable type is STRING rather than PATH, because otherwise passing
# -PATOMIC_SOURCE_DIR=relative/patomic on the command line would expand to an
# absolute path with the base being the current CMake directory, leading to
# unexpected errors.
set(
    PATOMIC_SOURCE_DIR ""
    CACHE STRING "Path to source files of patomic::patomic target for unit tests"
)
mark_as_advanced(PATOMIC_SOURCE_DIR)


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


# ---- Windows Tests Path ----

# By default we set PATH for tests run with CTest on Windows in order to prevent
# linker errors.
# Due to limitations in CMake, we can only completely override the PATH, rather
# than prepend or append to it.
# This gives users the option to disable this behaviour.
# This option has no effect when not running on Windows.
option(
    PATOMIC_WINDOWS_SET_CTEST_PATH_ENV
    "Set PATH environment variable for tests when run CTest on Windows"
    ON
)
mark_as_advanced(PATOMIC_WINDOWS_SET_CTEST_PATH_ENV)


# ---- Windows Path File ----

# On Windows we need to set PATH for tests but may not want to have the PATH be
# completely overridden, like with PATOMIC_WINDOWS_SET_CTEST_PATH_ENV.
# Instead we can generated a file per test kind that contains a string that can
# be manually prepended to PATH before running tests, in order to ensure that
# runtime dependencies can be found.
# Most of the time we don't need this file (since CTest will take care of that
# for us), so we don't generate it by default.
# Additionally disabled by default because it contains potentially private
# information about the target platform.
# This option has no effect when not running on Windows.
option(
    PATOMIC_WINDOWS_CREATE_PATH_ENV_FILE
    "Create file with PATH environment variables for tests on Windows"
    OFF
)
mark_as_advanced(PATOMIC_WINDOWS_CREATE_PATH_ENV_FILE)
