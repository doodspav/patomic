#ifndef PATOMIC_HELPER_COMBINE_INTERNAL_H
#define PATOMIC_HELPER_COMBINE_INTERNAL_H

#include <patomic/api/core.h>


/**
 * @addtogroup helper
 *
 * @brief
 *   Internal implementation of patomic_combine which is not exported.
 *
 * @note
 *   Called internally instead of exported version to avoid PLT indirection
 *   when building as a shared library.
 */
void
patomic_internal_combine(
    patomic_t *priority,
    const patomic_t *other
);


/**
 * @addtogroup helper
 *
 * @brief
 *   Internal implementation of patomic_combine_explicit which is not exported.
 *
 * @note
 *   Called internally instead of exported version to avoid PLT indirection
 *   when building as a shared library.
 */
void
patomic_internal_combine_explicit(
    patomic_explicit_t *priority,
    const patomic_explicit_t *other
);


#endif  /* PATOMIC_HELPER_COMBINE_INTERNAL_H */
