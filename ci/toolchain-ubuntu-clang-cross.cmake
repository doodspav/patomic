# get environment variables set by CI
set(architecture "$ENV{PATOMIC_CI_XARCH}")
set(triple "$ENV{PATOMIC_CI_XTRIPLE}")
set(version "$ENV{PATOMIC_CI_XCOMPILER_VERSION}")

# check environment variables are set and have reasonable values
if(NOT architecture)
    message(FATAL_ERROR "PATOMIC_CI_XARCH environment variable is not set")
elseif(NOT triple)
    message(FATAL_ERROR "PATOMIC_CI_XTRIPLE environment variable is not set")
elseif(NOT triple MATCHES "^${architecture}")
    message(FATAL_ERROR "PATOMIC_CI_XTRIPLE '${triple}' does not start with PATOMIC_CI_XARCH '${architecture}'")
elseif(NOT version)
    message(FATAL_ERROR "PATOMIC_CI_XCOMPILER_VERSION environment variable is not set")
endif()

# set basic target information
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR "${architecture}")

# use sysroot at compile time and pass to QEMU when executing target binaries
set(CMAKE_SYSROOT "$ENV{HOME}/sysroot")
set(CMAKE_CROSSCOMPILING_EMULATOR "qemu-${architecture};-L;${CMAKE_SYSROOT}")

# set the appropriate cross compilers and their target architecture triples
set(CMAKE_C_COMPILER "clang-${version}")
set(CMAKE_C_COMPILER_TARGET "${triple}")
set(CMAKE_CXX_COMPILER "clang++-${version}")
set(CMAKE_CXX_COMPILER_TARGET "${triple}")

# use an archiver that works with the compilers above
set(CMAKE_AR "llvm-ar-${version}")

# search for programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search for libraries, headers, and packages in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
