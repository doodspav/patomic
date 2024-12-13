#ifndef PATOMIC_INTERNAL_FEATURE_CHECK
#define PATOMIC_INTERNAL_FEATURE_CHECK

#include <patomic/api/feature_check.h>


/**
 * @addtogroup internal
 *
 * @brief
 *   Internal implementation of patomic_feature_check_any which is not
 *   exported.
 *
 * @note
 *   Called internally instead of exported version to avoid PLT indirection
 *   when building as a shared library.
 */
unsigned int
patomic_internal_feature_check_any(
    const patomic_ops_t *ops,
    unsigned int opcats
);


/**
 * @addtogroup internal
 *
 * @brief
 *   Internal implementation of patomic_feature_check_any_explicit which is not
 *   exported.
 *
 * @note
 *   Called internally instead of exported version to avoid PLT indirection
 *   when building as a shared library.
 */
unsigned int
patomic_internal_feature_check_any_explicit(
    const patomic_ops_explicit_t *ops,
    unsigned int opcats
);


/**
 * @addtogroup internal
 *
 * @brief
 *   Internal implementation of patomic_feature_check_any_transaction which is
 *   not exported.
 *
 * @note
 *   Called internally instead of exported version to avoid PLT indirection
 *   when building as a shared library.
 */
unsigned int
patomic_internal_feature_check_any_transaction(
    const patomic_ops_transaction_t *ops,
    unsigned int opcats
);


/**
 * @addtogroup internal
 *
 * @brief
 *   Internal implementation of patomic_feature_check_leaf_transaction which is
 *   not exported.
 *
 * @note
 *   Called internally instead of exported version to avoid PLT indirection
 *   when building as a shared library.
 */
unsigned int
patomic_internal_feature_check_leaf_transaction(
    const patomic_ops_transaction_t *ops,
    patomic_opcat_t opcat,
    unsigned int opkinds
);


#endif  /* PATOMIC_INTERNAL_FEATURE_CHECK */
