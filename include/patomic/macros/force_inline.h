#ifndef PATOMIC_FORCE_INLINE

/* temp macro */
#define PATOMIC__MSC 0
#define PATOMIC__GNU 0
#define PATOMIC__C99 0

/* msvc doesn't support inline in C */
#ifdef _MSC_VER
    #undef PATOMIC__MSC
    #define PATOMIC__MSC 1
#endif

/* gnu required for __attribute__ */
#ifdef __GNUC__
    #undef PATOMIC__GNU
    #define PATOMIC__GNU 1
#endif

/* inline introduced @C99 */
#if defined(__STDC_VERSION__)
    #if __STDC_VERSION__ >= 199901L
        #undef PATOMIC__C99
        #define PATOMIC__C99 1
    #endif
#endif

/* force inline */
#if PATOMIC__MSC
    #define PATOMIC_FORCE_INLINE __forceinline
#elif PATOMIC__GNU && PATOMIC__C99
    #define PATOMIC_FORCE_INLINE inline __attribute__((always_inline))
/* #elif PATOMIC__GNU - gcc will warn if attribute is used without inline
    #define PATOMIC_FORCE_INLINE __attribute__((always_inline)) */
#elif PATOMIC__C99
    #define PATOMIC_FORCE_INLINE inline
#else
    #define PATOMIC_FORCE_INLINE
#endif

/* undef temp macros */
#undef PATOMIC__MSC
#undef PATOMIC__GNU
#undef PATOMIC__C99

#endif  /* !PATOMIC_FORCE_INLINE */
