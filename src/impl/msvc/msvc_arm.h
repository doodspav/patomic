#ifndef PATOMIC_MSVC_ARM_H
#define PATOMIC_MSVC_ARM_H

#if defined(_MSC_VER)
#if defined(_M_ARM) || defined(_M_ARM64)

#include <limits.h>
#include <string.h>

#include <intrin.h>

#include <patomic/patomic.h>
#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>
#include <patomic/macros/static_assert.h>


/*
 * BIT WIDTHS SUPPORTED
 * - 8
 * - 16
 * - 32
 * - 64 (not on _M_ARM)
 */


#define _InterlockedAnd32 _InterlockedAnd
#define _InterlockedCompareExchange32 _InterlockedCompareExchange
#define _InterlockedDecrement32 _InterlockedDecrement
#define _InterlockedExchange32 _InterlockedExchange
#define _InterlockedExchangeAdd32 _InterlockedExchangeAdd
#define _InterlockedIncrement32 _InterlockedIncrement
#define _InterlockedOr32 _InterlockedOr
#define _InterlockedXor32 _InterlockedXor

#define _InterlockedAnd32_acq _InterlockedAnd_acq
#define _InterlockedCompareExchange32_acq _InterlockedCompareExchange_acq
#define _InterlockedDecrement32_acq _InterlockedDecrement_acq
#define _InterlockedExchange32_acq _InterlockedExchange_acq
#define _InterlockedExchangeAdd32_acq _InterlockedExchangeAdd_acq
#define _InterlockedIncrement32_acq _InterlockedIncrement_acq
#define _InterlockedOr32_acq _InterlockedOr_acq
#define _InterlockedXor32_acq _InterlockedXor_acq

#define _InterlockedAnd32_rel _InterlockedAnd_rel
#define _InterlockedCompareExchange32_rel _InterlockedCompareExchange_rel
#define _InterlockedDecrement32_rel _InterlockedDecrement_rel
#define _InterlockedExchange32_rel _InterlockedExchange_rel
#define _InterlockedExchangeAdd32_rel _InterlockedExchangeAdd_rel
#define _InterlockedIncrement32_rel _InterlockedIncrement_rel
#define _InterlockedOr32_rel _InterlockedOr_rel
#define _InterlockedXor32_rel _InterlockedXor_rel

#define _InterlockedAnd32_nf _InterlockedAnd_nf
#define _InterlockedCompareExchange32_nf _InterlockedCompareExchange_nf
#define _InterlockedDecrement32_nf _InterlockedDecrement_nf
#define _InterlockedExchange32_nf _InterlockedExchange_nf
#define _InterlockedExchangeAdd32_nf _InterlockedExchangeAdd_nf
#define _InterlockedIncrement32_nf _InterlockedIncrement_nf
#define _InterlockedOr32_nf _InterlockedOr_nf
#define _InterlockedXor32_nf _InterlockedXor_nf

#define _InterlockedDecrement8(x) 0
#define _InterlockedIncrement8(x) 0

#define _InterlockedDecrement8_acq(x) 0
#define _InterlockedIncrement8_acq(x) 0

#define _InterlockedDecrement8_rel(x) 0
#define _InterlockedIncrement8_rel(x) 0

#define _InterlockedDecrement8_nf(x) 0
#define _InterlockedIncrement8_nf(x) 0

#if defined(_M_ARM)
#define PATOMIC_ARM_RELAXED_LOAD64(x) __ldrexd(                          \
                                          (const volatile __int64 *) (x) \
                                      )
#define PATOMIC_ARM_SEQ_CST_STORE64(x, y) (void) _InterlockedExchange64( \
                                              (volatile __int64 *) (x),  \
                                              (__int64) (y)              \
                                          )
#elif defined(_M_ARM64)
#define PATOMIC_ARM_RELAXED_LOAD64(x) __iso_volatile_load64(             \
                                          (const volatile __int64 *) (x) \
                                      )
    #define PATOMIC_ARM_SEQ_CST_STORE64(x, y) __dmb(0xB);                   \
                                              __iso_volatile_store64(       \
                                                  (volatile __int64 *) (x), \
                                                  (__int64) (y)             \
                                              );                            \
                                              __dmb(0xB)
#else
    #error unknown architecture
#endif


/*
 * IL BASE FUNCTIONS
 * - Load
 * - Store
 * - using guidance from: (for __ldrexd and __dmb placement)
 * >> https://github.com/microsoft/STL/blob/master/stl/inc/atomic
 */

#define PATOMIC_DEFINE_IL_LOAD(iso_type, width)                     \
    static PATOMIC_FORCE_INLINE void                                \
    patomic_il_load_##width(                                        \
        const volatile void *obj,                                   \
        int order,                                                  \
        void *ret                                                   \
    )                                                               \
    {                                                               \
        iso_type val;                                               \
        if (width == 64) {                                          \
            val = (iso_type) PATOMIC_ARM_RELAXED_LOAD64(obj);       \
        } else {                                                    \
            val = (iso_type) __iso_volatile_load##width(            \
                (const volatile iso_type *) obj                     \
            );                                                      \
        }                                                           \
        switch (order)                                              \
        {                                                           \
            case patomic_RELAXED:                                   \
                break;                                              \
            case patomic_CONSUME:                                   \
            case patomic_ACQUIRE:                                   \
            case patomic_RELEASE:                                   \
            case patomic_ACQ_REL:                                   \
            case patomic_SEQ_CST:                                   \
            default:                                                \
                __dmb(0xB);                                         \
        }                                                           \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(iso_type))); \
    }

#define PATOMIC_DEFINE_IL_STORE(type, iso_type, width) \
    static PATOMIC_FORCE_INLINE void                   \
    patomic_il_store_##width(                          \
        volatile void *obj,                            \
        const void *desired,                           \
        int order                                      \
    )                                                  \
    {                                                  \
        switch (order)                                 \
        {                                              \
            case patomic_RELEASE:                      \
                __dmb(0xB);                            \
            case patomic_RELAXED:                      \
                __iso_volatile_store##width(           \
                    (volatile iso_type *) obj,         \
                    *((const iso_type *) desired)      \
                ); break;                              \
            case patomic_CONSUME:                      \
            case patomic_ACQUIRE:                      \
            case patomic_ACQ_REL:                      \
            case patomic_SEQ_CST:                      \
            default:                                   \
                if (width == 64) {                     \
                    PATOMIC_ARM_SEQ_CST_STORE64(       \
                        obj,                           \
                        *((const iso_type *) desired)  \
                    );                                 \
                } else {                               \
                    __dmb(0xB);                        \
                    __iso_volatile_store##width(       \
                        (volatile iso_type *) obj,     \
                        *((const iso_type *) desired)  \
                    );                                 \
                    __dmb(0xB);                        \
                }                                      \
        }                                              \
    }


/*
 * IL FUNCTIONS AVAILABLE (9/9)
 *
 * - full: _acq, _rel, _nf
 * - partial: _acq, _nf
 * - none: not supported
 *
 * - Add (y) [[8, 16, 32, 64]] (full)
 * - And (y) [[8, 16, 32, 64]] (full)
 * - CompareExchange (y) [[8, 16, 32, 64]] (full)
 * - Decrement (y) [[16, 32, 64]] (full)
 * - Exchange (y) [[8, 16, 32, 64]] (full)
 * - ExchangeAdd (y) [[8, 16, 32, 64]] (full)
 * - Increment (y) [[16, 32, 64]] (full)
 * - Or (y) [[8, 16, 32, 64]] (full)
 * - Xor (y) [[8, 16, 32, 64]] (full)
 */

/*
 * Add - not implemented
 * >> can be replaced by ExchangeAdd
 * >> ExchangeAdd returns the original value (which we want)
 * >> Add returns the result value (which we don't want)
 */

/* equivalent to patomic_fetch_and */
#define PATOMIC_DEFINE_IL_AND(type, width)                      \
    static PATOMIC_FORCE_INLINE void                            \
    patomic_il_and_##width(                                     \
        volatile void *obj,                                     \
        const void *arg,                                        \
        int order,                                              \
        void *ret                                               \
    )                                                           \
    {                                                           \
        type val;                                               \
        switch (order)                                          \
        {                                                       \
            case patomic_RELAXED:                               \
                val = (type) _InterlockedAnd##width##_nf(       \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                ); break;                                       \
            case patomic_CONSUME:                               \
            case patomic_ACQUIRE:                               \
                val = (type) _InterlockedAnd##width##_acq(      \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                ); break;                                       \
            case patomic_RELEASE:                               \
                val = (type) _InterlockedAnd##width##_rel(      \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                ); break;                                       \
            case patomic_ACQ_REL:                               \
            case patomic_SEQ_CST:                               \
            default:                                            \
                val = (type) _InterlockedAnd##width(            \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                );                                              \
        }                                                       \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type))); \
    }

/* equivalent to patomic_cmpxchg_strong */
#define PATOMIC_DEFINE_IL_COMPARE_EXCHANGE(type, width)                  \
    static PATOMIC_FORCE_INLINE int                                      \
    patomic_il_compare_exchange_##width(                                 \
        volatile void *obj,                                              \
        void *expected,                                                  \
        const void *desired,                                             \
        int succ,                                                        \
        int fail                                                         \
    )                                                                    \
    {                                                                    \
        type val;                                                        \
        PATOMIC_IGNORE_UNUSED(fail);                                     \
        /* MSVC flips expected and desired order */                      \
        switch (succ)                                                    \
        {                                                                \
            case patomic_RELAXED:                                        \
                val = (type) _InterlockedCompareExchange##width##_nf(    \
                    (volatile type *) obj,                               \
                    *((const type *) desired),                           \
                    *((const type *) expected)                           \
                ); break;                                                \
            case patomic_CONSUME:                                        \
            case patomic_ACQUIRE:                                        \
                val = (type) _InterlockedCompareExchange##width##_acq(   \
                    (volatile type *) obj,                               \
                    *((const type *) desired),                           \
                    *((const type *) expected)                           \
                ); break;                                                \
            case patomic_RELEASE:                                        \
                val = (type) _InterlockedCompareExchange##width##_rel(   \
                    (volatile type *) obj,                               \
                    *((const type *) desired),                           \
                    *((const type *) expected)                           \
                ); break;                                                \
            case patomic_ACQ_REL:                                        \
            case patomic_SEQ_CST:                                        \
            default:                                                     \
                val = (type) _InterlockedCompareExchange##width(         \
                    (volatile type *) obj,                               \
                    *((const type *) desired),                           \
                    *((const type *) expected)                           \
                );                                                       \
        }                                                                \
        if (val == *((const type *) expected)) { return 1; }             \
        else                                                             \
        {                                                                \
            PATOMIC_IGNORE_UNUSED(memcpy(expected, &val, sizeof(type))); \
            return 0;                                                    \
        }                                                                \
    }

/*
 * - equivalent to patomic_fetch_dec
 *
 * WARNING:
 * - msvc's version returns the result after decrementing
 * - we need the value before decrementing
 * - if type is signed, incrementing runs the risk of UB
 * - however there is no alternative, and it should be fine
 */
#define PATOMIC_DEFINE_IL_DECREMENT(type, width)                       \
    static PATOMIC_FORCE_INLINE void                                   \
    patomic_il_decrement_##width(                                      \
        volatile void *obj,                                            \
        int order,                                                     \
        void *ret                                                      \
    )                                                                  \
    {                                                                  \
        type val;                                                      \
        if (width == 8)                                                \
        {                                                              \
            switch (order)                                             \
            {                                                          \
                case patomic_RELAXED:                                  \
                    val = (type) _InterlockedExchangeAdd##width##_nf(  \
                        (volatile type *) obj,                         \
                        (type) -1                                      \
                    ); break;                                          \
                case patomic_CONSUME:                                  \
                case patomic_ACQUIRE:                                  \
                    val = (type) _InterlockedExchangeAdd##width##_acq( \
                        (volatile type *) obj,                         \
                        (type) -1                                      \
                    ); break;                                          \
                case patomic_RELEASE:                                  \
                    val = (type) _InterlockedExchangeAdd##width##_rel( \
                        (volatile type *) obj,                         \
                        (type) -1                                      \
                    ); break;                                          \
                case patomic_ACQ_REL:                                  \
                case patomic_SEQ_CST:                                  \
                default:                                               \
                    val = (type) _InterlockedExchangeAdd##width(       \
                        (volatile type *) obj,                         \
                        (type) -1                                      \
                    );                                                 \
            }                                                          \
        }                                                              \
        else                                                           \
        {                                                              \
            switch (order)                                             \
            {                                                          \
                case patomic_RELAXED:                                  \
                    val = (type) _InterlockedDecrement##width##_nf(    \
                        (volatile type *) obj                          \
                    ); break;                                          \
                case patomic_CONSUME:                                  \
                case patomic_ACQUIRE:                                  \
                    val = (type) _InterlockedDecrement##width##_acq(   \
                        (volatile type *) obj                          \
                    ); break;                                          \
                case patomic_RELEASE:                                  \
                    val = (type) _InterlockedDecrement##width##_rel(   \
                        (volatile type *) obj                          \
                    ); break;                                          \
                case patomic_ACQ_REL:                                  \
                case patomic_SEQ_CST:                                  \
                default:                                               \
                    val = (type) _InterlockedDecrement##width(         \
                        (volatile type *) obj                          \
                    );                                                 \
            }                                                          \
            ++val;                                                     \
        }                                                              \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type)));        \
    }

/* equivalent to patomic_exchange */
#define PATOMIC_DEFINE_IL_EXCHANGE(type, width)                 \
    static PATOMIC_FORCE_INLINE void                            \
    patomic_il_exchange_##width(                                \
        volatile void *obj,                                     \
        const void *desired,                                    \
        int order,                                              \
        void *ret                                               \
    )                                                           \
    {                                                           \
        type val;                                               \
        switch (order)                                          \
        {                                                       \
            case patomic_RELAXED:                               \
                val = (type) _InterlockedExchange##width##_nf(  \
                    (volatile type *) obj,                      \
                    *((const type *) desired)                   \
                ); break;                                       \
            case patomic_CONSUME:                               \
            case patomic_ACQUIRE:                               \
                val = (type) _InterlockedExchange##width##_acq( \
                    (volatile type *) obj,                      \
                    *((const type *) desired)                   \
                ); break;                                       \
            case patomic_RELEASE:                               \
                val = (type) _InterlockedExchange##width##_rel( \
                    (volatile type *) obj,                      \
                    *((const type *) desired)                   \
                ); break;                                       \
            case patomic_ACQ_REL:                               \
            case patomic_SEQ_CST:                               \
            default:                                            \
                val = (type) _InterlockedExchange##width(       \
                    (volatile type *) obj,                      \
                    *((const type *) desired)                   \
                );                                              \
        }                                                       \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type))); \
    }

/* equivalent to patomic_fetch_add */
#define PATOMIC_DEFINE_IL_EXCHANGE_ADD(type, width)                \
    static PATOMIC_FORCE_INLINE void                               \
    patomic_il_exchange_add_##width(                               \
        volatile void *obj,                                        \
        const void *arg,                                           \
        int order,                                                 \
        void *ret                                                  \
    )                                                              \
    {                                                              \
        type val;                                                  \
        switch (order)                                             \
        {                                                          \
            case patomic_RELAXED:                                  \
                val = (type) _InterlockedExchangeAdd##width##_nf(  \
                    (volatile type *) obj,                         \
                    *((const type *) arg)                          \
                ); break;                                          \
            case patomic_CONSUME:                                  \
            case patomic_ACQUIRE:                                  \
                val = (type) _InterlockedExchangeAdd##width##_acq( \
                    (volatile type *) obj,                         \
                    *((const type *) arg)                          \
                ); break;                                          \
            case patomic_RELEASE:                                  \
                val = (type) _InterlockedExchangeAdd##width##_rel( \
                    (volatile type *) obj,                         \
                    *((const type *) arg)                          \
                ); break;                                          \
            case patomic_ACQ_REL:                                  \
            case patomic_SEQ_CST:                                  \
            default:                                               \
                val = (type) _InterlockedExchangeAdd##width(       \
                    (volatile type *) obj,                         \
                    *((const type *) arg)                          \
                );                                                 \
        }                                                          \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type)));    \
    }

/*
 * - equivalent to patomic_fetch_inc
 *
 * WARNING:
 * - msvc's version returns the result after incrementing
 * - we need the value before incrementing
 * - if type is signed, decrementing runs the risk of UB
 * - should be fine
 */
#define PATOMIC_DEFINE_IL_INCREMENT(type, width)                       \
    static PATOMIC_FORCE_INLINE void                                   \
    patomic_il_increment_##width(                                      \
        volatile void *obj,                                            \
        int order,                                                     \
        void *ret                                                      \
    )                                                                  \
    {                                                                  \
        type val;                                                      \
        if (width == 8)                                                \
        {                                                              \
            switch (order)                                             \
            {                                                          \
                case patomic_RELAXED:                                  \
                    val = (type) _InterlockedExchangeAdd##width##_nf(  \
                        (volatile type *) obj,                         \
                        (type) 1                                       \
                    ); break;                                          \
                case patomic_CONSUME:                                  \
                case patomic_ACQUIRE:                                  \
                    val = (type) _InterlockedExchangeAdd##width##_acq( \
                        (volatile type *) obj,                         \
                        (type) 1                                       \
                    ); break;                                          \
                case patomic_RELEASE:                                  \
                    val = (type) _InterlockedExchangeAdd##width##_rel( \
                        (volatile type *) obj,                         \
                        (type) 1                                       \
                    ); break;                                          \
                case patomic_ACQ_REL:                                  \
                case patomic_SEQ_CST:                                  \
                default:                                               \
                    val = (type) _InterlockedExchangeAdd##width(       \
                        (volatile type *) obj,                         \
                        (type) 1                                       \
                    );                                                 \
            }                                                          \
        }                                                              \
        else                                                           \
        {                                                              \
            switch (order)                                             \
            {                                                          \
                case patomic_RELAXED:                                  \
                    val = (type) _InterlockedIncrement##width##_nf(    \
                        (volatile type *) obj                          \
                    ); break;                                          \
                case patomic_CONSUME:                                  \
                case patomic_ACQUIRE:                                  \
                    val = (type) _InterlockedIncrement##width##_acq(   \
                        (volatile type *) obj                          \
                    ); break;                                          \
                case patomic_RELEASE:                                  \
                    val = (type) _InterlockedIncrement##width##_rel(   \
                        (volatile type *) obj                          \
                    ); break;                                          \
                case patomic_ACQ_REL:                                  \
                case patomic_SEQ_CST:                                  \
                default:                                               \
                    val = (type) _InterlockedIncrement##width(         \
                        (volatile type *) obj                          \
                    );                                                 \
            }                                                          \
            --val;                                                     \
        }                                                              \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type)));        \
    }

/* equivalent to patomic_fetch_or */
#define PATOMIC_DEFINE_IL_OR(type, width)                       \
    static PATOMIC_FORCE_INLINE void                            \
    patomic_il_or_##width(                                      \
        volatile void *obj,                                     \
        const void *arg,                                        \
        int order,                                              \
        void *ret                                               \
    )                                                           \
    {                                                           \
        type val;                                               \
        switch (order)                                          \
        {                                                       \
            case patomic_RELAXED:                               \
                val = (type) _InterlockedOr##width##_nf(        \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                ); break;                                       \
            case patomic_CONSUME:                               \
            case patomic_ACQUIRE:                               \
                val = (type) _InterlockedOr##width##_acq(       \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                ); break;                                       \
            case patomic_RELEASE:                               \
                val = (type) _InterlockedOr##width##_rel(       \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                ); break;                                       \
            case patomic_ACQ_REL:                               \
            case patomic_SEQ_CST:                               \
            default:                                            \
                val = (type) _InterlockedOr##width(             \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                );                                              \
        }                                                       \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type))); \
    }

/* equivalent to patomic_fetch_xor */
#define PATOMIC_DEFINE_IL_XOR(type, width)                      \
    static PATOMIC_FORCE_INLINE void                            \
    patomic_il_xor_##width(                                     \
        volatile void *obj,                                     \
        const void *arg,                                        \
        int order,                                              \
        void *ret                                               \
    )                                                           \
    {                                                           \
        type val;                                               \
        switch (order)                                          \
        {                                                       \
            case patomic_RELAXED:                               \
                val = (type) _InterlockedXor##width##_nf(       \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                ); break;                                       \
            case patomic_CONSUME:                               \
            case patomic_ACQUIRE:                               \
                val = (type) _InterlockedXor##width##_acq(      \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                ); break;                                       \
            case patomic_RELEASE:                               \
                val = (type) _InterlockedXor##width##_rel(      \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                ); break;                                       \
            case patomic_ACQ_REL:                               \
            case patomic_SEQ_CST:                               \
            default:                                            \
                val = (type) _InterlockedXor##width(            \
                    (volatile type *) obj,                      \
                    *((const type *) arg)                       \
                );                                              \
        }                                                       \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type))); \
    }


/*
 * IL BIT TEST FUNCTIONS
 * - bit_test_set [[e8, e16, 32, e64]]
 * - bit_test_reset [[e8, e16, 32, e64]]
 */
PATOMIC_STATIC_ASSERT(msvc_arm_bittest, (sizeof(size_t) == sizeof(void *)));

#define PATOMIC_DEFINE_IL_BIT_SET(width)                         \
    static PATOMIC_FORCE_INLINE int                              \
    patomic_il_bit_test_and_set_##width(                         \
        volatile void *obj,                                      \
        int offset,                                              \
        int order                                                \
    )                                                            \
    {                                                            \
        static const size_t long_bits = sizeof(long) * CHAR_BIT; \
        static const size_t long_bytes = sizeof(long);           \
        volatile char *ptr = obj;                                \
        size_t diff;                                             \
        /* make sure access is 32bit aligned */                  \
        /* 8 or 16 bit op */                                     \
        if (width <= 16) {                                       \
            diff = (size_t) obj;                                 \
            diff %= long_bytes;                                  \
            ptr -= diff;                                         \
            /* may over-extend offset */                         \
            offset += (int) (diff * CHAR_BIT);                   \
        }                                                        \
        /* 64 bit op, or fixing over-extended offset */          \
        if (offset >= (int) long_bits) {                         \
            ptr += long_bytes;                                   \
            offset -= (int) long_bits;                           \
        }                                                        \
        /* 32 bit op assumed to be correctly aligned */          \
        /* perform op */                                         \
        switch (order)                                           \
        {                                                        \
            case patomic_RELAXED:                                \
                return _interlockedbittestandset_nf(             \
                    (volatile long *) ptr,                       \
                    offset                                       \
                );                                               \
            case patomic_CONSUME:                                \
            case patomic_ACQUIRE:                                \
                return _interlockedbittestandset_acq(            \
                    (volatile long *) ptr,                       \
                    offset                                       \
                );                                               \
            case patomic_RELEASE:                                \
                return _interlockedbittestandset_rel(            \
                    (volatile long *) ptr,                       \
                    offset                                       \
                );                                               \
            case patomic_ACQ_REL:                                \
            case patomic_SEQ_CST:                                \
            default:                                             \
                return _interlockedbittestandset(                \
                    (volatile long *) ptr,                       \
                    offset                                       \
                );                                               \
        }                                                        \
    }

#define PATOMIC_DEFINE_IL_BIT_RESET(width)                       \
    static PATOMIC_FORCE_INLINE int                              \
    patomic_il_bit_test_and_reset_##width(                       \
        volatile void *obj,                                      \
        int offset,                                              \
        int order                                                \
    )                                                            \
    {                                                            \
        static const size_t long_bits = sizeof(long) * CHAR_BIT; \
        static const size_t long_bytes = sizeof(long);           \
        volatile char *ptr = obj;                                \
        size_t diff;                                             \
        /* make sure access is 32bit aligned */                  \
        /* 8 or 16 bit op */                                     \
        if (width <= 16) {                                       \
            diff = (size_t) obj;                                 \
            diff %= long_bytes;                                  \
            ptr -= diff;                                         \
            /* may over-extend offset */                         \
            offset += (int) (diff * CHAR_BIT);                   \
        }                                                        \
        /* 64 bit op, or fixing over-extended offset */          \
        if (offset >= (int) long_bits) {                         \
            ptr += long_bytes;                                   \
            offset -= (int) long_bits;                           \
        }                                                        \
        /* 32 bit op assumed to be correctly aligned */          \
        /* perform op */                                         \
        switch (order)                                           \
        {                                                        \
            case patomic_RELAXED:                                \
                return _interlockedbittestandreset_nf(           \
                    (volatile long *) ptr,                       \
                    offset                                       \
                );                                               \
            case patomic_CONSUME:                                \
            case patomic_ACQUIRE:                                \
                return _interlockedbittestandreset_acq(          \
                    (volatile long *) ptr,                       \
                    offset                                       \
                );                                               \
            case patomic_RELEASE:                                \
                return _interlockedbittestandreset_rel(          \
                    (volatile long *) ptr,                       \
                    offset                                       \
                );                                               \
            case patomic_ACQ_REL:                                \
            case patomic_SEQ_CST:                                \
            default:                                             \
                return _interlockedbittestandreset(              \
                    (volatile long *) ptr,                       \
                    offset                                       \
                );                                               \
        }                                                        \
    }


#define PATOMIC_DEFINE_IL(type, iso_type, width)    \
    PATOMIC_DEFINE_IL_AND(type, width)              \
    PATOMIC_DEFINE_IL_COMPARE_EXCHANGE(type, width) \
    PATOMIC_DEFINE_IL_DECREMENT(type, width)        \
    PATOMIC_DEFINE_IL_EXCHANGE(type, width)         \
    PATOMIC_DEFINE_IL_EXCHANGE_ADD(type, width)     \
    PATOMIC_DEFINE_IL_INCREMENT(type, width)        \
    PATOMIC_DEFINE_IL_OR(type, width)               \
    PATOMIC_DEFINE_IL_XOR(type, width)              \
    PATOMIC_DEFINE_IL_LOAD(iso_type, width)         \
    PATOMIC_DEFINE_IL_STORE(type, iso_type, width)  \
    PATOMIC_DEFINE_IL_BIT_SET(width)                \
    PATOMIC_DEFINE_IL_BIT_RESET(width)


#endif  /* defined(_M_ARM || defined(_M_ARM64) */
#endif  /* defined(_MSC_VER) */

#endif  /* !PATOMIC_MSVC_ARM_H */
