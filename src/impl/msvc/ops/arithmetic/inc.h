/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_INC_H
#define PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_INC_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/cmpxchg.h>
#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#define do_fetch_inc_raw_explicit_n(n, type, obj, order, res)       \
    do {                                                            \
        switch (order)                                              \
        {                                                           \
            case patomic_RELAXED:                                   \
      VIS_IL_NF((res = _InterlockedIncrement##n##_nf(obj));)        \
      VIS_IL_NF(break;)                                             \
            case patomic_CONSUME:                                   \
            case patomic_ACQUIRE:                                   \
     VIS_IL_ACQ((res = _InterlockedIncrement##n##_acq(obj));)       \
     VIS_IL_ACQ(break;)                                             \
            case patomic_RELEASE:                                   \
     VIS_IL_REL((res = _InterlockedIncrement##n##_rel(obj));)       \
     VIS_IL_REL(break;)                                             \
            case patomic_ACQ_REL:                                   \
            case patomic_SEQ_CST:                                   \
            default:                                                \
                res = _InterlockedIncrement##n(obj);                \
        }                                                           \
        res = (type) (((unsigned type) res) - ((unsigned type) 1)); \
    }                                                               \
    while (0)

#define do_void_inc_raw_explicit_n(n, type, obj, order)        \
    do {                                                       \
        type res;                                              \
        do_fetch_inc_raw_explicit_n(n, type, obj, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                            \
    }                                                          \
    while (0)

#define do_fetch_inc_add_explicit_n(n, type, obj, order, res)  \
    do {                                                       \
        const type arg = (type) 1;                             \
        do_fetch_add_explicit_##n(type, obj, arg, order, res); \
    }                                                          \
    while (0)

#define do_void_inc_add_explicit_n(n, type, obj, order)        \
    do {                                                       \
        type res;                                              \
        do_fetch_inc_add_explicit_n(n, type, obj, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                            \
    }                                                          \
    while (0)

#define do_make_desired_inc(type, exp, des)                         \
    do {                                                            \
        des = (type) (((unsigned type) exp) + ((unsigned type) 1)); \
    }                                                               \
    while (0)

#define do_make_desired_inc_128(type, exp, des) \
    do {                                        \
        patomic_msvc128_t arg = {0};            \
        arg.low = 1;                            \
        des = patomic_msvc128_add(exp, arg);    \
    }                                           \
    while (0)


/**
 * Defines patomic_opimpl_{void, fetch}_inc_8_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_8 || \
    defined(do_fetch_add_explicit_8)

#define PATOMIC_IMPL_MSVC_HAS_OP_INC_8 1

#if PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_8

char _InterlockedIncrement8(char volatile *);
#pragma intrinsic(_InterlockedIncrement8)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    char _InterlockedIncrement8_nf(char volatile *);
    #pragma intrinsic(_InterlockedIncrement8_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    char _InterlockedIncrement8_acq(char volatile *);
    char _InterlockedIncrement8_rel(char volatile *);
    #pragma intrinsic(_InterlockedIncrement8_acq)
    #pragma intrinsic(_InterlockedIncrement8_rel)
#endif

#define do_void_inc_explicit_8(type, obj, order) \
    do_void_inc_raw_explicit_n(8, type, obj, order)

#define do_fetch_inc_explicit_8(type, obj, order, res) \
    do_fetch_inc_raw_explicit_n(8, type, obj, order, res)

#else  /* defined(do_fetch_add_explicit_8) */

#define do_void_inc_explicit_8(type, obj, order) \
    do_void_inc_add_explicit_n(8, type, obj, order)

#define do_fetch_inc_explicit_8(type, obj, order, res) \
    do_fetch_inc_add_explicit_n(8, type, obj, order, res)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_8 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_inc_8_explicit,
    SHOW_P, order, do_void_inc_explicit_8
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_inc_8_explicit,
    SHOW_P, order, do_fetch_inc_explicit_8
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_inc_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_inc_explicit_8
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_inc_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_inc_explicit_8
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_inc_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_inc_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_inc_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_inc_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_inc_8_release,
        HIDE_P, patomic_RELEASE, do_void_inc_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_inc_8_release,
        HIDE_P, patomic_RELEASE, do_fetch_inc_explicit_8
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_inc_8_relaxed,
        HIDE_P, patomic_RELAXED, do_void_inc_explicit_8
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_inc_8_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_inc_explicit_8
    )
#endif

#elif defined(do_cmpxchg_explicit_8)

#define PATOMIC_IMPL_MSVC_HAS_OP_INC_8 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_inc_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_inc
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_inc_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_inc
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_inc_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_inc
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_inc_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_inc
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_inc_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_inc_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_inc_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_inc_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_inc
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_inc_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_inc_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_inc
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{INCREMENT, EXCHANGE_ADD}_8 */


/**
 * Defines patomic_opimpl_{void, fetch}_inc_16_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_16 || \
    defined(do_fetch_add_explicit_16)

#define PATOMIC_IMPL_MSVC_HAS_OP_INC_16 1

#if PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_16

short _InterlockedIncrement16(short volatile *);
#pragma intrinsic(_InterlockedIncrement16)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    short _InterlockedIncrement16_nf(short volatile *);
    #pragma intrinsic(_InterlockedIncrement16_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    short _InterlockedIncrement16_acq(short volatile *);
    short _InterlockedIncrement16_rel(short volatile *);
    #pragma intrinsic(_InterlockedIncrement16_acq)
    #pragma intrinsic(_InterlockedIncrement16_rel)
#endif

#define do_void_inc_explicit_16(type, obj, order) \
    do_void_inc_raw_explicit_n(16, type, obj, order)

#define do_fetch_inc_explicit_16(type, obj, order, res) \
    do_fetch_inc_raw_explicit_n(16, type, obj, order, res)

#else  /* defined(do_fetch_add_explicit_16) */

#define do_void_inc_explicit_16(type, obj, order) \
    do_void_inc_add_explicit_n(16, type, obj, order)

#define do_fetch_inc_explicit_16(type, obj, order, res) \
    do_fetch_inc_add_explicit_n(16, type, obj, order, res)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_16 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_inc_16_explicit,
    SHOW_P, order, do_void_inc_explicit_16
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_inc_16_explicit,
    SHOW_P, order, do_fetch_inc_explicit_16
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_inc_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_inc_explicit_16
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_inc_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_inc_explicit_16
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_inc_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_inc_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_inc_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_inc_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_inc_16_release,
        HIDE_P, patomic_RELEASE, do_void_inc_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_inc_16_release,
        HIDE_P, patomic_RELEASE, do_fetch_inc_explicit_16
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_inc_16_relaxed,
        HIDE_P, patomic_RELAXED, do_void_inc_explicit_16
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_inc_16_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_inc_explicit_16
    )
#endif

#elif defined(do_cmpxchg_explicit_16)

#define PATOMIC_IMPL_MSVC_HAS_OP_INC_16 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_inc_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_inc
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_inc_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_inc
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_inc_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_inc
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_inc_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_inc
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_inc_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_inc_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_inc_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_inc_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_inc
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_inc_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_inc_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_inc
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{INCREMENT, EXCHANGE_ADD}_16 */


/**
 * Defines patomic_opimpl_{void, fetch}_inc_32_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_32 || \
    defined(do_fetch_add_explicit_32)

#define PATOMIC_IMPL_MSVC_HAS_OP_INC_32 1

#if PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_32

long _InterlockedIncrement(long volatile *);
#pragma intrinsic(_InterlockedIncrement)
#define _InterlockedIncrement32 _InterlockedIncrement

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    long _InterlockedIncrement_nf(long volatile *);
    #pragma intrinsic(_InterlockedIncrement_nf)
    #define _InterlockedIncrement32_nf _InterlockedIncrement_nf
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    long _InterlockedIncrement_acq(long volatile *);
    long _InterlockedIncrement_rel(long volatile *);
    #pragma intrinsic(_InterlockedIncrement_acq)
    #pragma intrinsic(_InterlockedIncrement_rel)
    #define _InterlockedIncrement32_acq _InterlockedIncrement_acq
    #define _InterlockedIncrement32_rel _InterlockedIncrement_rel
#endif

#define do_void_inc_explicit_32(type, obj, order) \
    do_void_inc_raw_explicit_n(32, type, obj, order)

#define do_fetch_inc_explicit_32(type, obj, order, res) \
    do_fetch_inc_raw_explicit_n(32, type, obj, order, res)

#else  /* defined(do_fetch_add_explicit_32) */

#define do_void_inc_explicit_32(type, obj, order) \
    do_void_inc_add_explicit_n(32, type, obj, order)

#define do_fetch_inc_explicit_32(type, obj, order, res) \
    do_fetch_inc_add_explicit_n(32, type, obj, order, res)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_32 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_inc_32_explicit,
    SHOW_P, order, do_void_inc_explicit_32
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_inc_32_explicit,
    SHOW_P, order, do_fetch_inc_explicit_32
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_inc_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_inc_explicit_32
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_inc_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_inc_explicit_32
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_inc_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_inc_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_inc_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_inc_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_inc_32_release,
        HIDE_P, patomic_RELEASE, do_void_inc_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_inc_32_release,
        HIDE_P, patomic_RELEASE, do_fetch_inc_explicit_32
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_inc_32_relaxed,
        HIDE_P, patomic_RELAXED, do_void_inc_explicit_32
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_inc_32_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_inc_explicit_32
    )
#endif

#elif defined(do_cmpxchg_explicit_32)

#define PATOMIC_IMPL_MSVC_HAS_OP_INC_32 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_inc_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_inc
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_inc_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_inc
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_inc_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_inc
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_inc_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_inc
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_inc_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_inc_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_inc_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_inc_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_inc
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_inc_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_inc_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_inc
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{INCREMENT, EXCHANGE_ADD}_32 */


/**
 * Defines patomic_opimpl_{void, fetch}_inc_64_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_64 || \
    defined(do_fetch_add_explicit_64)

#define PATOMIC_IMPL_MSVC_HAS_OP_INC_64 1

#if PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_64

__int64 _InterlockedIncrement64(__int64 volatile *);
#pragma intrinsic(_InterlockedIncrement64)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    __int64 _InterlockedIncrement64_nf(__int64 volatile *);
    #pragma intrinsic(_InterlockedIncrement64_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    __int64 _InterlockedIncrement64_acq(__int64 volatile *);
    __int64 _InterlockedIncrement64_rel(__int64 volatile *);
    #pragma intrinsic(_InterlockedIncrement64_acq)
    #pragma intrinsic(_InterlockedIncrement64_rel)
#endif

#define do_void_inc_explicit_64(type, obj, order) \
    do_void_inc_raw_explicit_n(64, type, obj, order)

#define do_fetch_inc_explicit_64(type, obj, order, res) \
    do_fetch_inc_raw_explicit_n(64, type, obj, order, res)

#else  /* defined(do_fetch_add_explicit_64) */

#define do_void_inc_explicit_64(type, obj, order) \
    do_void_inc_add_explicit_n(64, type, obj, order)

#define do_fetch_inc_explicit_64(type, obj, order, res) \
    do_fetch_inc_add_explicit_n(64, type, obj, order, res)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_64 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_inc_64_explicit,
    SHOW_P, order, do_void_inc_explicit_64
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_inc_64_explicit,
    SHOW_P, order, do_fetch_inc_explicit_64
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_inc_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_void_inc_explicit_64
)
PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_inc_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_fetch_inc_explicit_64
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_inc_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_void_inc_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_inc_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_fetch_inc_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_inc_64_release,
        HIDE_P, patomic_RELEASE, do_void_inc_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_inc_64_release,
        HIDE_P, patomic_RELEASE, do_fetch_inc_explicit_64
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_inc_64_relaxed,
        HIDE_P, patomic_RELAXED, do_void_inc_explicit_64
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_inc_64_relaxed,
        HIDE_P, patomic_RELAXED, do_fetch_inc_explicit_64
    )
#endif

#elif defined(do_cmpxchg_explicit_64)

#define PATOMIC_IMPL_MSVC_HAS_OP_INC_64 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_inc_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_inc
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_inc_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_inc
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_inc_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_inc
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_inc_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_inc
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_inc_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_inc_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_inc_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_inc_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_inc
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_inc_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_inc
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_inc_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_inc
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{INCREMENT, EXCHANGE_ADD}_64 */


/**
 * Defines patomic_opimpl_{void, fetch}_inc_128_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if defined(do_cmpxchg_explicit_128)

#define PATOMIC_IMPL_MSVC_HAS_OP_INC_128 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_inc_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_inc_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_inc_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_inc_128
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_inc_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_inc_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_inc_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_inc_128
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_inc_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_inc_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_inc_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_inc_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_inc_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_inc_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_inc_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_inc_128
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_inc_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_inc_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_inc_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_inc_128
    )
#endif

#endif  /* defined(do_cmpxchg_explicit_128) */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_INC_H */
