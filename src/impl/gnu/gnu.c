#include "gnu.h"

#include <patomic/patomic.h>
#include <patomic/patomic_config.h>

#include <patomic/macros/ignore_unused.h>


#if PATOMIC_HAVE_GNU_ATOMIC


#include <stddef.h>

#include <patomic/stdlib/assert.h>
#include <patomic/stdlib/stdalign.h>
#include <patomic/stdlib/stdint.h>

#include "gnu_atomic.h"


/*
 * MACROS EXPECTED IN GNU_ATOMIC_H:
 * - PATOMIC_IMPL_GNU_HAVE_LLONG
 * - PATOMIC_IMPL_GNU_HAVE_I128
 */
#ifndef PATOMIC_IMPL_GNU_HAVE_LLONG
    #error "PATOMIC_IMPL_GNU_HAVE_LLONG not defined"
#endif

#ifndef PATOMIC_IMPL_GNU_HAVE_I128
    #error "PATOMIC_IMPL_GNU_HAVE_I128 not defined"
#endif


#define PATOMIC_SET_RET(type, name, width, order, ret)                  \
        ((width) == sizeof(type))                                       \
    {                                                                   \
        if (patomic_impl_is_lock_free_##name())                         \
        {                                                               \
            switch (order)                                              \
            {                                                           \
                case patomic_RELAXED:                                   \
                    (ret) = patomic_ops_create_##name##_relaxed();      \
                    break;                                              \
                case patomic_CONSUME:                                   \
                case patomic_ACQUIRE:                                   \
                    (ret) = patomic_ops_create_##name##_acquire();      \
                    break;                                              \
                case patomic_RELEASE:                                   \
                    (ret) = patomic_ops_create_##name##_release();      \
                    break;                                              \
                case patomic_ACQ_REL:                                   \
                    (ret) = patomic_ops_create_##name##_acq_rel();      \
                    break;                                              \
                case patomic_SEQ_CST:                                   \
                    (ret) = patomic_ops_create_##name##_seq_cst();      \
                    break;                                              \
                default:                                                \
                    patomic_assert_always("invalid memory order" && 0); \
            }                                                           \
        }                                                               \
    }

#define PATOMIC_SET_RET_EXPLICIT(type, name, width, ret)    \
        ((width) == sizeof(type))                           \
    {                                                       \
        if (patomic_impl_is_lock_free_##name())             \
        {                                                   \
            (ret) = patomic_ops_create_##name##_explicit(); \
        }                                                   \
    }

#define PATOMIC_SET_ALIGN(type, width, member) \
        ((width) == sizeof(type))              \
    {                                          \
        (member) = patomic_alignof_type(type); \
    }


static patomic_ops_t
patomic_create_ops(
    size_t byte_width,
    patomic_memory_order_t order
)
{
    patomic_ops_t ret = {0};
    patomic_assert_always(patomic_is_valid_order((int) order));

    if PATOMIC_SET_RET(char, char, byte_width, order, ret)
    else if PATOMIC_SET_RET(short, short, byte_width, order, ret)
    else if PATOMIC_SET_RET(int, int, byte_width, order, ret)
    else if PATOMIC_SET_RET(long, long, byte_width, order, ret)
#if PATOMIC_IMPL_GNU_HAVE_LLONG
    else if PATOMIC_SET_RET(patomic_llong_unsigned, llong, byte_width, order, ret)
#endif
#if PATOMIC_IMPL_GNU_HAVE_I128
    else if PATOMIC_SET_RET(patomic_i128_unsigned, 128, byte_width, order, ret)
#endif

    return ret;
}

static patomic_ops_explicit_t
patomic_create_ops_explicit(
    size_t byte_width
)
{
    patomic_ops_explicit_t ret = {0};

    if PATOMIC_SET_RET_EXPLICIT(char, char, byte_width, ret)
    else if PATOMIC_SET_RET_EXPLICIT(short, short, byte_width, ret)
    else if PATOMIC_SET_RET_EXPLICIT(int, int, byte_width, ret)
    else if PATOMIC_SET_RET_EXPLICIT(long, long, byte_width, ret)
#if PATOMIC_IMPL_GNU_HAVE_LLONG
    else if PATOMIC_SET_RET_EXPLICIT(patomic_llong_unsigned, llong, byte_width, ret)
#endif
#if PATOMIC_IMPL_GNU_HAVE_I128
    else if PATOMIC_SET_RET_EXPLICIT(patomic_i128_unsigned, 128, byte_width, ret)
#endif

    return ret;
}

static patomic_align_t
patomic_create_align(
    size_t byte_width
)
{
    patomic_align_t ret = {0};

    if PATOMIC_SET_ALIGN(char, byte_width, ret.recommended)
    else if PATOMIC_SET_ALIGN(short, byte_width, ret.recommended)
    else if PATOMIC_SET_ALIGN(int, byte_width, ret.recommended)
    else if PATOMIC_SET_ALIGN(long, byte_width, ret.recommended)
#if PATOMIC_IMPL_GNU_HAVE_LLONG
    else if PATOMIC_SET_ALIGN(patomic_llong_unsigned, byte_width, ret.recommended)
#endif
#if PATOMIC_IMPL_GNU_HAVE_I128
    else if PATOMIC_SET_ALIGN(patomic_i128_unsigned, byte_width, ret.recommended)
#endif
    else { ret.recommended = 1; }

    ret.minimum = ret.recommended;
    ret.size_within = 0;
    return ret;
}


patomic_t
patomic_impl_create_gnu(
        size_t byte_width,
        patomic_memory_order_t order,
        int options
)
{
    patomic_t ret;
    PATOMIC_IGNORE_UNUSED(options);
    ret.ops = patomic_create_ops(byte_width, order);
    ret.align = patomic_create_align(byte_width);
    return ret;
}

patomic_explicit_t
patomic_impl_create_explicit_gnu(
        size_t byte_width,
        int options
)
{
    patomic_explicit_t ret;
    PATOMIC_IGNORE_UNUSED(options);
    ret.ops = patomic_create_ops_explicit(byte_width);
    ret.align = patomic_create_align(byte_width);
    return ret;
}


#else


patomic_t
patomic_impl_create_gnu(
    size_t byte_width,
    patomic_memory_order_t order,
    int options
)
{
    patomic_t ret = {0};
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(order);
    PATOMIC_IGNORE_UNUSED(options);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}

patomic_explicit_t
patomic_impl_create_explicit_gnu(
    size_t byte_width,
    int options
)
{
    patomic_explicit_t ret = {0};
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(options);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}


#endif  /* PATOMIC_HAVE_GNU_ATOMIC */


patomic_transaction_t
patomic_impl_create_transaction_gnu(
    int options
)
{
    patomic_transaction_t ret = {0};
    PATOMIC_IGNORE_UNUSED(options);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}
