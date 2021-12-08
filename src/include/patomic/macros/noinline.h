#ifndef PATOMIC_NOINLINE

#include <patomic/patomic_config.h>

#if PATOMIC_HAVE_NOINLINE_ATTR
    #define PATOMIC_NOINLINE __attribute__((noinline))
#elif PATOMIC_HAVE_NOINLINE_DSPC
    #define PATOMIC_NOINLINE __declspec(noinline)
#else
    #define PATOMIC_NOINLINE
#endif

#endif  /* !PATOMIC_NOINLINE */
