# ---- Test Install Directory ----

# Normally tests would be install in CMAKE_INSTALL_BINDIR by default since
# they're executables.
# This is undesirable, so this variable exists to override the install location
# of test binaries separately.
# It's not prefixed with patomic_test because it's ok for it to be shared and
# overridden by parent projects.
# Note: this is not an official CMake variable
set(
    CMAKE_INSTALL_TESTDIR "share/test"
    CACHE PATH "(unofficial) Default test install location"
)


# ---- Test Build Selection ----

# This option provides a way to selectively disable/enable tests based on target
# name (not suite/case name, or name passed to create_test).
# Regex must be written according to CMake Regex Specification.
# By default all test targets are enabled.
set(
    patomic_test_CREATE_TEST_TARGETS_MATCHING "^(.*)$"
    CACHE STRING "Only test targets matching regex are created and registered with CTest"
)


# ---- Windows Tests Path ----

# By default we set PATH for tests run with CTest on Windows in order to prevent
# linker errors.
# Due to limitations in CMake, we can only completely override the PATH, rather
# than prepend to it.
# This gives users the option to disable this behaviour.
option(
    patomic_test_SET_CTEST_PATH_ENV_WINDOWS
    "Set PATH environment variable for tests when run CTest on Windows"
    ON
)
mark_as_advanced(patomic_test_SET_CTEST_PATH_ENV_WINDOWS)


# ---- Windows Path File ----

# By default on Windows we generate a file per test kind that contains a string
# that can be prepended to PATH before running tests, in order to ensure that
# runtime dependencies can be found.
# Most of the time we don't need this file (since CTest will take care of that
# for us), so we don't need to generate it.
# Additionally disabled by default because it contains potentially private
# information about the target platform.
option(
    patomic_test_CREATE_WINDOWS_PATH_FILE
    "Create file with PATH environment variables for tests on Windows"
    OFF
)
mark_as_advanced(patomic_test_CREATE_WINDOWS_PATH_FILE)
