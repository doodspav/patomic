/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_H
#define PATOMIC_IMPL_MSVC_H

#include <patomic/patomic.h>


/**
 * @addtogroup impl.msvc
 *
 * @brief
 *   No operations are supported.
 *
 * @param byte_width
 *   Value is ignored.
 *
 * @param order
 *   Value is ignored.
 *
 * @param options
 *   Value is ignored.
 *
 * @return
 *   Implementation where no operations are supported and alignment requirements
 *   are the minimum possible.
 */
patomic_t
patomic_impl_create_msvc(
    size_t byte_width,
    patomic_memory_order_t order,
    unsigned int options
);


/**
 * @addtogroup impl.msvc
 *
 * @brief
 *   No operations are supported.
 *
 * @param byte_width
 *   Value is ignored.
 *
 * @param options
 *   Value is ignored.
 *
 * @return
 *   Implementation where no operations are supported and alignment requirements
 *   are the minimum possible.
 */
patomic_explicit_t
patomic_impl_create_explicit_msvc(
    size_t byte_width,
    unsigned int options
);


/**
 * @addtogroup impl.msvc
 *
 * @brief
 *   No operations are supported.
 *
 * @param options
 *   Value is ignored.
 *
 * @return
 *   Implementation where no operations are supported and alignment requirements
 *   are the minimum possible.
 */
patomic_transaction_t
patomic_impl_create_transaction_msvc(
    unsigned int options
);


#endif  /* PATOMIC_IMPL_MSVC_H */
