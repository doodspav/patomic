# fine with CML file here because include isn't installed for tests

# create interface target that is automatically linked to all tests
add_library(
    patomic-test-include INTERFACE
    # .
    "example.hpp"
)
