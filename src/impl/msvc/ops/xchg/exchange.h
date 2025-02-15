/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_XCHG_EXCHANGE_H
#define PATOMIC_IMPL_MSVC_OPS_XCHG_EXCHANGE_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/wrapped/cmpxchg.h>
#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#define do_exchange_explicit_n(                                   \
    n, type, obj, des, order, res                                 \
)                                                                 \
    do {                                                          \
        switch (order)                                            \
        {                                                         \
            case patomic_RELAXED:                                 \
      VIS_IL_NF((res = _InterlockedExchange##n##_nf(obj, des));)  \
      VIS_IL_NF(break;)                                           \
            case patomic_CONSUME:                                 \
            case patomic_ACQUIRE:                                 \
     VIS_IL_ACQ((res = _InterlockedExchange##n##_acq(obj, des));) \
     VIS_IL_ACQ(break;)                                           \
            case patomic_RELEASE:                                 \
     VIS_IL_REL((res = _InterlockedExchange##n##_rel(obj, des));) \
     VIS_IL_REL(break;)                                           \
            case patomic_ACQ_REL:                                 \
            case patomic_SEQ_CST:                                 \
            default:                                              \
                res = _InterlockedExchange##n(obj, des);          \
        }                                                         \
    }                                                             \
    while (0)


/**
 * Defines patomic_opimpl_exchange_8_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_8

#define PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_8 1

char _InterlockedExchange8(char volatile *, char);
#pragma intrinsic(_InterlockedExchange8)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    char _InterlockedExchange8_nf(char volatile *, char);
    #pragma intrinsic(_InterlockedExchange8_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    char _InterlockedExchange8_acq(char volatile *, char);
    char _InterlockedExchange8_rel(char volatile *, char);
    #pragma intrinsic(_InterlockedExchange8_acq)
    #pragma intrinsic(_InterlockedExchange8_rel)
#endif

#define do_exchange_explicit_8(type, obj, des, order, res) \
    do_exchange_explicit_n(8, type, obj, des, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
    char, char, patomic_opimpl_exchange_8_explicit,
    SHOW_P, order, do_exchange_explicit_8
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
    char, char, patomic_opimpl_exchange_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_exchange_explicit_8
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        char, char, patomic_opimpl_exchange_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_exchange_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        char, char, patomic_opimpl_exchange_8_release,
        HIDE_P, patomic_RELEASE, do_exchange_explicit_8
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        char, char, patomic_opimpl_exchange_8_relaxed,
        HIDE_P, patomic_RELAXED, do_exchange_explicit_8
    )
#endif

#elif defined(do_cmpxchg_explicit_8)

#define PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_8 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
    char, char, patomic_opimpl_exchange_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
    char, char, patomic_opimpl_exchange_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        char, char, patomic_opimpl_exchange_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        char, char, patomic_opimpl_exchange_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        char, char, patomic_opimpl_exchange_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8
    )
#endif

#endif /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_8 */


/**
 * Defines patomic_opimpl_exchange_16_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_16

#define PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_16 1

short _InterlockedExchange16(short volatile *, short);
#pragma intrinsic(_InterlockedExchange16)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    short _InterlockedExchange16_nf(short volatile *, short);
    #pragma intrinsic(_InterlockedExchange16_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    short _InterlockedExchange16_acq(short volatile *, short);
    short _InterlockedExchange16_rel(short volatile *, short);
    #pragma intrinsic(_InterlockedExchange16_acq)
    #pragma intrinsic(_InterlockedExchange16_rel)
#endif

#define do_exchange_explicit_16(type, obj, des, order, res) \
    do_exchange_explicit_n(16, type, obj, des, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
    short, short, patomic_opimpl_exchange_16_explicit,
    SHOW_P, order, do_exchange_explicit_16
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
    short, short, patomic_opimpl_exchange_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_exchange_explicit_16
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        short, short, patomic_opimpl_exchange_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_exchange_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        short, short, patomic_opimpl_exchange_16_release,
        HIDE_P, patomic_RELEASE, do_exchange_explicit_16
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        short, short, patomic_opimpl_exchange_16_relaxed,
        HIDE_P, patomic_RELAXED, do_exchange_explicit_16
    )
#endif

#elif defined(do_cmpxchg_explicit_16)

#define PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_16 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
    short, short, patomic_opimpl_exchange_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
    short, short, patomic_opimpl_exchange_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        short, short, patomic_opimpl_exchange_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        short, short, patomic_opimpl_exchange_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        short, short, patomic_opimpl_exchange_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16
    )
#endif

#endif /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_16 */


/**
 * Defines patomic_opimpl_exchange_32_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_32

#define PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_32 1

long _InterlockedExchange(long volatile *, long);
#pragma intrinsic(_InterlockedExchange)
#define _InterlockedExchange32 _InterlockedExchange

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    long _InterlockedExchange_nf(long volatile *, long);
    #pragma intrinsic(_InterlockedExchange_nf)
    #define _InterlockedExchange32_nf _InterlockedExchange_nf
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    long _InterlockedExchange_acq(long volatile *, long);
    long _InterlockedExchange_rel(long volatile *, long);
    #pragma intrinsic(_InterlockedExchange_acq)
    #pragma intrinsic(_InterlockedExchange_rel)
    #define _InterlockedExchange32_acq _InterlockedExchange_acq
    #define _InterlockedExchange32_rel _InterlockedExchange_rel
#endif

#define do_exchange_explicit_32(type, obj, des, order, res) \
    do_exchange_explicit_n(32, type, obj, des, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
    long, long, patomic_opimpl_exchange_32_explicit,
    SHOW_P, order, do_exchange_explicit_32
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
    long, long, patomic_opimpl_exchange_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_exchange_explicit_32
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        long, long, patomic_opimpl_exchange_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_exchange_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        long, long, patomic_opimpl_exchange_32_release,
        HIDE_P, patomic_RELEASE, do_exchange_explicit_32
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        long, long, patomic_opimpl_exchange_32_relaxed,
        HIDE_P, patomic_RELAXED, do_exchange_explicit_32
    )
#endif

#elif defined(do_cmpxchg_explicit_32)

#define PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_32 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
    long, long, patomic_opimpl_exchange_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
    long, long, patomic_opimpl_exchange_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        long, long, patomic_opimpl_exchange_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        long, long, patomic_opimpl_exchange_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        long, long, patomic_opimpl_exchange_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32
    )
#endif

#endif /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_32 */


/**
 * Defines patomic_opimpl_exchange_64_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_64

#define PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_64 1

__int64 _InterlockedExchange64(__int64 volatile *, __int64);
#pragma intrinsic(_InterlockedExchange64)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    __int64 _InterlockedExchange64_nf(__int64 volatile *, __int64);
    #pragma intrinsic(_InterlockedExchange64_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    __int64 _InterlockedExchange64_acq(__int64 volatile *, __int64);
    __int64 _InterlockedExchange64_rel(__int64 volatile *, __int64);
    #pragma intrinsic(_InterlockedExchange64_acq)
    #pragma intrinsic(_InterlockedExchange64_rel)
#endif

#define do_exchange_explicit_64(type, obj, des, order, res) \
    do_exchange_explicit_n(64, type, obj, des, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
    __int64, __int64, patomic_opimpl_exchange_64_explicit,
    SHOW_P, order, do_exchange_explicit_64
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
    __int64, __int64, patomic_opimpl_exchange_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_exchange_explicit_64
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        __int64, __int64, patomic_opimpl_exchange_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_exchange_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        __int64, __int64, patomic_opimpl_exchange_64_release,
        HIDE_P, patomic_RELEASE, do_exchange_explicit_64
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(
        __int64, __int64, patomic_opimpl_exchange_64_relaxed,
        HIDE_P, patomic_RELAXED, do_exchange_explicit_64
    )
#endif

#elif defined(do_cmpxchg_explicit_64)

#define PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_64 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
    __int64, __int64, patomic_opimpl_exchange_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
    __int64, __int64, patomic_opimpl_exchange_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        __int64, __int64, patomic_opimpl_exchange_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        __int64, __int64, patomic_opimpl_exchange_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        __int64, __int64, patomic_opimpl_exchange_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64
    )
#endif

#endif /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_64 */


/**
 * Defines patomic_opimpl_exchange_128_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if defined(do_cmpxchg_explicit_128)

#define PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_128 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_exchange_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_exchange_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_exchange_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_exchange_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_exchange_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128
    )
#endif

#endif  /* defined(do_cmpxchg_explicit_128) */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_XCHG_EXCHANGE_H */
