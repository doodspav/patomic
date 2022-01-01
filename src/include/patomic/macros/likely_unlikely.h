#ifndef PATOMIC_LIKELY

#include <patomic/patomic_config.h>

#if PATOMIC_HAVE_BUILTIN_EXPECT
    #define PATOMIC_LIKELY(x) __builtin_expect((x), 1)
#else
    #define PATOMIC_LIKELY(x) x
#endif

#endif  /* !PATOMIC_LIKELY */


#ifndef PATOMIC_UNLIKELY

#include <patomic/patomic_config.h>

#if PATOMIC_HAVE_BUILTIN_EXPECT
    #define PATOMIC_UNLIKELY(x) __builtin_expect((x), 0)
#else
    #define PATOMIC_UNLIKELY(x) x
#endif

#endif  /* !PATOMIC_UNLIKELY */
