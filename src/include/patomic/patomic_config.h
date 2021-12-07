#ifndef PATOMIC_PATOMIC_CONFIG_H
#define PATOMIC_PATOMIC_CONFIG_H


#include <limits.h>

/* integer representation is two's complement */
#undef PATOMIC_HAVE_TWOS_COMPL
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

/* '__inline__ __attribute__((always_inline))' is available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_GNU_INLINE_ALWAYS_INLINE_ATTR
    #define PATOMIC_HAVE_GNU_INLINE_ALWAYS_INLINE_ATTR 0
#endif

/* '__attribute__((noinline))' is available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_NOINLINE_ATTR
    #define PATOMIC_HAVE_NOINLINE_ATTR 0
#endif

/* '__declspec(noinline)' is available */
/* requires: Microsoft compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_NOINLINE_DSPC
    #define PATOMIC_HAVE_NOINLINE_DSPC 0
#endif

/* '_Noreturn' keyword is available */
/* requires: C11 */
#ifndef PATOMIC_HAVE_NORETURN
    #define PATOMIC_HAVE_NORETURN 0
#endif

/* '__declspec(noreturn)' is available */
/* requires: Microsoft compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_NORETURN_DSPC
    #define PATOMIC_HAVE_NORETURN_DSPC 0
#endif

/* '__attribute__((noreturn))' is available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_NORETURN_ATTR
    #define PATOMIC_HAVE_NORETURN_ATTR 0
#endif

/* '_Alignof' and '_Alignas' keywords are available */
/* requires: C11 */
#ifndef PATOMIC_HAVE_ALIGNOF
    #define PATOMIC_HAVE_ALIGNOF 0
#endif

/* '__alignof' keyword and '__declspec(align(#))' are available */
/* requires: MS compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_MS_ALIGNOF_ALIGN_DSPC
    #define PATOMIC_HAVE_MS_ALIGNOF_ALIGN_DSPC 0
#endif

/* '__alignof__' keyword and '__attribute__((aligned(#)))' are available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_GNU_ALIGNOF_ALIGNED_ATTR
    #define PATOMIC_HAVE_GNU_ALIGNOF_ALIGNED_ATTR 0
#endif

/* <stdatomic.h> header and '_Atomic' keyword are available */
/* requires: C11 and __STD_NO_ATOMICS__ to not be defined */
/* note: currently MSVC defines __STD_NO_ATOMICS__ */
#ifndef PATOMIC_HAVE_STD_ATOMIC
    #define PATOMIC_HAVE_STD_ATOMIC 0
#endif


#endif  /* !PATOMIC_PATOMIC_CONFIG_H */
