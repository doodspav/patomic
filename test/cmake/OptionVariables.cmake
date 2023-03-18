# ---- Windows Tests Path ----

# By default we set PATH for tests run with CTest on Windows in order to prevent
# linker errors.
# Due to limitations in CMake, we can only completely override the PATH, rather
# than prepend to it.
# This gives users the option to disable this behaviour.
option(
    patomic_test_SET_CTEST_PATH_ENV_WINDOWS
    "Set PATH environment variable for tests when run using CTest on Windows"
    ON
)
mark_as_advanced(patomic_test_SET_CTEST_PATH_ENV_WINDOWS)
