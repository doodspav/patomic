/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_XCHG_H
#define PATOMIC_IMPL_MSVC_OPS_XCHG_H

#define _M_ARM64
#define _MSC_VER 2000
#ifdef _MSC_VER

#include "base.h"

#include <patomic/wrapped/direct.h>
#include <patomic/wrapped/cmpxchg.h>

#include <patomic/api/ops.h>

#include <stddef.h>


#define do_cmpxchg_explicit_n(n, type, obj, exp, des, succ, fail, ok)         \
    do {                                                                      \
        type res;                                                             \
        switch (succ)                                                         \
        {                                                                     \
            case patomic_RELAXED:                                             \
      VIS_IL_NF((res = _InterlockedCompareExchange##n##_nf(obj, des, exp));)  \
      VIS_IL_NF(break;)                                                       \
            case patomic_CONSUME:                                             \
            case patomic_ACQUIRE:                                             \
     VIS_IL_ACQ((res = _InterlockedCompareExchange##n##_acq(obj, des, exp));) \
     VIS_IL_ACQ(break;)                                                       \
            case patomic_RELEASE:                                             \
     VIS_IL_REL((res = _InterlockedCompareExchange##n##_rel(obj, des, exp));) \
     VIS_IL_REL(break;)                                                       \
            case patomic_ACQ_REL:                                             \
            case patomic_SEQ_CST:                                             \
            default:                                                          \
                res = _InterlockedCompareExchange##n(obj, des, exp);          \
        }                                                                     \
        if (res == exp)                                                       \
        {                                                                     \
            ok = 1;                                                           \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            exp = res;                                                        \
            ok = 0;                                                           \
        }                                                                     \
    }                                                                         \
    while (0)


/**
 * Defines patomic_opimpl_cmpxchg_8_<order> (possibly as NULL) with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_8

char _InterlockedCompareExchange8(char volatile *, char, char);
#pragma intrinsic(_InterlockedCompareExchange8)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    char _InterlockedCompareExchange8_nf(char volatile *, char, char);
    #pragma intrinsic(_InterlockedCompareExchange8_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    char _InterlockedCompareExchange8_acq(char volatile *, char, char);
    char _InterlockedCompareExchange8_rel(char volatile *, char, char);
    #pragma intrinsic(_InterlockedCompareExchange8_acq)
    #pragma intrinsic(_InterlockedCompareExchange8_rel)
#endif

#define do_cmpxchg_explicit_8(type, obj, exp, des, succ, fail, ok) \
    do_cmpxchg_explicit_n(8, type, obj, exp, des, succ, fail, ok)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(
    char, char, patomic_opimpl_cmpxchg_8_explicit,
    SHOW_P, HIDE, order, do_cmpxchg_explicit_8
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(
    char, char, patomic_opimpl_cmpxchg_8_seq_cst,
    HIDE_P, SHOW, patomic_SEQ_CST, do_cmpxchg_explicit_8
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(
        char, char, patomic_opimpl_cmpxchg_8_acquire,
        HIDE_P, SHOW, patomic_ACQUIRE, do_cmpxchg_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(
        char, char, patomic_opimpl_cmpxchg_8_release,
        HIDE_P, SHOW, patomic_RELEASE, do_cmpxchg_explicit_8
    )
#else
    #define patomic_opimpl_cmpxchg_8_acquire patomic_opimpl_cmpxchg_8_seq_cst
    #define patomic_opimpl_cmpxchg_8_release patomic_opimpl_cmpxchg_8_seq_cst
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(
        char, char, patomic_opimpl_cmpxchg_8_relaxed,
        HIDE_P, SHOW, patomic_RELAXED, do_cmpxchg_explicit_8
    )
#else
    #define patomic_opimpl_cmpxchg_8_relaxed patomic_opimpl_cmpxchg_8_acquire
#endif

#else  /* ^^ PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_8 vv */

#define patomic_opimpl_cmpxchg_8_explicit NULL
#define patomic_opimpl_cmpxchg_8_relaxed NULL
#define patomic_opimpl_cmpxchg_8_acquire NULL
#define patomic_opimpl_cmpxchg_8_release NULL
#define patomic_opimpl_cmpxchg_8_seq_cst NULL

#endif  /* ^^ PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_8 */


/**
 * Defines patomic_opimpl_cmpxchg_16_<order> (possibly as NULL) with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_16

short _InterlockedCompareExchange16(short volatile *, short, short);
#pragma intrinsic(_InterlockedCompareExchange16)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    short _InterlockedCompareExchange16_nf(short volatile *, short, short);
    #pragma intrinsic(_InterlockedCompareExchange16_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    short _InterlockedCompareExchange16_acq(short volatile *, short, short);
    short _InterlockedCompareExchange16_rel(short volatile *, short, short);
    #pragma intrinsic(_InterlockedCompareExchange16_acq)
    #pragma intrinsic(_InterlockedCompareExchange16_rel)
#endif

#define do_cmpxchg_explicit_16(type, obj, exp, des, succ, fail, ok) \
    do_cmpxchg_explicit_n(16, type, obj, exp, des, succ, fail, ok)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(
    short, short, patomic_opimpl_cmpxchg_16_explicit,
    SHOW_P, HIDE, order, do_cmpxchg_explicit_16
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(
    short, short, patomic_opimpl_cmpxchg_16_seq_cst,
    HIDE_P, SHOW, patomic_SEQ_CST, do_cmpxchg_explicit_16
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(
        short, short, patomic_opimpl_cmpxchg_16_acquire,
        HIDE_P, SHOW, patomic_ACQUIRE, do_cmpxchg_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(
        short, short, patomic_opimpl_cmpxchg_16_release,
        HIDE_P, SHOW, patomic_RELEASE, do_cmpxchg_explicit_16
    )
#else
    #define patomic_opimpl_cmpxchg_16_acquire patomic_opimpl_cmpxchg_16_seq_cst
    #define patomic_opimpl_cmpxchg_16_release patomic_opimpl_cmpxchg_16_seq_cst
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(
        short, short, patomic_opimpl_cmpxchg_16_relaxed,
        HIDE_P, SHOW, patomic_RELAXED, do_cmpxchg_explicit_16
    )
#else
    #define patomic_opimpl_cmpxchg_16_relaxed patomic_opimpl_cmpxchg_16_acquire
#endif

#else  /* ^^ PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_16 vv */

#define patomic_opimpl_cmpxchg_16_explicit NULL
#define patomic_opimpl_cmpxchg_16_relaxed NULL
#define patomic_opimpl_cmpxchg_16_acquire NULL
#define patomic_opimpl_cmpxchg_16_release NULL
#define patomic_opimpl_cmpxchg_16_seq_cst NULL

#endif  /* ^^ PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_16 */


#endif  /* ^^ defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_XCHG_H */
