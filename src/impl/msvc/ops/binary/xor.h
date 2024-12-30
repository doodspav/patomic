/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_BINARY_XOR_H
#define PATOMIC_IMPL_MSVC_OPS_BINARY_XOR_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/cmpxchg.h>
#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#define do_fetch_xor_explicit_n(n, type, obj, arg, order, res) \
    do {                                                       \
        switch (order)                                         \
        {                                                      \
            case patomic_RELAXED:                              \
      VIS_IL_NF((res = _InterlockedXor##n##_nf(obj, arg));)    \
      VIS_IL_NF(break;)                                        \
            case patomic_CONSUME:                              \
            case patomic_ACQUIRE:                              \
     VIS_IL_ACQ((res = _InterlockedXor##n##_acq(obj, arg));)   \
     VIS_IL_ACQ(break;)                                        \
            case patomic_RELEASE:                              \
     VIS_IL_REL((res = _InterlockedXor##n##_rel(obj, arg));)   \
     VIS_IL_REL(break;)                                        \
            case patomic_ACQ_REL:                              \
            case patomic_SEQ_CST:                              \
            default:                                           \
                res = _InterlockedXor##n(obj, arg);            \
        }                                                      \
    }                                                          \
    while (0)

#define do_void_xor_explicit_n(n, type, obj, arg, order)        \
    do {                                                        \
        type res;                                               \
        do_fetch_xor_explicit_n(n, type, obj, arg, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                             \
    }                                                           \
    while (0)

#define do_make_desired_xor(type, exp, arg, des)                      \
    do {                                                              \
        des = (type) (((unsigned type) exp) | ((unsigned type) arg)); \
    }                                                                 \
    while (0)

#define do_make_desired_xor_128(type, exp, arg, des) \
    do {                                             \
        des = patomic_msvc128_xor(exp, arg);         \
    }                                                \
    while (0)


/**
 * Defines patomic_opimpl_{void, fetch}_xor_8_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_XOR_8

char _InterlockedXor8(char volatile *, char);
#pragma intrinsic(_InterlockedXor8)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    char _InterlockedXor8_nf(char volatile *, char);
    #pragma intrinsic(_InterlockedXor8_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    char _InterlockedXor8_acq(char volatile *, char);
    char _InterlockedXor8_rel(char volatile *, char);
    #pragma intrinsic(_InterlockedXor8_acq)
    #pragma intrinsic(_InterlockedXor8_rel)
#endif

#define do_void_xor_explicit_8(type, obj, arg, order) \
    do_void_xor_explicit_n(8, type, obj, arg, order)

#define do_fetch_xor_explicit_8(type, obj, arg, order, res) \
    do_fetch_xor_explicit_n(8, type, obj, arg, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_xor_8_explicit,
    SHOW_P, order, do_void_xor_explicit_8
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_xor_8_explicit,
    SHOW_P, order, do_fetch_xor_explicit_8
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_xor_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_xor_explicit_8
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_xor_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_xor_explicit_8
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_xor_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_xor_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_xor_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_xor_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_xor_8_release,
        HIDE_P, patomic_RELEASE, do_void_xor_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_xor_8_release,
        HIDE_P, patomic_RELEASE, do_fetch_xor_explicit_8
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_xor_8_relaxed,
        HIDE_P, patomic_RELAXED, do_void_xor_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_xor_8_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_xor_explicit_8
    )
#endif

#elif PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_8

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_xor_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_xor
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_xor_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_xor
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_xor_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_xor
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_xor_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_xor
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_xor_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_xor
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_xor_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_xor
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_xor_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_xor
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_xor_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_xor
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_xor_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_xor
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_xor_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_xor
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_XOR_8 */




#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_BINARY_XOR_H */
