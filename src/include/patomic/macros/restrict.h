#ifndef PATOMIC_RESTRICT

#include <patomic/config.h>

/* used internally */
#undef PATOMIC_RESTRICT_

#if PATOMIC_HAS_RESTRICT
    #define PATOMIC_RESTRICT_ restrict
#elif PATOMIC_HAS_MS_RESTRICT
    #define PATOMIC_RESTRICT_ __restrict
#elif PATOMIC_HAS_GNU_RESTRICT
    #define PATOMIC_RESTRICT_ __restrict__
#else
    #define PATOMIC_RESTRICT_
#endif

/**
 * @addtogroup macros
 *
 * @brief
 *   Object pointer qualifier to declare that a pointer will not alias with any
 *   other pointer. Behaves like C99's 'restrict' keyword.
 */
#define PATOMIC_RESTRICT PATOMIC_RESTRICT_

#endif  /* PATOMIC_RESTRICT */
