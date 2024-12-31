/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_BINARY_NOT_H
#define PATOMIC_IMPL_MSVC_OPS_BINARY_NOT_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/wrapped/cmpxchg.h>

#include <patomic/api/ops.h>


#define do_make_desired_not(type, exp, des)  \
    do {                                     \
        des = (type) ~((unsigned type) exp); \
    }                                        \
    while (0)

#define do_make_desired_not_128(type, exp, des)              \
    do {                                                     \
        des.low = (__int64) ~((unsigned __int64) exp.low);   \
        des.high = (__int64) ~((unsigned __int64) exp.high); \
    }                                                        \
    while (0)


/**
 * Defines patomic_opimpl_{void, fetch}_not_8_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if defined(do_cmpxchg_explicit_8)

#define PATOMIC_IMPL_MSVC_HAS_OP_NOT_8 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_not_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_not
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_not_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8, do_make_desired_not
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    char, char, patomic_opimpl_void_not_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_not
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    char, char, patomic_opimpl_fetch_not_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8, do_make_desired_not
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_not_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_not_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_8, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_not_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_not_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8, do_make_desired_not
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        char, char, patomic_opimpl_void_not_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        char, char, patomic_opimpl_fetch_not_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8, do_make_desired_not
    )
#endif

#endif  /* defined(do_cmpxchg_explicit_8) */


/**
 * Defines patomic_opimpl_{void, fetch}_not_16_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if defined(do_cmpxchg_explicit_16)

#define PATOMIC_IMPL_MSVC_HAS_OP_NOT_16 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_not_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_not
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_not_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16, do_make_desired_not
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    short, short, patomic_opimpl_void_not_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_not
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    short, short, patomic_opimpl_fetch_not_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16, do_make_desired_not
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_not_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_not_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_16, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_not_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_not_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16, do_make_desired_not
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        short, short, patomic_opimpl_void_not_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        short, short, patomic_opimpl_fetch_not_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16, do_make_desired_not
    )
#endif

#endif  /* defined(do_cmpxchg_explicit_16) */


/**
 * Defines patomic_opimpl_{void, fetch}_not_32_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if defined(do_cmpxchg_explicit_32)

#define PATOMIC_IMPL_MSVC_HAS_OP_NOT_32 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_not_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_not
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_not_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32, do_make_desired_not
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    long, long, patomic_opimpl_void_not_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_not
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    long, long, patomic_opimpl_fetch_not_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32, do_make_desired_not
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_not_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_not_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_32, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_not_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_not_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32, do_make_desired_not
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        long, long, patomic_opimpl_void_not_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        long, long, patomic_opimpl_fetch_not_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32, do_make_desired_not
    )
#endif

#endif  /* defined(do_cmpxchg_explicit_32) */


/**
 * Defines patomic_opimpl_{void, fetch}_not_64_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if defined(do_cmpxchg_explicit_64)

#define PATOMIC_IMPL_MSVC_HAS_OP_NOT_64 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_not_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_not
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_not_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64, do_make_desired_not
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    __int64, __int64, patomic_opimpl_void_not_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_not
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    __int64, __int64, patomic_opimpl_fetch_not_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64, do_make_desired_not
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_not_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_not_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_64, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_not_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_not_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64, do_make_desired_not
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        __int64, __int64, patomic_opimpl_void_not_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_not
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        __int64, __int64, patomic_opimpl_fetch_not_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64, do_make_desired_not
    )
#endif

#endif  /* defined(do_cmpxchg_explicit_64) */


/**
 * Defines patomic_opimpl_{void, fetch}_not_128_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */
#if defined(do_cmpxchg_explicit_128)

#define PATOMIC_IMPL_MSVC_HAS_OP_NOT_128 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_not_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_not_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_not_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128, do_make_desired_not_128
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_not_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_not_128
)
PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_not_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128, do_make_desired_not_128
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_not_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_not_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_not_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_cmpxchg_explicit_128, do_make_desired_not_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_not_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_not_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_not_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128, do_make_desired_not_128
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_void_not_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_not_128
    )
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_fetch_not_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128, do_make_desired_not_128
    )
#endif

#endif  /* defined(do_cmpxchg_explicit_128) */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_BINARY_NOT_H */
