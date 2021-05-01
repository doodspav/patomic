cmake_minimum_required(VERSION 3.14)

project(patomicTests LANGUAGES C)

include(../cmake/project-is-top-level.cmake)
include(../cmake/windows-set-path.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(patomic REQUIRED)
  enable_testing()
endif()

add_executable(patomic_test patomic_test.c)
target_link_libraries(patomic_test PRIVATE patomic::patomic)
target_compile_features(patomic_test PRIVATE c_std_90)

add_test(NAME patomic_test COMMAND patomic_test)
windows_set_path(patomic_test patomic::patomic)
