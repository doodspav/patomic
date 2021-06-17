#ifndef PATOMIC_MSVC_E64_H
#define PATOMIC_MSVC_E64_H

#if defined (_MSC_VER) && defined(_M_IX86)

#include <string.h>

#include <intrin.h>

#include <patomic/patomic.h>
#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>


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
 * IL EXTENDED 64bit FUNCTIONS
 *
 * - 64bit atomic functions for 32bit x86
 * - all ops are available
 * - implementing only IL functions
 * - then relying on msvc.c to generate everything
 */


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
                    (volatile __int64 *) obj, 0, 0
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


#define PATOMIC_DEFINE_IL_E64_FETCH(op, expr, vis_p, vis)                    \
    static PATOMIC_FORCE_INLINE void                                         \
    patomic_il_##op##_64(                                                    \
        volatile void *obj                                                   \
 vis_p(_,const void *arg)                                                    \
        ,int order                                                           \
        ,void *ret                                                           \
    )                                                                        \
    {                                                                        \
        /* declarations */                                                   \
        unsigned __int64 expected;                                           \
        unsigned __int64 desired;                                            \
    vis(unsigned __int64 arg_val;)                                           \
        /* ignore order */                                                   \
        PATOMIC_IGNORE_UNUSED(order);                                        \
        /* load initial values */                                            \
        expected = (unsigned __int64) __iso_volatile_load64(                 \
            (const volatile __int64 *) obj                                   \
        );                                                                   \
    vis(PATOMIC_IGNORE_UNUSED(memcpy(&arg_val, arg, sizeof(__int64))));      \
        /* cas loop */                                                       \
        do {                                                                 \
            desired = (unsigned __int64) (expr);                             \
        }                                                                    \
        /* MSVC flips expected and desired order */                          \
        while (expected != (unsigned __int64) _InterlockedCompareExchange64( \
            (volatile __int64 *) obj,                                        \
            (__int64) desired,                                               \
            (__int64) expected                                               \
        ));                                                                  \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &expected, sizeof(__int64)));      \
    }

#define PATOMIC_DEFINE_IL_E64_BIT(op)               \
    static PATOMIC_FORCE_INLINE int                 \
    patomic_il_bit_test_and_##op##_64(              \
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


#define PATOMIC_DEFINE_IL_E64()                                                \
    PATOMIC_DEFINE_IL_E64_FETCH(and, expected & arg_val, SHOW_P, SHOW)         \
    PATOMIC_DEFINE_IL_E64_FETCH(decrement, expected - 1u, HIDE_P, HIDE)        \
    PATOMIC_DEFINE_IL_E64_FETCH(exchange, arg_val, SHOW_P, SHOW)               \
    PATOMIC_DEFINE_IL_E64_FETCH(exchange_add, expected + arg_val, SHOW_P, SHOW)\
    PATOMIC_DEFINE_IL_E64_FETCH(increment, expected + 1u, HIDE_P, HIDE)        \
    PATOMIC_DEFINE_IL_E64_FETCH(or, expected | arg_val, SHOW_P, SHOW)          \
    PATOMIC_DEFINE_IL_E64_FETCH(xor, expected ^ arg_val, SHOW_P, SHOW)         \
    PATOMIC_DEFINE_IL_E64_BIT(set)                                             \
    PATOMIC_DEFINE_IL_E64_BIT(reset)


#endif  /* defined(_MSC_VER) && defined(_M_IX86) */

#endif  /* !PATOMIC_MSVC_E64_H */
