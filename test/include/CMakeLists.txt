# create interface target that is automatically linked to all tests
add_library(${test_target_name}-include INTERFACE)

# add include directory paths
target_include_directories(${test_target_name}-include INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${PATOMIC_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${PATOMIC_BINARY_DIR}/include>"
    "$<BUILD_INTERFACE:${GTEST_INCLUDE_DIRS}>"
)

# require C++14 as minimum
target_compile_features(${test_target_name}-include INTERFACE
    cxx_std_14
)

# add all subdirectories
add_subdirectory(test)
