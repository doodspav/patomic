#ifndef PATOMIC_PATOMIC_CONFIG_H
#define PATOMIC_PATOMIC_CONFIG_H


#include <limits.h>

/* integer representation is two's complement */
#ifdef PATOMIC_HAVE_TWOS_COMPL
    #undef PATOMIC_HAVE_TWOS_COMPL
#endif
#define PATOMIC_HAVE_TWOS_COMPL (-INT_MAX != INT_MIN)


/*
 * The following macros are defined in '_patomic_config.h' which is generated
 * by CMake at build time.
 * If you do not use CMake to build this project, comment out the '#include'
 * and modify the macro values.
 * The 'requires:' comments are to be taken as helpful hints.
 */

#include <patomic/_patomic_config.h>

/* 'long long' is a fundamental type */
/* requires: C99 */
#ifndef PATOMIC_HAVE_LONG_LONG
    #define PATOMIC_HAVE_LONG_LONG 0
#endif

/* '__forceinline' keyword is available */
/* requires: MS compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_FORCEINLINE
    #define PATOMIC_HAVE_FORCEINLINE 0
#endif

/* '__attribute__((always_inline)) is available to use without 'inline' */
/* requires: Clang (and maybe other non-GCC GNU compatible compilers) */
#ifndef PATOMIC_HAVE_ALWAYS_INLINE_ATTR
    #define PATOMIC_HAVE_ALWAYS_INLINE_ATTR 0
#endif

/* 'inline __attribute__((always_inline))' is available */
/* requires: 'inline' keyword and GNU compatible(-ish) compiler */
/* note: TCC supports this, but doesn't define __GNUC__ */
#ifndef PATOMIC_HAVE_INLINE_ALWAYS_INLINE_ATTR
    #define PATOMIC_HAVE_INLINE_ALWAYS_INLINE_ATTR 0
#endif

/* '_Alignof' and '_Alignas' keywords are available */
/* requires: C11 */
#ifndef PATOMIC_HAVE_ALIGNOF
    #define PATOMIC_HAVE_ALIGNOF 0
#endif

/* <stdatomic.h> header and '_Atomic' keyword are available */
/* requires: C11 and __STD_NO_ATOMICS__ to not be defined */
/* note: currently MSVC defines __STD_NO_ATOMICS__ */
#ifndef PATOMIC_HAVE_STD_ATOMIC
    #define PATOMIC_HAVE_STD_ATOMIC 0
#endif

/* <immintrin.h> header and RTM intrinsics are available */
/* requires: supporting compiler and x86 target */
#ifndef PATOMIC_HAVE_IMMINTRIN_RTM
    #define PATOMIC_HAVE_IMMINTRIN_RTM 0
#endif

/* <intrin.h> header and '__cpuid(ex)' intrinsics are available */
/* requires: MS compatible(-ish) compiler and x86 target */
#ifndef PATOMIC_HAVE_INTRIN_CPUID
    #define PATOMIC_HAVE_INTRIN_CPUID 0
#endif

/* <cpuid.h> header and  '__cpuid(_count)' intrinsics are available */
/* requires: GNU compatible(-ish) compiler and x86 target */
#ifndef PATOMIC_HAVE_CPUID_CPUID
    #define PATOMIC_HAVE_CPUID_CPUID 0
#endif


#endif  /* !PATOMIC_PATOMIC_CONFIG_H */
