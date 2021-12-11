#ifndef PATOMIC_UNREACHABLE

#include <patomic/patomic_config.h>

#if PATOMIC_HAVE_MS_ASSUME
    #define PATOMIC_UNREACHABLE() __assume(0)
#elif PATOMIC_HAVE_BUILTIN_UNREACHABLE
    #define PATOMIC_UNREACHABLE() __builtin_unreachable()
#else
    #define PATOMIC_UNREACHABLE()
#endif

#endif  /* !PATOMIC_UNREACHABLE */
