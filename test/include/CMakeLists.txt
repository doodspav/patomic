# fine with CML file here because include isn't installed for tests

# create interface target that is automatically linked to all tests
# this is solely so that IDEs understand that these are source files
add_library(
    patomic-test-include INTERFACE
    # .
)

# require C++14 as minimum
target_compile_features(
    patomic-test-include INTERFACE
    cxx_std_14
)
