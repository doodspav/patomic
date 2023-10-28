# create object target that is automatically linked to all tests
add_library(
    patomic-test-src OBJECT
    # .
    "gtest_sanitizer_support.cpp"
)

# add include directories
target_include_directories(
    patomic-test-src PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${GTEST_INCLUDE_DIRS}>"
)

# require C++14 as minimum
target_compile_features(
    patomic-test-src PRIVATE
    cxx_std_14
)
