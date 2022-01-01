#ifndef PATOMIC_STDLIB_LIMITS_H
#define PATOMIC_STDLIB_LIMITS_H

#include <limits.h>

#include <patomic/patomic_config.h>

#include <patomic/stdlib/stdint.h>

#if defined(LLONG_MIN)
    #if PATOMIC_HAVE_LONG_LONG
        #define PATOMIC_ULLONG_MAX ULLONG_MAX
        #define PATOMIC_LLONG_MAX  LLONG_MAX
        #define PATOMIC_LLONG_MIN  LLONG_MIN
    #elif PATOMIC_HAVE_LONG_LONG_EXTN
        #define PATOMIC_ULLONG_MAX (__extension__ ULLONG_MAX)
        #define PATOMIC_LLONG_MAX  (__extension__ LLONG_MAX)
        #define PATOMIC_LLONG_MIN  (__extension__ LLONG_MIN)
    #endif
#endif

#if defined(INT128_MIN)
    #if PATOMIC_HAVE_MS_INT128
        #define PATOMIC_UINT128_MAX UINT128_MAX
        #define PATOMIC_INT128_MAX  INT128_MAX
        #define PATOMIC_INT128_MIN  INT128_MIN
    #elif PATOMIC_HAVE_MS_INT128_EXTN
        #define PATOMIC_UINT128_MAX (__extension__ UINT128_MAX)
        #define PATOMIC_INT128_MAX  (__extension__ INT128_MAX)
        #define PATOMIC_INT128_MIN  (__extension__ INT128_MIN)
    #endif
#elif PATOMIC_HAVE_IR_TWOS_COMPL && PATOMIC_STDINT_HAVE_I128
    #define PATOMIC_UINT128_MAX ((patomic_i128_unsigned)((patomic_i128_signed)(-1)))
    #define PATOMIC_INT128_MAX  ((patomic_i128_signed)(PATOMIC_UINT128_MAX >> 1u))
    #define PATOMIC_INT128_MIN  ((patomic_i128_signed)((-PATOMIC_INT128_MAX) - 1))
#endif


#endif  /* !PATOMIC_STDLIB_LIMITS_H */
