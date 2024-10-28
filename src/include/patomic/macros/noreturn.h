#ifndef PATOMIC_NORETURN

#include <patomic/config.h>

/* used internally */
#undef PATOMIC_NORETURN_

#if PATOMIC_HAS_NORETURN
    #define PATOMIC_NORETURN_ _Noreturn
#elif PATOMIC_HAS_NORETURN_ATTR
    #define PATOMIC_NORETURN_ __attribute__((noreturn))
#elif PATOMIC_HAS_NORETURN_DSPC
    #define PATOMIC_NORETURN_ __declspec(noreturn)
#else
    #define PATOMIC_NORETURN_
#endif

/**
 * @addtogroup macros
 *
 * @brief
 *   Function attribute to declare that the function shall not return. This must
 *   come after the static qualifier.
 */
#define PATOMIC_NORETURN PATOMIC_NORETURN_

#endif  /* PATOMIC_NORETURN */
