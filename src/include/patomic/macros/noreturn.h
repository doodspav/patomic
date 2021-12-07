#ifndef PATOMIC_NORETURN

#include <patomic/patomic_config.h>

#if PATOMIC_HAVE_NORETURN
    #define PATOMIC_NORETURN _Noreturn
#elif PATOMIC_HAVE_NORETURN_ATTR
    #define PATOMIC_NORETURN __attribute__((noreturn))
#elif PATOMIC_HAVE_NORETURN_DSPC
    #define PATOMIC_NORETURN __declspec(noreturn)
#else
    #define PATOMIC_NORETURN
#endif

#endif  /* !PATOMIC_NORETURN */
