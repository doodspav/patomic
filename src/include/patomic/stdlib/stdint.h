#ifndef PATOMIC_STDLIB_STDINT_H
#define PATOMIC_STDLIB_STDINT_H

#include <patomic/patomic_config.h>

#include <stddef.h>
#if PATOMIC_HAVE_STD_INT_UINTPTR
    #include <stdint.h>
#endif

typedef signed char   patomic_char_signed;
typedef unsigned char patomic_char_unsigned;

typedef signed short   patomic_short_signed;
typedef unsigned short patomic_short_unsigned;

typedef signed int   patomic_int_signed;
typedef unsigned int patomic_int_unsigned;

typedef signed long   patomic_long_signed;
typedef unsigned long patomic_long_unsigned;

#if PATOMIC_HAVE_LONG_LONG
    typedef signed long long   patomic_llong_signed;
    typedef unsigned long long patomic_llong_unsigned;
    #define PATOMIC_STDINT_HAVE_LLONG 1
#elif PATOMIC_HAVE_LONG_LONG_EXTN
    __extension__ typedef signed long long   patomic_llong_signed;
    __extension__ typedef unsigned long long patomic_llong_unsigned;
    #define PATOMIC_STDINT_HAVE_LLONG 1
#else
    #define PATOMIC_STDINT_HAVE_LLONG 0
#endif

#if PATOMIC_HAVE_MS_INT128
    typedef signed __int128   patomic_i128_signed;
    typedef unsigned __int128 patomic_i128_unsigned;
    #define PATOMIC_STDINT_HAVE_I128 1
#elif PATOMIC_HAVE_MS_INT128_EXTN
    __extension__ typedef signed __int128   patomic_i128_signed;
    __extension__ typedef unsigned __int128 patomic_i128_unsigned;
    #define PATOMIC_STDINT_HAVE_I128 1
#else
    #define PATOMIC_STDINT_HAVE_I128 0
#endif

#if PATOMIC_HAVE_STD_INT_UINTPTR || PATOMIC_HAVE_STD_DEF_UINTPTR
    typedef intptr_t  patomic_intptr_t;
    typedef uintptr_t patomic_uintptr_t;
#elif defined(__INTPTR_TYPE__) && defined(__UINTPTR_TYPE__)
    typedef __INTPTR_TYPE__  patomic_intptr_t;
    typedef __UINTPTR_TYPE__ patomic_uintptr_t;
#elif PPATOMIC_STDINT_HAVE_I128
    typedef patomic_i128_signed   patomic_intptr_t;
    typedef patomic_i128_unsigned patomic_uintptr_t;
#elif PATOMIC_SPATOMIC_STDINT_HAVE_LLONG
    typedef patomic_llong_signed   patomic_intptr_t;
    typedef patomic_llong_unsigned patomic_uintptr_t;
#else
    typedef signed long   patomic_intptr_t;
    typedef unsigned long patomic_uintptr_t;
#endif

#endif  /* !PATOMIC_STDLIB_STDINT_H */
