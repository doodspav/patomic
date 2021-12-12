#include "std.h"

#include <patomic/patomic.h>
#include <patomic/patomic_config.h>

#include <patomic/macros/ignore_unused.h>


#if PATOMIC_HAVE_STD_ATOMIC


#include <limits.h>
#include <stdatomic.h>
#include <stddef.h>

#include <patomic/macros/force_inline.h>

#include <patomic/stdlib/assert.h>
#include <patomic/stdlib/string.h>

#include <patomic/types/intptr.h>


static PATOMIC_FORCE_INLINE int
patomic_is_aligned(
    const volatile void *ptr,
    size_t align
)
{
    /* align will always be a power of 2 */
    patomic_uintptr_t addr = (patomic_uintptr_t) ptr;
    return (addr & (align - 1u)) == 0;
}


#if PATOMIC_HAVE_ALIGNOF
    #define patomic_alignof(t) _Alignof(t)
#elif PATOMIC_HAVE_MS_ALIGNOF_ALIGN_DSPC
    #define patomic_alignof(t) __alignof(t)
#elif PATOMIC_HAVE_GNU_ALIGNOF_ALIGNED_ATTR
    #define patomic_alignof(t) __alignof__(t)
#else
    #define patomic_alignof(t) sizeof(t)
#endif


#define patomic_assert_unreachable_aligned(obj, type) \
    patomic_assert_unreachable(patomic_is_aligned(obj, patomic_alignof(type)))

#define patomic_ignored_memcpy(dst, src, sz) \
    PATOMIC_IGNORE_UNUSED(patomic_memcpy(dst, src, sz))


/* hide/show anything */
#define HIDE(x)
#define SHOW(x) x

/* hide/show function param with leading comma */
#define HIDE_P(x, y)
#define SHOW_P(x, y) ,y


/*
 * FUNCTION/STRUCT DEFINE MACRO PARAMS
 *
 * - type: operand type (e.g. int)
 * - name: 'explicit' or memory order (e.g. acquire)
 * - order: memory order value (e.g. memory_order_acquire)
 * - vis_p: order parameter visibility (e.g. SHOW_P)
 * - inv: the inverse of vis_p, but for anything (e.g. HIDE, not HIDE_P)
 * - opsk: return type ops kind (e.g. ops_explicit)
 * - min: minimum value of arithmetic type (e.g. INT_MIN)
 */


/*
 * BASE:
 * - store (y)
 * - load (y)
 */
#define PATOMIC_DEFINE_STORE_OPS(type, name, order, vis_p)               \
    static PATOMIC_FORCE_INLINE void                                     \
    patomic_opimpl_store_##name(                                         \
        volatile void *obj                                               \
        ,const void *desired                                             \
   vis_p(_,int order)                                                    \
    )                                                                    \
    {                                                                    \
        /* declarations */                                               \
        type des_val;                                                    \
        /* assertions */                                                 \
        patomic_assert_unreachable(obj != NULL);                         \
        patomic_assert_unreachable(desired != NULL);                     \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));          \
        patomic_assert_unreachable(patomic_is_valid_store_order(order)); \
        /* operation */                                                  \
        patomic_ignored_memcpy(&des_val, desired, sizeof(type));         \
        atomic_store_explicit(                                           \
            (volatile _Atomic(type) *) obj,                              \
            des_val,                                                     \
            order                                                        \
        );                                                               \
    }

#define PATOMIC_DEFINE_LOAD_OPS(type, name, order, vis_p)               \
    static PATOMIC_FORCE_INLINE void                                    \
    patomic_opimpl_load_##name(                                         \
        const volatile void *obj                                        \
   vis_p(_,int order)                                                   \
        ,void *ret                                                      \
    )                                                                   \
    {                                                                   \
        /* declarations */                                              \
        type val;                                                       \
        /* assertions */                                                \
        patomic_assert_unreachable(obj != NULL);                        \
        patomic_assert_unreachable(ret != NULL);                        \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));         \
        patomic_assert_unreachable(patomic_is_valid_load_order(order)); \
        /* operation */                                                 \
        val = atomic_load_explicit(                                     \
            (const volatile _Atomic(type) *) obj,                       \
            order                                                       \
        );                                                              \
        patomic_ignored_memcpy(ret, &val, sizeof(type));                \
    }


/*
 * XCHG:
 * - exchange (y)
 * - compare_exchange_weak (y)
 * - compare_exchange_strong (y)
 */
#define PATOMIC_DEFINE_XCHG_OPS_CREATE(type, name, order, vis_p, inv, opsk)  \
    static PATOMIC_FORCE_INLINE void                                         \
    patomic_opimpl_exchange_##name(                                          \
        volatile void *obj                                                   \
        ,const void *desired                                                 \
   vis_p(_,int order)                                                        \
        ,void *ret                                                           \
    )                                                                        \
    {                                                                        \
        /* declarations */                                                   \
        type val;                                                            \
        type des_val;                                                        \
        /* assertions */                                                     \
        patomic_assert_unreachable(obj != NULL);                             \
        patomic_assert_unreachable(desired != NULL);                         \
        patomic_assert_unreachable(ret != NULL);                             \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));              \
        patomic_assert_unreachable(patomic_is_valid_order(order));           \
        /* operation */                                                      \
        patomic_ignored_memcpy(&des_val, desired, sizeof(type));             \
        val = atomic_exchange_explicit(                                      \
            (volatile _Atomic(type) *) obj,                                  \
            des_val,                                                         \
            order                                                            \
        );                                                                   \
        patomic_ignored_memcpy(ret, &val, sizeof(type));                     \
    }                                                                        \
    static PATOMIC_FORCE_INLINE int                                          \
    patomic_opimpl_cmpxchg_weak_##name(                                      \
        volatile void *obj                                                   \
        ,void *expected                                                      \
        ,const void *desired                                                 \
   vis_p(_,int succ)                                                         \
   vis_p(_,int fail)                                                         \
    )                                                                        \
    {                                                                        \
        /* declarations */                                                   \
        type exp_val;                                                        \
        type des_val;                                                        \
        int ret;                                                             \
        inv(int succ = order;)                                               \
        inv(int fail = patomic_cmpxchg_fail_order(order);)                   \
        /* assertions */                                                     \
        patomic_assert_unreachable(obj != NULL);                             \
        patomic_assert_unreachable(expected != NULL);                        \
        patomic_assert_unreachable(desired != NULL);                         \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));              \
        patomic_assert_unreachable(patomic_is_valid_order(succ));            \
        patomic_assert_unreachable(patomic_is_valid_fail_order(succ, fail)); \
        /* operation */                                                      \
        patomic_ignored_memcpy(&exp_val, expected, sizeof(type));            \
        patomic_ignored_memcpy(&des_val, desired, sizeof(type));             \
        ret = (int) atomic_compare_exchange_weak_explicit(                   \
            (volatile _Atomic(type) *) obj,                                  \
            &exp_val,                                                        \
            des_val,                                                         \
            succ,                                                            \
            fail                                                             \
        );                                                                   \
        patomic_ignored_memcpy(expected, &exp_val, sizeof(type));            \
        return ret;                                                          \
    }                                                                        \
    static PATOMIC_FORCE_INLINE int                                          \
    patomic_opimpl_cmpxchg_strong_##name(                                    \
        volatile void *obj                                                   \
        ,void *expected                                                      \
        ,const void *desired                                                 \
   vis_p(_,int succ)                                                         \
   vis_p(_,int fail)                                                         \
    )                                                                        \
    {                                                                        \
        /* declarations */                                                   \
        type exp_val;                                                        \
        type des_val;                                                        \
        int ret;                                                             \
        inv(int succ = order;)                                               \
        inv(int fail = patomic_cmpxchg_fail_order(order);)                   \
        /* assertions */                                                     \
        patomic_assert_unreachable(obj != NULL);                             \
        patomic_assert_unreachable(expected != NULL);                        \
        patomic_assert_unreachable(desired != NULL);                         \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));              \
        patomic_assert_unreachable(patomic_is_valid_order(succ));            \
        patomic_assert_unreachable(patomic_is_valid_fail_order(succ, fail)); \
        /* operation */                                                      \
        patomic_ignored_memcpy(&exp_val, expected, sizeof(type));            \
        patomic_ignored_memcpy(&des_val, desired, sizeof(type));             \
        ret = (int) atomic_compare_exchange_weak_explicit(                   \
            (volatile _Atomic(type) *) obj,                                  \
            &exp_val,                                                        \
            des_val,                                                         \
            succ,                                                            \
            fail                                                             \
        );                                                                   \
        patomic_ignored_memcpy(expected, &exp_val, sizeof(type));            \
        return ret;                                                          \
    }                                                                        \
    static patomic_##opsk##_xchg_t                                           \
    patomic_ops_xchg_create_##name(void)                                     \
    {                                                                        \
        patomic_##opsk##_xchg_t pao;                                         \
        pao.fp_exchange = patomic_opimpl_exchange_##name;                    \
        pao.fp_cmpxchg_weak = patomic_opimpl_cmpxchg_weak_##name;            \
        pao.fp_cmpxchg_strong = patomic_opimpl_cmpxchg_strong_##name;        \
        return pao;                                                          \
    }


/*
 * BITWISE:
 * - bit_test (y)
 * - bit_test_compl (y)
 * - bit_test_set (y)
 * - bit_test_reset (y)
 *
 * OPS SECTIONS:
 * - test: relaxed, consume, acquire, seq_cst
 * - test_modify: all
 *
 * CREATE SECTIONS:
 * - nrar: no {release, acq_rel} orders
 * - any: all orders permitted
 *
 * WARNING:
 * - type MUST be an unsigned integer type
 */
#define PATOMIC_DEFINE_BIT_TEST_OPS(type, name, order, vis_p)                  \
    static PATOMIC_FORCE_INLINE int                                            \
    patomic_opimpl_bit_test_##name(                                            \
        const volatile void *obj                                               \
        ,int offset                                                            \
   vis_p(_,int order)                                                          \
    )                                                                          \
    {                                                                          \
        /* declarations */                                                     \
        type mask;                                                             \
        type val;                                                              \
        /* assertions */                                                       \
        patomic_assert_unreachable(obj != NULL);                               \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));                \
        patomic_assert_unreachable(offset >= 0);                               \
        patomic_assert_unreachable((CHAR_BIT * sizeof(type)) > (size_t)offset);\
        patomic_assert_unreachable(patomic_is_valid_load_order(order));        \
        /* operation */                                                        \
        mask = (type) (((type) 1u) << offset);                                 \
        val = atomic_load_explicit(                                            \
            (const volatile _Atomic(type) *) obj,                              \
            order                                                              \
        );                                                                     \
        return (val & mask) != 0;                                              \
    }

#define PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(type, name, order, vis_p)           \
    static PATOMIC_FORCE_INLINE int                                            \
    patomic_opimpl_bit_test_compl_##name(                                      \
        volatile void *obj                                                     \
        ,int offset                                                            \
   vis_p(_,int order)                                                          \
    )                                                                          \
    {                                                                          \
        /* declarations */                                                     \
        type expected;                                                         \
        type desired;                                                          \
        type mask;                                                             \
        int succ;                                                              \
        int fail;                                                              \
        /* assertions */                                                       \
        patomic_assert_unreachable(obj != NULL);                               \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));                \
        patomic_assert_unreachable(offset >= 0);                               \
        patomic_assert_unreachable((CHAR_BIT * sizeof(type)) > (size_t)offset);\
        patomic_assert_unreachable(patomic_is_valid_order(order));             \
        /* operation */                                                        \
        /* setup memory orders and mask */                                     \
        mask = (type) (((type) 1u) << offset);                                 \
        succ = order;                                                          \
        fail = patomic_cmpxchg_fail_order(order);                              \
        /* load initial value */                                               \
        expected = atomic_load_explicit(                                       \
            (const volatile _Atomic(type) *) obj,                              \
            fail                                                               \
        );                                                                     \
        /* cas loop */                                                         \
        do {                                                                   \
            /* complement bit at offset */                                     \
            desired = expected;                                                \
            desired ^= mask;                                                   \
        }                                                                      \
        while (!atomic_compare_exchange_weak_explicit(                         \
            (volatile _Atomic(type) *) obj,                                    \
            &expected,                                                         \
            desired,                                                           \
            succ,                                                              \
            fail                                                               \
        ));                                                                    \
        /* return old bit value */                                             \
        return (expected & mask) != 0;                                         \
    }                                                                          \
    static PATOMIC_FORCE_INLINE int                                            \
    patomic_opimpl_bit_test_set_##name(                                        \
        volatile void *obj                                                     \
        ,int offset                                                            \
   vis_p(_,int order)                                                          \
    )                                                                          \
    {                                                                          \
        /* declarations */                                                     \
        type expected;                                                         \
        type desired;                                                          \
        type mask;                                                             \
        int succ;                                                              \
        int fail;                                                              \
        /* assertions */                                                       \
        patomic_assert_unreachable(obj != NULL);                               \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));                \
        patomic_assert_unreachable(offset >= 0);                               \
        patomic_assert_unreachable((CHAR_BIT * sizeof(type)) > (size_t)offset);\
        patomic_assert_unreachable(patomic_is_valid_order(order));             \
        /* operation */                                                        \
        /* setup memory orders and mask */                                     \
        mask = (type) (((type) 1u) << offset);                                 \
        succ = order;                                                          \
        fail = patomic_cmpxchg_fail_order(order);                              \
        /* load initial value */                                               \
        expected = atomic_load_explicit(                                       \
            (const volatile _Atomic(type) *) obj,                              \
            fail                                                               \
        );                                                                     \
        /* cas loop */                                                         \
        do {                                                                   \
            /* set bit at offset */                                            \
            desired = expected;                                                \
            desired |= mask;                                                   \
        }                                                                      \
        while (!atomic_compare_exchange_weak_explicit(                         \
            (volatile _Atomic(type) *) obj,                                    \
            &expected,                                                         \
            desired,                                                           \
            succ,                                                              \
            fail                                                               \
        ));                                                                    \
        /* return old bit value */                                             \
        return (expected & mask) != 0;                                         \
    }                                                                          \
    static PATOMIC_FORCE_INLINE int                                            \
    patomic_opimpl_bit_test_reset_##name(                                      \
        volatile void *obj                                                     \
        ,int offset                                                            \
   vis_p(_,int order)                                                          \
    )                                                                          \
    {                                                                          \
        /* declarations */                                                     \
        type expected;                                                         \
        type desired;                                                          \
        type mask;                                                             \
        int succ;                                                              \
        int fail;                                                              \
        /* assertions */                                                       \
        patomic_assert_unreachable(obj != NULL);                               \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));                \
        patomic_assert_unreachable(offset >= 0);                               \
        patomic_assert_unreachable((CHAR_BIT * sizeof(type)) > (size_t)offset);\
        patomic_assert_unreachable(patomic_is_valid_order(order));             \
        /* operation */                                                        \
        /* setup memory orders and mask */                                     \
        mask = (type) (((type) 1u) << offset);                                 \
        succ = order;                                                          \
        fail = patomic_cmpxchg_fail_order(order);                              \
        /* load initial value */                                               \
        expected = atomic_load_explicit(                                       \
            (const volatile _Atomic(type) *) obj,                              \
            fail                                                               \
        );                                                                     \
        /* cas loop */                                                         \
        do {                                                                   \
            /* clear bit at offset */                                          \
            desired = expected;                                                \
            desired &= (type) (~mask);                                         \
        }                                                                      \
        while (!atomic_compare_exchange_weak_explicit(                         \
            (volatile _Atomic(type) *) obj,                                    \
            &expected,                                                         \
            desired,                                                           \
            succ,                                                              \
            fail                                                               \
        ));                                                                    \
        /* return old bit value */                                             \
        return (expected & mask) != 0;                                         \
    }

#define PATOMIC_DEFINE_BIT_OPS_CREATE_ANY(type, name, order, vis_p, opsk) \
    PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(type, name, order, vis_p)          \
    static patomic_##opsk##_bitwise_t                                     \
    patomic_ops_bitwise_create_##name(void)                               \
    {                                                                     \
        patomic_##opsk##_bitwise_t pao;                                   \
        pao.fp_test = NULL;                                               \
        pao.fp_test_compl = patomic_opimpl_bit_test_compl_##name;         \
        pao.fp_test_set = patomic_opimpl_bit_test_set_##name;             \
        pao.fp_test_reset = patomic_opimpl_bit_test_reset_##name;         \
        return pao;                                                       \
    }

/* NRAR: no release or acq_rel orders */
#define PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(type, name, order, vis_p, opsk) \
    PATOMIC_DEFINE_BIT_TEST_OPS(type, name, order, vis_p)                  \
    PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(type, name, order, vis_p)           \
    static patomic_##opsk##_bitwise_t                                      \
    patomic_ops_bitwise_create_##name(void)                                \
    {                                                                      \
        patomic_##opsk##_bitwise_t pao;                                    \
        pao.fp_test = patomic_opimpl_bit_test_##name;                      \
        pao.fp_test_compl = patomic_opimpl_bit_test_compl_##name;          \
        pao.fp_test_set = patomic_opimpl_bit_test_set_##name;              \
        pao.fp_test_reset = patomic_opimpl_bit_test_reset_##name;          \
        return pao;                                                        \
    }


/*
 * BINARY:
 * - (fetch_)or (y)
 * - (fetch_)xor (y)
 * - (fetch_)and (y)
 * - (fetch_)not (y)
 */
#define PATOMIC_DEFINE_BIN_OPS_CREATE(type, name, order, vis_p, opsk)     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_fetch_or_##name(                                       \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis_p(_,int order)                                                     \
        ,void *ret                                                        \
    )                                                                     \
    {                                                                     \
        /* declarations */                                                \
        type val;                                                         \
        type arg_val;                                                     \
        /* assertions */                                                  \
        patomic_assert_unreachable(obj != NULL);                          \
        patomic_assert_unreachable(arg != NULL);                          \
        patomic_assert_unreachable(ret != NULL);                          \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));           \
        patomic_assert_unreachable(patomic_is_valid_order(order));        \
        /* operation */                                                   \
        patomic_ignored_memcpy(&arg_val, arg, sizeof(type));              \
        val = atomic_fetch_or_explicit(                                   \
            (volatile _Atomic(type) *) obj,                               \
            arg_val,                                                      \
            order                                                         \
        );                                                                \
        patomic_ignored_memcpy(ret, &val, sizeof(type));                  \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_fetch_xor_##name(                                      \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis_p(_,int order)                                                     \
        ,void *ret                                                        \
    )                                                                     \
    {                                                                     \
        /* declarations */                                                \
        type val;                                                         \
        type arg_val;                                                     \
        /* assertions */                                                  \
        patomic_assert_unreachable(obj != NULL);                          \
        patomic_assert_unreachable(arg != NULL);                          \
        patomic_assert_unreachable(ret != NULL);                          \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));           \
        patomic_assert_unreachable(patomic_is_valid_order(order));        \
        /* operation */                                                   \
        patomic_ignored_memcpy(&arg_val, arg, sizeof(type));              \
        val = atomic_fetch_xor_explicit(                                  \
            (volatile _Atomic(type) *) obj,                               \
            arg_val,                                                      \
            order                                                         \
        );                                                                \
        patomic_ignored_memcpy(ret, &val, sizeof(type));                  \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_fetch_and_##name(                                      \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis_p(_,int order)                                                     \
        ,void *ret                                                        \
    )                                                                     \
    {                                                                     \
        /* declarations */                                                \
        type val;                                                         \
        type arg_val;                                                     \
        /* assertions */                                                  \
        patomic_assert_unreachable(obj != NULL);                          \
        patomic_assert_unreachable(arg != NULL);                          \
        patomic_assert_unreachable(ret != NULL);                          \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));           \
        patomic_assert_unreachable(patomic_is_valid_order(order));        \
        /* operation */                                                   \
        patomic_ignored_memcpy(&arg_val, arg, sizeof(type));              \
        val = atomic_fetch_and_explicit(                                  \
            (volatile _Atomic(type) *) obj,                               \
            arg_val,                                                      \
            order                                                         \
        );                                                                \
        patomic_ignored_memcpy(ret, &val, sizeof(type));                  \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_fetch_not_##name(                                      \
        volatile void *obj                                                \
   vis_p(_,int order)                                                     \
        ,void *ret                                                        \
    )                                                                     \
    {                                                                     \
        /* declarations */                                                \
        type expected;                                                    \
        type desired;                                                     \
        int succ;                                                         \
        int fail;                                                         \
        unsigned char const *src;                                         \
        unsigned char *begin;                                             \
        unsigned char const *const end = (unsigned char *)(&desired + 1); \
        /* assertions */                                                  \
        patomic_assert_unreachable(obj != NULL);                          \
        patomic_assert_unreachable(ret != NULL);                          \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));           \
        patomic_assert_unreachable(patomic_is_valid_order(order));        \
        /* operation */                                                   \
        /* setup memory orders */                                         \
        succ = order;                                                     \
        fail = patomic_cmpxchg_fail_order(order);                         \
        /* load initial value */                                          \
        expected = atomic_load_explicit(                                  \
            (const volatile _Atomic(type) *) obj,                         \
            fail                                                          \
        );                                                                \
        /* cas loop */                                                    \
        do {                                                              \
            /* "not" bytes to create desired */                           \
            src = (unsigned char *) &expected;                            \
            begin = (unsigned char *) &desired;                           \
            for (; begin != end; ++begin, ++src) {                        \
                *begin = (unsigned char ) ~(*src);                        \
            }                                                             \
        }                                                                 \
        while (!atomic_compare_exchange_weak_explicit(                    \
            (volatile _Atomic(type) *) obj,                               \
            &expected,                                                    \
            desired,                                                      \
            succ,                                                         \
            fail                                                          \
        ));                                                               \
        patomic_ignored_memcpy(ret, &expected, sizeof(type));             \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_or_##name(                                             \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis_p(_,int order)                                                     \
    )                                                                     \
    {                                                                     \
        /* declarations */                                                \
        type val;                                                         \
        /* assertions */                                                  \
        patomic_assert_unreachable(obj != NULL);                          \
        patomic_assert_unreachable(arg != NULL);                          \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));           \
        patomic_assert_unreachable(patomic_is_valid_order(order));        \
        /* operation */                                                   \
        patomic_opimpl_fetch_or_##name(                                   \
            obj                                                           \
            ,arg                                                          \
       vis_p(_,order)                                                     \
            ,&val                                                         \
        );                                                                \
        PATOMIC_IGNORE_UNUSED(val);                                       \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_xor_##name(                                            \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis_p(_,int order)                                                     \
    )                                                                     \
    {                                                                     \
        /* declarations */                                                \
        type val;                                                         \
        /* assertions */                                                  \
        patomic_assert_unreachable(obj != NULL);                          \
        patomic_assert_unreachable(arg != NULL);                          \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));           \
        patomic_assert_unreachable(patomic_is_valid_order(order));        \
        /* operation */                                                   \
        patomic_opimpl_fetch_xor_##name(                                  \
            obj                                                           \
            ,arg                                                          \
       vis_p(_,order)                                                     \
            ,&val                                                         \
        );                                                                \
        PATOMIC_IGNORE_UNUSED(val);                                       \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_and_##name(                                            \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis_p(_,int order)                                                     \
    )                                                                     \
    {                                                                     \
        /* declarations */                                                \
        type val;                                                         \
        /* assertions */                                                  \
        patomic_assert_unreachable(obj != NULL);                          \
        patomic_assert_unreachable(arg != NULL);                          \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));           \
        patomic_assert_unreachable(patomic_is_valid_order(order));        \
        /* operation */                                                   \
        patomic_opimpl_fetch_and_##name(                                  \
            obj                                                           \
            ,arg                                                          \
       vis_p(_,order)                                                     \
            ,&val                                                         \
        );                                                                \
        PATOMIC_IGNORE_UNUSED(val);                                       \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_not_##name(                                            \
        volatile void *obj                                                \
   vis_p(_,int order)                                                     \
    )                                                                     \
    {                                                                     \
        /* declarations */                                                \
        type val;                                                         \
        /* assertions */                                                  \
        patomic_assert_unreachable(obj != NULL);                          \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));           \
        patomic_assert_unreachable(patomic_is_valid_order(order));        \
        /* operation */                                                   \
        patomic_opimpl_fetch_not_##name(                                  \
            obj                                                           \
       vis_p(_,order)                                                     \
            ,&val                                                         \
        );                                                                \
        PATOMIC_IGNORE_UNUSED(val);                                       \
    }                                                                     \
    static patomic_##opsk##_binary_t                                      \
    patomic_ops_binary_create_##name(void)                                \
    {                                                                     \
        patomic_##opsk##_binary_t pao;                                    \
        pao.fp_or = patomic_opimpl_or_##name;                             \
        pao.fp_xor = patomic_opimpl_xor_##name;                           \
        pao.fp_and = patomic_opimpl_and_##name;                           \
        pao.fp_not = patomic_opimpl_not_##name;                           \
        pao.fp_fetch_or = patomic_opimpl_fetch_or_##name;                 \
        pao.fp_fetch_xor = patomic_opimpl_fetch_xor_##name;               \
        pao.fp_fetch_and = patomic_opimpl_fetch_and_##name;               \
        pao.fp_fetch_not = patomic_opimpl_fetch_not_##name;               \
        return pao;                                                       \
    }


/*
 * ARITHMETIC:
 * - (fetch_)add (y)
 * - (fetch_)sub (y)
 * - (fetch_)inc (y)
 * - (fetch_)dec (y)
 * - (fetch_)neg (y)
 */
#define PATOMIC_DEFINE_ARI_OPS_CREATE(type, name, order, vis_p, opsk, min) \
    static PATOMIC_FORCE_INLINE void                                       \
    patomic_opimpl_fetch_add_##name(                                       \
        volatile void *obj                                                 \
        ,const void *arg                                                   \
   vis_p(_,int order)                                                      \
        ,void *ret                                                         \
    )                                                                      \
    {                                                                      \
        /* declarations */                                                 \
        type val;                                                          \
        type arg_val;                                                      \
        /* assertions */                                                   \
        patomic_assert_unreachable(obj != NULL);                           \
        patomic_assert_unreachable(arg != NULL);                           \
        patomic_assert_unreachable(ret != NULL);                           \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));            \
        patomic_assert_unreachable(patomic_is_valid_order(order));         \
        /* operation */                                                    \
        patomic_ignored_memcpy(&arg_val, arg, sizeof(type));               \
        val = atomic_fetch_add_explicit(                                   \
            (volatile _Atomic(type) *) obj,                                \
            arg_val,                                                       \
            order                                                          \
        );                                                                 \
        patomic_ignored_memcpy(ret, &val, sizeof(type));                   \
    }                                                                      \
    static PATOMIC_FORCE_INLINE void                                       \
    patomic_opimpl_fetch_sub_##name(                                       \
        volatile void *obj                                                 \
        ,const void *arg                                                   \
   vis_p(_,int order)                                                      \
        ,void *ret                                                         \
    )                                                                      \
    {                                                                      \
        /* declarations */                                                 \
        type val;                                                          \
        type arg_val;                                                      \
        /* assertions */                                                   \
        patomic_assert_unreachable(obj != NULL);                           \
        patomic_assert_unreachable(arg != NULL);                           \
        patomic_assert_unreachable(ret != NULL);                           \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));            \
        patomic_assert_unreachable(patomic_is_valid_order(order));         \
        /* operation */                                                    \
        patomic_ignored_memcpy(&arg_val, arg, sizeof(type));               \
        val = atomic_fetch_sub_explicit(                                   \
            (volatile _Atomic(type) *) obj,                                \
            arg_val,                                                       \
            order                                                          \
        );                                                                 \
        patomic_ignored_memcpy(ret, &val, sizeof(type));                   \
    }                                                                      \
    static PATOMIC_FORCE_INLINE void                                       \
    patomic_opimpl_fetch_inc_##name(                                       \
        volatile void *obj                                                 \
   vis_p(_,int order)                                                      \
        ,void *ret                                                         \
    )                                                                      \
    {                                                                      \
        /* declarations */                                                 \
        type val;                                                          \
        /* assertions */                                                   \
        patomic_assert_unreachable(obj != NULL);                           \
        patomic_assert_unreachable(ret != NULL);                           \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));            \
        patomic_assert_unreachable(patomic_is_valid_order(order));         \
        /* operation */                                                    \
        val = atomic_fetch_add_explicit(                                   \
            (volatile _Atomic(type) *) obj,                                \
            (type) 1,                                                      \
            order                                                          \
        );                                                                 \
        patomic_ignored_memcpy(ret, &val, sizeof(type));                   \
    }                                                                      \
    static PATOMIC_FORCE_INLINE void                                       \
    patomic_opimpl_fetch_dec_##name(                                       \
        volatile void *obj                                                 \
   vis_p(_,int order)                                                      \
        ,void *ret                                                         \
    )                                                                      \
    {                                                                      \
        /* declarations */                                                 \
        type val;                                                          \
        /* assertions */                                                   \
        patomic_assert_unreachable(obj != NULL);                           \
        patomic_assert_unreachable(ret != NULL);                           \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));            \
        patomic_assert_unreachable(patomic_is_valid_order(order));         \
        /* operation */                                                    \
        val = atomic_fetch_sub_explicit(                                   \
            (volatile _Atomic(type) *) obj,                                \
            (type) 1,                                                      \
            order                                                          \
        );                                                                 \
        patomic_ignored_memcpy(ret, &val, sizeof(type));                   \
    }                                                                      \
    static PATOMIC_FORCE_INLINE void                                       \
    patomic_opimpl_fetch_neg_##name(                                       \
        volatile void *obj                                                 \
   vis_p(_,int order)                                                      \
        ,void *ret                                                         \
    )                                                                      \
    {                                                                      \
        /* declarations */                                                 \
        type expected;                                                     \
        type desired;                                                      \
        int succ;                                                          \
        int fail;                                                          \
        /* assertions */                                                   \
        patomic_assert_unreachable(obj != NULL);                           \
        patomic_assert_unreachable(ret != NULL);                           \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));            \
        patomic_assert_unreachable(patomic_is_valid_order(order));         \
        /* operation */                                                    \
        /* setup memory orders */                                          \
        succ = order;                                                      \
        fail = patomic_cmpxchg_fail_order(order);                          \
        /* load initial value */                                           \
        expected = atomic_load_explicit(                                   \
            (const volatile _Atomic(type) *) obj,                          \
            fail                                                           \
        );                                                                 \
        /* cas loop */                                                     \
        do {                                                               \
            /* unsigned */                                                 \
            if ((type) (min) == (type) 0) { desired = (type) -expected; }  \
            /* signed */                                                   \
            else {                                                         \
                /* twos complement */                                      \
                if (PATOMIC_HAVE_TWOS_COMPL) {                             \
                    /* -INT_MIN is UB but also equal to INT_MIN */         \
                    if ((type) (min) == expected) {                        \
                        /* value would be the same after negation */       \
                        /* we just need to enforce memory order */         \
                        if (succ > fail) {                                 \
                            PATOMIC_IGNORE_UNUSED(                         \
                                atomic_fetch_or_explicit(                  \
                                    (volatile _Atomic(type) *) obj,        \
                                    (type) 0,                              \
                                    succ                                   \
                            ));                                            \
                        }                                                  \
                        break;                                             \
                    }                                                      \
                    /* not INT_MIN so fine to negate */                    \
                    else { desired = (type) -expected; }                   \
                }                                                          \
                /* not twos complement so fine to negate */                \
                else { desired = (type) -expected; }                       \
            }                                                              \
        }                                                                  \
        while (!atomic_compare_exchange_weak_explicit(                     \
            (volatile _Atomic(type) *) obj,                                \
            &expected,                                                     \
            desired,                                                       \
            succ,                                                          \
            fail                                                           \
        ));                                                                \
        patomic_ignored_memcpy(ret, &expected, sizeof(type));              \
    }                                                                      \
    static PATOMIC_FORCE_INLINE void                                       \
    patomic_opimpl_add_##name(                                             \
        volatile void *obj                                                 \
        ,const void *arg                                                   \
   vis_p(_,int order)                                                      \
    )                                                                      \
    {                                                                      \
        /* declarations */                                                 \
        type val;                                                          \
        /* assertions */                                                   \
        patomic_assert_unreachable(obj != NULL);                           \
        patomic_assert_unreachable(arg != NULL);                           \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));            \
        patomic_assert_unreachable(patomic_is_valid_order(order));         \
        /* operation */                                                    \
        patomic_opimpl_fetch_add_##name(                                   \
            obj                                                            \
            ,arg                                                           \
       vis_p(_,order)                                                      \
            ,&val                                                          \
        );                                                                 \
        PATOMIC_IGNORE_UNUSED(val);                                        \
    }                                                                      \
    static PATOMIC_FORCE_INLINE void                                       \
    patomic_opimpl_sub_##name(                                             \
        volatile void *obj                                                 \
        ,const void *arg                                                   \
   vis_p(_,int order)                                                      \
    )                                                                      \
    {                                                                      \
        /* declarations */                                                 \
        type val;                                                          \
        /* assertions */                                                   \
        patomic_assert_unreachable(obj != NULL);                           \
        patomic_assert_unreachable(arg != NULL);                           \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));            \
        patomic_assert_unreachable(patomic_is_valid_order(order));         \
        /* operation */                                                    \
        patomic_opimpl_fetch_sub_##name(                                   \
            obj                                                            \
            ,arg                                                           \
       vis_p(_,order)                                                      \
            ,&val                                                          \
        );                                                                 \
        PATOMIC_IGNORE_UNUSED(val);                                        \
    }                                                                      \
    static PATOMIC_FORCE_INLINE void                                       \
    patomic_opimpl_inc_##name(                                             \
        volatile void *obj                                                 \
   vis_p(_,int order)                                                      \
    )                                                                      \
    {                                                                      \
        /* declarations */                                                 \
        type val;                                                          \
        /* assertions */                                                   \
        patomic_assert_unreachable(obj != NULL);                           \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));            \
        patomic_assert_unreachable(patomic_is_valid_order(order));         \
        /* operation */                                                    \
        patomic_opimpl_fetch_inc_##name(                                   \
            obj                                                            \
       vis_p(_,order)                                                      \
            ,&val                                                          \
        );                                                                 \
        PATOMIC_IGNORE_UNUSED(val);                                        \
    }                                                                      \
    static PATOMIC_FORCE_INLINE void                                       \
    patomic_opimpl_dec_##name(                                             \
        volatile void *obj                                                 \
   vis_p(_,int order)                                                      \
    )                                                                      \
    {                                                                      \
        /* declarations */                                                 \
        type val;                                                          \
        /* assertions */                                                   \
        patomic_assert_unreachable(obj != NULL);                           \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));            \
        patomic_assert_unreachable(patomic_is_valid_order(order));         \
        /* operation */                                                    \
        patomic_opimpl_fetch_dec_##name(                                   \
            obj                                                            \
       vis_p(_,order)                                                      \
            ,&val                                                          \
        );                                                                 \
        PATOMIC_IGNORE_UNUSED(val);                                        \
    }                                                                      \
    static PATOMIC_FORCE_INLINE void                                       \
    patomic_opimpl_neg_##name(                                             \
        volatile void *obj                                                 \
   vis_p(_,int order)                                                      \
    )                                                                      \
    {                                                                      \
        /* declarations */                                                 \
        type val;                                                          \
        /* assertions */                                                   \
        patomic_assert_unreachable(obj != NULL);                           \
        patomic_assert_unreachable_aligned(obj, _Atomic(type));            \
        patomic_assert_unreachable(patomic_is_valid_order(order));         \
        /* operation */                                                    \
        patomic_opimpl_fetch_neg_##name(                                   \
            obj                                                            \
       vis_p(_,order)                                                      \
            ,&val                                                          \
        );                                                                 \
        PATOMIC_IGNORE_UNUSED(val);                                        \
    }                                                                      \
    static patomic_##opsk##_arithmetic_t                                   \
    patomic_ops_arithmetic_create_##name(void)                             \
    {                                                                      \
        patomic_##opsk##_arithmetic_t pao;                                 \
        pao.fp_add = patomic_opimpl_add_##name;                            \
        pao.fp_sub = patomic_opimpl_sub_##name;                            \
        pao.fp_inc = patomic_opimpl_inc_##name;                            \
        pao.fp_dec = patomic_opimpl_dec_##name;                            \
        pao.fp_neg = patomic_opimpl_neg_##name;                            \
        pao.fp_fetch_add = patomic_opimpl_fetch_add_##name;                \
        pao.fp_fetch_sub = patomic_opimpl_fetch_sub_##name;                \
        pao.fp_fetch_inc = patomic_opimpl_fetch_inc_##name;                \
        pao.fp_fetch_dec = patomic_opimpl_fetch_dec_##name;                \
        pao.fp_fetch_neg = patomic_opimpl_fetch_neg_##name;                \
        return pao;                                                        \
    }


/*
 * CREATE STRUCTS
 *
 * MEMORY ORDER SUFFIXES
 * - ca: only {consume, acquire} orders
 * - r: only {release} orders
 * - ar: only {acq_rel} orders
 * - rsc: only {relaxed, seq_cst} orders
 * - explicit: explicit
 */
#define PATOMIC_DEFINE_OPS_CREATE_CA(type, name, order, vis_p, inv, opsk, min)      \
    /* no store in consume/acquire */                                               \
    PATOMIC_DEFINE_LOAD_OPS(unsigned type, u##name, order, vis_p)                   \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(unsigned type, u##name, order, vis_p, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(unsigned type, u##name, order, vis_p, opsk)  \
    PATOMIC_DEFINE_BIN_OPS_CREATE(unsigned type, u##name, order, vis_p, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(unsigned type, u##name, order, vis_p, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(signed type, s##name, order, vis_p, opsk, min)    \
    static patomic_##opsk##_t                                                       \
    patomic_ops_create_##name(void)                                                 \
    {                                                                               \
        patomic_##opsk##_t pao;                                                     \
        pao.fp_store = NULL;                                                        \
        pao.fp_load = patomic_opimpl_load_u##name;                                  \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                           \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                     \
        pao.binary_ops = patomic_ops_binary_create_u##name();                       \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();                 \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                   \
        return pao;                                                                 \
    }

#define PATOMIC_DEFINE_OPS_CREATE_R(type, name, order, vis_p, inv, opsk, min)       \
    PATOMIC_DEFINE_STORE_OPS(unsigned type, u##name, order, vis_p)                  \
    /* no load in release */                                                        \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(unsigned type, u##name, order, vis_p, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_ANY(unsigned type, u##name, order, vis_p, opsk)   \
    PATOMIC_DEFINE_BIN_OPS_CREATE(unsigned type, u##name, order, vis_p, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(unsigned type, u##name, order, vis_p, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(signed type, s##name, order, vis_p, opsk, min)    \
    static patomic_##opsk##_t                                                       \
    patomic_ops_create_##name(void)                                                 \
    {                                                                               \
        patomic_##opsk##_t pao;                                                     \
        pao.fp_store = patomic_opimpl_store_u##name;                                \
        pao.fp_load = NULL;                                                         \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                           \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                     \
        pao.binary_ops = patomic_ops_binary_create_u##name();                       \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();                 \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                   \
        return pao;                                                                 \
    }

#define PATOMIC_DEFINE_OPS_CREATE_AR(type, name, order, vis_p, inv, opsk, min)      \
    /* no store/load in acq_rel */                                                  \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(unsigned type, u##name, order, vis_p, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_ANY(unsigned type, u##name, order, vis_p, opsk)   \
    PATOMIC_DEFINE_BIN_OPS_CREATE(unsigned type, u##name, order, vis_p, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(unsigned type, u##name, order, vis_p, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(signed type, s##name, order, vis_p, opsk, min)    \
    static patomic_##opsk##_t                                                       \
    patomic_ops_create_##name(void)                                                 \
    {                                                                               \
        patomic_##opsk##_t pao;                                                     \
        pao.fp_store = NULL;                                                        \
        pao.fp_load = NULL;                                                         \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                           \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                     \
        pao.binary_ops = patomic_ops_binary_create_u##name();                       \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();                 \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                   \
        return pao;                                                                 \
    }

#define PATOMIC_DEFINE_OPS_CREATE_RSC(type, name, order, vis_p, inv, opsk, min)     \
    PATOMIC_DEFINE_STORE_OPS(unsigned type, u##name, order, vis_p)                  \
    PATOMIC_DEFINE_LOAD_OPS(unsigned type, u##name, order, vis_p)                   \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(unsigned type, u##name, order, vis_p, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(unsigned type, u##name, order, vis_p, opsk)  \
    PATOMIC_DEFINE_BIN_OPS_CREATE(unsigned type, u##name, order, vis_p, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(unsigned type, u##name, order, vis_p, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(signed type, s##name, order, vis_p, opsk, min)    \
    static patomic_##opsk##_t                                                       \
    patomic_ops_create_##name(void)                                                 \
    {                                                                               \
        patomic_##opsk##_t pao;                                                     \
        pao.fp_store = patomic_opimpl_store_u##name;                                \
        pao.fp_load = patomic_opimpl_load_u##name;                                  \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                           \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                     \
        pao.binary_ops = patomic_ops_binary_create_u##name();                       \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();                 \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                   \
        return pao;                                                                 \
    }


#define PATOMIC_DEFINE_OPS_CREATE_ALL(type, name, min)                     \
    PATOMIC_DEFINE_OPS_CREATE_CA(                                          \
        type, name##_acquire, memory_order_acquire, HIDE_P, SHOW, ops, min \
    )                                                                      \
    PATOMIC_DEFINE_OPS_CREATE_R(                                           \
        type, name##_release, memory_order_release, HIDE_P, SHOW, ops, min \
    )                                                                      \
    PATOMIC_DEFINE_OPS_CREATE_AR(                                          \
        type, name##_acq_rel, memory_order_acq_rel, HIDE_P, SHOW, ops, min \
    )                                                                      \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                         \
        type, name##_relaxed, memory_order_relaxed, HIDE_P, SHOW, ops, min \
    )                                                                      \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                         \
        type, name##_seq_cst, memory_order_seq_cst, HIDE_P, SHOW, ops, min \
    )                                                                      \
    /* all ops are in RSC */                                               \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                         \
        type, name##_explicit, order, SHOW_P, HIDE, ops_explicit, min      \
    )


#if ATOMIC_CHAR_LOCK_FREE
    PATOMIC_DEFINE_OPS_CREATE_ALL(char, char, SCHAR_MIN)
#endif
#if ATOMIC_SHORT_LOCK_FREE
    PATOMIC_DEFINE_OPS_CREATE_ALL(short, short, SHRT_MIN)
#endif
#if ATOMIC_INT_LOCK_FREE
    PATOMIC_DEFINE_OPS_CREATE_ALL(int, int, INT_MIN)
#endif
#if ATOMIC_LONG_LOCK_FREE
    PATOMIC_DEFINE_OPS_CREATE_ALL(long, long, LONG_MIN)
#endif
#if PATOMIC_HAVE_LONG_LONG
    #if ATOMIC_LLONG_LOCK_FREE
        PATOMIC_DEFINE_OPS_CREATE_ALL(long long, llong, LLONG_MIN)
    #endif
#endif


#define PATOMIC_SET_RET(type, name, width, order, ret)              \
        (((width) == sizeof(type)) &&                               \
         ((width) == sizeof(_Atomic(type))))                        \
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
    }

#define PATOMIC_SET_RET_EXPLICIT(type, name, width, ret) \
        (((width) == sizeof(type)) &&                    \
         ((width) == sizeof(_Atomic(type))))             \
    {                                                    \
        (ret) = patomic_ops_create_##name##_explicit();  \
    }

#define PATOMIC_SET_ALIGN(type, width, member)     \
        (((width) == sizeof(type)) &&              \
         ((width) == sizeof(_Atomic(type))))       \
    {                                              \
        (member) = patomic_alignof(_Atomic(type)); \
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
#if PATOMIC_HAVE_LONG_LONG
#if ATOMIC_LLONG_LOCK_FREE
    else if PATOMIC_SET_RET(long long, llong, byte_width, order, ret)
#endif
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
#if PATOMIC_HAVE_LONG_LONG
#if ATOMIC_LLONG_LOCK_FREE
    else if PATOMIC_SET_RET_EXPLICIT(long long, llong, byte_width, ret)
#endif
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
#if PATOMIC_HAVE_LONG_LONG
#if ATOMIC_LLONG_LOCK_FREE
    else if PATOMIC_SET_ALIGN(long long, byte_width, ret.recommended)
#endif
#endif
    else { ret.recommended = 1; }

    ret.minimum = ret.recommended;
    ret.size_within = 0;
    return ret;
}


patomic_t
patomic_impl_create_std(
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
patomic_impl_create_explicit_std(
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
patomic_impl_create_std(
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
patomic_impl_create_explicit_std(
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


#endif  /* PATOMIC_HAVE_STD_ATOMIC */


patomic_transaction_t
patomic_impl_create_transaction_std(
    int options
)
{
    patomic_transaction_t ret = {0};
    PATOMIC_IGNORE_UNUSED(options);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}
