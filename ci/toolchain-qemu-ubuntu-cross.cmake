# get environment variables set by CI
set(triple "$ENV{PATOMIC_CI_XTRIPLE}")
set(architecture "$ENV{PATOMIC_CI_XARCH}")
set(compiler "$ENV{PATOMIC_CI_XCOMPILER}")
set(version "$ENV{PATOMIC_CI_XCOMPILER_VERSION}")

# check environment variables are set
if(NOT triple)
    message(FATAL_ERROR "PATOMIC_CI_XTRIPLE environment variable is not set")
elseif(NOT architecture)
    message(FATAL_ERROR "PATOMIC_CI_XARCH environment variable is not set")
elseif(NOT compiler)
    message(FATAL_ERROR "PATOMIC_CI_XCOMPILER environment variable is not set")
elseif(NOT version)
    message(FATAL_ERROR "PATOMIC_CI_XCOMPILER_VERSION environment variable is not set")
endif()

# check environment variables have reasonable values
# do this separately to checking if the value is empty for nicer error messages
if(NOT triple MATCHES "^([a-zA-Z0-9]+-[a-zA-Z0-9]+-[a-zA-Z0-9]+(-[a-zA-Z0-9]+)?)$")
    message(FATAL_ERROR "PATOMIC_CI_XTRIPLE value '${triple}' does not match expected pattern '<arch>-<vendor?>-<os>-<abi>'")
# fixme: elseif(NOT architecture MATCHES "^(${triple}.*)$")
#    message(FATAL_ERROR "PATOMIC_CI_XARCH value '${architecture}' does not match first component of triple '${triple}'")
elseif(NOT compiler MATCHES "^(clang|gcc)$")
    message(FATAL_ERROR "PATOMIC_CI_XCOMPILER '${compiler}' is not 'clang' or 'gcc'")
# fixme: elseif(NOT version MATCHES "^(\\d+)$")
#    message(FATAL_ERROR "PATOMIC_CI_XCOMPILER_VERSION '${version}' is not a number")
endif()

# set basic target information
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR "${architecture}")

# use sysroot at compile time and pass to QEMU when executing target binaries
set(qemu_command "qemu-${architecture}")
if(architecture MATCHES "^(i386|i486|i686|x86)$")
    set(qemu_command "qemu-x86_64")
endif()
set(CMAKE_SYSROOT "$ENV{HOME}/sysroot")
set(CMAKE_CROSSCOMPILING_EMULATOR "${qemu_command};-L;${CMAKE_SYSROOT}")

# set the appropriate cross compilers and archiver
# value of ${compiler} is checked at start when obtained from environment variable
if(compiler MATCHES "clang")
    set(CMAKE_C_COMPILER "clang-${version}")
    set(CMAKE_CXX_COMPILER "clang++-${version}")
    set(CMAKE_AR "llvm-ar-${version}")
elseif(compiler MATCHES "gcc")
    set(CMAKE_C_COMPILER "${triple}-gcc-${version}")
    set(CMAKE_CXX_COMPILER "${triple}-g++-${version}")
    set(CMAKE_AR "${triple}-ar")
endif()

# set the target triple matching the cross compilers above
set(CMAKE_C_COMPILER_TARGET "${triple}")
set(CMAKE_CXX_COMPILER_TARGET "${triple}")

# search for programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search for libraries, headers, and packages in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
