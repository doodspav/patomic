#ifndef PATOMIC_INTERNAL_ALIGN_H
#define PATOMIC_INTERNAL_ALIGN_H

#include <patomic/api/core.h>


/**
 * @addtogroup internal
 *
 * @brief
 *   Compares the "align" member of two patomic_t objects.
 *
 * @details
 *   Priority is given to "recommended", then "minimum", then "size_within".
 */
int
patomic_internal_compare_implicit_align(
    const patomic_t *lhs,
    const patomic_t *rhs
);


/**
 * @addtogroup internal
 *
 * @brief
 *   Compares the "align" member of two patomic_explicit_t objects.
 *
 * @details
 *   Priority is given to "recommended", then "minimum", then "size_within".
 */
int
patomic_internal_compare_explicit_align(
    const patomic_explicit_t *lhs,
    const patomic_explicit_t *rhs
);


/**
 * @addtogroup internal
 *
 * @brief
 *   Compares the "align" member of two patomic_transaction_t objects.
 *
 * @details
 *   Priority is given to "recommended", then "minimum", then "size_within".
 */
int
patomic_internal_compare_transaction_align(
    const patomic_transaction_t *lhs,
    const patomic_transaction_t *rhs
);


#endif  /* PATOMIC_INTERNAL_ALIGN_H */
