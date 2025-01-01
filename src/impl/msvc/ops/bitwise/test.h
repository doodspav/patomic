/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_BITWISE_TEST_H
#define PATOMIC_IMPL_MSVC_OPS_BITWISE_TEST_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/static_assert.h>

#include <patomic/stdlib/stdint.h>

#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>

#include <limits.h>


/**
 * @note
 *   Doing pointer hacks like this is _technically_ undefined behaviour,
 *   but msvc won't care and I would be surprised if this broke anything.
 *   It shouldn't be affected by CET or equivalent pointer checks.
 *
 * @warning
 *   The value of 'n' represents the width of the internal operation, not the
 *   width of supported inputs.
 */
#define do_bit_test_explicit_n(n, type, obj, offset, order, res)           \
    do {                                                                   \
        PATOMIC_STATIC_ASSERT(char_bit_is_8, CHAR_BIT == 8u);              \
                                                                           \
        const patomic_intptr_unsigned_t addr =                             \
            (patomic_intptr_unsigned_t) obj;                               \
                                                                           \
        /* declarations */                                                 \
        patomic_intptr_unsigned_t new_addr = addr;                         \
        patomic_intptr_unsigned_t addr_mask;                               \
        int new_offset = offset;                                           \
        const __int##n * new_obj;                                          \
        __int##n value;                                                    \
        unsigned __int##n value_mask;                                      \
                                                                           \
        /* align our address to n/CHAR_BIT bytes */                        \
        addr_mask = n##ul / CHAR_BIT;                                      \
        addr_mask -= 1ul;                                                  \
        addr_mask = (patomic_intptr_unsigned_t) ~addr_mask;                \
        new_addr = (patomic_intptr_unsigned_t) (addr & addr_mask);         \
        new_offset += (int) ((addr - new_addr) * CHAR_BIT);                \
                                                                           \
        /* update offset to be less than n */                              \
        new_addr += (patomic_intptr_unsigned_t) (new_offset / n);          \
        new_offset %= n;                                                   \
                                                                           \
        /* load */                                                         \
        new_obj = (const __int##n *) new_addr;                             \
        do_load_explicit_##n(__int##n, new_obj, order, value);             \
                                                                           \
        /* get bit */                                                      \
        value_mask = (unsigned __int##n) 1ul;                              \
        value_mask <<= new_offset;                                         \
        value_mask &= (unsigned __int##n) value;                           \
        res = (value_mask != (unsigned __int##n) 0);                       \
    }                                                                      \
    while (0)


#if defined(do_load_explicit_32)

    #define do_bit_test_explicit(type, obj, offset, order, res) \
        do_bit_test_explicit_n(32, type, obj, offset, order, res)

#elif defined(do_load_explicit_16)

    #define do_bit_test_explicit(type, obj, offset, order, res) \
        do_bit_test_explicit_n(16, type, obj, offset, order, res)

#elif defined(do_load_explicit_8)

    #define do_bit_test_explicit(type, obj, offset, order, res) \
        do_bit_test_explicit_n(8, type, obj, offset, order, res)

#elif defined(do_load_explicit_64)

    #define do_bit_test_explicit(type, obj, offset, order, res) \
        do_bit_test_explicit_n(64, type, obj, offset, order, res)

#endif  /* defined(do_load_explicit_n) */


#if defined(do_bit_test_explicit)


/**
 * Defines patomic_opimpl_bit_test_8_<order> with order:
 * - relaxed
 * - acquire
 * - seq_cst
 * - explicit
 *
 * There is no release variant.
 */

#define PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_8 1

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST(
    __int8, __int8, patomic_opimpl_bit_test_8_explicit,
    SHOW_P, order, do_bit_test_explicit
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST(
    __int8, __int8, patomic_opimpl_bit_test_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_bit_test_explicit
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST(
        __int8, __int8, patomic_opimpl_bit_test_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_bit_test_explicit
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST(
        __int8, __int8, patomic_opimpl_bit_test_8_relaxed,
        HIDE_P, patomic_RELAXED, do_bit_test_explicit
    )
#endif



#endif  /* defined(do_bit_test_explicit) */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_BITWISE_TEST_H */
