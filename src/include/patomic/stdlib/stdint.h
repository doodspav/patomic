#ifndef PATOMIC_STDLIB_STDINT_H
#define PATOMIC_STDLIB_STDINT_H

#include <patomic/patomic_config.h>

#include <stddef.h>
#if PATOMIC_HAVE_STD_INT_UINTPTR
    #include <stdint.h>
#endif

#if PATOMIC_HAVE_STD_INT_UINTPTR || PATOMIC_HAVE_STD_DEF_UINTPTR
    typedef intptr_t  patomic_intptr_t;
    typedef uintptr_t patomic_uintptr_t;
#elif defined(__INTPTR_TYPE__) && defined(__UINTPTR_TYPE__)
    typedef __INTPTR_TYPE__  patomic_intptr_t;
    typedef __UINTPTR_TYPE__ patomic_uintptr_t;
#elif PATOMIC_HAVE_LONG_LONG
    typedef signed long long   patomic_intptr_t;
    typedef unsigned long long patomic_uintptr_t;
#else
    typedef signed long   patomic_intptr_t;
    typedef unsigned long patomic_uintptr_t;
#endif

#endif  /* !PATOMIC_STDLIB_STDINT_H */
