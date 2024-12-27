/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#include "msvc.h"

#include <patomic/macros/ignore_unused.h>


patomic_t
patomic_impl_create_msvc(
    const size_t byte_width,
    const patomic_memory_order_t order,
    const unsigned int options
)
{
    /* zero all fields */
    patomic_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(order);
    PATOMIC_IGNORE_UNUSED(options);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}


patomic_explicit_t
patomic_impl_create_explicit_msvc(
    const size_t byte_width,
    const unsigned int options
)
{
    /* zero all fields */
    patomic_explicit_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(options);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}


patomic_transaction_t
patomic_impl_create_transaction_msvc(
    const unsigned int options
)
{
    /* zero all fields */
    patomic_transaction_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(options);

    /* return */
    return impl;
}
