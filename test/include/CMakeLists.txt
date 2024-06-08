# create interface target that is automatically linked to all tests
add_library(${test_target_name}-include INTERFACE)

# add include directory paths
target_include_directories(${test_target_name}-include INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${GTEST_INCLUDE_DIRS}>"
)

# require C++14 as minimum
target_compile_features(${test_target_name}-include INTERFACE
    cxx_std_14
)

# add all subdirectories
add_subdirectory(test)

# mark all header files as C++ in case of IDE confusion thinking they're C
# this does not affect compilation
get_target_property(sources ${test_target_name}-include INTERFACE_SOURCES)
set_source_files_properties("${sources}" PROPERTIES
    LANGUAGE CXX
)
unset(sources)
