#ifndef PATOMIC_MSVC_EXT_H
#define PATOMIC_MSVC_EXT_H

#if defined(_MSC_VER)
#if defined(_M_IX86) || defined(_M_X64)||defined(_M_AMD64) || defined(_M_ARM64)

#include <string.h>

#include <intrin.h>

#include <patomic/patomic.h>
#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>

#include "generic_uint.h"


/* hide/show anything */
#ifndef HIDE
    #define HIDE(x)
#endif
#ifndef SHOW
    #define SHOW(x) x
#endif

/* hide/show function param with leading comma */
#ifndef HIDE_P
    #define HIDE_P(x, y)
#endif
#ifndef SHOW_P
    #define SHOW_P(x, y) ,y
#endif


/*
 * IL EXTENDED FUNCTIONS
 *
 * - 64bit on x86
 * - 128bit on x64/AMD64/ARM64
 *
 * - need to define load, store, and cmpxchg
 * - all other IL functions derived from these
 * - msvc.c then uses IL functions to generate patomic functions
 *
 * WARNING:
 * - 128bit load MUST NOT be used with const objects
 * - it accepts const objects to fulfil api requirements
 * - internally it casts away the const and uses cmpxchg
 * - 128bit load is NOT publicly supported (set to NULL in msvc.c)
 */


/*
 * ILE BASE FUNCTIONS
 *
 * - load
 * - store
 * - cmpxchg
 */

#if defined(_M_IX86)

static PATOMIC_FORCE_INLINE void
patomic_il_load_64(
    const volatile void *obj,
    int order,
    void *ret
)
{
    __int64 val = (__int64) __iso_volatile_load64(
        (const volatile __int64 *) obj
    );
    switch (order)
    {
        case patomic_RELAXED:
        case patomic_CONSUME:
        case patomic_ACQUIRE:
        case patomic_RELEASE:
        case patomic_ACQ_REL:
            break;
        case patomic_SEQ_CST:
        default:
            _ReadWriteBarrier();
    }
    PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(__int64)));
}

static PATOMIC_FORCE_INLINE void
patomic_il_store_64(
        volatile void *obj,
        const void *desired,
        int order
)
{
    switch (order)
    {
        case patomic_RELEASE:
            _ReadWriteBarrier();
        case patomic_RELAXED:
            __iso_volatile_store64(
                (volatile __int64 *) obj,
                *((const __int64 *) desired)
            );
            break;
        case patomic_CONSUME:
        case patomic_ACQUIRE:
        case patomic_ACQ_REL:
        case patomic_SEQ_CST:
        default:
            __iso_volatile_store64(
                (volatile __int64 *) obj,
                *((const __int64 *) desired)
            );
            /* cmpxchg here for serialisation */
            PATOMIC_IGNORE_UNUSED(_InterlockedCompareExchange64(
                (volatile __int64 *) obj, 0i64, 0i64
            ));
    }
}

static PATOMIC_FORCE_INLINE int
patomic_il_compare_exchange_64(
        volatile void *obj,
        void *expected,
        const void *desired,
        int succ,
        int fail
)
{
    __int64 val;
    PATOMIC_IGNORE_UNUSED(succ);
    PATOMIC_IGNORE_UNUSED(fail);
    /* MSVC flips expected and desired order */
    val = (__int64) _InterlockedCompareExchange64(
        (volatile __int64 *) obj,
        *((const __int64 *) desired),
        *((const __int64 *) expected)
    );
    if (val == *((const __int64 *) expected)) { return 1; }
    else
    {
        PATOMIC_IGNORE_UNUSED(memcpy(expected, &val, sizeof(__int64)));
        return 0;
    }
}

#else  /* defined(_M_X64)||defined(_M_AMD64) || defined(_M_ARM64) */

static PATOMIC_FORCE_INLINE int
patomic_il_compare_exchange_128(
    volatile void *obj,
    void *expected,
    const void *desired,
    int succ,
    int fail
)
{
    /* copy input */
    patomic_uint128_t des_val;
#if defined(_M_X64) || defined(_M_AMD64)
    PATOMIC_IGNORE_UNUSED(succ);
#endif
    PATOMIC_IGNORE_UNUSED(fail);
    PATOMIC_IGNORE_UNUSED(memcpy(&des_val, desired, sizeof des_val));
    /* perform op */
#if defined(_M_X64) || defined(_M_AMD64)
    return (int) _InterlockedCompareExchange128(
        (volatile __int64 *) obj,
        (__int64) PATOMIC_HIGH_64(des_val),
        (__int64) PATOMIC_LOW_64(des_val),
        (__int64 *) expected
    );
#else
    switch (succ)
    {
        case patomic_RELAXED:
            return (int) _InterlockedCompareExchange128_nf(
                (volatile __int64 *) obj,
                (__int64) PATOMIC_HIGH_64(des_val),
                (__int64) PATOMIC_LOW_64(des_val),
                (__int64 *) expected
            );
        case patomic_CONSUME:
        case patomic_ACQUIRE:
            return (int) _InterlockedCompareExchange128_acq(
                (volatile __int64 *) obj,
                (__int64) PATOMIC_HIGH_64(des_val),
                (__int64) PATOMIC_LOW_64(des_val),
                (__int64 *) expected
            );
        case patomic_RELEASE:
            return (int) _InterlockedCompareExchange128_rel(
                (volatile __int64 *) obj,
                (__int64) PATOMIC_HIGH_64(des_val),
                (__int64) PATOMIC_LOW_64(des_val),
                (__int64 *) expected
            );
        case patomic_ACQ_REL:
        case patomic_SEQ_CST:
        default:
            return (int) _InterlockedCompareExchange128(
                (volatile __int64 *) obj,
                (__int64) PATOMIC_HIGH_64(des_val),
                (__int64) PATOMIC_LOW_64(des_val),
                (__int64 *) expected
            );
    }
#endif
}

static PATOMIC_FORCE_INLINE void
patomic_il_load_128(
    const volatile void *obj,
    int order,
    void *ret
)
{
    /* declarations */
    patomic_uint128_t expected = {0};
    patomic_uint128_t desired = {0};
    int succ;
    int fail;
    /* setup memory orders */
    succ = order;
    fail = patomic_cmpxchg_fail_order(order);
    /* load */
    /* NOTE: ret may not have been initialised (cannot be used as expected) */
    PATOMIC_IGNORE_UNUSED(patomic_il_compare_exchange_128(
        (volatile void *) obj,
        &expected.arr[0],
        &desired.arr[0],
        succ,
        fail
    ));
    PATOMIC_IGNORE_UNUSED(memcpy(ret, &expected, sizeof(patomic_uint128_t)));
}

static PATOMIC_FORCE_INLINE void
patomic_il_store_128(
    volatile void *obj,
    const void *desired,
    int order
)
{
    /* declarations */
    patomic_uint128_t expected = {0};
    int succ;
    int fail;
    /* setup memory orders */
    succ = order;
    fail = patomic_cmpxchg_fail_order(order);
    /* store */
    while (!patomic_il_compare_exchange_128(
        obj,
        &expected.arr[0],
        desired,
        succ,
        fail
    ));
}

#endif /* defining load, store, cmpxchg */


/*
 * ILE BASE MACROS
 *
 * - fetch
 * - bit
 */

#define PATOMIC_DEFINE_ILE_FETCH(width, op, expr, vis_p, vis)           \
    static PATOMIC_FORCE_INLINE void                                    \
    patomic_il_##op##_##width(                                          \
        volatile void *obj                                              \
 vis_p(_,const void *arg)                                               \
        ,int order                                                      \
        ,void *ret                                                      \
    )                                                                   \
    {                                                                   \
        /* declarations */                                              \
        patomic_uint##width##_t expected;                               \
        patomic_uint##width##_t desired;                                \
    vis(patomic_uint##width##_t arg_val;)                               \
        int succ;                                                       \
        int fail;                                                       \
        /* setup memory orders */                                       \
        succ = order;                                                   \
        fail = patomic_cmpxchg_fail_order(order);                       \
        /* load initial values */                                       \
        patomic_il_load_##width(                                        \
            obj,                                                        \
            fail,                                                       \
            &expected                                                   \
        );                                                              \
    vis(PATOMIC_IGNORE_UNUSED(memcpy(&arg_val, arg, sizeof arg_val)));  \
        /* cas loop */                                                  \
        do {                                                            \
            desired = PATOMIC_UINT_CAST(width) (expr);                  \
        }                                                               \
        while (!patomic_il_compare_exchange_##width(                    \
            obj,                                                        \
            &expected,                                                  \
            &desired,                                                   \
            succ,                                                       \
            fail                                                        \
        ));                                                             \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &expected, sizeof expected)); \
    }

#if defined(_M_IX86) || defined(_M_x64) || defined(_M_AMD64)

#define PATOMIC_DEFINE_ILE_BIT(width, op)           \
    static PATOMIC_FORCE_INLINE int                 \
    patomic_il_bit_test_and_##op##_##width(         \
        volatile void *obj,                         \
        int offset,                                 \
        int order                                   \
    )                                               \
    {                                               \
        volatile long *ptr = (volatile long *) obj; \
        PATOMIC_IGNORE_UNUSED(order);               \
        while (offset >= 32)                        \
        {                                           \
            ++ptr;                                  \
            offset -= 32;                           \
        }                                           \
        return (int) _interlockedbittestand##op(    \
            ptr,                                    \
            offset                                  \
        );                                          \
    }

#else  /* defined(_M_ARM64) */

#define PATOMIC_DEFINE_ILE_BIT(width, op)                      \
    static PATOMIC_FORCE_INLINE int                            \
    patomic_il_bit_test_and_##op##_128(                        \
        volatile void *obj,                                    \
        int offset,                                            \
        int order                                              \
    )                                                          \
    {                                                          \
        volatile long *ptr = (volatile long *) obj;            \
        while (offset >= 32)                                   \
        {                                                      \
            ++ptr;                                             \
            offset -= 32;                                      \
        }                                                      \
        switch (order)                                         \
        {                                                      \
            case patomic_RELAXED:                              \
                return (int) _interlockedbittestand##op##_nf(  \
                    (volatile long *) ptr,                     \
                    offset                                     \
                );                                             \
            case patomic_CONSUME:                              \
            case patomic_ACQUIRE:                              \
                return (int) _interlockedbittestand##op##_acq( \
                    (volatile long *) ptr,                     \
                    offset                                     \
                );                                             \
            case patomic_RELEASE:                              \
                return (int) _interlockedbittestand##op##_rel( \
                    (volatile long *) ptr,                     \
                    offset                                     \
                );                                             \
            case patomic_ACQ_REL:                              \
            case patomic_SEQ_CST:                              \
            default:                                           \
                return (int) _interlockedbittestand##op(       \
                    (volatile long *) ptr,                     \
                    offset                                     \
                );                                             \
        }                                                      \
    }

#endif /* defining fetch and bit macros */


/*
 * ILE MACRO
 */

#if defined(_M_IX86)

#define PATOMIC_DEFINE_ILE()                                             \
    PATOMIC_DEFINE_ILE_FETCH(64, and, expected & arg_val, SHOW_P, SHOW)  \
    PATOMIC_DEFINE_ILE_FETCH(64, decrement, expected - 1u, HIDE_P, HIDE) \
    PATOMIC_DEFINE_ILE_FETCH(64, exchange, arg_val, SHOW_P, SHOW)        \
    PATOMIC_DEFINE_ILE_FETCH(64, exchange_add, expected + arg_val,       \
                             SHOW_P, SHOW)                               \
    PATOMIC_DEFINE_ILE_FETCH(64, increment, expected + 1u, HIDE_P, HIDE) \
    PATOMIC_DEFINE_ILE_FETCH(64, or, expected | arg_val, SHOW_P, SHOW)   \
    PATOMIC_DEFINE_ILE_FETCH(64, xor, expected ^ arg_val, SHOW_P, SHOW)  \
    PATOMIC_DEFINE_ILE_BIT(64, set)                                      \
    PATOMIC_DEFINE_ILE_BIT(64, reset)

#else  /* 64bit */

#define PATOMIC_DEFINE_ILE()                                    \
    PATOMIC_DEFINE_ILE_FETCH(128, and,                          \
        PATOMIC_UINT_AND(128, expected, arg_val), SHOW_P, SHOW) \
    PATOMIC_DEFINE_ILE_FETCH(128, decrement,                    \
        patomic_dec_128(expected), HIDE_P, HIDE)                \
    PATOMIC_DEFINE_ILE_FETCH(128, exchange,                     \
        arg_val, SHOW_P, SHOW)                                  \
    PATOMIC_DEFINE_ILE_FETCH(128, exchange_add,                 \
        patomic_add_128(expected, arg_val), SHOW_P, SHOW)       \
    PATOMIC_DEFINE_ILE_FETCH(128, increment,                    \
        patomic_inc_128(expected), HIDE_P, HIDE)                \
    PATOMIC_DEFINE_ILE_FETCH(128, or,                           \
        PATOMIC_UINT_OR(128, expected, arg_val), SHOW_P, SHOW)  \
    PATOMIC_DEFINE_ILE_FETCH(128, xor,                          \
        PATOMIC_UINT_XOR(128, expected, arg_val), SHOW_P, SHOW) \
    PATOMIC_DEFINE_ILE_BIT(128, set)                            \
    PATOMIC_DEFINE_ILE_BIT(128, reset)

#endif  /* defining ILE macro */


#endif  /* defined x86, x64/amd64, arm64 */
#endif  /* defined(_MSC_VER) */

#endif  /* !PATOMIC_MSVC_EXT_H */
