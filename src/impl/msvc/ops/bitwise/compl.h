/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_BITWISE_COMPL_H
#define PATOMIC_IMPL_MSVC_OPS_BITWISE_COMPL_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/wrapped/cmpxchg.h>

#include <patomic/api/ops.h>


#define do_make_desired_compl(type, exp, offset, des)                  \
    do {                                                               \
        const unsigned type mask =                                     \
            (unsigned type) ((unsigned type) 1 << offset);             \
        des = (type) ((unsigned type) (((unsigned type) exp) ^ mask)); \
    }                                                                  \
    while (0)

#define do_make_desired_compl_128(type, exp, offset, des) \
    do {                                                  \
        type mask = {0};                                  \
        mask.low = 1;                                     \
        mask = patomic_msvc128_shift_left(mask, offset);  \
        des = patomic_msvc128_xor(exp, mask);             \
    }                                                     \
    while (0)

#define do_get_bit(type, exp, offset, bit)                 \
    do {                                                   \
        const unsigned type mask =                         \
            (unsigned type) ((unsigned type) 1 << offset); \
        bit = (((unsigned type) exp) & mask) != 0;         \
    }                                                      \
    while (0)

#define do_get_bit_128(type, exp, offset, bit)           \
    do {                                                 \
        type mask = {0};                                 \
        mask.low = 1;                                    \
        mask = patomic_msvc128_shift_left(mask, offset); \
        mask = patomic_msvc128_and(mask, exp);           \
        bit = (mask.low != 0 || mask.high != 0);         \
    }                                                    \
    while (0)


/**
 * Defines patomic_opimpl_bit_test_compl_8_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_8

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
    char, char, patomic_opimpl_bit_test_compl_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8,
    do_get_bit, do_make_desired_compl
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
    char, char, patomic_opimpl_bit_test_compl_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8,
    do_get_bit, do_make_desired_compl
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        char, char, patomic_opimpl_bit_test_compl_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8,
        do_get_bit, do_make_desired_compl
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        char, char, patomic_opimpl_bit_test_compl_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8,
        do_get_bit, do_make_desired_compl
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        char, char, patomic_opimpl_bit_test_compl_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8,
        do_get_bit, do_make_desired_compl
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_8 */


/**
 * Defines patomic_opimpl_bit_test_compl_16_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_16

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
    short, short, patomic_opimpl_bit_test_compl_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16,
    do_get_bit, do_make_desired_compl
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
    short, short, patomic_opimpl_bit_test_compl_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16,
    do_get_bit, do_make_desired_compl
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        short, short, patomic_opimpl_bit_test_compl_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16,
        do_get_bit, do_make_desired_compl
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        short, short, patomic_opimpl_bit_test_compl_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16,
        do_get_bit, do_make_desired_compl
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        short, short, patomic_opimpl_bit_test_compl_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16,
        do_get_bit, do_make_desired_compl
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_16 */


/**
 * Defines patomic_opimpl_bit_test_compl_32_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_32

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
    long, long, patomic_opimpl_bit_test_compl_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32,
    do_get_bit, do_make_desired_compl
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
    long, long, patomic_opimpl_bit_test_compl_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32,
    do_get_bit, do_make_desired_compl
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        long, long, patomic_opimpl_bit_test_compl_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32,
        do_get_bit, do_make_desired_compl
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        long, long, patomic_opimpl_bit_test_compl_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32,
        do_get_bit, do_make_desired_compl
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        long, long, patomic_opimpl_bit_test_compl_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32,
        do_get_bit, do_make_desired_compl
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_32 */


/**
 * Defines patomic_opimpl_bit_test_compl_64_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_64

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
    __int64, __int64, patomic_opimpl_bit_test_compl_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64,
    do_get_bit, do_make_desired_compl
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
    __int64, __int64, patomic_opimpl_bit_test_compl_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64,
    do_get_bit, do_make_desired_compl
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        __int64, __int64, patomic_opimpl_bit_test_compl_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64,
        do_get_bit, do_make_desired_compl
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        __int64, __int64, patomic_opimpl_bit_test_compl_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64,
        do_get_bit, do_make_desired_compl
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        __int64, __int64, patomic_opimpl_bit_test_compl_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64,
        do_get_bit, do_make_desired_compl
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_64 */


/**
 * Defines patomic_opimpl_bit_test_compl_128_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_128

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_bit_test_compl_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128,
    do_get_bit_128, do_make_desired_compl_128
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_bit_test_compl_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128,
    do_get_bit_128, do_make_desired_compl_128
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_bit_test_compl_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128,
        do_get_bit_128, do_make_desired_compl_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_bit_test_compl_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128,
        do_get_bit_128, do_make_desired_compl_128
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_bit_test_compl_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128,
        do_get_bit_128, do_make_desired_compl_128
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_128 */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_BITWISE_COMPL_H */
