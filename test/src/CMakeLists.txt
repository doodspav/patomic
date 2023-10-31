# create object target that is automatically linked to all tests
# add_library(
#     patomic-test-src INTERFACE
#     # .
#     "gtest_sanitizer.cpp"
# )
#
# # add include directories
# target_include_directories(
#     patomic-test-src INTERFACE
#     "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
#     "$<BUILD_INTERFACE:${GTEST_INCLUDE_DIRS}>"
# )
#
# # don't need to link against GTest or patomic:
# # - the test libraries already link against GTest which should be enough
# # - we shouldn't need anything from patomic here (or UTs would break)
#
# # require C++14 as minimum
# target_compile_features(
#     patomic-test-src INTERFACE
#     cxx_std_14
# )
