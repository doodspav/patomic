# variable that might come from outside
set(processor aarch64)
set(version 11)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ${processor})

set(CMAKE_SYSROOT "$ENV{HOME}/sysroot")
set(CMAKE_C_COMPILER ${processor}-linux-gnu-gcc-${version})
set(CMAKE_CXX_COMPILER ${processor}-linux-gnu-g++-${version})
set(CMAKE_AR ${processor}-linux-gnu-ar)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
