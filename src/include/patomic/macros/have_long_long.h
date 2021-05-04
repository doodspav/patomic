#ifndef PATOMIC_HAVE_LONG_LONG

#define PATOMIC_HAVE_LONG_LONG 0

/* minimum standard */
#if defined(__STDC_VERSION__)
    #if __STDC_VERSION__ >= 199901L
        #undef PATOMIC_HAVE_LONG_LONG
        #define PATOMIC_HAVE_LONG_LONG 1
    #endif
#endif

#endif  /* !PATOMIC_HAVE_LONG_LONG */
