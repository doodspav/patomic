#ifndef PATOMIC_PATOMIC_H
#define PATOMIC_PATOMIC_H

#include "api/align.h"
#include "api/combine.h"
#include "api/core.h"
#include "api/feature_check.h"
#include "api/ids.h"
#include "api/memory_order.h"
#include "api/ops.h"
#include "api/options.h"
#include "api/transaction.h"

#include <patomic/api/export.h>
#include <patomic/api/version.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


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
 *   Memory order to implicitly use for all atomic operations. This must be a
 *   valid memory order.
 *
 * @param options
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
    unsigned int options,
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
 * @param options
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
    unsigned int options,
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
 *   than one set of APIs using hardware support for performing lock-free
 *   transactional operations.
 *
 * @param options
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
 */
PATOMIC_EXPORT patomic_transaction_t
patomic_create_transaction(
    unsigned int options,
    unsigned int kinds,
    unsigned long ids
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_PATOMIC_H */
