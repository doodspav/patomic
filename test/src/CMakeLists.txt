# these sources are automatically linked to all tests

# set variable so that we can add the sources to all our test targets later
set(
    patomic_test_src_sources
    # src
    "gtest_sanitizer_support.cpp"
)

# transform to absolute path otherwise wrong current directory will be used
list(
    TRANSFORM patomic_test_src_sources
    PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/"
)
