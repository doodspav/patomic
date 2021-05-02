#ifndef PATOMIC_HAVE_STD_ATOMIC_H

/* define temp macros */
#define PATOMIC__MIN_STD 0
#define PATOMIC__GCC_BUG 0
#define PATOMIC__HAS_INC 0

/* minimum standard */
#if defined(__STDC_VERSION__)
    #if __STDC_VERSION__ >= 201112L
        #undef PATOMIC__MIN_STD
        #define PATOMIC__MIN_STD 1
    #endif
#endif

/* GCC standards conformance bug (C11@4.7 but _Atomic@4.9) */
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
    #if __GNUC__ == 4 && __GNUC_MINOR__ < 9
        #undef PATOMIC__GCC_BUG
        #define PATOMIC__GCC_BUG 1
    #endif
#endif

/* __has_include */
#if defined(__has_include)
    #if __has_include(<stdatomic.h>)
        #undef PATOMIC__HAS_INC
        #define PATOMIC__HAS_INC 1
    #endif
#endif

/* define macro */
#if PATOMIC__MIN_STD
    #if defined(__has_include) && !PATOMIC__HAS_INC
        #define PATOMIC_HAVE_STD_ATOMIC 0
    #elif PATOMIC__GCC_BUG && !PATOMIC__HAS_INC
        #define PATOMIC_HAVE_STD_ATOMIC 0
    #else
        #define PATOMIC_HAVE_STD_ATOMIC 1
    #endif
#else
    #define PATOMIC_HAVE_STD_ATOMIC 0
#endif

/* __STD_NO_ATOMICS__ overrules everything else */
#if defined(__STD_NO_ATOMICS__)
    #undef PATOMIC_HAVE_STD_ATOMIC
    #define PATOMIC_HAVE_STD_ATOMIC 0
#endif

/* undef temp macros */
#undef PATOMIC__MIN_STD
#undef PATOMIC__GCC_BUG
#undef PATOMIC__HAS_INC

#endif  /* !PATOMIC_HAVE_STD_ATOMIC_H */
