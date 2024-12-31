/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_LDST_LOAD_H
#define PATOMIC_IMPL_MSVC_OPS_LDST_LOAD_H

#define _MSC_VER 2000
#define _M_ARM64
#define __int64 long long

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#if PATOMIC_IMPL_MSVC_HAS_ISO_VOLATILE_LDST
    #define do_volatile_load_8(obj)  __iso_volatile_load8(obj)
    #define do_volatile_load_16(obj) __iso_volatile_load16(obj)
    #define do_volatile_load_32(obj) __iso_volatile_load32(obj)
    #define do_volatile_load_64(obj) __iso_volatile_load64(obj)
#else
    #define do_volatile_load_8(obj)  (* (const volatile __int8  *) (obj)
    #define do_volatile_load_16(obj) (* (const volatile __int16 *) (obj)
    #define do_volatile_load_32(obj) (* (const volatile __int32 *) (obj)
    #define do_volatile_load_64(obj) (* (const volatile __int64 *) (obj)
#endif  /* PATOMIC_IMPL_MSVC_HAS_ISO_VOLATILE_LDST */


#if defined(_M_ARM) || defined(_M_ARM64)
#if PATOMIC_IMPL_MSVC_HAS_MEMORY_BARRIER_DMB

#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_8 1
#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_16 1
#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_32 1
#if defined(_M_ARM)
#if PATOMIC_IMPL_MSVC_HAS_LDREXD
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
                res = do_volatile_load_##n(obj);     \
                __dmb(0xB);                          \
                break;                               \
            case patomic_RELEASE:                    \
            case patomic_ACQ_REL:                    \
            case patomic_SEQ_CST:                    \
            default:                                 \
                __dmb(0xB);                          \
                res = do_volatile_load_##n(obj);     \
                __dmb(0xB);                          \
        }                                            \
    }                                                \
    while (0)

#endif  /* PATOMIC_IMPL_MSVC_HAS_MEMORY_BARRIER_DMB */
#endif  /* defined(_M_ARM) || defined(_M_ARM64) */


#if defined(_M_IX86) || defined(_M_X64)
#if PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_32
#if PATOMIC_IMPL_MSVC_HAS_COMPILER_READ_WRITE_BARRIER

#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_8 1
#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_16 1
#define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_32 1
#if defined(_M_X64)
    #define PATOMIC_IMPL_MSVC_HAS_IL_LOAD_64 1
#endif

#define do_load_explicit_n(n, type, obj, order, res) \
    do {                                             \
        /* intentionally uninitialized */            \
        volatile long _guard;                        \
        switch (order)                               \
        {                                            \
            case patomic_RELAXED:                    \
                res = do_volatile_load_##n(obj);     \
                break;                               \
            case patomic_CONSUME:                    \
            case patomic_ACQUIRE:                    \
                res = do_volatile_load_##n(obj);     \
                _ReadWriteBarrier();                 \
                break;                               \
            case patomic_RELEASE:                    \
            case patomic_ACQ_REL:                    \
            case patomic_SEQ_CST:                    \
            default:                                 \
                _ReadWriteBarrier();                 \
                res = do_volatile_load_##n(obj);     \
                PATOMIC_IGNORE_UNUSED(               \
                    _InterlockedIncrement(&_guard)   \
                );                                   \
        }                                            \
    }                                                \
    while (0)

#endif  /* PATOMIC_IMPL_MSVC_HAS_COMPILER_READ_WRITE_BARRIER */
#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_32 */
#endif  /* defined(_M_IX86) || defined(_M_X64) */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_LDST_LOAD_H */
