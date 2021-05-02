#ifndef PATOMIC_HAVE_STD_ALIGNOF

#define PATOMIC_HAVE_STD_ALIGNOF 0

/* minimum standard */
#if defined(__STDC_VERSION__)
    #if __STDC_VERSION__ >= 201112L
        #undef PATOMIC_HAVE_STD_ALIGNOF
        #define PATOMIC_HAVE_STD_ALIGNOF 1
    #endif
#endif

#endif  /* !PATOMIC_HAVE_STD_ALIGNOF */
