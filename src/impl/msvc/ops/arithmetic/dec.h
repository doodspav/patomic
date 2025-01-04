/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_DEC_H
#define PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_DEC_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/cmpxchg.h>
#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#define do_fetch_dec_raw_explicit_n(n, type, obj, order, res)       \
    do {                                                            \
        switch (order)                                              \
        {                                                           \
            case patomic_RELAXED:                                   \
      VIS_IL_NF((res = _InterlockedDecrement##n##_nf(obj));)        \
      VIS_IL_NF(break;)                                             \
            case patomic_CONSUME:                                   \
            case patomic_ACQUIRE:                                   \
     VIS_IL_ACQ((res = _InterlockedDecrement##n##_acq(obj));)       \
     VIS_IL_ACQ(break;)                                             \
            case patomic_RELEASE:                                   \
     VIS_IL_REL((res = _InterlockedDecrement##n##_rel(obj));)       \
     VIS_IL_REL(break;)                                             \
            case patomic_ACQ_REL:                                   \
            case patomic_SEQ_CST:                                   \
            default:                                                \
                res = _InterlockedDecrement##n(obj);                \
        }                                                           \
        res = (type) (((unsigned type) res) + ((unsigned type) 1)); \
    }                                                               \
    while (0)

#define do_void_dec_raw_explicit_n(n, type, obj, order)        \
    do {                                                       \
        type res;                                              \
        do_fetch_dec_raw_explicit_n(n, type, obj, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                            \
    }                                                          \
    while (0)

#define do_fetch_dec_sub_explicit_n(n, type, obj, order, res)  \
    do {                                                       \
        type arg = (type) 1;                                   \
        do_fetch_sub_explicit_##n(type, obj, arg, order, res); \
    }                                                          \
    while (0)

#define do_void_dec_sub_explicit_n(n, type, obj, order)        \
    do {                                                       \
        type res;                                              \
        do_fetch_dec_sub_explicit_n(n, type, obj, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                            \
    }                                                          \
    while (0)

#define do_make_desired_dec(type, exp, des)                         \
    do {                                                            \
        des = (type) (((unsigned type) exp) - ((unsigned type) 1)); \
    }                                                               \
    while (0)

#define do_make_desired_dec_128(type, exp, des) \
    do {                                        \
        patomic_msvc128_t arg = {0};            \
        arg.low = 1;                            \
        des = patomic_msvc128_sub(exp, arg);    \
    }                                           \
    while (0)


/**
 * Defines patomic_opimpl_{void, fetch}_dec_8_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_8 || \
    defined(do_fetch_sub_explicit_8)

#define PATOMIC_IMPL_MSVC_HAS_OP_DEC_8 1

#if PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_8

char _InterlockedDecrement8(char volatile *);
#pragma intrinsic(_InterlockedDecrement8)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    char _InterlockedDecrement8_nf(char volatile *);
    #pragma intrinsic(_InterlockedDecrement8_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    char _InterlockedDecrement8_acq(char volatile *);
    char _InterlockedDecrement8_rel(char volatile *);
    #pragma intrinsic(_InterlockedDecrement8_acq)
    #pragma intrinsic(_InterlockedDecrement8_rel)
#endif

#define do_void_dec_explicit_8(type, obj, order) \
    do_void_dec_raw_explicit_n(8, type, obj, order)

#define do_fetch_dec_explicit_8(type, obj, order, res) \
    do_fetch_dec_raw_explicit_n(8, type, obj, order, res)

#else  /* defined(do_fetch_sub_explicit_8) */

#define do_void_dec_explicit_8(type, obj, order) \
    do_void_dec_sub_explicit_n(8, type, obj, order)

#define do_fetch_dec_explicit_8(type, obj, order, res) \
    do_fetch_dec_sub_explicit_n(8, type, obj, order, res)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_8 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_dec_8_explicit,
    SHOW_P, order, do_void_dec_explicit_8
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_dec_8_explicit,
    SHOW_P, order, do_fetch_dec_explicit_8
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_dec_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_dec_explicit_8
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_dec_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_dec_explicit_8
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_dec_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_dec_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_dec_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_dec_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_dec_8_release,
        HIDE_P, patomic_RELEASE, do_void_dec_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_dec_8_release,
        HIDE_P, patomic_RELEASE, do_fetch_dec_explicit_8
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_dec_8_relaxed,
        HIDE_P, patomic_RELAXED, do_void_dec_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_dec_8_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_dec_explicit_8
    )
#endif

#elif defined(do_cmpxchg_explicit_8)

#define PATOMIC_IMPL_MSVC_HAS_OP_DEC_8 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_dec_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_dec
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_dec_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_dec
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_dec_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_dec
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_dec_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_dec
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_dec_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_dec_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_dec_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_dec_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_dec
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_dec_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_dec_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_dec
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{DECREMENT, EXCHANGE_ADD}_8 */


/**
 * Defines patomic_opimpl_{void, fetch}_dec_16_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_16 || \
    defined(do_fetch_sub_explicit_16)

#define PATOMIC_IMPL_MSVC_HAS_OP_DEC_16 1

#if PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_16

short _InterlockedDecrement16(short volatile *);
#pragma intrinsic(_InterlockedDecrement16)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    short _InterlockedDecrement16_nf(short volatile *);
    #pragma intrinsic(_InterlockedDecrement16_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    short _InterlockedDecrement16_acq(short volatile *);
    short _InterlockedDecrement16_rel(short volatile *);
    #pragma intrinsic(_InterlockedDecrement16_acq)
    #pragma intrinsic(_InterlockedDecrement16_rel)
#endif

#define do_void_dec_explicit_16(type, obj, order) \
    do_void_dec_raw_explicit_n(16, type, obj, order)

#define do_fetch_dec_explicit_16(type, obj, order, res) \
    do_fetch_dec_raw_explicit_n(16, type, obj, order, res)

#else  /* defined(do_fetch_sub_explicit_16) */

#define do_void_dec_explicit_16(type, obj, order) \
    do_void_dec_sub_explicit_n(16, type, obj, order)

#define do_fetch_dec_explicit_16(type, obj, order, res) \
    do_fetch_dec_sub_explicit_n(16, type, obj, order, res)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_16 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_dec_16_explicit,
    SHOW_P, order, do_void_dec_explicit_16
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_dec_16_explicit,
    SHOW_P, order, do_fetch_dec_explicit_16
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_dec_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_dec_explicit_16
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_dec_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_dec_explicit_16
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_dec_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_dec_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_dec_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_dec_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_dec_16_release,
        HIDE_P, patomic_RELEASE, do_void_dec_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_dec_16_release,
        HIDE_P, patomic_RELEASE, do_fetch_dec_explicit_16
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_dec_16_relaxed,
        HIDE_P, patomic_RELAXED, do_void_dec_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_dec_16_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_dec_explicit_16
    )
#endif

#elif defined(do_cmpxchg_explicit_16)

#define PATOMIC_IMPL_MSVC_HAS_OP_DEC_16 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_dec_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_dec
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_dec_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_dec
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_dec_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_dec
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_dec_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_dec
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_dec_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_dec_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_dec_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_dec_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_dec
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_dec_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_dec_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_dec
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{DECREMENT, EXCHANGE_ADD}_16 */


/**
 * Defines patomic_opimpl_{void, fetch}_dec_32_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_32 || \
    defined(do_fetch_sub_explicit_32)

#define PATOMIC_IMPL_MSVC_HAS_OP_DEC_32 1

#if PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_32

long _InterlockedDecrement(long volatile *);
#pragma intrinsic(_InterlockedDecrement)
#define _InterlockedDecrement32 _InterlockedDecrement

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    long _InterlockedDecrement_nf(long volatile *);
    #pragma intrinsic(_InterlockedDecrement_nf)
    #define _InterlockedDecrement32_nf _InterlockedDecrement_nf
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    long _InterlockedDecrement_acq(long volatile *);
    long _InterlockedDecrement_rel(long volatile *);
    #pragma intrinsic(_InterlockedDecrement_acq)
    #pragma intrinsic(_InterlockedDecrement_rel)
    #define _InterlockedDecrement32_acq _InterlockedDecrement_acq
    #define _InterlockedDecrement32_rel _InterlockedDecrement_rel
#endif

#define do_void_dec_explicit_32(type, obj, order) \
    do_void_dec_raw_explicit_n(32, type, obj, order)

#define do_fetch_dec_explicit_32(type, obj, order, res) \
    do_fetch_dec_raw_explicit_n(32, type, obj, order, res)

#else  /* defined(do_fetch_sub_explicit_32) */

#define do_void_dec_explicit_32(type, obj, order) \
    do_void_dec_sub_explicit_n(32, type, obj, order)

#define do_fetch_dec_explicit_32(type, obj, order, res) \
    do_fetch_dec_sub_explicit_n(32, type, obj, order, res)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_32 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_dec_32_explicit,
    SHOW_P, order, do_void_dec_explicit_32
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_dec_32_explicit,
    SHOW_P, order, do_fetch_dec_explicit_32
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_dec_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_dec_explicit_32
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_dec_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_dec_explicit_32
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_dec_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_dec_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_dec_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_dec_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_dec_32_release,
        HIDE_P, patomic_RELEASE, do_void_dec_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_dec_32_release,
        HIDE_P, patomic_RELEASE, do_fetch_dec_explicit_32
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_dec_32_relaxed,
        HIDE_P, patomic_RELAXED, do_void_dec_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_dec_32_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_dec_explicit_32
    )
#endif

#elif defined(do_cmpxchg_explicit_32)

#define PATOMIC_IMPL_MSVC_HAS_OP_DEC_32 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_dec_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_dec
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_dec_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_dec
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_dec_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_dec
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_dec_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_dec
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_dec_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_dec_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_dec_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_dec_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_dec
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_dec_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_dec_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_dec
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{DECREMENT, EXCHANGE_ADD}_32 */


/**
 * Defines patomic_opimpl_{void, fetch}_dec_64_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_64 || \
    defined(do_fetch_sub_explicit_64)

#define PATOMIC_IMPL_MSVC_HAS_OP_DEC_64 1

#if PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_64

__int64 _InterlockedDecrement64(__int64 volatile *);
#pragma intrinsic(_InterlockedDecrement64)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    __int64 _InterlockedDecrement64_nf(__int64 volatile *);
    #pragma intrinsic(_InterlockedDecrement64_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    __int64 _InterlockedDecrement64_acq(__int64 volatile *);
    __int64 _InterlockedDecrement64_rel(__int64 volatile *);
    #pragma intrinsic(_InterlockedDecrement64_acq)
    #pragma intrinsic(_InterlockedDecrement64_rel)
#endif

#define do_void_dec_explicit_64(type, obj, order) \
    do_void_dec_raw_explicit_n(64, type, obj, order)

#define do_fetch_dec_explicit_64(type, obj, order, res) \
    do_fetch_dec_raw_explicit_n(64, type, obj, order, res)

#else  /* defined(do_fetch_sub_explicit_64) */

#define do_void_dec_explicit_64(type, obj, order) \
    do_void_dec_sub_explicit_n(64, type, obj, order)

#define do_fetch_dec_explicit_64(type, obj, order, res) \
    do_fetch_dec_sub_explicit_n(64, type, obj, order, res)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_64 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_dec_64_explicit,
    SHOW_P, order, do_void_dec_explicit_64
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_dec_64_explicit,
    SHOW_P, order, do_fetch_dec_explicit_64
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_dec_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_dec_explicit_64
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_dec_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_dec_explicit_64
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_dec_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_dec_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_dec_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_dec_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_dec_64_release,
        HIDE_P, patomic_RELEASE, do_void_dec_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_dec_64_release,
        HIDE_P, patomic_RELEASE, do_fetch_dec_explicit_64
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_dec_64_relaxed,
        HIDE_P, patomic_RELAXED, do_void_dec_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_dec_64_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_dec_explicit_64
    )
#endif

#elif defined(do_cmpxchg_explicit_64)

#define PATOMIC_IMPL_MSVC_HAS_OP_DEC_64 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_dec_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_dec
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_dec_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_dec
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_dec_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_dec
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_dec_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_dec
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_dec_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_dec_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_dec_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_dec_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_dec
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_dec_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_dec
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_dec_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_dec
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{DECREMENT, EXCHANGE_ADD}_64 */


/**
 * Defines patomic_opimpl_{void, fetch}_dec_128_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if defined(do_cmpxchg_explicit_128)

#define PATOMIC_IMPL_MSVC_HAS_OP_DEC_128 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_dec_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_dec_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_dec_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_dec_128
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_dec_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_dec_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_dec_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_dec_128
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_dec_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_dec_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_dec_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_dec_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_dec_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_dec_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_dec_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_dec_128
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_dec_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_dec_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_dec_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_dec_128
    )
#endif

#endif  /* defined(do_cmpxchg_explicit_128) */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_DEC_H */