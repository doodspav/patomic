/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#include "../impl/register.h"

#include <patomic/api/ids.h>

#include <patomic/stdlib/assert.h>
#include <patomic/stdlib/math.h>


unsigned long
patomic_get_ids(
    const unsigned int kinds
)
{
    /* declarations */
    patomic_impl_t const *begin = patomic_impl_register;
    patomic_impl_t const *const end = begin + PATOMIC_IMPL_REGISTER_SIZE;
    unsigned long ids = 0ul;

    /* combine ids */
    for (; begin != end; ++begin)
    {
        const unsigned int kind = (unsigned int) begin->kind;
        const unsigned long id = (unsigned long) begin->id;
        if (kind & kinds)
        {
            ids |= id;
        }
    }

    /* result */
    return ids;
}


unsigned int
patomic_get_kind(
    const unsigned long id
)
{
    /* declarations */
    patomic_impl_t const *begin = patomic_impl_register;
    patomic_impl_t const *const end = begin + PATOMIC_IMPL_REGISTER_SIZE;

    /* always assert because we have no other way to surface error */
    patomic_assert_always(patomic_unsigned_is_pow2_or_zero(id));

    /* find kind */
    for (; begin != end; ++begin)
    {
        if (begin->id == id)
        {
            return (unsigned int) begin->kind;
        }
    }

    /* didn't find kind */
    return patomic_kind_UNKN;
}
