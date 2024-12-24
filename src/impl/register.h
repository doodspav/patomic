/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_REGISTER_H
#define PATOMIC_REGISTER_H

#include "null/null.h"
#include "std/std.h"

#include <patomic/patomic.h>


/**
 * @addtogroup register
 *
 * @brief
 *   Public "interface" through which the rest of this library obtains
 *   information and functionality from each internal implementation.
 */
typedef struct {

    /** @brief Unique ID corresponding only to this implementation. */
    patomic_id_t id;

    /** @brief Estimate of implementation's runtime overhead. */
    patomic_kind_t kind;

    /** @brief Function called directly by patomic_create to obtain atomic
     *         operations with implicit memory order from this
     *         implementation. */
    patomic_t (* fp_create) (size_t, patomic_memory_order_t, unsigned int);

    /** @brief Function called directly by patomic_create_explicit to obtain
     *         atomic operations with explicit memory order from this
     *         implementation. */
    patomic_explicit_t (* fp_create_explicit) (size_t, unsigned int);

    /** @brief Function called directly by patomic_create_transaction to obtain
     *         atomic operations implemented using a sequentially consistent
     *         transaction from this implementation. */
    patomic_transaction_t (* fp_create_transaction) (unsigned int);

} patomic_impl_t;


/**
 * @addtogroup register
 *
 * @brief
 *   Array containing set of all existing implementations, regardless of whether
 *   they are supported on the current platform.
 *
 * @warning
 *   Each implementation element must have a unique ID, which may not have more
 *   than a single bit set.
 *
 * @note
 *   An implementation may use the NULL implementation's creation functions if
 *   it cannot create any implicit, explicit, or transaction operations.
 */
static const patomic_impl_t
patomic_impl_register[] = {
    {
        patomic_id_NULL,
        patomic_kind_UNKN,
        patomic_impl_create_null,
        patomic_impl_create_explicit_null,
        patomic_impl_create_transaction_null
    },
    {
        patomic_id_STDC,
        patomic_kind_BLTN,
        patomic_impl_create_std,
        patomic_impl_create_explicit_std,
        patomic_impl_create_transaction_std
    }
};


/**
 * @addtogroup register
 *
 * @brief
 *   Number of elements in patomic_impl_register array.
 */
#define PATOMIC_IMPL_REGISTER_SIZE \
    ((sizeof patomic_impl_register) / sizeof(patomic_impl_t))


#endif  /* PATOMIC_REGISTER_H */
