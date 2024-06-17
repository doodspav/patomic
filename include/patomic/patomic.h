#ifndef PATOMIC_PATOMIC_H
#define PATOMIC_PATOMIC_H

#include "types/align.h"
#include "types/feature_check.h"
#include "types/ids.h"
#include "types/memory_order.h"
#include "types/ops.h"
#include "types/options.h"
#include "types/transaction.h"

#include <patomic/types/export.h>
#include <patomic/types/version.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup patomic
 *
 * @brief
 *   Struct containing all information and functionality required to perform
 *   atomic operations with implicit memory order.
 */
typedef struct {

    /** @brief Atomic operations with implicit memory order. */
    patomic_ops_t ops;

    /** @brief Alignment requirements for atomic operations. */
    patomic_align_t align;

} patomic_t;


/**
 * @addtogroup patomic
 *
 * @brief
 *   Struct containing all information and functionality required to perform
 *   atomic operations with explicit memory order.
 */
typedef struct {

    /** @brief Atomic operations with explicit memory order. */
    patomic_ops_explicit_t ops;

    /** @brief Alignment requirements for atomic operations. */
    patomic_align_t align;

} patomic_explicit_t;


/**
 * @addtogroup patomic
 *
 * @brief
 *   Struct containing al information and functionality required to perform
 *   atomic operations implemented using a sequentially consistent transaction.
 */
typedef struct {

    /** @brief Atomic operations implemented using a sequentially consistent
     *         transaction, and non-atomic transaction specific operations. */
    patomic_ops_transaction_t ops;

    /** @brief Alignment requirements for atomic operations. */
    patomic_align_t align;

    /** @brief Recommended time and space bounds for atomic operations. */
    patomic_transaction_recommended_t recommended;

    /** @brief Transaction safe versions of core <string.h> functions. */
    patomic_transaction_safe_string_t string;

} patomic_transaction_t;


/**
 * @addtogroup patomic
 *
 * @brief
 *   Combines two atomic structs with implicit memory order operations. The
 *   first struct always takes priority, and only missing members are copied
 *   over from the second struct.
 *
 * @details
 *   Initially, the .ops member may be modified. If it is modified, the .align
 *   member may also be modified to meet the requirements for all the operations
 *   now present.
 *
 * @note
 *   It is advisable to sort structs by the .align member when combining more
 *   than two structs, in order to end up with the least restrictive values for
 *   the .align member.
 *
 * @param priority
 *   Struct which takes priority if both structs support an operation, and into
 *   which unsupported operations are added from the other struct.
 *
 * @param other
 *   Struct to combine into priority struct.
 */
PATOMIC_EXPORT void
patomic_combine(
    patomic_t *priority,
    const patomic_t *other
);


/**
 * @addtogroup patomic
 *
 * @brief
 *   Combines two atomic structs with explicit memory order operations. The
 *   first struct always takes priority, and only missing members are copied
 *   over from the second struct.
 *
 * @details
 *   Initially, the .ops member may be modified. If it is modified, the .align
 *   member may also be modified to meet the requirements for all the operations
 *   now present.
 *
 * @note
 *   It is advisable to sort structs by the .align member when combining more
 *   than two structs, in order to end up with the least restrictive values for
 *   the .align member.
 *
 * @param priority
 *   Struct which takes priority if both structs support an operation, and into
 *   which unsupported operations are added from the other struct.
 *
 * @param other
 *   Struct to combine into priority struct.
 */
PATOMIC_EXPORT void
patomic_combine_explicit(
    patomic_explicit_t *priority,
    const patomic_explicit_t *other
);


/**
 * @addtogroup patomic
 *
 * @brief
 *   Combines all implementations with implicit memory order matching both kinds
 *   and ids in an order that yields the least strict alignment requirements,
 *   with recommended alignment being prioritised over minimum alignment.
 *
 * @param byte_width
 *   Width in bytes of type to support.
 *
 * @param order
 *   Memory order to implicitly use for all atomic operations.
 *
 * @param opts
 *   One or more patomic_option_t flags combined. Passed on to each internal
 *   implementation to be used in an unspecified manner.
 *
 * @param kinds
 *   One or more patomic_kind_t flags combined.
 *
 * @param ids
 *   One or more patomic_id_t flags combined.
 *
 * @returns
 *   Combined implementations matching both kinds and ids. If no such
 *   implementations exist, the NULL implementation is returned.
 *
 * @note
 *   The alignment requirements returned by this function will always be valid
 *   even if no operations are supported.
 */
PATOMIC_EXPORT patomic_t
patomic_create(
    size_t byte_width,
    patomic_memory_order_t order,
    unsigned int opts,
    unsigned int kinds,
    unsigned long ids
);


/**
 * @addtogroup patomic
 *
 * @brief
 *   Combines all implementations with implicit memory order matching both kinds
 *   and ids in an order that yields the least strict alignment requirements,
 *   with recommended alignment being prioritised over minimum alignment.
 *
 * @param byte_width
 *   Width in bytes of type to support.
 *
 * @param opts
 *   One or more patomic_option_t flags combined. Passed on to each internal
 *   implementation to be used in an unspecified manner.
 *
 * @param kinds
 *   One or more patomic_kind_t flags combined.
 *
 * @param ids
 *   One or more patomic_id_t flags combined.
 *
 * @returns
 *   Combined implementations matching both kinds and ids. If no such
 *   implementations exist, the NULL implementation is returned.
 *
 * @note
 *   The alignment requirements returned by this function will always be valid
 *   even if no operations are supported.
 */
PATOMIC_EXPORT patomic_explicit_t
patomic_create_explicit(
    size_t byte_width,
    unsigned int opts,
    unsigned int kinds,
    unsigned long ids
);


/**
 * @addtogroup patomic
 *
 * @brief
 *   Provides the implementation implemented using a sequentially consistent
 *   transaction with the most efficient kind that supports at least a single
 *   operation. If multiple implementations fulfil these requirements, it is
 *   unspecified which one will be returned.
 *
 * @note
 *   Implementations are not combined because it is too complicated to do
 *   properly, and because it is not expected that any platform will have more
 *   than one set of assembly instructions for performing lock-free
 *   transactional operations.
 *
 * @param opts
 *   One or more patomic_option_t flags combined. Passed on to each internal
 *   implementation to be used in an unspecified manner.
 *
 * @param kinds
 *   One or more patomic_kind_t flags combined.
 *
 * @param ids
 *   One or more patomic_id_t flags combined.
 *
 * @returns
 *   The implementation with the most efficient kind that supports at least a
 *   single operation. If no such implementations exist, the NULL implementation
 *   is returned.
 *
 * @note
 *   The alignment requirements returned by this function will always be valid
 *   even if no operations are supported.
 */
PATOMIC_EXPORT patomic_transaction_t
patomic_create_transaction(
    unsigned int opts,
    unsigned int kinds,
    unsigned long ids
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_PATOMIC_H */
