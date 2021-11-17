#ifndef PATOMIC_PATOMIC_CONFIG_H
#define PATOMIC_PATOMIC_CONFIG_H


#include <limits.h>

/* integer representation is two's complement */
#ifdef PATOMIC_HAVE_TWOS_COMPL
    #undef PATOMIC_HAVE_TWOS_COMPL
#endif
#define PATOMIC_HAVE_TWOS_COMPL (-INT_MAX != INT_MIN)

/* x86 based platform */
#ifndef PATOMIC_HAVE_X86
#if defined(i386)          || defined(__i386)    || defined(__i386__) || \
    defined(__i486__)      || defined(__i586__)  || defined(__i686__) || \
    defined(__IA32__)      ||                                            \
    defined(_M_I86)        || defined(_M_IX86)   || defined(_M_I386)  || \
    defined(__X86__)       || defined(_X86_)     ||                      \
    defined(__I86__)       || defined(__386)     ||                      \
    defined(__THW_INTEL__) || defined(__INTEL__) ||                      \
    defined(__amd64__)     || defined(__amd64)   ||                      \
    defined(__x86_64__)    || defined(__x86_64)  ||                      \
    defined(_M_AMD64)      || defined(_M_X64)
    #define PATOMIC_HAVE_X86 1
#else
    #define PATOMIC_HAVE_X86 0
#endif
#endif


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
/* requires: Microsoft compatible(-ish) compiler */
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

/* <memoryapi.h> header available and provides Virtual functions */
/* requires: compiler with WinAPI support (also Windows XP or higher) */
#ifndef PATOMIC_HAVE_WIN32_MEMORYAPI_VIRTUAL
    #define PATOMIC_HAVE_WIN32_MEMORYAPI_VIRTUAL 0
#endif


#endif  /* !PATOMIC_PATOMIC_CONFIG_H */
