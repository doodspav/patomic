# ---- Option ----

set(PATOMIC_CI_PLATFORM "" CACHE STRING "Check toolchain file for more information")
set(platform ${PATOMIC_CI_PLATFORM})

set(platform_regex "^(warning|coverage|sanitizer)-(windows|macos|linux)-(clang|gcc|msvc)-(ansi-)?(x86_64|x86|x64)$")
if(NOT platform MATCHES "${platform_regex}")
    message(FATAL_ERROR "Unsupported platform: ${platform} (does not match ${platform_regex})")
endif()


# ---- Platform ----

# clang
if(platform MATCHES "clang")
    if(platform MATCHES "-x86$")
        string(APPEND CMAKE_C_FLAGS_INIT " -m32")
        string(APPEND CMAKE_CXX_FLAGS_INIT " -m32")
    endif()

# gcc
elseif(platform MATCHES "gcc")
    if(platform MATCHES "-x86$")
        string(APPEND CMAKE_C_FLAGS_INIT " -m32")
        string(APPEND CMAKE_CXX_FLAGS_INIT " -m32")
    endif()

# msvc
elseif(platform MATCHES "msvc")
    if(platform MATCHES "-x86$")
        set(CMAKE_GENERATOR_PLATFORM "Win32")
    elseif(platform MATCHES "-(x64|x86_64)$")
        set(CMAKE_GENERATOR_PLATFORM "x64")
    endif()

# no need to error on unknown compilers, done at start
endif()


# ---- Language Standard ----

# standard version
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
if(platform MATCHES "ansi")
    set(CMAKE_C_STANDARD 90)
endif()

# we need C++14 or GTest won't compile
# don't need C standard because it can only use older standards
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# disallow compiler extensions
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)


# ---- Mode ----

# warning
if(platform MATCHES "warning")

    # ansi
    if(platform MATCHES "ansi")
        if(NOT platform MATCHES "(clang|gcc)")
            message(FATAL_ERROR "Only clang/gcc are supported for ansi, unsupported platform: ${platform}")
        endif()
        string(APPEND CMAKE_C_FLAGS_INIT " -Wall -Wextra -Werror -Wpedantic -Wno-unused-function -Wno-atomic-alignment")
        string(APPEND CMAKE_CXX_FLAGS_INIT " -Wall -Wextra -Werror -Wpedantic")

    # clang
    elseif(platform MATCHES "clang")
        string(APPEND CMAKE_C_FLAGS_INIT " -Weverything -Werror -Wpedantic -Wno-c++98-compat -Wno-covered-switch-default -Wno-padded -Wno-unused-function -Wno-atomic-alignment -Wno-poison-system-directories")
        string(APPEND CMAKE_CXX_FLAGS_INIT " -Wall -Wextra -Werror -Wpedantic")

    # gcc
    elseif(platform MATCHES "gcc")
        string(APPEND CMAKE_C_FLAGS_INIT " -Wall -Wextra -Werror -Wpedantic -Wshadow -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wstrict-prototypes -Wmisleading-indentation -Wduplicated-branches -Wlogical-op -Wdeclaration-after-statement -Wno-unused-function")
        string(APPEND CMAKE_CXX_FLAGS_INIT " -Wall -Wextra -Werror -Wpedantic")

    # msvc
    elseif(platform MATCHES "msvc")
        string(APPEND CMAKE_C_FLAGS_INIT " /permissive- /volatile:iso /Wall /WX /wd4464 /wd4132 /wd4820 /wd4127 /wd5045 /wd4710 /wd4711 /wd4668")
        string(APPEND CMAKE_CXX_FLAGS_INIT " /permissive- /volatile:iso /W4 /WX")

    # no need to error on unknown compilers, done at start
    endif()

# coverage
elseif(platform MATCHES "coverage")

    # clang
    if(platform MATCHES "clang")
        string(APPEND CMAKE_C_FLAGS_INIT " -fprofile-instr-generate -fcoverage-mapping")
        string(APPEND CMAKE_CXX_FLAGS_INIT " -DPATOMIC_CI_COVERAGE=1 -fprofile-instr-generate -fcoverage-mapping")
        string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT " -fprofile-instr-generate -fcoverage-mapping")
        string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT " -fprofile-instr-generate -fcoverage-mapping")
        string(APPEND CMAKE_MODULE_LINKER_FLAGS_INIT " -fprofile-instr-generate -fcoverage-mapping")

    # unsupported
    else()
        message(FATAL_ERROR "Only clang is supported for coverage, unsupported platform: ${platform}")
    endif()

# sanitizer
elseif(platform MATCHES "sanitizer")

    # gnu
    if(platform MATCHES "(clang|gcc)")
        string(APPEND CMAKE_C_FLAGS_INIT " -fsanitize=address,undefined -fno-omit-frame-pointer")
        string(APPEND CMAKE_CXX_FLAGS_INIT " -DPATOMIC_CI_UBSAN=1 -DPATOMIC_CI_ASAN=1 -fsanitize=address,undefined -fno-omit-frame-pointer")
        string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT " -fsanitize=address,undefined")
        string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT " -fsanitize=address,undefined")
        string(APPEND CMAKE_MODULE_LINKER_FLAGS_INIT " -fsanitize=address,undefined")

    # unsupported
    else()
        message(FATAL_ERROR "Only clang/gcc are supported for sanitizer, unsupported platform: ${platform}")
    endif()

# no need to error on missing mode, done at start
endif()
