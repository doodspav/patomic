#ifndef PATOMIC_RESTRICT

#include <patomic/patomic_config.h>

#if PATOMIC_HAVE_RESTRICT
    #define PATOMIC_RESTRICT restrict
#elif PATOMIC_HAVE_MS_RESTRICT
    #define PATOMIC_RESTRICT __restrict
#elif PATOMIC_HAVE_GNU_RESTRICT
    #define PATOMIC_RESTRICT __restrict__
#else
    #define PATOMIC_RESTRICT
#endif

#endif  /* !PATOMIC_RESTRICT */
