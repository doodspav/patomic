/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_BINARY_OR_H
#define PATOMIC_IMPL_MSVC_OPS_BINARY_OR_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/cmpxchg.h>
#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#define do_fetch_or_explicit_n(n, type, obj, arg, order, res) \
    do {                                                       \
        switch (order)                                         \
        {                                                      \
            case patomic_RELAXED:                              \
      VIS_IL_NF((res = _InterlockedOr##n##_nf(obj, arg));)    \
      VIS_IL_NF(break;)                                        \
            case patomic_CONSUME:                              \
            case patomic_ACQUIRE:                              \
     VIS_IL_ACQ((res = _InterlockedOr##n##_acq(obj, arg));)   \
     VIS_IL_ACQ(break;)                                        \
            case patomic_RELEASE:                              \
     VIS_IL_REL((res = _InterlockedOr##n##_rel(obj, arg));)   \
     VIS_IL_REL(break;)                                        \
            case patomic_ACQ_REL:                              \
            case patomic_SEQ_CST:                              \
            default:                                           \
                res = _InterlockedOr##n(obj, arg);            \
        }                                                      \
    }                                                          \
    while (0)

#define do_void_or_explicit_n(n, type, obj, arg, order)        \
    do {                                                        \
        type res;                                               \
        do_fetch_or_explicit_n(n, type, obj, arg, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                             \
    }                                                           \
    while (0)

#define do_make_desired_or(type, exp, arg, des)                      \
    do {                                                              \
        des = (type) (((unsigned type) exp) | ((unsigned type) arg)); \
    }                                                                 \
    while (0)

#define do_make_desired_or_128(type, exp, arg, des) \
    do {                                             \
        des = patomic_msvc128_or(exp, arg);         \
    }                                                \
    while (0)


/**
 * Defines patomic_opimpl_{void, fetch}_or_8_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_OR_8

char _InterlockedOr8(char volatile *, char);
#pragma intrinsic(_InterlockedOr8)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    char _InterlockedOr8_nf(char volatile *, char);
    #pragma intrinsic(_InterlockedOr8_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    char _InterlockedOr8_acq(char volatile *, char);
    char _InterlockedOr8_rel(char volatile *, char);
    #pragma intrinsic(_InterlockedOr8_acq)
    #pragma intrinsic(_InterlockedOr8_rel)
#endif

#define do_void_or_explicit_8(type, obj, arg, order) \
    do_void_or_explicit_n(8, type, obj, arg, order)

#define do_fetch_or_explicit_8(type, obj, arg, order, res) \
    do_fetch_or_explicit_n(8, type, obj, arg, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_or_8_explicit,
    SHOW_P, order, do_void_or_explicit_8
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_or_8_explicit,
    SHOW_P, order, do_fetch_or_explicit_8
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_or_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_or_explicit_8
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_or_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_or_explicit_8
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_or_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_or_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_or_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_or_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_or_8_release,
        HIDE_P, patomic_RELEASE, do_void_or_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_or_8_release,
        HIDE_P, patomic_RELEASE, do_fetch_or_explicit_8
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_or_8_relaxed,
        HIDE_P, patomic_RELAXED, do_void_or_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_or_8_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_or_explicit_8
    )
#endif

#elif PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_8

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_or_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_or
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_or_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_or
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_or_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_or
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_or_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_or
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_or_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_or_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_or_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_or_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_or
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_or_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_or_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_or
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_OR_8 */


/**
 * Defines patomic_opimpl_{void, fetch}_or_16_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_OR_16

short _InterlockedOr16(short volatile *, short);
#pragma intrinsic(_InterlockedOr16)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    short _InterlockedOr16_nf(short volatile *, short);
    #pragma intrinsic(_InterlockedOr16_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    short _InterlockedOr16_acq(short volatile *, short);
    short _InterlockedOr16_rel(short volatile *, short);
    #pragma intrinsic(_InterlockedOr16_acq)
    #pragma intrinsic(_InterlockedOr16_rel)
#endif

#define do_void_or_explicit_16(type, obj, arg, order) \
    do_void_or_explicit_n(16, type, obj, arg, order)

#define do_fetch_or_explicit_16(type, obj, arg, order, res) \
    do_fetch_or_explicit_n(16, type, obj, arg, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    short, short, patomic_opimpl_void_or_16_explicit,
    SHOW_P, order, do_void_or_explicit_16
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    short, short, patomic_opimpl_fetch_or_16_explicit,
    SHOW_P, order, do_fetch_or_explicit_16
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    short, short, patomic_opimpl_void_or_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_or_explicit_16
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    short, short, patomic_opimpl_fetch_or_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_or_explicit_16
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_or_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_or_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_or_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_or_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_or_16_release,
        HIDE_P, patomic_RELEASE, do_void_or_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_or_16_release,
        HIDE_P, patomic_RELEASE, do_fetch_or_explicit_16
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_or_16_relaxed,
        HIDE_P, patomic_RELAXED, do_void_or_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_or_16_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_or_explicit_16
    )
#endif

#elif PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_16

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    short, short, patomic_opimpl_void_or_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_or
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    short, short, patomic_opimpl_fetch_or_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_or
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    short, short, patomic_opimpl_void_or_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_or
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    short, short, patomic_opimpl_fetch_or_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_or
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_or_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_or_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_or_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_or_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_or
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_or_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_or_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_or
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_OR_16 */


/**
 * Defines patomic_opimpl_{void, fetch}_or_32_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_OR_32

long _InterlockedOr(long volatile *, long);
#pragma intrinsic(_InterlockedOr)
#define _InterlockedOr32 _InterlockedOr

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    long _InterlockedOr_nf(long volatile *, long);
    #pragma intrinsic(_InterlockedOr_nf)
    #define _InterlockedOr32_nf _InterlockedOr_nf
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    long _InterlockedOr_acq(long volatile *, long);
    long _InterlockedOr_rel(long volatile *, long);
    #pragma intrinsic(_InterlockedOr_acq)
    #pragma intrinsic(_InterlockedOr_rel)
    #define _InterlockedOr32_acq _InterlockedOr_acq
    #define _InterlockedOr32_rel _InterlockedOr_rel
#endif

#define do_void_or_explicit_32(type, obj, arg, order) \
    do_void_or_explicit_n(32, type, obj, arg, order)

#define do_fetch_or_explicit_32(type, obj, arg, order, res) \
    do_fetch_or_explicit_n(32, type, obj, arg, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    long, long, patomic_opimpl_void_or_32_explicit,
    SHOW_P, order, do_void_or_explicit_32
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    long, long, patomic_opimpl_fetch_or_32_explicit,
    SHOW_P, order, do_fetch_or_explicit_32
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    long, long, patomic_opimpl_void_or_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_or_explicit_32
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    long, long, patomic_opimpl_fetch_or_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_or_explicit_32
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_or_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_or_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_or_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_or_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_or_32_release,
        HIDE_P, patomic_RELEASE, do_void_or_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_or_32_release,
        HIDE_P, patomic_RELEASE, do_fetch_or_explicit_32
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_or_32_relaxed,
        HIDE_P, patomic_RELAXED, do_void_or_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_or_32_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_or_explicit_32
    )
#endif

#elif PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_32

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    long, long, patomic_opimpl_void_or_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_or
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    long, long, patomic_opimpl_fetch_or_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_or
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    long, long, patomic_opimpl_void_or_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_or
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    long, long, patomic_opimpl_fetch_or_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_or
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_or_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_or_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_or_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_or_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_or
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_or_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_or_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_or
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_OR_32 */


/**
 * Defines patomic_opimpl_{void, fetch}_or_64_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_OR_64

__int64 _InterlockedOr64(__int64 volatile *, __int64);
#pragma intrinsic(_InterlockedOr64)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    __int64 _InterlockedOr64_nf(__int64 volatile *, __int64);
    #pragma intrinsic(_InterlockedOr64_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    __int64 _InterlockedOr64_acq(__int64 volatile *, __int64);
    __int64 _InterlockedOr64_rel(__int64 volatile *, __int64);
    #pragma intrinsic(_InterlockedOr64_acq)
    #pragma intrinsic(_InterlockedOr64_rel)
#endif

#define do_void_or_explicit_64(type, obj, arg, order) \
    do_void_or_explicit_n(64, type, obj, arg, order)

#define do_fetch_or_explicit_64(type, obj, arg, order, res) \
    do_fetch_or_explicit_n(64, type, obj, arg, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    __int64, __int64, patomic_opimpl_void_or_64_explicit,
    SHOW_P, order, do_void_or_explicit_64
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    __int64, __int64, patomic_opimpl_fetch_or_64_explicit,
    SHOW_P, order, do_fetch_or_explicit_64
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    __int64, __int64, patomic_opimpl_void_or_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_or_explicit_64
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    __int64, __int64, patomic_opimpl_fetch_or_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_or_explicit_64
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_or_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_or_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_or_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_or_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_or_64_release,
        HIDE_P, patomic_RELEASE, do_void_or_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_or_64_release,
        HIDE_P, patomic_RELEASE, do_fetch_or_explicit_64
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_or_64_relaxed,
        HIDE_P, patomic_RELAXED, do_void_or_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_or_64_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_or_explicit_64
    )
#endif

#elif PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_64

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    __int64, __int64, patomic_opimpl_void_or_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_or
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    __int64, __int64, patomic_opimpl_fetch_or_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_or
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    __int64, __int64, patomic_opimpl_void_or_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_or
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    __int64, __int64, patomic_opimpl_fetch_or_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_or
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_or_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_or_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_or_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_or_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_or
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_or_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_or
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_or_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_or
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_OR_64 */


/**
 * Defines patomic_opimpl_{void, fetch}_or_128_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_128

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_or_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_or_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_or_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_or_128
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_or_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_or_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_or_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_or_128
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_or_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_or_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_or_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_or_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_or_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_or_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_or_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_or_128
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_or_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_or_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_or_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_or_128
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_OR_128 */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_BINARY_OR_H */
