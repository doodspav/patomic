# fine with CML file here because include isn't installed for tests

# set variable so we can add the sources to all our test targets later
set(
    patomic_test_include_sources
    # test
)

# transform to absolute path otherwise wrong current directory will be used
list(
    TRANSFORM patomic_test_include_sources
    PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/"
)
