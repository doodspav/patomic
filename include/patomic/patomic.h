#ifndef PATOMIC_PATOMIC_H
#define PATOMIC_PATOMIC_H

#include <stddef.h>

#include <patomic/types/align.h>
#include <patomic/types/feature_check.h>
#include <patomic/types/ids.h>
#include <patomic/types/ops.h>
#include <patomic/types/options.h>
#include <patomic/types/memory_order.h>
#include <patomic/types/transaction.h>

#include <patomic/patomic_export.h>
#include <patomic/patomic_version.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * ATOMIC STRUCTS
 *
 * implicit/explicit:
 * - atomic operations
 * - alignment requirements
 *
 * transaction:
 * - atomic operations
 * - alignment requirements
 * - recommended time and space bounds
 * - transaction safe versions of <string.h> functions
 */

typedef struct {
    patomic_ops_t ops;
    patomic_align_t align;
} patomic_t;

typedef struct {
    patomic_ops_explicit_t ops;
    patomic_align_t align;
} patomic_explicit_t;

typedef struct {
    patomic_ops_transaction_t ops;
    patomic_align_t align;
    patomic_transaction_recommended_t recommended;
    patomic_transaction_safe_string_t sstring;
} patomic_transaction_t;


/*
 * COMBINE
 *
 * - combines two atomic structs
 * - the first parameter always takes priority, and only missing members are
 *   copied over from the second parameter
 *
 * - initially .ops may be modified
 * - if it is, .align may also be modified to meet the requirements for all the
 *   operations now present
 * - therefore it is advisable to sort structs by .align when combining more
 *   than two, in order to end up with the least restrictive values for .align
 *
 * - transaction structs are not combinable
 * - a transaction provides .sstring which cannot be combined with other
 *   implementations
 * - additionally, it is deemed fairly unlikely that any platform will have more
 *   than a single transaction implementation
 */

PATOMIC_EXPORT void
patomic_combine(
    patomic_t *priority,
    const patomic_t *other
);

PATOMIC_EXPORT void
patomic_combine_explicit(
    patomic_explicit_t *priority,
    const patomic_explicit_t *other
);


/*
 * CREATE
 *
 * - opts: one or more option_t flags combined
 * - kinds: one or more kind_t flags combined
 * - ids: one or more id_t flags combined
 *
 * - selects all implementations matching both kinds and ids
 *
 * - for implicit/explicit, combines selected implementations in an order that
 *   yields the least strict alignment requirement
 * - recommended alignment is prioritised over minimum alignment
 *
 * - for transaction, the implementation with the most efficient kind will be
 *   returned that supports at least one op (from the selected implementations)
 * - if multiple implementations fulfil these requirements, it is unspecified
 *   which one will be returned
 *
 * - in both cases, if no implementation exists matching both kinds and ids, the
 *   NULL implementation is returned
 */

PATOMIC_EXPORT patomic_t
patomic_create(
    size_t byte_width,
    patomic_memory_order_t order,
    unsigned int opts,
    unsigned int kinds,
    unsigned int ids
);

PATOMIC_EXPORT patomic_explicit_t
patomic_create_explicit(
    size_t byte_width,
    unsigned int opts,
    unsigned int kinds,
    unsigned int ids
);

PATOMIC_EXPORT patomic_transaction_t
patomic_create_transaction(
    unsigned int opts,
    unsigned int kinds,
    unsigned int ids
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_PATOMIC_H */
