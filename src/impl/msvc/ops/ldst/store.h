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


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_LDST_STORE_H */
