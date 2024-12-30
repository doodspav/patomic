/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_BASE_CONFIG_H
#define PATOMIC_IMPL_MSVC_OPS_BASE_CONFIG_H

#if _MSC_VER >= 1923


/* compiler barriers */

/** @brief _ReadBarrier intrinsic supported. */
#define PATOMIC_IMPL_MSVC_HAS_COMPILER_READ_BARRIER 1

/** @brief _WriteBarrier intrinsic supported. */
#define PATOMIC_IMPL_MSVC_HAS_COMPILER_WRITE_BARRIER 1

/** @brief _ReadWriteBarrier intrinsic supported. */
#define PATOMIC_IMPL_MSVC_HAS_COMPILER_READ_WRITE_BARRIER 1


/* weak memory targets */

#if defined(_M_ARM) || defined(_M_ARM64)

    /** @brief __iso_volatile intrinsics supported. */
    #define PATOMIC_IMPL_MSVC_HAS_ISO_VOLATILE_LDST 1

    #if !defined(_M_ARM64)

        /** @brief __ldrexd intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_LDREXD 1

    #endif

    /** @brief __dmb(0xb) intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_MEMORY_BARRIER_DMB 1

    /** @brief _acq and _rel _Interlocked variants supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL 1

    /** @brief _nf _Interlocked variant supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_NF 1

#endif


/* _Interlocked intrinsics */

#if defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM) || defined(_M_ARM64)

    /* add */
    #if defined (_M_ARM) || defined (_M_ARM64)

        /** @brief _InterlockedAdd intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_ADD_32 1

        /** @brief _InterlockedAdd64 intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_ADD_64 1

    #endif

    /* and */

    /** @brief _InterlockedAnd8 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_AND_8 1

    /** @brief _InterlockedAnd16 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_AND_16 1

    /** @brief _InterlockedAnd intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_AND_32 1

    #if !defined(_M_IX86)

        /** @brief _InterlockedAnd64 intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_AND_64 1

    #endif

    /* bit test */

    /** @brief _interlockedbittestreset intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_BIT_TEST_RESET_32 1

    /** @brief _interlockedbittestset intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_BIT_TEST_SET_32 1

    #if defined(_M_X64) || defined(_M_ARM64)

        /** @brief _interlockedbittestreset64 intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_BIT_TEST_RESET_64 1

        /** @brief _interlockedbittestset64 intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_BIT_TEST_SET_64 1

    #endif

    /* compare exchange */

    /** @brief _InterlockedCompareExchange8 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_8 1

    /** @brief _InterlockedCompareExchange16 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_16 1

    /** @brief _InterlockedCompareExchange intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_32 1

    /** @brief _InterlockedCompareExchange64 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_64 1

    #if defined(_M_X64) || defined(_M_ARM64)

        /** @brief _InterlockedCompareExchange128 intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_COMPARE_EXCHANGE_128 1

    #endif

    /* decrement */

    /** @brief _InterlockedDecrement16 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_16 1

    /** @brief _InterlockedDecrement intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_32 1

    #if !defined(_M_IX86)

        /** @brief _InterlockedDecrement64 intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_DECREMENT_64 1

    #endif

    /* exchange */

    /** @brief _InterlockedExchange8 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_8 1

    /** @brief _InterlockedExchange16 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_16 1

    /** @brief _InterlockedExchange intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_32 1

    #if !defined(_M_IX86)

        /** @brief _InterlockedExchange64 intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_64 1

    #endif

    /* exchange add */

    /** @brief _InterlockedExchangeAdd8 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_8 1

    /** @brief _InterlockedExchangeAdd16 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_16 1

    /** @brief _InterlockedExchangeAdd intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_32 1

    #if !defined(_M_IX86)

        /** @brief _InterlockedExchangeAdd64 intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_EXCHANGE_ADD_64 1

    #endif

    /* increment */

    /** @brief _InterlockedIncrement16 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_16 1

    /** @brief _InterlockedIncrement intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_32 1

    #if !defined(_M_IX86)

        /** @brief _InterlockedIncrement64 intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_INCREMENT_64 1

    #endif

    /* or */

    /** @brief _InterlockedOr8 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_OR_8 1

    /** @brief _InterlockedOr16 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_OR_16 1

    /** @brief _InterlockedOr intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_OR_32 1

    #if !defined(_M_IX86)

        /** @brief _InterlockedOr64 intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_OR_64 1

    #endif

    /* xor */

    /** @brief _InterlockedXor8 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_XOR_8 1

    /** @brief _InterlockedXor16 intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_XOR_16 1

    /** @brief _InterlockedXor intrinsic supported. */
    #define PATOMIC_IMPL_MSVC_HAS_IL_XOR_32 1

    #if !defined(_M_IX86)

        /** @brief _InterlockedXor64 intrinsic supported. */
        #define PATOMIC_IMPL_MSVC_HAS_IL_XOR_64 1

    #endif

#endif


#endif  /* ^^ _MSC_VER >= 1923 */

#endif  /* PATOMIC_IMPL_MSVC_OPS_BASE_CONFIG_H */
