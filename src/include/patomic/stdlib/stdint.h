#ifndef PATOMIC_STDLIB_STDINT_H
#define PATOMIC_STDLIB_STDINT_H

#include <patomic/config.h>

#include <stddef.h>
#if PATOMIC_HAS_STDINT_H
    #include <stdint.h>
#endif


#if PATOMIC_HAS_LONG_LONG
    typedef signed long long   patomic_llong_signed_t;
    typedef unsigned long long patomic_llong_unsigned_t;
    #define PATOMIC_STDINT_HAS_LLONG 1
#elif PATOMIC_HAS_LONG_LONG_EXTN
    __extension__ typedef signed long long   patomic_llong_signed_t;
    __extension__ typedef unsigned long long patomic_llong_unsigned_t;
    #define PATOMIC_STDINT_HAS_LLONG 1
#else
    #define PATOMIC_STDINT_HAS_LLONG 0
#endif


#if PATOMIC_HAS_MS_INT128
    typedef signed __int128   patomic_int128_signed_t;
    typedef unsigned __int128 patomic_int128_unsigned_t;
    #define PATOMIC_STDINT_HAS_INT128 1
#elif PATOMIC_HAS_MS_INT128_EXTN
    __extension__ typedef signed __int128   patomic_int128_signed_t;
    __extension__ typedef unsigned __int128 patomic_int128_unsigned_t;
    #define PATOMIC_STDINT_HAS_INT128 1
#else
    #define PATOMIC_STDINT_HAS_INT128 0
#endif


#if PATOMIC_HAS_STDINT_INTPTR || PATOMIC_HAS_STDDEF_INTPTR
    typedef intptr_t  patomic_intptr_signed_t;
    typedef uintptr_t patomic_intptr_unsigned_t;
#elif PATOMIC_HAS_STDINT_INTPTR_EXTN || PATOMIC_HAS_STDDEF_INTPTR_EXTN
    __extension__ typedef intptr_t  patomic_intptr_signed_t;
    __extension__ typedef uintptr_t patomic_intptr_unsigned_t;
#elif defined(__INTPTR_TYPE__) && defined(__UINTPTR_TYPE__)
    typedef __INTPTR_TYPE__  patomic_intptr_signed_t;
    typedef __UINTPTR_TYPE__ patomic_intptr_unsigned_t;
#elif PATOMIC_STDINT_HAS_I128
    typedef patomic_i128_signed_t   patomic_intptr_signed_t;
    typedef patomic_i128_unsigned_t patomic_intptr_unsigned_t;
#elif PATOMIC_STDINT_HAS_LLONG
    typedef patomic_llong_signed_t   patomic_intptr_signed_t;
    typedef patomic_llong_unsigned_t patomic_intptr_unsigned_t;
#else
    typedef signed long   patomic_intptr_signed_t;
    typedef unsigned long patomic_intptr_unsigned_t;
#endif


#endif  /* PATOMIC_STDLIB_STDINT_H */
