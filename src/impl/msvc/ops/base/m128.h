/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_BASE_M128_H
#define PATOMIC_IMPL_MSVC_OPS_BASE_M128_H

#ifdef _MSC_VER

#include <patomic/macros/static_assert.h>


/**
 * @addtogroup impl.msvc
 *
 * @brief
 *   Helper type to use when implementing 128bit integer arithmetic operations.
 *
 * @details
 *   Microsoft Visual C++ only supports little endian targets, so no need to
 *   check for endianness.
 */
typedef struct {
    __int64 low;
    __int64 high;
} patomic_msvc128_t;

PATOMIC_STATIC_ASSERT(msvc128_size, (sizeof(patomic_msvc128_t) == 16u));


/**
 * @addtogroup impl.msvc
 *
 * @brief
 *   Implement shift-left operator for patomic_msvc128_t.
 *
 * @pre
 *   offset >= 0 && offset < 128
 */
static patomic_msvc128_t
patomic_msvc128_shift_left(
    const patomic_msvc128_t obj,
    int offset
)
{
    const unsigned __int64 obj_low = (unsigned __int64) obj.low;
    const unsigned __int64 obj_high = (unsigned __int64) obj.high;
    patomic_msvc128_t res = {0};

    if (offset < 64)
    {
        res.low = (__int64) (obj_low << offset);
        res.high = (__int64) ((obj_high << offset) | (obj_low >> (64 - offset)));
    }
    else
    {
        res.low = 0;
        res.high = (__int64) (obj_low << (offset - 64));
    }

    return res;
}


/**
 * @addtogroup impl.msvc
 *
 * @brief
 *   Implement addition operator for patomic_msvc128_t.
 */
static patomic_msvc128_t
patomic_msvc128_add(
    const patomic_msvc128_t lhs,
    const patomic_msvc128_t rhs
)
{
    const unsigned __int64 lhs_low = (unsigned __int64) lhs.low;
    const unsigned __int64 lhs_high = (unsigned __int64) lhs.high;
    const unsigned __int64 rhs_low = (unsigned __int64) rhs.low;
    const unsigned __int64 rhs_high = (unsigned __int64) rhs.high;
    patomic_msvc128_t res = {0};

    res.low = (__int64) (lhs_low + rhs_low);
    unsigned __int64 carry = (((unsigned __int64) res.low) < lhs_low) ? 1 : 0;
    res.high = (__int64) (lhs_high + rhs_high + carry);

    return res;
}


#endif  /* ^^ defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_BASE_M128_H */
