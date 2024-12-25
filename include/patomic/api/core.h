/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_API_CORE_H
#define PATOMIC_API_CORE_H

#include "align.h"
#include "ops.h"
#include "transaction.h"

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
 *   Struct containing all information and functionality required to perform
 *   atomic operations implemented using a sequentially consistent transaction.
 */
typedef struct {

    /** @brief Atomic operations implemented using a sequentially consistent
     *         transaction, helper non-transactional atomic flag operations,
     *         and raw transaction primitives. */
    patomic_ops_transaction_t ops;

} patomic_transaction_t;


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_API_CORE_H */
