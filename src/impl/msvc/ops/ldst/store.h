/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_LDST_STORE_H
#define PATOMIC_IMPL_MSVC_OPS_LDST_STORE_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#if PATOMIC_IMPL_MSVC_HAS_ISO_VOLATILE_LDST
    void __iso_volatile_store8 (const volatile __int8  *, __int8 );
    void __iso_volatile_store16(const volatile __int16 *, __int16);
    void __iso_volatile_store32(const volatile __int32 *, __int32);
    void __iso_volatile_store64(const volatile __int64 *, __int64);
    #pragma intrinsic(__iso_volatile_store8)
    #pragma intrinsic(__iso_volatile_store16)
    #pragma intrinsic(__iso_volatile_store32)
    #pragma intrinsic(__iso_volatile_store64)
    #define do_volatile_store_8(obj, des)   __iso_volatile_store8(obj, des)
    #define do_volatile_store_16(obj, des)  __iso_volatile_store16(obj, des)
    #define do_volatile_store_32(obj, des)  __iso_volatile_store32(obj, des)
    #define do_volatile_store_64(obj, des)  __iso_volatile_store64(obj, des)
#else
    #define do_volatile_store_8(obj, des) \
        ((void) (* (volatile __int8  *) obj = (__int8)  des))
    #define do_volatile_store_16(obj, des) \
        ((void) (* (volatile __int16 *) obj = (__int16) des))
    #define do_volatile_store_32(obj, des) \
        ((void) (* (volatile __int32 *) obj = (__int32) des))
    #define do_volatile_store_64(obj, des) \
        ((void) (* (volatile __int64 *) obj = (__int64) des))
#endif  /* PATOMIC_IMPL_MSVC_HAS_ISO_VOLATILE_LDST */


#if defined(_M_ARM) || defined(_M_ARM64)
#if PATOMIC_IMPL_MSVC_HAS_MEMORY_BARRIER_DMB

void __dmb(unsigned int);
#pragma intrinsic(__dmb)

#define PATOMIC_IMPL_MSVC_HAS_IL_STORE_8 1
#define PATOMIC_IMPL_MSVC_HAS_IL_STORE_16 1
#define PATOMIC_IMPL_MSVC_HAS_IL_STORE_32 1
#if defined(_M_ARM64)
    #define PATOMIC_IMPL_MSVC_HAS_IL_STORE_64 1
#endif

#define do_store_raw_explicit_n(n, type, obj, des, order) \
    do {                                                  \
        switch (order)                                    \
        {                                                 \
            case patomic_RELAXED:                         \
                do_volatile_store_##n(obj, des);          \
                break;                                    \
            case patomic_RELEASE:                         \
                __dmb(0xB);                               \
                do_volatile_store_##n(obj, des);          \
                break;                                    \
            case patomic_CONSUME:                         \
            case patomic_ACQUIRE:                         \
            case patomic_ACQ_REL:                         \
            case patomic_SEQ_CST:                         \
            default:                                      \
                __dmb(0xB);                               \
                do_volatile_store_##n(obj, des);          \
                __dmb(0xB);                               \
        }                                                 \
    }                                                     \
    while (0)

#endif  /* PATOMIC_IMPL_MSVC_HAS_MEMORY_BARRIER_DMB */
#endif  /* defined(_M_ARM) || defined(_M_ARM64) */


#if defined(_M_IX86) || defined(_M_X64)
#if PATOMIC_IMPL_MSVC_HAS_COMPILER_READ_WRITE_BARRIER

void _ReadWriteBarrier(void);
#pragma intrinsic(_ReadWriteBarrier)

#define PATOMIC_IMPL_MSVC_HAS_IL_STORE_8 1
#define PATOMIC_IMPL_MSVC_HAS_IL_STORE_16 1
#define PATOMIC_IMPL_MSVC_HAS_IL_STORE_32 1
#define PATOMIC_IMPL_MSVC_HAS_IL_STORE_64 1

#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_8
    char _InterlockedExchange8(char volatile *, char);
    #pragma intrinsic(_InterlockedExchange8)
    #define do_store_seq_cst_8(obj, des) \
        PATOMIC_IGNORE_UNUSED(_InterlockedExchange8(obj, des))
#elif PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_32
    long _InterlockedIncrement(long volatile *);
    #pragma intrinsic(_InterlockedIncrement)
    #define do_store_seq_cst_8(obj, des) \
        _ReadWriteBarrier();             \
        do_volatile_store_8(obj, des);   \
        PATOMIC_IGNORE_UNUSED(_InterlockedIncrement(&guard))
#else
    #undef PATOMIC_IMPL_MSVC_HAS_IL_STORE_8
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_16
    short _InterlockedExchange16(short volatile *, short);
    #pragma intrinsic(_InterlockedExchange16)
    #define do_store_seq_cst_16(obj, des) \
        PATOMIC_IGNORE_UNUSED(_InterlockedExchange16(obj, des))
#elif PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_32
    long _InterlockedIncrement(long volatile *);
    #pragma intrinsic(_InterlockedIncrement)
    #define do_store_seq_cst_16(obj, des) \
        _ReadWriteBarrier();              \
        do_volatile_store_16(obj, des);   \
        PATOMIC_IGNORE_UNUSED(_InterlockedIncrement(&guard))
#else
    #undef PATOMIC_IMPL_MSVC_HAS_IL_STORE_16
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_32
    long _InterlockedExchange(long volatile *, long);
    #pragma intrinsic(_InterlockedExchange)
    #define do_store_seq_cst_32(obj, des) \
        PATOMIC_IGNORE_UNUSED(_InterlockedExchange(obj, des))
#elif PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_32
    long _InterlockedIncrement(long volatile *);
    #pragma intrinsic(_InterlockedIncrement)
    #define do_store_seq_cst_32(obj, des) \
        _ReadWriteBarrier();              \
        do_volatile_store_32(obj, des);   \
        PATOMIC_IGNORE_UNUSED(_InterlockedIncrement(&guard))
#else
    #undef PATOMIC_IMPL_MSVC_HAS_IL_STORE_32
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_64
    __int64 _InterlockedExchange64(__int64 volatile *, __int64);
    #pragma intrinsic(_InterlockedExchange64)
    #define do_store_seq_cst_64(obj, des) \
        PATOMIC_IGNORE_UNUSED(_InterlockedExchange64(obj, des))
#elif PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_32
    long _InterlockedIncrement(long volatile *);
    #pragma intrinsic(_InterlockedIncrement)
    #define do_store_seq_cst_64(obj, des) \
        _ReadWriteBarrier();              \
        do_volatile_store_64(obj, des);   \
        PATOMIC_IGNORE_UNUSED(_InterlockedIncrement(&guard))
#else
    #undef PATOMIC_IMPL_MSVC_HAS_IL_STORE_64
#endif

#define do_store_raw_explicit_n(n, type, obj, des, order) \
    do {                                                  \
        /* intentionally uninitialized */                 \
        volatile long guard;                              \
        switch (order)                                    \
        {                                                 \
            case patomic_RELAXED:                         \
                do_volatile_store_##n(obj, des);          \
                break;                                    \
            case patomic_RELEASE:                         \
                _ReadWriteBarrier();                      \
                do_volatile_store_##n(obj, des);          \
                break;                                    \
            case patomic_CONSUME:                         \
            case patomic_ACQUIRE:                         \
            case patomic_ACQ_REL:                         \
            case patomic_SEQ_CST:                         \
            default:                                      \
                do_store_seq_cst_##n(obj, des);           \
        }                                                 \
        PATOMIC_IGNORE_UNUSED(guard);                     \
    }                                                     \
    while (0)

#endif  /* PATOMIC_IMPL_MSVC_HAS_COMPILER_READ_WRITE_BARRIER */
#endif  /* defined(_M_IX86) || defined(_M_X64) */


/**
 * Defines patomic_opimpl_store_8_<order> with order:
 * - relaxed
 * - release
 * - seq_cst
 * - explicit
 *
 * There is no acquire variant.
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_STORE_8 || \
    PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_8

#define PATOMIC_IMPL_MSVC_HAS_OP_STORE_8 1

#if PATOMIC_IMPL_MSVC_HAS_IL_STORE_8

#define do_store_explicit_8(type, obj, des, order) \
    do_store_raw_explicit_n(8, type, obj, des, order)

#else  /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_8 */

#define do_store_explicit_8(type, obj, des, order)          \
    do {                                                    \
        type res;                                           \
        do_exchange_explicit_8(type, obj, des, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                         \
    }                                                       \
    while (0)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_STORE_8 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
    __int8, __int8, patomic_opimpl_store_8_explicit,
    SHOW_P, order, do_store_explicit_8
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
    __int8, __int8, patomic_opimpl_store_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_store_explicit_8
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
        __int8, __int8, patomic_opimpl_store_8_release,
        HIDE_P, patomic_RELEASE, do_store_explicit_8
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
        __int8, __int8, patomic_opimpl_store_8_relaxed,
        HIDE_P, patomic_RELAXED, do_store_explicit_8
    )
#endif

#elif defined(do_cmpxchg_explicit_8)

#define PATOMIC_IMPL_MSVC_HAS_OP_STORE_8 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
    __int8, __int8, patomic_opimpl_store_8_explicit,
    SHOW_P, order, do_cmpxchg_explicit_8
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
    __int8, __int8, patomic_opimpl_store_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_8
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
        __int8, __int8, patomic_opimpl_store_8_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_8
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
        __int8, __int8, patomic_opimpl_store_8_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_8
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{STORE, EXCHANGE}_8 */


/**
 * Defines patomic_opimpl_store_16_<order> with order:
 * - relaxed
 * - release
 * - seq_cst
 * - explicit
 *
 * There is no acquire variant.
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_STORE_16 || \
    PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_16

#define PATOMIC_IMPL_MSVC_HAS_OP_STORE_16 1

#if PATOMIC_IMPL_MSVC_HAS_IL_STORE_16

#define do_store_explicit_16(type, obj, des, order) \
    do_store_raw_explicit_n(16, type, obj, des, order)

#else  /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_16 */

#define do_store_explicit_16(type, obj, des, order)          \
    do {                                                     \
        type res;                                            \
        do_exchange_explicit_16(type, obj, des, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                          \
    }                                                        \
    while (0)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_STORE_16 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
    __int16, __int16, patomic_opimpl_store_16_explicit,
    SHOW_P, order, do_store_explicit_16
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
    __int16, __int16, patomic_opimpl_store_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_store_explicit_16
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
        __int16, __int16, patomic_opimpl_store_16_release,
        HIDE_P, patomic_RELEASE, do_store_explicit_16
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
        __int16, __int16, patomic_opimpl_store_16_relaxed,
        HIDE_P, patomic_RELAXED, do_store_explicit_16
    )
#endif

#elif defined(do_cmpxchg_explicit_16)

#define PATOMIC_IMPL_MSVC_HAS_OP_STORE_16 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
    __int16, __int16, patomic_opimpl_store_16_explicit,
    SHOW_P, order, do_cmpxchg_explicit_16
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
    __int16, __int16, patomic_opimpl_store_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_16
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
        __int16, __int16, patomic_opimpl_store_16_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_16
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
        __int16, __int16, patomic_opimpl_store_16_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_16
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{STORE, EXCHANGE}_16 */


/**
 * Defines patomic_opimpl_store_32_<order> with order:
 * - relaxed
 * - release
 * - seq_cst
 * - explicit
 *
 * There is no acquire variant.
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_STORE_32 || \
    PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_32

#define PATOMIC_IMPL_MSVC_HAS_OP_STORE_32 1

#if PATOMIC_IMPL_MSVC_HAS_IL_STORE_32

#define do_store_explicit_32(type, obj, des, order) \
    do_store_raw_explicit_n(32, type, obj, des, order)

#else  /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_32 */

#define do_store_explicit_32(type, obj, des, order)          \
    do {                                                     \
        type res;                                            \
        do_exchange_explicit_32(type, obj, des, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                          \
    }                                                        \
    while (0)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_STORE_32 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
    __int32, __int32, patomic_opimpl_store_32_explicit,
    SHOW_P, order, do_store_explicit_32
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
    __int32, __int32, patomic_opimpl_store_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_store_explicit_32
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
        __int32, __int32, patomic_opimpl_store_32_release,
        HIDE_P, patomic_RELEASE, do_store_explicit_32
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
        __int32, __int32, patomic_opimpl_store_32_relaxed,
        HIDE_P, patomic_RELAXED, do_store_explicit_32
    )
#endif

#elif defined(do_cmpxchg_explicit_32)

#define PATOMIC_IMPL_MSVC_HAS_OP_STORE_32 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
    __int32, __int32, patomic_opimpl_store_32_explicit,
    SHOW_P, order, do_cmpxchg_explicit_32
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
    __int32, __int32, patomic_opimpl_store_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_32
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
        __int32, __int32, patomic_opimpl_store_32_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_32
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
        __int32, __int32, patomic_opimpl_store_32_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_32
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{STORE, EXCHANGE}_32 */


/**
 * Defines patomic_opimpl_store_64_<order> with order:
 * - relaxed
 * - release
 * - seq_cst
 * - explicit
 *
 * There is no acquire variant.
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_STORE_64 || \
    PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_64

#define PATOMIC_IMPL_MSVC_HAS_OP_STORE_64 1

#if PATOMIC_IMPL_MSVC_HAS_IL_STORE_64

#define do_store_explicit_64(type, obj, des, order) \
    do_store_raw_explicit_n(64, type, obj, des, order)

#else  /* PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_64 */

#define do_store_explicit_64(type, obj, des, order)          \
    do {                                                     \
        type res;                                            \
        do_exchange_explicit_64(type, obj, des, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                          \
    }                                                        \
    while (0)

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_STORE_64 */

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
    __int64, __int64, patomic_opimpl_store_64_explicit,
    SHOW_P, order, do_store_explicit_64
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
    __int64, __int64, patomic_opimpl_store_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_store_explicit_64
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
        __int64, __int64, patomic_opimpl_store_64_release,
        HIDE_P, patomic_RELEASE, do_store_explicit_64
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(
        __int64, __int64, patomic_opimpl_store_64_relaxed,
        HIDE_P, patomic_RELAXED, do_store_explicit_64
    )
#endif

#elif defined(do_cmpxchg_explicit_64)

#define PATOMIC_IMPL_MSVC_HAS_OP_STORE_64 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
    __int64, __int64, patomic_opimpl_store_64_explicit,
    SHOW_P, order, do_cmpxchg_explicit_64
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
    __int64, __int64, patomic_opimpl_store_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_64
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
        __int64, __int64, patomic_opimpl_store_64_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_64
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
        __int64, __int64, patomic_opimpl_store_64_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_64
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_{STORE, EXCHANGE}_64 */


/**
 * Defines patomic_opimpl_store_128_<order> with order:
 * - relaxed
 * - release
 * - seq_cst
 * - explicit
 *
 * There is no acquire variant.
 */
#if defined(do_cmpxchg_explicit_128)

#define PATOMIC_IMPL_MSVC_HAS_OP_STORE_128 1

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
    __int128, __int128, patomic_opimpl_store_128_explicit,
    SHOW_P, order, do_cmpxchg_explicit_128
)

PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
    __int128, __int128, patomic_opimpl_store_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_cmpxchg_explicit_128
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
        __int128, __int128, patomic_opimpl_store_128_release,
        HIDE_P, patomic_RELEASE, do_cmpxchg_explicit_128
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(
        __int128, __int128, patomic_opimpl_store_128_relaxed,
        HIDE_P, patomic_RELAXED, do_cmpxchg_explicit_128
    )
#endif

#endif  /* defined(do_cmpxchg_explicit_128) */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_LDST_STORE_H */
