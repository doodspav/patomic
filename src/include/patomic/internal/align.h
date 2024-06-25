#ifndef PATOMIC_INTERNAL_ALIGN_H
#define PATOMIC_INTERNAL_ALIGN_H

#include <patomic/api/align.h>


/**
 * @addtogroup internal
 *
 * @brief
 *   Compares two patomic_align_t objects with an API that can be used by
 *   qsort.
 *
 * @details
 *   Priority is given to "recommended", then "minimum", then "size_within".
 */
int
patomic_internal_compare_align(
    const void *lhs_void,
    const void *rhs_void
);


#endif  /* PATOMIC_INTERNAL_ALIGN_H */
