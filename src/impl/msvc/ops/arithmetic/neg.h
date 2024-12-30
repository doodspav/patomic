/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_NEG_H
#define PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_NEG_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/wrapped/cmpxchg.h>

#include <patomic/api/ops.h>


#define do_make_desired_neg(type, exp, des)                          \
    do {                                                             \
        des = (type) (~((unsigned type) exp) + ((unsigned type) 1)); \
    }                                                                \
    while (0)

#define do_make_desired_neg_128(type, exp, des)              \
    do {                                                     \
        patomic_msvc128_t arg = {0};                         \
        arg.low = 1;                                         \
        des.low = (__int64) ~((unsigned __int64) exp.low);   \
        des.high = (__int64) ~((unsigned __int64) exp.high); \
        des = patomic_msvc128_add(des, arg);                 \
    }                                                        \
    while (0)


/**
 * Defines patomic_opimpl_{void, fetch}_neg_8_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_8

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_neg_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_neg
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_neg_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_neg
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_neg_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_neg
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_neg_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_neg
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_neg_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_neg_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_neg_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_neg_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_neg
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_neg_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_neg_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_neg
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_8 */


/**
 * Defines patomic_opimpl_{void, fetch}_neg_16_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_16

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_neg_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_neg
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_neg_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_neg
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_neg_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_neg
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_neg_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_neg
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_neg_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_neg_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_neg_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_neg_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_neg
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_neg_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_neg_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_neg
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_16 */


/**
 * Defines patomic_opimpl_{void, fetch}_neg_32_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_32

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_neg_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_neg
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_neg_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_neg
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_neg_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_neg
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_neg_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_neg
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_neg_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_neg_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_neg_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_neg_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_neg
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_neg_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_neg_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_neg
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_32 */


/**
 * Defines patomic_opimpl_{void, fetch}_neg_64_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_64

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_neg_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_neg
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_neg_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_neg
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_neg_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_neg
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_neg_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_neg
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_neg_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_neg_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_neg_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_neg_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_neg
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_neg_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_neg
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_neg_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_neg
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_64 */


/**
 * Defines patomic_opimpl_{void, fetch}_neg_128_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_128

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_neg_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_neg_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_neg_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_neg_128
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_neg_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_neg_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_neg_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_neg_128
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_neg_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_neg_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_neg_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_neg_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_neg_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_neg_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_neg_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_neg_128
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_neg_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_neg_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_neg_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_neg_128
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_128 */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_NEG_H */
