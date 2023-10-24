# ---- Options Summary ----

# --------------------------------------------------------------------
# | Option                               | Availability | Default    |
# |======================================|==============|============|
# | CMAKE_INSTALL_TESTDIR (unofficial)   | Always       | share/test |
# |--------------------------------------|--------------|------------|
# | PATOMIC_CREATE_TEST_TARGETS_MATCHING | Always       | ^(.*)$     |
# --------------------------------------------------------------------


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
