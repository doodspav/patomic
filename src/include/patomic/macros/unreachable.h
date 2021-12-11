#ifndef PATOMIC_UNREACHABLE
#undef PATOMIC_UNREACHABLE_IS_VCZ

#include <patomic/patomic_config.h>

#if PATOMIC_HAVE_MS_ASSUME
    #define PATOMIC_UNREACHABLE() __assume(0)
    #define PATOMIC_UNREACHABLE_IS_VCZ 0
#elif PATOMIC_HAVE_BUILTIN_UNREACHABLE
    #define PATOMIC_UNREACHABLE() __builtin_unreachable()
    #define PATOMIC_UNREACHABLE_IS_VCZ 0
#else
    #define PATOMIC_UNREACHABLE() ((void) 0)
    #define PATOMIC_UNREACHABLE_IS_VCZ 1
#endif

#endif  /* !PATOMIC_UNREACHABLE */
