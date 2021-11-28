#include "std.h"

#include <patomic/patomic_config.h>

#if PATOMIC_HAVE_STD_ATOMIC || defined(PATOMIC_TEST_C90)

#if PATOMIC_HAVE_STD_ATOMIC
    #include <stdatomic.h>
#else  /* testing c90 */
    #include <patomic/fake/stdatomic.h>
#endif

static const patomic_ops_t patomic_ops_NULL;
static const patomic_ops_explicit_t patomic_ops_explicit_NULL;

#include <assert.h>
#include <limits.h>
#include <string.h>

#include <patomic/patomic.h>

#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>

#if !PATOMIC_HAVE_ALIGNOF
    #if PATOMIC_HAVE_GNU_ALIGNOF_ALIGNED_ATTR
        #define _Alignof(t) __alignof__(t)
    #elif PPATOMIC_HAVE_MS_ALIGNOF_ALIGN_DSPC
        #define _Alignof(t) __alignof(t)
    #else
        #define _Alignof(t) sizeof(t)
    #endif
#endif

/* hide/show anything */
#define HIDE(x)
#define SHOW(x) x

/* hide/show function param with leading comma */
#define HIDE_P(x, y)
#define SHOW_P(x, y) ,y


/*
 * FUNCTION/STRUCT DEFINE MACRO PARAMS
 *
 * - type: operand type (i.e. int)
 * - name: explicit or memory order (e.g. acquire)
 * - order: memory order value (e.g. memory_order_acquire)
 * - vis: order parameter visibility (e.g. SHOW_P)
 * - inv: the inverse of vis, but for anything (e.g. HIDE, not HIDE_P)
 * - opsk: return type ops kind (e.g. ops_explicit)
 * - min: minimum value of arithmetic type (e.g. INT_MIN)
 */


/*
 * BASE:
 * - store (y)
 * - load (y)
 */
#define PATOMIC_DEFINE_STORE_OPS(type, name, order, vis)                \
    static PATOMIC_FORCE_INLINE void                                    \
    patomic_opimpl_store_##name(                                        \
        volatile void *obj                                              \
        ,const void *desired                                            \
   vis(_,int order)                                                     \
    )                                                                   \
    {                                                                   \
        type des_val;                                                   \
        assert(patomic_is_valid_store_order(order));                    \
        PATOMIC_IGNORE_UNUSED(memcpy(&des_val, desired, sizeof(type))); \
        atomic_store_explicit(                                          \
            (volatile _Atomic(type) *) obj,                             \
            des_val,                                                    \
            order                                                       \
        );                                                              \
    }                                                                   \

#define PATOMIC_DEFINE_LOAD_OPS(type, name, order, vis)         \
    static PATOMIC_FORCE_INLINE void                            \
    patomic_opimpl_load_##name(                                 \
        const volatile void *obj                                \
   vis(_,int order)                                             \
        ,void *ret                                              \
    )                                                           \
    {                                                           \
        type val;                                               \
        assert(patomic_is_valid_load_order(order));             \
        val = atomic_load_explicit(                             \
            (const volatile _Atomic(type) *) obj,               \
            order                                               \
        );                                                      \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type))); \
    }


/*
 * XCHG:
 * - exchange (y)
 * - compare_exchange_weak (y)
 * - compare_exchange_strong (y)
 */
#define PATOMIC_DEFINE_XCHG_OPS_CREATE(type, name, order, vis, inv, opsk) \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_exchange_##name(                                       \
        volatile void *obj                                                \
        ,const void *desired                                              \
   vis(_,int order)                                                       \
        ,void *ret                                                        \
    )                                                                     \
    {                                                                     \
        type val;                                                         \
        type des_val;                                                     \
        assert(patomic_is_valid_order(order));                            \
        PATOMIC_IGNORE_UNUSED(memcpy(&des_val, desired, sizeof(type)));   \
        val = atomic_exchange_explicit(                                   \
            (volatile _Atomic(type) *) obj,                               \
            des_val,                                                      \
            order                                                         \
        );                                                                \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type)));           \
    }                                                                     \
    static PATOMIC_FORCE_INLINE int                                       \
    patomic_opimpl_cmpxchg_weak_##name(                                   \
        volatile void *obj                                                \
        ,void *expected                                                   \
        ,const void *desired                                              \
   vis(_,int succ)                                                        \
   vis(_,int fail)                                                        \
    )                                                                     \
    {                                                                     \
        type exp_val;                                                     \
        type des_val;                                                     \
        int ret;                                                          \
        inv(int succ = order;)                                            \
        inv(int fail = patomic_cmpxchg_fail_order(order);)                \
        assert(patomic_is_valid_order(succ));                             \
        assert(patomic_is_valid_fail_order(succ, fail));                  \
        PATOMIC_IGNORE_UNUSED(memcpy(&exp_val, expected, sizeof(type)));  \
        PATOMIC_IGNORE_UNUSED(memcpy(&des_val, desired, sizeof(type)));   \
        ret = (int) atomic_compare_exchange_weak_explicit(                \
            (volatile _Atomic(type) *) obj,                               \
            &exp_val,                                                     \
            des_val,                                                      \
            succ,                                                         \
            fail                                                          \
        );                                                                \
        PATOMIC_IGNORE_UNUSED(memcpy(expected, &exp_val, sizeof(type)));  \
        return ret;                                                       \
    }                                                                     \
    static PATOMIC_FORCE_INLINE int                                       \
    patomic_opimpl_cmpxchg_strong_##name(                                 \
        volatile void *obj                                                \
        ,void *expected                                                   \
        ,const void *desired                                              \
   vis(_,int succ)                                                        \
   vis(_,int fail)                                                        \
    )                                                                     \
    {                                                                     \
        type exp_val;                                                     \
        type des_val;                                                     \
        int ret;                                                          \
        inv(int succ = order;)                                            \
        inv(int fail = patomic_cmpxchg_fail_order(order);)                \
        assert(patomic_is_valid_order(succ));                             \
        assert(patomic_is_valid_fail_order(succ, fail));                  \
        PATOMIC_IGNORE_UNUSED(memcpy(&exp_val, expected, sizeof(type)));  \
        PATOMIC_IGNORE_UNUSED(memcpy(&des_val, desired, sizeof(type)));   \
        ret = (int) atomic_compare_exchange_strong_explicit(              \
            (volatile _Atomic(type) *) obj,                               \
            &exp_val,                                                     \
            des_val,                                                      \
            succ,                                                         \
            fail                                                          \
        );                                                                \
        PATOMIC_IGNORE_UNUSED(memcpy(expected, &exp_val, sizeof(type)));  \
        return ret;                                                       \
    }                                                                     \
    static patomic_##opsk##_xchg_t                                        \
    patomic_ops_xchg_create_##name(void)                                  \
    {                                                                     \
        patomic_##opsk##_xchg_t pao;                                      \
        pao.fp_exchange = patomic_opimpl_exchange_##name;                 \
        pao.fp_cmpxchg_weak = patomic_opimpl_cmpxchg_weak_##name;         \
        pao.fp_cmpxchg_strong = patomic_opimpl_cmpxchg_strong_##name;     \
        return pao;                                                       \
    }

/*
 * BITWISE:
 * - bit_test (y)
 * - bit_test_complement (y)
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
 * NOTE:
 * - we make the assumption that type is always an unsigned integer type
 * - these will not work properly if type can be anything
 */
#define PATOMIC_DEFINE_BIT_TEST_OPS(type, name, order, vis)  \
    static PATOMIC_FORCE_INLINE int                          \
    patomic_opimpl_test_##name(                              \
        const volatile void *obj                             \
        ,int offset                                          \
   vis(_,int order)                                          \
    )                                                        \
    {                                                        \
        type mask;                                           \
        type val;                                            \
        assert(offset >= 0);                                 \
        assert((CHAR_BIT * sizeof(type)) > (size_t) offset); \
        assert(patomic_is_valid_load_order(order));          \
        mask = (type) (((type) 1u) << offset);               \
        val = atomic_load_explicit(                          \
            (const volatile _Atomic(type) *) obj,            \
            order                                            \
        );                                                   \
        return (val & mask) != 0;                            \
    }

#define PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(type, name, order, vis) \
    static PATOMIC_FORCE_INLINE int                                \
    patomic_opimpl_test_compl_##name(                              \
        volatile void *obj                                         \
        ,int offset                                                \
   vis(_,int order)                                                \
    )                                                              \
    {                                                              \
        /* declarations */                                         \
        type expected;                                             \
        type desired;                                              \
        type mask;                                                 \
        int succ;                                                  \
        int fail;                                                  \
        /* assertions */                                           \
        assert(offset >= 0);                                       \
        assert((CHAR_BIT * sizeof(type)) > (size_t) offset);       \
        assert(patomic_is_valid_order(order));                     \
        /* setup memory orders and mask */                         \
        mask = (type) (((type) 1u) << offset);                     \
        succ = order;                                              \
        fail = patomic_cmpxchg_fail_order(order);                  \
        /* load initial value */                                   \
        expected = atomic_load_explicit(                           \
            (const volatile _Atomic(type) *) obj,                  \
            fail                                                   \
        );                                                         \
        /* cas loop */                                             \
        do {                                                       \
            /* complement bit at offset */                         \
            desired = expected;                                    \
            desired ^= mask;                                       \
        }                                                          \
        while (!atomic_compare_exchange_weak_explicit(             \
            (volatile _Atomic(type) *) obj,                        \
            &expected,                                             \
            desired,                                               \
            succ,                                                  \
            fail                                                   \
        ));                                                        \
        /* return old bit value */                                 \
        return (expected & mask) != 0;                             \
    }                                                              \
    static PATOMIC_FORCE_INLINE int                                \
    patomic_opimpl_test_set_##name(                                \
        volatile void *obj                                         \
        ,int offset                                                \
   vis(_,int order)                                                \
    )                                                              \
    {                                                              \
        /* declarations */                                         \
        type expected;                                             \
        type desired;                                              \
        type mask;                                                 \
        int succ;                                                  \
        int fail;                                                  \
        /* assertions */                                           \
        assert(offset >= 0);                                       \
        assert((CHAR_BIT * sizeof(type)) > (size_t) offset);       \
        assert(patomic_is_valid_order(order));                     \
        /* setup memory orders and mask */                         \
        mask = (type) (((type) 1u) << offset);                     \
        succ = order;                                              \
        fail = patomic_cmpxchg_fail_order(order);                  \
        /* load initial value */                                   \
        expected = atomic_load_explicit(                           \
            (const volatile _Atomic(type) *) obj,                  \
            fail                                                   \
        );                                                         \
        /* cas loop */                                             \
        do {                                                       \
            /* set bit at offset */                                \
            desired = expected;                                    \
            desired |= mask;                                       \
        }                                                          \
        while (!atomic_compare_exchange_weak_explicit(             \
            (volatile _Atomic(type) *) obj,                        \
            &expected,                                             \
            desired,                                               \
            succ,                                                  \
            fail                                                   \
        ));                                                        \
        /* return old bit value */                                 \
        return (expected & mask) != 0;                             \
    }                                                              \
    static PATOMIC_FORCE_INLINE int                                \
    patomic_opimpl_test_reset_##name(                              \
        volatile void *obj                                         \
        ,int offset                                                \
   vis(_,int order)                                                \
    )                                                              \
    {                                                              \
        /* declarations */                                         \
        type expected;                                             \
        type desired;                                              \
        type mask;                                                 \
        type mask_inv;                                             \
        int succ;                                                  \
        int fail;                                                  \
        /* assertions */                                           \
        assert(offset >= 0);                                       \
        assert((CHAR_BIT * sizeof(type)) > (size_t) offset);       \
        assert(patomic_is_valid_order(order));                     \
        /* setup memory orders and masks */                        \
        mask = (type) (((type) 1u) << offset);                     \
        mask_inv = (type) (~mask);                                 \
        succ = order;                                              \
        fail = patomic_cmpxchg_fail_order(order);                  \
        /* load initial value */                                   \
        expected = atomic_load_explicit(                           \
            (const volatile _Atomic(type) *) obj,                  \
            fail                                                   \
        );                                                         \
        /* cas loop */                                             \
        do {                                                       \
            /* reset bit at offset */                              \
            desired = expected;                                    \
            desired &= mask_inv;                                   \
        }                                                          \
        while (!atomic_compare_exchange_weak_explicit(             \
            (volatile _Atomic(type) *) obj,                        \
            &expected,                                             \
            desired,                                               \
            succ,                                                  \
            fail                                                   \
        ));                                                        \
        /* return old bit value */                                 \
        return (expected & mask) != 0;                             \
    }

#define PATOMIC_DEFINE_BIT_OPS_CREATE_ANY(type, name, order, vis, opsk) \
    PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(type, name, order, vis)          \
    static patomic_##opsk##_bitwise_t                                   \
    patomic_ops_bitwise_create_##name(void)                             \
    {                                                                   \
        patomic_##opsk##_bitwise_t pao;                                 \
        pao.fp_test = NULL;                                             \
        pao.fp_test_compl = patomic_opimpl_test_compl_##name;           \
        pao.fp_test_set = patomic_opimpl_test_set_##name;               \
        pao.fp_test_reset = patomic_opimpl_test_reset_##name;           \
        return pao;                                                     \
    }

#define PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(type, name, order, vis, opsk) \
    PATOMIC_DEFINE_BIT_TEST_OPS(type, name, order, vis)                  \
    PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(type, name, order, vis)           \
    static patomic_##opsk##_bitwise_t                                    \
    patomic_ops_bitwise_create_##name(void)                              \
    {                                                                    \
        patomic_##opsk##_bitwise_t pao;                                  \
        pao.fp_test = patomic_opimpl_test_##name;                        \
        pao.fp_test_compl = patomic_opimpl_test_compl_##name;            \
        pao.fp_test_set = patomic_opimpl_test_set_##name;                \
        pao.fp_test_reset = patomic_opimpl_test_reset_##name;            \
        return pao;                                                      \
    }


/*
 * BINARY:
 * - (fetch_)or (y)
 * - (fetch_)xor (y)
 * - (fetch_)and (y)
 * - (fetch_)not (y)
 */
#define PATOMIC_DEFINE_BIN_OPS_CREATE(type, name, order, vis, opsk)       \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_fetch_or_##name(                                       \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis(_,int order)                                                       \
        ,void *ret                                                        \
    )                                                                     \
    {                                                                     \
        type val;                                                         \
        type arg_val;                                                     \
        assert(patomic_is_valid_order(order));                            \
        PATOMIC_IGNORE_UNUSED(memcpy(&arg_val, arg, sizeof(type)));       \
        val = atomic_fetch_or_explicit(                                   \
            (volatile _Atomic(type) *) obj,                               \
            arg_val,                                                      \
            order                                                         \
        );                                                                \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type)));           \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_or_##name(                                             \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis(_,int order)                                                       \
    )                                                                     \
    {                                                                     \
        type val;                                                         \
        /* assertion and arg memcpy in called function */                 \
        patomic_opimpl_fetch_or_##name(                                   \
            obj                                                           \
            ,arg                                                          \
       vis(_,order)                                                       \
            ,&val                                                         \
        );                                                                \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_fetch_xor_##name(                                      \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis(_,int order)                                                       \
        ,void *ret                                                        \
    )                                                                     \
    {                                                                     \
        type val;                                                         \
        type arg_val;                                                     \
        assert(patomic_is_valid_order(order));                            \
        PATOMIC_IGNORE_UNUSED(memcpy(&arg_val, arg, sizeof(type)));       \
        val = atomic_fetch_xor_explicit(                                  \
            (volatile _Atomic(type) *) obj,                               \
            arg_val,                                                      \
            order                                                         \
        );                                                                \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type)));           \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_xor_##name(                                            \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis(_,int order)                                                       \
    )                                                                     \
    {                                                                     \
        type val;                                                         \
        /* assertion and arg memcpy in called function */                 \
        patomic_opimpl_fetch_xor_##name(                                  \
            obj                                                           \
            ,arg                                                          \
       vis(_,order)                                                       \
            ,&val                                                         \
        );                                                                \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_fetch_and_##name(                                      \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis(_,int order)                                                       \
        ,void *ret                                                        \
    )                                                                     \
    {                                                                     \
        type val;                                                         \
        type arg_val;                                                     \
        assert(patomic_is_valid_order(order));                            \
        PATOMIC_IGNORE_UNUSED(memcpy(&arg_val, arg, sizeof(type)));       \
        val = atomic_fetch_and_explicit(                                  \
            (volatile _Atomic(type) *) obj,                               \
            arg_val,                                                      \
            order                                                         \
        );                                                                \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type)));           \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_and_##name(                                            \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis(_,int order)                                                       \
    )                                                                     \
    {                                                                     \
        type val;                                                         \
        /* assertion and arg memcpy in called function */                 \
        patomic_opimpl_fetch_and_##name(                                  \
            obj                                                           \
            ,arg                                                          \
       vis(_,order)                                                       \
            ,&val                                                         \
        );                                                                \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_fetch_not_##name(                                      \
        volatile void *obj                                                \
   vis(_,int order)                                                       \
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
        /* assertion */                                                   \
        assert(patomic_is_valid_order(order));                            \
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
                *begin = (unsigned char) ~(*src);                         \
            }                                                             \
        }                                                                 \
        while (!atomic_compare_exchange_weak_explicit(                    \
            (volatile _Atomic(type) *) obj,                               \
            &expected,                                                    \
            desired,                                                      \
            succ,                                                         \
            fail                                                          \
        ));                                                               \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &expected, sizeof(type)));      \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_not_##name(                                            \
        volatile void *obj                                                \
   vis(_,int order)                                                       \
    )                                                                     \
    {                                                                     \
        type val;                                                         \
        /* assertion in called function */                                \
        patomic_opimpl_fetch_not_##name(                                  \
            obj                                                           \
       vis(_,order)                                                       \
            ,&val                                                         \
        );                                                                \
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
#define PATOMIC_DEFINE_ARI_OPS_CREATE(type, name, order, vis, opsk, min) \
    static PATOMIC_FORCE_INLINE void                                     \
    patomic_opimpl_fetch_add_##name(                                     \
        volatile void *obj                                               \
        ,const void *arg                                                 \
   vis(_,int order)                                                      \
        ,void *ret                                                       \
    )                                                                    \
    {                                                                    \
        type val;                                                        \
        type arg_val;                                                    \
        assert(patomic_is_valid_order(order));                           \
        PATOMIC_IGNORE_UNUSED(memcpy(&arg_val, arg, sizeof(type)));      \
        val = atomic_fetch_add_explicit(                                 \
            (volatile _Atomic(type) *) obj,                              \
            arg_val,                                                     \
            order                                                        \
        );                                                               \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type)));          \
    }                                                                    \
    static PATOMIC_FORCE_INLINE void                                     \
    patomic_opimpl_add_##name(                                           \
        volatile void *obj                                               \
        ,const void *arg                                                 \
   vis(_,int order)                                                      \
    )                                                                    \
    {                                                                    \
        type val;                                                        \
        /* assertion and arg memcpy in called function */                \
        patomic_opimpl_fetch_add_##name(                                 \
            obj                                                          \
            ,arg                                                         \
       vis(_,order)                                                      \
            ,&val                                                        \
        );                                                               \
    }                                                                    \
    static PATOMIC_FORCE_INLINE void                                     \
    patomic_opimpl_fetch_sub_##name(                                     \
        volatile void *obj                                               \
        ,const void *arg                                                 \
   vis(_,int order)                                                      \
        ,void *ret                                                       \
    )                                                                    \
    {                                                                    \
        type val;                                                        \
        type arg_val;                                                    \
        assert(patomic_is_valid_order(order));                           \
        PATOMIC_IGNORE_UNUSED(memcpy(&arg_val, arg, sizeof(type)));      \
        val = atomic_fetch_sub_explicit(                                 \
            (volatile _Atomic(type) *) obj,                              \
            arg_val,                                                     \
            order                                                        \
        );                                                               \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type)));          \
    }                                                                    \
    static PATOMIC_FORCE_INLINE void                                     \
    patomic_opimpl_sub_##name(                                           \
        volatile void *obj                                               \
        ,const void *arg                                                 \
   vis(_,int order)                                                      \
    )                                                                    \
    {                                                                    \
        type val;                                                        \
        /* assertion and arg memcpy in called function */                \
        patomic_opimpl_fetch_sub_##name(                                 \
            obj                                                          \
            ,arg                                                         \
       vis(_,order)                                                      \
            ,&val                                                        \
        );                                                               \
    }                                                                    \
    static PATOMIC_FORCE_INLINE void                                     \
    patomic_opimpl_fetch_inc_##name(                                     \
        volatile void *obj                                               \
   vis(_,int order)                                                      \
        ,void *ret                                                       \
    )                                                                    \
    {                                                                    \
        type val;                                                        \
        assert(patomic_is_valid_order(order));                           \
        val = atomic_fetch_add_explicit(                                 \
            (volatile _Atomic(type) *) obj,                              \
            (type) 1,                                                    \
            order                                                        \
        );                                                               \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type)));          \
    }                                                                    \
    static PATOMIC_FORCE_INLINE void                                     \
    patomic_opimpl_inc_##name(                                           \
        volatile void *obj                                               \
   vis(_,int order)                                                      \
    )                                                                    \
    {                                                                    \
        type val;                                                        \
        /* assertion in called function */                               \
        patomic_opimpl_fetch_inc_##name(                                 \
            obj                                                          \
       vis(_,order)                                                      \
            ,&val                                                        \
        );                                                               \
    }                                                                    \
    static PATOMIC_FORCE_INLINE void                                     \
    patomic_opimpl_fetch_dec_##name(                                     \
        volatile void *obj                                               \
   vis(_,int order)                                                      \
        ,void *ret                                                       \
    )                                                                    \
    {                                                                    \
        type val;                                                        \
        assert(patomic_is_valid_order(order));                           \
        val = atomic_fetch_sub_explicit(                                 \
            (volatile _Atomic(type) *) obj,                              \
            (type) 1,                                                    \
            order                                                        \
        );                                                               \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &val, sizeof(type)));          \
    }                                                                    \
    static PATOMIC_FORCE_INLINE void                                     \
    patomic_opimpl_dec_##name(                                           \
        volatile void *obj                                               \
   vis(_,int order)                                                      \
    )                                                                    \
    {                                                                    \
        type val;                                                        \
        /* assertion in called function */                               \
        patomic_opimpl_fetch_dec_##name(                                 \
            obj                                                          \
       vis(_,order)                                                      \
            ,&val                                                        \
        );                                                               \
    }                                                                    \
    static PATOMIC_FORCE_INLINE void                                     \
    patomic_opimpl_fetch_neg_##name(                                     \
        volatile void *obj                                               \
   vis(_,int order)                                                      \
        ,void *ret                                                       \
    )                                                                    \
    {                                                                    \
        /* declarations */                                               \
        type expected;                                                   \
        type desired;                                                    \
        int succ;                                                        \
        int fail;                                                        \
        /* assertion */                                                  \
        assert(patomic_is_valid_order(order));                           \
        /* setup memory orders */                                        \
        succ = order;                                                    \
        fail = patomic_cmpxchg_fail_order(order);                        \
        /* load initial value */                                         \
        expected = atomic_load_explicit(                                 \
            (const volatile _Atomic(type) *) obj,                        \
            fail                                                         \
        );                                                               \
        /* cas loop */                                                   \
        do {                                                             \
            /* unsigned */                                               \
            if ((type) (min) == (type) 0) { desired = (type)-expected; } \
            /* signed */                                                 \
            else {                                                       \
                /* twos complement */                                    \
                if (PATOMIC_HAVE_TWOS_COMPL) {                           \
                    /* -INT_MIN is UB but also evaluates to INT_MIN */   \
                    if ((type) (min) == expected) {                      \
                        /* value would be the same after negation */     \
                        /* enforce memory order */                       \
                        if (succ > fail) {                               \
                            PATOMIC_IGNORE_UNUSED(                       \
                                atomic_fetch_or_explicit(                \
                                    (volatile _Atomic(type) *) obj,      \
                                    (type) 0,                            \
                                    succ                                 \
                            ));                                          \
                        }                                                \
                        break;                                           \
                    }                                                    \
                    /* not INT_MIN so fine to negate */                  \
                    else { desired = (type) -expected; }                 \
                }                                                        \
                /* not twos complement (ones compl or sign-magnitude) */ \
                else { desired = (type) -expected; }                     \
            }                                                            \
        }                                                                \
        while (!atomic_compare_exchange_weak_explicit(                   \
            (volatile _Atomic(type) *) obj,                              \
            &expected,                                                   \
            desired,                                                     \
            succ,                                                        \
            fail                                                         \
        ));                                                              \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &expected, sizeof(type)));     \
    }                                                                    \
    static PATOMIC_FORCE_INLINE void                                     \
    patomic_opimpl_neg_##name(                                           \
        volatile void *obj                                               \
   vis(_,int order)                                                      \
    )                                                                    \
    {                                                                    \
        type val;                                                        \
        /* assertion in called function */                               \
        patomic_opimpl_fetch_neg_##name(                                 \
            obj                                                          \
       vis(_,order)                                                      \
            ,&val                                                        \
        );                                                               \
    }                                                                    \
    static patomic_##opsk##_arithmetic_t                                 \
    patomic_ops_arithmetic_create_##name(void)                           \
    {                                                                    \
        patomic_##opsk##_arithmetic_t pao;                               \
        pao.fp_add = patomic_opimpl_add_##name;                          \
        pao.fp_sub = patomic_opimpl_sub_##name;                          \
        pao.fp_inc = patomic_opimpl_inc_##name;                          \
        pao.fp_dec = patomic_opimpl_dec_##name;                          \
        pao.fp_neg = patomic_opimpl_neg_##name;                          \
        pao.fp_fetch_add = patomic_opimpl_fetch_add_##name;              \
        pao.fp_fetch_sub = patomic_opimpl_fetch_sub_##name;              \
        pao.fp_fetch_inc = patomic_opimpl_fetch_inc_##name;              \
        pao.fp_fetch_dec = patomic_opimpl_fetch_dec_##name;              \
        pao.fp_fetch_neg = patomic_opimpl_fetch_neg_##name;              \
        return pao;                                                      \
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
#define PATOMIC_DEFINE_OPS_CREATE_CA(type, name, order, vis, inv, opsk, min)      \
    /* no store in consume/acquire */                                             \
    PATOMIC_DEFINE_LOAD_OPS(unsigned type, u##name, order, vis)                   \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(unsigned type, u##name, order, vis, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(unsigned type, u##name, order, vis, opsk)  \
    PATOMIC_DEFINE_BIN_OPS_CREATE(unsigned type, u##name, order, vis, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(unsigned type, u##name, order, vis, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(signed type, s##name, order, vis, opsk, min)    \
    static patomic_##opsk##_t                                                     \
    patomic_ops_create_##name(void)                                               \
    {                                                                             \
        patomic_##opsk##_t pao;                                                   \
        pao.fp_store = NULL;                                                      \
        pao.fp_load = patomic_opimpl_load_u##name;                                \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                         \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                   \
        pao.binary_ops = patomic_ops_binary_create_u##name();                     \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();               \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                 \
        return pao;                                                               \
    }

#define PATOMIC_DEFINE_OPS_CREATE_R(type, name, order, vis, inv, opsk, min)       \
    PATOMIC_DEFINE_STORE_OPS(unsigned type, u##name, order, vis)                  \
    /* no load in release */                                                      \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(unsigned type, u##name, order, vis, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_ANY(unsigned type, u##name, order, vis, opsk)   \
    PATOMIC_DEFINE_BIN_OPS_CREATE(unsigned type, u##name, order, vis, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(unsigned type, u##name, order, vis, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(signed type, s##name, order, vis, opsk, min)    \
    static patomic_##opsk##_t                                                     \
    patomic_ops_create_##name(void)                                               \
    {                                                                             \
        patomic_##opsk##_t pao;                                                   \
        pao.fp_store = patomic_opimpl_store_u##name;                              \
        pao.fp_load = NULL;                                                       \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                         \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                   \
        pao.binary_ops = patomic_ops_binary_create_u##name();                     \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();               \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                 \
        return pao;                                                               \
    }

#define PATOMIC_DEFINE_OPS_CREATE_AR(type, name, order, vis, inv, opsk, min)      \
    /* no store/load in acq_rel */                                                \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(unsigned type, u##name, order, vis, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_ANY(unsigned type, u##name, order, vis, opsk)   \
    PATOMIC_DEFINE_BIN_OPS_CREATE(unsigned type, u##name, order, vis, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(unsigned type, u##name, order, vis, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(signed type, s##name, order, vis, opsk, min)    \
    static patomic_##opsk##_t                                                     \
    patomic_ops_create_##name(void)                                               \
    {                                                                             \
        patomic_##opsk##_t pao;                                                   \
        pao.fp_store = NULL;                                                      \
        pao.fp_load = NULL;                                                       \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                         \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                   \
        pao.binary_ops = patomic_ops_binary_create_u##name();                     \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();               \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                 \
        return pao;                                                               \
    }

#define PATOMIC_DEFINE_OPS_CREATE_RSC(type, name, order, vis, inv, opsk, min)     \
    PATOMIC_DEFINE_STORE_OPS(unsigned type, u##name, order, vis)                  \
    PATOMIC_DEFINE_LOAD_OPS(unsigned type, u##name, order, vis)                   \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(unsigned type, u##name, order, vis, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(unsigned type, u##name, order, vis, opsk)  \
    PATOMIC_DEFINE_BIN_OPS_CREATE(unsigned type, u##name, order, vis, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(unsigned type, u##name, order, vis, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(signed type, s##name, order, vis, opsk, min)    \
    static patomic_##opsk##_t                                                     \
    patomic_ops_create_##name(void)                                               \
    {                                                                             \
        patomic_##opsk##_t pao;                                                   \
        pao.fp_store = patomic_opimpl_store_u##name;                              \
        pao.fp_load = patomic_opimpl_load_u##name;                                \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                         \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                   \
        pao.binary_ops = patomic_ops_binary_create_u##name();                     \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();               \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                 \
        return pao;                                                               \
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


#define PATOMIC_SET_RET(type, name, width, order, ret)                         \
       ((width == sizeof(_Atomic(type)))                                       \
        && (sizeof(_Atomic(type)) == sizeof(type))                             \
        && (_Alignof(_Atomic(type)) == _Alignof(type)))                        \
    {                                                                          \
        switch(order)                                                          \
        {                                                                      \
            case patomic_RELAXED: ret = patomic_ops_create_##name##_relaxed(); \
                                  break;                                       \
            case patomic_CONSUME:                                              \
            case patomic_ACQUIRE: ret = patomic_ops_create_##name##_acquire(); \
                                  break;                                       \
            case patomic_RELEASE: ret = patomic_ops_create_##name##_release(); \
                                  break;                                       \
            case patomic_ACQ_REL: ret = patomic_ops_create_##name##_acq_rel(); \
                                  break;                                       \
            case patomic_SEQ_CST: ret = patomic_ops_create_##name##_seq_cst(); \
                                  break;                                       \
            default: break;                                                    \
        }                                                                      \
    }

#define PATOMIC_SET_RET_EXPLICIT(type, name, width, ret)        \
       ((width == sizeof(_Atomic(type)))                        \
        && (sizeof(_Atomic(type)) == sizeof(type))              \
        && (_Alignof(_Atomic(type)) == _Alignof(type)))         \
    {                                                           \
        ret = patomic_ops_create_##name##_explicit();           \
    }


static patomic_ops_t
patomic_create_ops(
    size_t byte_width,
    patomic_memory_order_t order
)
{
    patomic_ops_t ret = patomic_ops_NULL;
    assert(patomic_is_valid_order((int) order));

    if PATOMIC_SET_RET(char, char, byte_width, order, ret)
    else if PATOMIC_SET_RET(short, short, byte_width, order, ret)
    else if PATOMIC_SET_RET(int, int, byte_width, order, ret)
    else if PATOMIC_SET_RET(long, long, byte_width, order, ret)
#if PATOMIC_HAVE_LONG_LONG
    else if PATOMIC_SET_RET(long long, llong, byte_width, order, ret)
#endif

    return ret;
}

static patomic_ops_explicit_t
patomic_create_ops_explicit(
    size_t byte_width
)
{
    patomic_ops_explicit_t ret = patomic_ops_explicit_NULL;

    if PATOMIC_SET_RET_EXPLICIT(char, char, byte_width, ret)
    else if PATOMIC_SET_RET_EXPLICIT(short, short, byte_width, ret)
    else if PATOMIC_SET_RET_EXPLICIT(int, int, byte_width, ret)
    else if PATOMIC_SET_RET_EXPLICIT(long, long, byte_width, ret)
#if PATOMIC_HAVE_LONG_LONG
    else if PATOMIC_SET_RET_EXPLICIT(long long, llong, byte_width, ret)
#endif

    return ret;
}

#define PATOMIC_SET_ALIGNOF(width, type, val) \
    ((width) == sizeof(type)) { (val) = _Alignof(type); }\

static patomic_align_t
patomic_create_align(
    size_t byte_width
)
{
    patomic_align_t pat;

    if PATOMIC_SET_ALIGNOF(byte_width, char, pat.recommended)
    else if PATOMIC_SET_ALIGNOF(byte_width, short, pat.recommended)
    else if PATOMIC_SET_ALIGNOF(byte_width, int, pat.recommended)
    else if PATOMIC_SET_ALIGNOF(byte_width, long, pat.recommended)
#if PATOMIC_HAVE_LONG_LONG
    else if PATOMIC_SET_ALIGNOF(byte_width, long long, pat.recommended)
#endif
    else { pat.recommended = 1; }

    pat.minimum = pat.recommended;
    pat.size_within = 0;
    return pat;
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

#include <patomic/macros/ignore_unused.h>

static const patomic_t patomic_NULL;
static const patomic_explicit_t patomic_explicit_NULL;

patomic_t
patomic_impl_create_std(
    size_t byte_width,
    patomic_memory_order_t order,
    int options
)
{
    patomic_t ret;
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(order);
    PATOMIC_IGNORE_UNUSED(options);
    ret = patomic_NULL;
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
    patomic_explicit_t ret;
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(options);
    ret = patomic_explicit_NULL;
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}

#endif
