/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_ADD_H
#define PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_ADD_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/cmpxchg.h>
#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#define do_fetch_add_explicit_n(n, type, obj, arg, order, res)       \
    do {                                                             \
        switch (order)                                               \
        {                                                            \
            case patomic_RELAXED:                                    \
      VIS_IL_NF((res = _InterlockedExchangeAdd##n##_nf(obj, arg));)  \
      VIS_IL_NF(break;)                                              \
            case patomic_CONSUME:                                    \
            case patomic_ACQUIRE:                                    \
     VIS_IL_ACQ((res = _InterlockedExchangeAdd##n##_acq(obj, arg));) \
     VIS_IL_ACQ(break;)                                              \
            case patomic_RELEASE:                                    \
     VIS_IL_REL((res = _InterlockedExchangeAdd##n##_rel(obj, arg));) \
     VIS_IL_REL(break;)                                              \
            case patomic_ACQ_REL:                                    \
            case patomic_SEQ_CST:                                    \
            default:                                                 \
                res = _InterlockedExchangeAdd##n(obj, arg);          \
        }                                                            \
    }                                                                \
    while (0)

#define do_void_add_explicit_n(n, type, obj, arg, order)        \
    do {                                                        \
        type res;                                               \
        do_fetch_add_explicit_n(n, type, obj, arg, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                             \
    }                                                           \
    while (0)

#define do_make_desired_add(type, exp, arg, des)                      \
    do {                                                              \
        des = (type) (((unsigned type) exp) + ((unsigned type) arg)); \
    }                                                                 \
    while (0)

#define do_make_desired_add_128(type, exp, arg, des) \
    do {                                             \
        des = patomic_msvc128_add(exp, arg);         \
    }                                                \
    while (0)
    
    
/**
 * Defines patomic_opimpl_{void, fetch}_add_8_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_8

#define PATOMIC_IMPL_MSVC_HAS_OP_ADD_8 1

char _InterlockedExchangeAdd8(char volatile *, char);
#pragma intrinsic(_InterlockedExchangeAdd8)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    char _InterlockedExchangeAdd8_nf(char volatile *, char);
    #pragma intrinsic(_InterlockedExchangeAdd8_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    char _InterlockedExchangeAdd8_acq(char volatile *, char);
    char _InterlockedExchangeAdd8_rel(char volatile *, char);
    #pragma intrinsic(_InterlockedExchangeAdd8_acq)
    #pragma intrinsic(_InterlockedExchangeAdd8_rel)
#endif

#define do_void_add_explicit_8(type, obj, arg, order) \
    do_void_add_explicit_n(8, type, obj, arg, order)

#define do_fetch_add_explicit_8(type, obj, arg, order, res) \
    do_fetch_add_explicit_n(8, type, obj, arg, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_add_8_explicit,
    SHOW_P, order, do_void_add_explicit_8
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_add_8_explicit,
    SHOW_P, order, do_fetch_add_explicit_8
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_add_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_add_explicit_8
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_add_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_add_explicit_8
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_add_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_add_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_add_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_add_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_add_8_release,
        HIDE_P, patomic_RELEASE, do_void_add_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_add_8_release,
        HIDE_P, patomic_RELEASE, do_fetch_add_explicit_8
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_add_8_relaxed,
        HIDE_P, patomic_RELAXED, do_void_add_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_add_8_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_add_explicit_8
    )
#endif

#elif defined(do_cmpxchg_explicit_8)

#define PATOMIC_IMPL_MSVC_HAS_OP_ADD_8 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_add_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_add
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_add_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_add
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    char, char, patomic_opimpl_void_add_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_add
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    char, char, patomic_opimpl_fetch_add_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_add
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_add_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_add_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_add_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_add_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_add
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        char, char, patomic_opimpl_void_add_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        char, char, patomic_opimpl_fetch_add_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_add
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_8 */


/**
 * Defines patomic_opimpl_{void, fetch}_add_16_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_16

#define PATOMIC_IMPL_MSVC_HAS_OP_ADD_16 1

short _InterlockedExchangeAdd16(short volatile *, short);
#pragma intrinsic(_InterlockedExchangeAdd16)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    short _InterlockedExchangeAdd16_nf(short volatile *, short);
    #pragma intrinsic(_InterlockedExchangeAdd16_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    short _InterlockedExchangeAdd16_acq(short volatile *, short);
    short _InterlockedExchangeAdd16_rel(short volatile *, short);
    #pragma intrinsic(_InterlockedExchangeAdd16_acq)
    #pragma intrinsic(_InterlockedExchangeAdd16_rel)
#endif

#define do_void_add_explicit_16(type, obj, arg, order) \
    do_void_add_explicit_n(16, type, obj, arg, order)

#define do_fetch_add_explicit_16(type, obj, arg, order, res) \
    do_fetch_add_explicit_n(16, type, obj, arg, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    short, short, patomic_opimpl_void_add_16_explicit,
    SHOW_P, order, do_void_add_explicit_16
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    short, short, patomic_opimpl_fetch_add_16_explicit,
    SHOW_P, order, do_fetch_add_explicit_16
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    short, short, patomic_opimpl_void_add_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_add_explicit_16
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    short, short, patomic_opimpl_fetch_add_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_add_explicit_16
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_add_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_add_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_add_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_add_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_add_16_release,
        HIDE_P, patomic_RELEASE, do_void_add_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_add_16_release,
        HIDE_P, patomic_RELEASE, do_fetch_add_explicit_16
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_add_16_relaxed,
        HIDE_P, patomic_RELAXED, do_void_add_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_add_16_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_add_explicit_16
    )
#endif

#elif defined(do_cmpxchg_explicit_16)

#define PATOMIC_IMPL_MSVC_HAS_OP_ADD_16 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    short, short, patomic_opimpl_void_add_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_add
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    short, short, patomic_opimpl_fetch_add_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_add
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    short, short, patomic_opimpl_void_add_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_add
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    short, short, patomic_opimpl_fetch_add_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_add
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_add_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_add_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_add_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_add_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_add
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        short, short, patomic_opimpl_void_add_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        short, short, patomic_opimpl_fetch_add_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_add
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_16 */


/**
 * Defines patomic_opimpl_{void, fetch}_add_32_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_32

#define PATOMIC_IMPL_MSVC_HAS_OP_ADD_32 1

long _InterlockedExchangeAdd(long volatile *, long);
#pragma intrinsic(_InterlockedExchangeAdd)
#define _InterlockedExchangeAdd32 _InterlockedExchangeAdd

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    long _InterlockedExchangeAdd_nf(long volatile *, long);
    #pragma intrinsic(_InterlockedExchangeAdd_nf)
    #define _InterlockedExchangeAdd32_nf _InterlockedExchangeAdd_nf
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    long _InterlockedExchangeAdd_acq(long volatile *, long);
    long _InterlockedExchangeAdd_rel(long volatile *, long);
    #pragma intrinsic(_InterlockedExchangeAdd_acq)
    #pragma intrinsic(_InterlockedExchangeAdd_rel)
    #define _InterlockedExchangeAdd32_acq _InterlockedExchangeAdd_acq
    #define _InterlockedExchangeAdd32_rel _InterlockedExchangeAdd_rel
#endif

#define do_void_add_explicit_32(type, obj, arg, order) \
    do_void_add_explicit_n(32, type, obj, arg, order)

#define do_fetch_add_explicit_32(type, obj, arg, order, res) \
    do_fetch_add_explicit_n(32, type, obj, arg, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    long, long, patomic_opimpl_void_add_32_explicit,
    SHOW_P, order, do_void_add_explicit_32
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    long, long, patomic_opimpl_fetch_add_32_explicit,
    SHOW_P, order, do_fetch_add_explicit_32
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    long, long, patomic_opimpl_void_add_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_add_explicit_32
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    long, long, patomic_opimpl_fetch_add_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_add_explicit_32
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_add_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_add_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_add_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_add_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_add_32_release,
        HIDE_P, patomic_RELEASE, do_void_add_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_add_32_release,
        HIDE_P, patomic_RELEASE, do_fetch_add_explicit_32
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_add_32_relaxed,
        HIDE_P, patomic_RELAXED, do_void_add_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_add_32_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_add_explicit_32
    )
#endif

#elif defined(do_cmpxchg_explicit_32)

#define PATOMIC_IMPL_MSVC_HAS_OP_ADD_32 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    long, long, patomic_opimpl_void_add_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_add
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    long, long, patomic_opimpl_fetch_add_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_add
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    long, long, patomic_opimpl_void_add_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_add
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    long, long, patomic_opimpl_fetch_add_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_add
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_add_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_add_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_add_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_add_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_add
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        long, long, patomic_opimpl_void_add_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        long, long, patomic_opimpl_fetch_add_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_add
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_32 */


/**
 * Defines patomic_opimpl_{void, fetch}_add_64_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_64

#define PATOMIC_IMPL_MSVC_HAS_OP_ADD_64 1

__int64 _InterlockedExchangeAdd64(__int64 volatile *, __int64);
#pragma intrinsic(_InterlockedExchangeAdd64)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    __int64 _InterlockedExchangeAdd64_nf(__int64 volatile *, __int64);
    #pragma intrinsic(_InterlockedExchangeAdd64_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    __int64 _InterlockedExchangeAdd64_acq(__int64 volatile *, __int64);
    __int64 _InterlockedExchangeAdd64_rel(__int64 volatile *, __int64);
    #pragma intrinsic(_InterlockedExchangeAdd64_acq)
    #pragma intrinsic(_InterlockedExchangeAdd64_rel)
#endif

#define do_void_add_explicit_64(type, obj, arg, order) \
    do_void_add_explicit_n(64, type, obj, arg, order)

#define do_fetch_add_explicit_64(type, obj, arg, order, res) \
    do_fetch_add_explicit_n(64, type, obj, arg, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    __int64, __int64, patomic_opimpl_void_add_64_explicit,
    SHOW_P, order, do_void_add_explicit_64
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    __int64, __int64, patomic_opimpl_fetch_add_64_explicit,
    SHOW_P, order, do_fetch_add_explicit_64
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
    __int64, __int64, patomic_opimpl_void_add_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_add_explicit_64
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
    __int64, __int64, patomic_opimpl_fetch_add_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_add_explicit_64
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_add_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_add_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_add_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_add_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_add_64_release,
        HIDE_P, patomic_RELEASE, do_void_add_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_add_64_release,
        HIDE_P, patomic_RELEASE, do_fetch_add_explicit_64
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_add_64_relaxed,
        HIDE_P, patomic_RELAXED, do_void_add_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_add_64_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_add_explicit_64
    )
#endif

#elif defined(do_cmpxchg_explicit_64)

#define PATOMIC_IMPL_MSVC_HAS_OP_ADD_64 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    __int64, __int64, patomic_opimpl_void_add_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_add
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    __int64, __int64, patomic_opimpl_fetch_add_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_add
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    __int64, __int64, patomic_opimpl_void_add_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_add
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    __int64, __int64, patomic_opimpl_fetch_add_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_add
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_add_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_add_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_add_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_add_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_add
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        __int64, __int64, patomic_opimpl_void_add_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_add
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        __int64, __int64, patomic_opimpl_fetch_add_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_add
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_64 */


/**
 * Defines patomic_opimpl_{void, fetch}_add_128_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if defined(do_cmpxchg_explicit_128)

#define PATOMIC_IMPL_MSVC_HAS_OP_ADD_128 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_add_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_add_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_add_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_add_128
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_add_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_add_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_add_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_add_128
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_add_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_add_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_add_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_add_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_add_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_add_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_add_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_add_128
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_add_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_add_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_add_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_add_128
    )
#endif

#endif  /* defined(do_cmpxchg_explicit_128) */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_ADD_H */
