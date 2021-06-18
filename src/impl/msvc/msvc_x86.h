#ifndef PATOMIC_MSVC_X86_H
#define PATOMIC_MSVC_X86_H

#if defined(_MSC_VER)
#if defined(_M_IX86) || defined(_M_AMD64) || defined(_M_X64)

#include <limits.h>
#include <string.h>

#include <intrin.h>

#include <patomic/patomic.h>
#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>
#include <patomic/macros/static_assert.h>


/*
 * BIT WIDTHS SUPPORTED:
 * - 8
 * - 16
 * - 32
 * - 64 (not on _M_IX86)
 */


#define _InterlockedAnd32 _InterlockedAnd
#define _InterlockedCompareExchange32 _InterlockedCompareExchange
#define _InterlockedDecrement32 _InterlockedDecrement
#define _InterlockedExchange32 _InterlockedExchange
#define _InterlockedExchangeAdd32 _InterlockedExchangeAdd
#define _InterlockedIncrement32 _InterlockedIncrement
#define _InterlockedOr32 _InterlockedOr
#define _InterlockedXor32 _InterlockedXor

#define _InterlockedDecrement8(x) 0
#define _InterlockedIncrement8(x) 0


/*
 * IL BASE FUNCTIONS
 * - Load (y) [[8, 16, 32], 64]
 * - Store (y) [[8, 16, 32], 64]
 * - using guidance from:
 * >> https://github.com/microsoft/STL/blob/master/stl/inc/atomic
 */

/*
 * - technically anything above RELAXED needs _ReadWriteBarrier()
 * - this is to prevent compiler reordering (CPU will be fine)
 * - in this case we can omit it because this function will always
 *   be called opaquely (since it's chosen at runtime)
 * - the compiler cannot reorder across opaque function calls
 */
#define PATOMIC_DEFINE_IL_LOAD(iso_type, width)                     \
    static PATOMIC_FORCE_INLINE void                                \
    patomic_il_load_##width(                                        \
        const volatile void *obj,                                   \
        int order,                                                  \
        void *ret                                                   \
    )                                                               \
    {                                                               \
        iso_type val = (iso_type) __iso_volatile_load##width(       \
            (const volatile iso_type *) obj                         \
        );                                                          \
        switch (order)                                              \
        {                                                           \
            case patomic_RELAXED:                                   \
            case patomic_CONSUME:                                   \
            case patomic_ACQUIRE:                                   \
            case patomic_RELEASE:                                   \
            case patomic_ACQ_REL:                                   \
                break;                                              \
            case patomic_SEQ_CST:                                   \
            default:                                                \
                _ReadWriteBarrier();                                \
        }                                                           \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(iso_type))); \
    }

#define PATOMIC_DEFINE_IL_STORE(type, iso_type, width)             \
    static PATOMIC_FORCE_INLINE void                               \
    patomic_il_store_##width(                                      \
        volatile void *obj,                                        \
        const void *desired,                                       \
        int order                                                  \
    )                                                              \
    {                                                              \
        switch (order)                                             \
        {                                                          \
            case patomic_RELEASE:                                  \
                _ReadWriteBarrier();                               \
            case patomic_RELAXED:                                  \
                __iso_volatile_store##width(                       \
                    (volatile iso_type *) obj,                     \
                    *((const iso_type *) desired)                  \
                );                                                 \
                break;                                             \
            case patomic_CONSUME:                                  \
            case patomic_ACQUIRE:                                  \
            case patomic_ACQ_REL:                                  \
            case patomic_SEQ_CST:                                  \
            default:                                               \
                PATOMIC_IGNORE_UNUSED(_InterlockedExchange##width( \
                    (volatile type *) obj,                         \
                    *((const type *) desired)                      \
                ));                                                \
        }                                                          \
    }


/*
 * IL FUNCTIONS AVAILABLE (8/9):
 * - Add (n)
 * - And (y) [[8, 16, 32], 64]
 * - CompareExchange (y) [[8, 16, 32, 64]]
 * - Decrement (y) [[16, 32], 64
 * - Exchange (y) [[8, 16, 32], 64]
 * - ExchangeAdd (y) [[8, 16, 32], 64]
 * - Increment (y) [[16, 32], 64]
 * - Or (y) [[8, 16, 32], 64]
 * - Xor (y) [[8, 16, 32], 64]
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
        PATOMIC_IGNORE_UNUSED(order);                           \
        val = (type) _InterlockedAnd##width(                    \
            (volatile type *) obj,                              \
            *((const type *) arg)                               \
        );                                                      \
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
        PATOMIC_IGNORE_UNUSED(succ);                                     \
        PATOMIC_IGNORE_UNUSED(fail);                                     \
        /* MSVC flips expected and desired order */                      \
        val = (type) _InterlockedCompareExchange##width(                 \
            (volatile type *) obj,                                       \
            *((const type *) desired),                                   \
            *((const type*) expected)                                    \
        );                                                               \
        if (val == *((const type*) expected)) { return 1; }              \
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
#define PATOMIC_DEFINE_IL_DECREMENT(type, width)                \
    static PATOMIC_FORCE_INLINE void                            \
    patomic_il_decrement_##width(                               \
        volatile void *obj,                                     \
        int order,                                              \
        void *ret                                               \
    )                                                           \
    {                                                           \
        type val;                                               \
        PATOMIC_IGNORE_UNUSED(order);                           \
        if (width == 8)                                         \
        {                                                       \
            val = (type) _InterlockedExchangeAdd##width(        \
                (volatile type *) obj,                          \
                (type) -1                                       \
            );                                                  \
        }                                                       \
        else                                                    \
        {                                                       \
            val = (type) _InterlockedDecrement##width(          \
                (volatile type *) obj                           \
            );                                                  \
            ++val;                                              \
        }                                                       \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type))); \
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
        PATOMIC_IGNORE_UNUSED(order);                           \
        val = (type) _InterlockedExchange##width(               \
            (volatile type *) obj,                              \
            *((const type *) desired)                           \
        );                                                      \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type))); \
    }

/* equivalent to patomic_fetch_add */
#define PATOMIC_DEFINE_IL_EXCHANGE_ADD(type, width)             \
    static PATOMIC_FORCE_INLINE void                            \
    patomic_il_exchange_add_##width(                            \
        volatile void *obj,                                     \
        const void *arg,                                        \
        int order,                                              \
        void *ret                                               \
    )                                                           \
    {                                                           \
        type val;                                               \
        PATOMIC_IGNORE_UNUSED(order);                           \
        val = (type) _InterlockedExchangeAdd##width(            \
            (volatile type *) obj,                              \
            *((const type *) arg)                               \
        );                                                      \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type))); \
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
#define PATOMIC_DEFINE_IL_INCREMENT(type, width)                \
    static PATOMIC_FORCE_INLINE void                            \
    patomic_il_increment_##width(                               \
        volatile void *obj,                                     \
        int order,                                              \
        void *ret                                               \
    )                                                           \
    {                                                           \
        type val;                                               \
        PATOMIC_IGNORE_UNUSED(order);                           \
        if (width == 8)                                         \
        {                                                       \
            val = (type) _InterlockedExchangeAdd##width(        \
                (volatile type *) obj,                          \
                (type) 1                                        \
            );                                                  \
        }                                                       \
        else                                                    \
        {                                                       \
            val = (type) _InterlockedIncrement##width(          \
                (volatile type *) obj                           \
            );                                                  \
            --val;                                              \
        }                                                       \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type))); \
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
        PATOMIC_IGNORE_UNUSED(order);                           \
        val = (type) _InterlockedOr##width(                     \
            (volatile type *) obj,                              \
            *((const type *) arg)                               \
        );                                                      \
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
        PATOMIC_IGNORE_UNUSED(order);                           \
        val = (type) _InterlockedXor##width(                    \
            (volatile type *) obj,                              \
            *((const type *) arg)                               \
        );                                                      \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type))); \
    }


/*
 * IL BIT TEST FUNCTIONS
 * - bit_test_set [[e8, e16, 32, e64]]
 * - bit_test_reset [[e8, e16, 32, e64]]
 */
PATOMIC_STATIC_ASSERT(msvc_x86_bittest, (sizeof(size_t) == sizeof(void *)));

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
        PATOMIC_IGNORE_UNUSED(order);                            \
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
        return (int) _interlockedbittestandset(                  \
            (volatile long *) ptr,                               \
            offset                                               \
        );                                                       \
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
        PATOMIC_IGNORE_UNUSED(order);                            \
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
        return (int) _interlockedbittestandreset(                \
            (volatile long *) ptr,                               \
            offset                                               \
        );                                                       \
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


#endif  /* defined(_M_IX86) || defined(_M_AMD64) || defined(_M_X64) */
#endif  /* defined(_MSC_VER) */

#endif  /* !PATOMIC_MSVC_X86_H */
