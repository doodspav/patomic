#ifndef PATOMIC_INTERNAL_ALIGN_H
#define PATOMIC_INTERNAL_ALIGN_H

#include <patomic/api/core.h>


/**
 * @addtogroup internal
 *
 * @brief
 *   Compares two patomic_align_t objects.
 *
 * @details
 *   Priority is given to "recommended", then "minimum", then "size_within".
 */
int
patomic_internal_compare_align(
    patomic_align_t lhs,
    patomic_align_t rhs
);


#endif  /* PATOMIC_INTERNAL_ALIGN_H */
