# get environment variables set by CI
set(triple "$ENV{PATOMIC_CI_XTRIPLE}")
set(compiler "$ENV{PATOMIC_CI_XCOMPILER}")
set(version "$ENV{PATOMIC_CI_XCOMPILER_VERSION}")

# check environment variables are set and have reasonable values
# if(NOT triple)
#     message(FATAL_ERROR "PATOMIC_CI_XTRIPLE environment variable is not set")
# elseif(NOT triple MATCHES "^([a-zA-Z0-9]+-[a-zA-Z0-9]+-[a-zA-Z0-9]+(-[a-zA-Z0-9]+)?)$")
#     message(FATAL_ERROR "PATOMIC_CI_XTRIPLE '${triple}' does not match expected regex pattern")
# elseif(NOT compiler)
#     message(FATAL_ERROR "PATOMIC_CI_XCOMPILER environment variable is not set")
# elseif(NOT compiler MATCHES "^(clang|gcc)$")
#     message(FATAL_ERROR "PATOMIC_CI_XCOMPILER environment variable '${compiler}' is not 'clang' or 'gcc'")
# elseif(NOT version)
#     message(FATAL_ERROR "PATOMIC_CI_XCOMPILER_VERSION environment variable is not set")
# elseif(NOT version MATCHES "^(\\d+)$")
#     message(FATAL_ERROR "PATOMIC_CI_XCOMPILER_VERSION environment variable '${version}' is not a number")
# endif()

# get target architecture from triple
# string(REGEX_MATCH "[a-zA-Z0-9]+" architecture "${triple}")
set(architecture "aarch64")

# set basic target information
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR "${architecture}")

# use sysroot at compile time and pass to QEMU when executing target binaries
set(CMAKE_SYSROOT "$ENV{HOME}/sysroot")
set(CMAKE_CROSSCOMPILING_EMULATOR "qemu-${architecture};-L;${CMAKE_SYSROOT}")

# set the appropriate cross compilers and their target architecture triples
# value of ${compiler} is checked at start when obtained from environment variable
if(compiler MATCHES "clang")
    set(CMAKE_C_COMPILER "clang-${version}")
    set(CMAKE_CXX_COMPILER "clang++-${version}")
elseif(compiler MATCHES "gcc")
    set(CMAKE_C_COMPILER "${triple}-gcc-${version}")
    set(CMAKE_CXX_COMPILER "${triple}-g++-${version}")
endif()

# set the target triple matching the cross compilers above
set(CMAKE_C_COMPILER_TARGET "${triple}")
set(CMAKE_CXX_COMPILER_TARGET "${triple}")

# use an archiver that works with the compilers above
set(CMAKE_AR "llvm-ar-${version}")

# search for programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search for libraries, headers, and packages in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
