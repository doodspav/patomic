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

/* 'inline' keyword is available */
/* requires: C99 */
#ifndef PATOMIC_HAVE_INLINE
    #define PATOMIC_HAVE_INLINE 0
#endif

/* '_Alignof' and '_Alignas' keywords are available */
/* requires: C11 */
#ifndef PATOMIC_HAVE_ALIGNOF
    #define PATOMIC_HAVE_ALIGNOF 0
#endif

/* <stdatomic.h> header and '_Atomic' keyword are available */
/* requires: C11 and __STD_NO_ATOMICS__ to not be defined */
#ifndef PATOMIC_HAVE_STD_ATOMIC
    #define PATOMIC_HAVE_STD_ATOMIC 0
#endif


#endif  /* !PATOMIC_PATOMIC_CONFIG_H */
