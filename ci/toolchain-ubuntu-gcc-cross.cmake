set(architecture "$ENV{PATOMIC_CI_XARCH}")
set(version "$ENV{PATOMIC_CI_XCOMPILER_VERSION}")

if(NOT architecture)
    message(FATAL_ERROR "PATOMIC_CI_XARCH environment variable is not set")
elseif(NOT version)
    message(FATAL_ERROR "PATOMIC_CI_XCOMPILER_VERSION environment variable is not set")
endif()

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_architecture ${architecture})

set(CMAKE_SYSROOT "$ENV{HOME}/sysroot")
set(CMAKE_CROSSCOMPILING_EMULATOR "qemu-${architecture};-L;${CMAKE_SYSROOT}")

set(CMAKE_C_COMPILER ${architecture}-linux-gnu-gcc-${version})
set(CMAKE_CXX_COMPILER ${architecture}-linux-gnu-g++-${version})
set(CMAKE_AR ${architecture}-linux-gnu-ar)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
