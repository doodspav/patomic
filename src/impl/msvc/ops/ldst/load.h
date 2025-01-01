/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_LDST_LOAD_H
#define PATOMIC_IMPL_MSVC_OPS_LDST_LOAD_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#if PATOMIC_IMPL_MSVC_HAS_ISO_VOLATILE_LDST
    __int8  __iso_volatile_load8 (const volatile __int8  *);
    __int16 __iso_volatile_load16(const volatile __int16 *);
    __int32 __iso_volatile_load32(const volatile __int32 *);
    __int64 __iso_volatile_load64(const volatile __int64 *);
    #pragma intrinsic(__iso_volatile_load8)
    #pragma intrinsic(__iso_volatile_load16)
    #pragma intrinsic(__iso_volatile_load32)
    #pragma intrinsic(__iso_volatile_load64)
    #define do_volatile_load_8(obj)  __iso_volatile_load8(obj)
    #define do_volatile_load_16(obj) __iso_volatile_load16(obj)
    #define do_volatile_load_32(obj) __iso_volatile_load32(obj)
    #define do_volatile_load_64(obj) __iso_volatile_load64(obj)
#else
    #define do_volatile_load_8(obj)  (* (const volatile __int8  *) (obj))
    #define do_volatile_load_16(obj) (* (const volatile __int16 *) (obj))
    #define do_volatile_load_32(obj) (* (const volatile __int32 *) (obj))
    #define do_volatile_load_64(obj) (* (const volatile __int64 *) (obj))
#endif  /* PATOMIC_IMPL_MSVC_HAS_ISO_VOLATILE_LDST */


#if defined(_M_ARM) || defined(_M_ARM64)
#if PATOMIC_IMPL_MSVC_HAS_MEMORY_BARRIER_DMB

void __dmb(unsigned int);
#pragma intrinsic(__dmb)

#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_8 1
#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_16 1
#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_32 1
#if defined(_M_ARM)
#if PATOMIC_IMPL_MSVC_HAS_LDREXD
    __int64 __ldrexd(const volatile __int64 *);
    #pragma intrinsic(__ldrexd)
    #undef do_volatile_load_64
    #define do_volatile_load_64(obj) __ldrexd((const volatile __int64 *) (obj))
    #define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_64 1
#endif
#else  /* defined(_M_ARM64) */
    #define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_64 1
#endif

#define do_load_explicit_n(n, type, obj, order, res) \
    do {                                             \
        switch (order)                               \
        {                                            \
            case patomic_RELAXED:                    \
                res = do_volatile_load_##n(obj);     \
                break;                               \
            case patomic_CONSUME:                    \
            case patomic_ACQUIRE:                    \
            case patomic_RELEASE:                    \
            case patomic_ACQ_REL:                    \
            case patomic_SEQ_CST:                    \
            default:                                 \
                res = do_volatile_load_##n(obj);     \
                __dmb(0xB);                          \
        }                                            \
    }                                                \
    while (0)

#endif  /* PATOMIC_IMPL_MSVC_HAS_MEMORY_BARRIER_DMB */
#endif  /* defined(_M_ARM) || defined(_M_ARM64) */


#if defined(_M_IX86) || defined(_M_X64)
#if PATOMIC_IMPL_MSVC_HAS_COMPILER_READ_WRITE_BARRIER

void _ReadWriteBarrier(void);
#pragma intrinsic(_ReadWriteBarrier)

#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_8 1
#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_16 1
#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_32 1
#if defined(_M_X64)
    #define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_64 1
#endif

#define do_load_explicit_n(n, type, obj, order, res) \
    do {                                             \
        /* intentionally uninitialized */            \
        volatile long guard;                         \
        switch (order)                               \
        {                                            \
            case patomic_RELAXED:                    \
                res = do_volatile_load_##n(obj);     \
                break;                               \
            case patomic_CONSUME:                    \
            case patomic_ACQUIRE:                    \
            case patomic_RELEASE:                    \
            case patomic_ACQ_REL:                    \
            case patomic_SEQ_CST:                    \
            default:                                 \
                _ReadWriteBarrier();                 \
                res = do_volatile_load_##n(obj);     \
                _ReadWriteBarrier();                 \
        }                                            \
    }                                                \
    while (0)

#endif  /* PATOMIC_IMPL_MSVC_HAS_COMPILER_READ_WRITE_BARRIER */
#endif  /* defined(_M_IX86) || defined(_M_X64) */


/**
 * Defines patomic_opimpl_load_8_<order> with order:
 * - relaxed
 * - acquire
 * - seq_cst
 * - explicit
 *
 * There is no release variant.
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_LOAD_8

#define PATOMIC_IMPL_MSVC_HAS_OP_LOAD_8 1

#define do_load_explicit_8(type, obj, order, res) \
    do_load_explicit_n(8, type, obj, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
    __int8, __int8, patomic_opimpl_load_8_explicit,
    SHOW_P, order, do_load_explicit_8
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
    __int8, __int8, patomic_opimpl_load_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_load_explicit_8
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
        __int8, __int8, patomic_opimpl_load_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_load_explicit_8
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
        __int8, __int8, patomic_opimpl_load_8_relaxed,
        HIDE_P, patomic_RELAXED, do_load_explicit_8
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_LOAD_8 */


/**
 * Defines patomic_opimpl_load_16_<order> with order:
 * - relaxed
 * - acquire
 * - seq_cst
 * - explicit
 *
 * There is no release variant.
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_LOAD_16

#define PATOMIC_IMPL_MSVC_HAS_OP_LOAD_16 1

#define do_load_explicit_16(type, obj, order, res) \
    do_load_explicit_n(16, type, obj, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
    __int16, __int16, patomic_opimpl_load_16_explicit,
    SHOW_P, order, do_load_explicit_16
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
    __int16, __int16, patomic_opimpl_load_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_load_explicit_16
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
        __int16, __int16, patomic_opimpl_load_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_load_explicit_16
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
        __int16, __int16, patomic_opimpl_load_16_relaxed,
        HIDE_P, patomic_RELAXED, do_load_explicit_16
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_LOAD_16 */


/**
 * Defines patomic_opimpl_load_32_<order> with order:
 * - relaxed
 * - acquire
 * - seq_cst
 * - explicit
 *
 * There is no release variant.
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_LOAD_32

#define PATOMIC_IMPL_MSVC_HAS_OP_LOAD_32 1

#define do_load_explicit_32(type, obj, order, res) \
    do_load_explicit_n(32, type, obj, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
    __int32, __int32, patomic_opimpl_load_32_explicit,
    SHOW_P, order, do_load_explicit_32
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
    __int32, __int32, patomic_opimpl_load_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_load_explicit_32
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
        __int32, __int32, patomic_opimpl_load_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_load_explicit_32
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
        __int32, __int32, patomic_opimpl_load_32_relaxed,
        HIDE_P, patomic_RELAXED, do_load_explicit_32
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_LOAD_32 */


/**
 * Defines patomic_opimpl_load_64_<order> with order:
 * - relaxed
 * - acquire
 * - seq_cst
 * - explicit
 *
 * There is no release variant.
 */
#if PATOMIC_IMPL_MSVC_HAS_IL_LOAD_64

#define PATOMIC_IMPL_MSVC_HAS_OP_LOAD_64 1

#define do_load_explicit_64(type, obj, order, res) \
    do_load_explicit_n(64, type, obj, order, res)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
    __int64, __int64, patomic_opimpl_load_64_explicit,
    SHOW_P, order, do_load_explicit_64
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
    __int64, __int64, patomic_opimpl_load_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_load_explicit_64
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
        __int64, __int64, patomic_opimpl_load_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_load_explicit_64
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(
        __int64, __int64, patomic_opimpl_load_64_relaxed,
        HIDE_P, patomic_RELAXED, do_load_explicit_64
    )
#endif

#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_LOAD_64 */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_LDST_LOAD_H */
