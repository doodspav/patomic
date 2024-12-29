/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_M128_H
#define PATOMIC_IMPL_MSVC_OPS_M128_H

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
    unsigned __int64 low;
    unsigned __int64 high;
} patomic_mscv128_t;

PATOMIC_STATIC_ASSERT(mscv128_size, (sizeof(patomic_mscv128_t) == 16u));


/**
 * @addtogroup impl.msvc
 *
 * @brief
 *   Implement shift-left operator for patomic_msvc128_t.
 *
 * @pre
 *   offset >= 0 && offset < 128
 */
static patomic_mscv128_t
patomic_msvc128_shift_left(
    const patomic_mscv128_t obj,
    int offset
)
{
    patomic_mscv128_t res = {0};

    if (offset < 64)
    {
        res.low = obj.low << offset;
        res.high = (obj.high << offset) | (obj.low >> (64 - offset));
    }
    else
    {
        res.low = 0;
        res.high = obj.low << (offset - 64);
    }

    return res;
}


/**
 * @addtogroup impl.msvc
 *
 * @brief
 *   Implement addition operator for patomic_msvc128_t.
 */
static patomic_mscv128_t
patomic_msvc128_add(
    const patomic_mscv128_t lhs,
    const patomic_mscv128_t rhs
)
{
    patomic_mscv128_t res = {0};

    res.low = lhs.low + rhs.low;
    unsigned __int64 carry = (res.low < lhs.low) ? 1 : 0;
    res.high = lhs.high + rhs.high + carry;

    return res;
}


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_M128_H */
