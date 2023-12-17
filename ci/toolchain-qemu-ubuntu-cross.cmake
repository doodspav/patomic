# ---- Options Summary ----

# The follow options MUST ALL be set, either as cache or environment variables.

# -------------------------------------------------------------------------------------------------------------
# | Option                       | Docs                                                                       |
# |==============================|============================================================================|
# | PATOMIC_CI_SYSROOT           | sysroot directory path, passed on to CMake                                 |
# | PATOMIC_CI_XARCH             | target architecture, used to select qemu architecture, e.g. aarch64        |
# | PATOMIC_CI_XCOMPILER         | host cross-compiler, supported values are 'gcc' and 'clang'                |
# | PATOMIC_CI_XCOMPILER_VERSION | host cross-compiler version, major version of the compiler being used      |
# | PATOMIC_CI_XTRIPLE           | target triple, used to set compiler and its target, e.g. aarch64-linux-gnu |
# -------------------------------------------------------------------------------------------------------------


# ---- Get Cache/Environment Variables ----

# target triple, used to set compiler (and its target), e.g. aarch64-linux-gnu
set(
    PATOMIC_CI_XTRIPLE "$ENV{PATOMIC_CI_XTRIPLE}" CACHE STRING
    "target triple, used to set compiler and its target, e.g. aarch64-linux-gnu"
)
if(NOT PATOMIC_CI_XTRIPLE)
    message(FATAL_ERROR "PATOMIC_CI_XTRIPLE cache/environment variable is not set")
elseif(NOT PATOMIC_CI_XTRIPLE MATCHES "^([a-zA-Z0-9]+-[a-zA-Z0-9]+-[a-zA-Z0-9]+(-[a-zA-Z0-9]+)?)$")
    message(FATAL_ERROR "PATOMIC_CI_XTRIPLE value '${PATOMIC_CI_XTRIPLE}' does not match expected pattern '<arch>-<vendor?>-<os>-<abi>'")
endif()

# target architecture, used to select qemu architecture, e.g. aarch64
set(
    PATOMIC_CI_XARCH "$ENV{PATOMIC_CI_XARCH}" CACHE STRING
    "target architecture, used to select qemu architecture, e.g. aarch64"
)
if(NOT PATOMIC_CI_XARCH)
    message(FATAL_ERROR "PATOMIC_CI_XARCH cache/environment variable is not set")
endif()

# host cross-compiler, supported values are 'gcc' and 'clang'
set(
    PATOMIC_CI_XCOMPILER "$ENV{PATOMIC_CI_XCOMPILER}" CACHE STRING
    "host cross-compiler, supported values are 'gcc' and 'clang'"
)
if(NOT PATOMIC_CI_XCOMPILER)
    message(FATAL_ERROR "PATOMIC_CI_XCOMPILER cache/environment variable is not set")
elseif(NOT PATOMIC_CI_XCOMPILER MATCHES "^(clang|gcc)$")
    message(FATAL_ERROR "PATOMIC_CI_XCOMPILER value '${PATOMIC_CI_XCOMPILER}' is not 'clang' or 'gcc'")
endif()

# host cross-compiler version, major version of the compiler being used
set(
    PATOMIC_CI_XCOMPILER_VERSION "$ENV{PATOMIC_CI_XCOMPILER_VERSION}" CACHE STRING
    "host cross-compiler version, major version of the compiler being used"
)
if(NOT PATOMIC_CI_XCOMPILER_VERSION)
    message(FATAL_ERROR "PATOMIC_CI_XCOMPILER_VERSION cache/environment variable is not set")
elseif(NOT PATOMIC_CI_XCOMPILER_VERSION MATCHES "^(\\d+)$")
    message(FATAL_ERROR "PATOMIC_CI_XCOMPILER_VERSION '${PATOMIC_CI_XCOMPILER_VERSION}' does not match regex '\\d+'")
endif()

# sysroot directory path, passed on to CMake
set(
    PATOMIC_CI_SYSROOT "$ENV{PATOMIC_CI_SYSROOT}" CACHE FILEPATH
    "sysroot directory path, passed on to CMake"
)
if(NOT PATOMIC_CI_SYSROOT)
    message(FATAL_ERROR "PATOMIC_CI_SYSROOT cache/environment variable is not set")
endif()


# ---- Set CMake Variables ----

# set basic target information
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR "${PATOMIC_CI_XARCH}")

# use sysroot at compile time and pass to QEMU when executing target binaries
set(qemu_command "qemu-${PATOMIC_CI_XARCH}")
if(qemu_command STREQUAL "qemu-armhf")
    set(qemu_command "qemu-arm")
elseif(qemu_command STREQUAL "qemu-x86_32")
    set(qemu_command "qemu-i386")
endif()
set(CMAKE_SYSROOT "${PATOMIC_CI_SYSROOT}")
set(CMAKE_CROSSCOMPILING_EMULATOR "${qemu_command};-L;${PATOMIC_CI_SYSROOT}")

# set the appropriate cross compilers and archiver
if(PATOMIC_CI_XCOMPILER STREQUAL "clang")
    set(CMAKE_C_COMPILER "clang-${PATOMIC_CI_XCOMPILER_VERSION}")
    set(CMAKE_CXX_COMPILER "clang++-${PATOMIC_CI_XCOMPILER_VERSION}")
    set(CMAKE_AR "llvm-ar-${PATOMIC_CI_XCOMPILER_VERSION}")
elseif(PATOMIC_CI_XCOMPILER STREQUAL "gcc")
    set(CMAKE_C_COMPILER "${PATOMIC_CI_XTRIPLE}-gcc-${PATOMIC_CI_XCOMPILER_VERSION}")
    set(CMAKE_CXX_COMPILER "${PATOMIC_CI_XTRIPLE}-g++-${PATOMIC_CI_XCOMPILER_VERSION}")
    set(CMAKE_AR "${PATOMIC_CI_XTRIPLE}-ar")
endif()

# set the target triple matching the cross compilers above
set(CMAKE_C_COMPILER_TARGET "${PATOMIC_CI_XTRIPLE}")
set(CMAKE_CXX_COMPILER_TARGET "${PATOMIC_CI_XTRIPLE}")

# search for programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search for libraries, headers, and packages in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
