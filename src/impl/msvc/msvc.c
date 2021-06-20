#include "msvc.h"

#include <assert.h>
#include <stddef.h>

#include <patomic/patomic.h>
#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>

static const patomic_t patomic_NULL;
static const patomic_explicit_t patomic_explicit_NULL;

#include "generic_uint.h"

#include "msvc_x86.h"
#include "msvc_arm.h"
#include "msvc_ext.h"


#if defined(PATOMIC_DEFINE_IL)

PATOMIC_DEFINE_IL(char, __int8, 8)
PATOMIC_DEFINE_IL(short, __int16, 16)
PATOMIC_DEFINE_IL(long, __int32, 32)
#ifndef _M_IX86
    PATOMIC_DEFINE_IL(__int64, __int64, 64)
#endif

/*
 * WARNING:
 * - 64bit ops for x86/ARM
 * - 128bit ops for x64/AMD64/ARM64
 * - patomic_il_load_128 is NOT const safe
 * - it casts away the const and uses cmpxchg internally
 * - DO NOT USE IT with const objects
 * - fp_load and fp_test are set to NULL for 128bit widths
 */
#if defined(PATOMIC_DEFINE_ILE)
    PATOMIC_DEFINE_ILE()
#endif


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
 * FUNCTION/STRUCT DEFINE MACRO PARAMS
 *
 * - width: operand bit width (e.g. 32)
 * - name: explicit or memory order plus bit width (e.g. explicit32)
 * - order: memory order value (e.g. memory_order_acquire)
 * - vis: order parameter visibility (e.g. SHOW_P)
 * - inv: the inverse of vis, but for anything (e.g. HIDE, not HIDE_P)
 * - opsk: return type ops kind (e.g. ops_explicit)
 */


/*
 * BASE:
 * - store (y)
 * - load (y)
 */
#define PATOMIC_DEFINE_STORE_OPS(width, name, order, vis) \
    static PATOMIC_FORCE_INLINE void                      \
    patomic_opimpl_store_##name(                          \
        volatile void *obj                                \
        ,const void *desired                              \
   vis(_,int order)                                       \
    )                                                     \
    {                                                     \
        assert(patomic_is_valid_store_order(order));      \
        patomic_il_store_##width(                         \
            obj,                                          \
            desired,                                      \
            order                                         \
        );                                                \
    }

#define PATOMIC_DEFINE_LOAD_OPS(width, name, order, vis)  \
    static PATOMIC_FORCE_INLINE void                      \
    patomic_opimpl_load_##name(                           \
        const volatile void *obj                          \
   vis(_,int order)                                       \
        ,void *ret                                        \
    )                                                     \
    {                                                     \
        assert(patomic_is_valid_load_order(order));       \
        patomic_il_load_##width(                          \
            obj,                                          \
            order,                                        \
            ret                                           \
        );                                                \
    }


/*
 * XCHG:
 * - exchange (y)
 * - compare_exchange_weak (y)
 * - compare_exchange_strong (y)
 */
#define PATOMIC_DEFINE_XCHG_OPS_CREATE(width, name, order, vis, inv, opsk) \
    static PATOMIC_FORCE_INLINE void                                       \
    patomic_opimpl_exchange_##name(                                        \
        volatile void *obj                                                 \
        ,const void *desired                                               \
   vis(_,int order)                                                        \
        ,void *ret                                                         \
    )                                                                      \
    {                                                                      \
        assert(patomic_is_valid_order(order));                             \
        patomic_il_exchange_##width(                                       \
            obj,                                                           \
            desired,                                                       \
            order,                                                         \
            ret                                                            \
        );                                                                 \
    }                                                                      \
    static PATOMIC_FORCE_INLINE int                                        \
    patomic_opimpl_cmpxchg_##name(                                         \
        volatile void *obj                                                 \
        ,void *expected                                                    \
        ,const void *desired                                               \
   vis(_,int succ)                                                         \
   vis(_,int fail)                                                         \
    )                                                                      \
    {                                                                      \
        inv(int succ = order;)                                             \
        inv(int fail = patomic_cmpxchg_fail_order(order);)                 \
        assert(patomic_is_valid_order(succ));                              \
        assert(patomic_is_valid_fail_order(succ, fail));                   \
        return patomic_il_compare_exchange_##width(                        \
            obj,                                                           \
            expected,                                                      \
            desired,                                                       \
            succ,                                                          \
            fail                                                           \
        );                                                                 \
    }                                                                      \
    static patomic_##opsk##_xchg_t                                         \
    patomic_ops_xchg_create_##name(void)                                   \
    {                                                                      \
        static const patomic_##opsk##_xchg_t patomic_xchg_NULL;            \
        patomic_##opsk##_xchg_t pao = patomic_xchg_NULL;                   \
        pao.fp_exchange = patomic_opimpl_exchange_##name;                  \
        pao.fp_cmpxchg_weak = patomic_opimpl_cmpxchg_##name;               \
        pao.fp_cmpxchg_strong = pao.fp_cmpxchg_weak;                       \
        return pao;                                                        \
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
 */
#define PATOMIC_DEFINE_BIT_TEST_OPS(width, name, order, vis) \
    static PATOMIC_FORCE_INLINE int                          \
    patomic_opimpl_test_##name(                              \
        const volatile void *obj                             \
        ,int offset                                          \
   vis(_,int order)                                          \
    )                                                        \
    {                                                        \
        /* declarations */                                   \
        patomic_uint##width##_t mask;                        \
        patomic_uint##width##_t val;                         \
        /* assertions */                                     \
        assert(offset >= 0);                                 \
        assert(width > offset);                              \
        assert(patomic_is_valid_load_order(order));          \
        /* setup mask */                                     \
        PATOMIC_UINT_SET_TO_ONE(width, mask);                \
        mask = PATOMIC_UINT_SHL(width, mask, offset);        \
        /* op and return */                                  \
        patomic_il_load_##width(                             \
            obj,                                             \
            order,                                           \
            &val                                             \
        );                                                   \
        mask = PATOMIC_UINT_AND(width, val, mask);           \
        return PATOMIC_UINT_NEQ_ZERO(width, mask);           \
    }

#define PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(width, name, order, vis) \
    static PATOMIC_FORCE_INLINE int                                 \
    patomic_opimpl_test_compl_##name(                               \
        volatile void *obj                                          \
        ,int offset                                                 \
   vis(_,int order)                                                 \
    )                                                               \
    {                                                               \
        /* declarations */                                          \
        patomic_uint##width##_t expected;                           \
        patomic_uint##width##_t desired;                            \
        patomic_uint##width##_t mask;                               \
        int succ;                                                   \
        int fail;                                                   \
        /* assertions */                                            \
        assert(offset >= 0);                                        \
        assert(width > offset);                                     \
        assert(patomic_is_valid_order(order));                      \
        /* setup memory orders and mask */                          \
        PATOMIC_UINT_SET_TO_ONE(width, mask);                       \
        mask = PATOMIC_UINT_SHL(width, mask, offset);               \
        succ = order;                                               \
        fail = patomic_cmpxchg_fail_order(order);                   \
        /* load initial value */                                    \
        patomic_il_load_##width(                                    \
            obj,                                                    \
            fail,                                                   \
            &expected                                               \
        );                                                          \
        /* cas loop */                                              \
        do {                                                        \
            /* complement bit at offset */                          \
            desired = expected;                                     \
            desired = PATOMIC_UINT_XOR(width, desired, mask);       \
        }                                                           \
        while (!patomic_il_compare_exchange_##width(                \
            obj,                                                    \
            &expected,                                              \
            &desired,                                               \
            succ,                                                   \
            fail                                                    \
        ));                                                         \
        /* return old bit value */                                  \
        mask = PATOMIC_UINT_AND(width, expected, mask);             \
        return PATOMIC_UINT_NEQ_ZERO(width, mask);                  \
    }                                                               \
    static PATOMIC_FORCE_INLINE int                                 \
    patomic_opimpl_test_set_##name(                                 \
        volatile void *obj                                          \
        ,int offset                                                 \
   vis(_,int order)                                                 \
    )                                                               \
    {                                                               \
        assert(offset >= 0);                                        \
        assert(width > offset);                                     \
        assert(patomic_is_valid_order(order));                      \
        return patomic_il_bit_test_and_set_##width(                 \
            obj,                                                    \
            offset,                                                 \
            order                                                   \
        );                                                          \
    }                                                               \
    static PATOMIC_FORCE_INLINE int                                 \
    patomic_opimpl_test_reset_##name(                               \
        volatile void *obj                                          \
        ,int offset                                                 \
   vis(_,int order)                                                 \
    )                                                               \
    {                                                               \
        assert(offset >= 0);                                        \
        assert(width > offset);                                     \
        assert(patomic_is_valid_order(order));                      \
        return patomic_il_bit_test_and_reset_##width(               \
            obj,                                                    \
            offset,                                                 \
            order                                                   \
        );                                                          \
    }

#define PATOMIC_DEFINE_BIT_OPS_CREATE_ANY(width, name, order, vis, opsk) \
    PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(width, name, order, vis)          \
    static patomic_##opsk##_bitwise_t                                    \
    patomic_ops_bitwise_create_##name(void)                              \
    {                                                                    \
        patomic_##opsk##_bitwise_t pao;                                  \
        pao.fp_test = NULL;                                              \
        pao.fp_test_compl = patomic_opimpl_test_compl_##name;            \
        pao.fp_test_set = patomic_opimpl_test_set_##name;                \
        pao.fp_test_reset = patomic_opimpl_test_reset_##name;            \
        return pao;                                                      \
    }

#define PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(width, name, order, vis, opsk) \
    PATOMIC_DEFINE_BIT_TEST_OPS(width, name, order, vis)                  \
    PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(width, name, order, vis)           \
    static patomic_##opsk##_bitwise_t                                     \
    patomic_ops_bitwise_create_##name(void)                               \
    {                                                                     \
        static const patomic_##opsk##_bitwise_t patomic_bitwise_NULL;     \
        patomic_##opsk##_bitwise_t pao = patomic_bitwise_NULL;            \
        pao.fp_test = patomic_opimpl_test_##name;                         \
        pao.fp_test_compl = patomic_opimpl_test_compl_##name;             \
        pao.fp_test_set = patomic_opimpl_test_set_##name;                 \
        pao.fp_test_reset = patomic_opimpl_test_reset_##name;             \
        return pao;                                                       \
    }


/*
 * BINARY:
 * - (fetch_)or (y)
 * - (fetch_)xor (y)
 * - (fetch_)and (y)
 * - (fetch_)not (y)
 */
#define PATOMIC_DEFINE_BIN_OPS_CREATE(width, name, order, vis, opsk)      \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_fetch_or_##name(                                       \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis(_,int order)                                                       \
        ,void *ret                                                        \
    )                                                                     \
    {                                                                     \
        assert(patomic_is_valid_order(order));                            \
        patomic_il_or_##width(                                            \
            obj,                                                          \
            arg,                                                          \
            order,                                                        \
            ret                                                           \
        );                                                                \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_or_##name(                                             \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis(_,int order)                                                       \
    )                                                                     \
    {                                                                     \
        patomic_uint##width##_t val;                                      \
        /* assertion in called function */                                \
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
        assert(patomic_is_valid_order(order));                            \
        patomic_il_xor_##width(                                           \
            obj,                                                          \
            arg,                                                          \
            order,                                                        \
            ret                                                           \
        );                                                                \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_xor_##name(                                            \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis(_,int order)                                                       \
    )                                                                     \
    {                                                                     \
        patomic_uint##width##_t val;                                      \
        /* assertion in called function */                                \
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
        assert(patomic_is_valid_order(order));                            \
        patomic_il_and_##width(                                           \
            obj,                                                          \
            arg,                                                          \
            order,                                                        \
            ret                                                           \
        );                                                                \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_and_##name(                                            \
        volatile void *obj                                                \
        ,const void *arg                                                  \
   vis(_,int order)                                                       \
    )                                                                     \
    {                                                                     \
        patomic_uint##width##_t val;                                      \
        /* assertion in called function */                                \
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
        patomic_uint##width##_t expected;                                 \
        patomic_uint##width##_t desired;                                  \
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
        patomic_il_load_##width(                                          \
            obj,                                                          \
            fail,                                                         \
            &expected                                                     \
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
        while(!patomic_il_compare_exchange_##width(                       \
            obj,                                                          \
            &expected,                                                    \
            &desired,                                                     \
            succ,                                                         \
            fail                                                          \
        ));                                                               \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &expected, (sizeof expected))); \
    }                                                                     \
    static PATOMIC_FORCE_INLINE void                                      \
    patomic_opimpl_not_##name(                                            \
        volatile void *obj                                                \
   vis(_,int order)                                                       \
    )                                                                     \
    {                                                                     \
        patomic_uint##width##_t val;                                      \
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
        static const patomic_##opsk##_binary_t patomic_binary_NULL;       \
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
#define PATOMIC_DEFINE_ARI_OPS_CREATE(width, name, order, vis, opsk)        \
    static PATOMIC_FORCE_INLINE void                                        \
    patomic_opimpl_fetch_add_##name(                                        \
        volatile void *obj                                                  \
        ,const void *arg                                                    \
   vis(_,int order)                                                         \
        ,void *ret                                                          \
    )                                                                       \
    {                                                                       \
        assert(patomic_is_valid_order(order));                              \
        patomic_il_exchange_add_##width(                                    \
            obj,                                                            \
            arg,                                                            \
            order,                                                          \
            ret                                                             \
        );                                                                  \
    }                                                                       \
    static PATOMIC_FORCE_INLINE void                                        \
    patomic_opimpl_add_##name(                                              \
        volatile void *obj                                                  \
        ,const void *arg                                                    \
   vis(_,int order)                                                         \
    )                                                                       \
    {                                                                       \
        patomic_uint##width##_t val;                                        \
        /* assertion in called function */                                  \
        patomic_opimpl_fetch_add_##name(                                    \
            obj                                                             \
            ,arg                                                            \
       vis(_,order)                                                         \
            ,&val                                                           \
        );                                                                  \
    }                                                                       \
    static PATOMIC_FORCE_INLINE void                                        \
    patomic_opimpl_fetch_sub_##name(                                        \
        volatile void *obj                                                  \
        ,const void *arg                                                    \
   vis(_,int order)                                                         \
        ,void *ret                                                          \
    )                                                                       \
    {                                                                       \
        patomic_uint##width##_t arg_val;                                    \
        assert(patomic_is_valid_order(order));                              \
        PATOMIC_IGNORE_UNUSED(memcpy(&arg_val, arg, sizeof arg_val));       \
        /* negation - msvc only supports 2's complement */                  \
        arg_val = PATOMIC_UINT_NEG(width, arg_val);                         \
        patomic_il_exchange_add_##width(                                    \
            obj,                                                            \
            &arg_val,                                                       \
            order,                                                          \
            ret                                                             \
        );                                                                  \
    }                                                                       \
    static PATOMIC_FORCE_INLINE void                                        \
    patomic_opimpl_sub_##name(                                              \
        volatile void *obj                                                  \
        ,const void *arg                                                    \
   vis(_,int order)                                                         \
    )                                                                       \
    {                                                                       \
        patomic_uint##width##_t val;                                        \
        /* assertion in called function */                                  \
        patomic_opimpl_fetch_sub_##name(                                    \
            obj                                                             \
            ,arg                                                            \
       vis(_,order)                                                         \
            ,&val                                                           \
        );                                                                  \
    }                                                                       \
    static PATOMIC_FORCE_INLINE void                                        \
    patomic_opimpl_fetch_inc_##name(                                        \
        volatile void *obj                                                  \
   vis(_,int order)                                                         \
        , void *ret                                                         \
    )                                                                       \
    {                                                                       \
        assert(patomic_is_valid_order(order));                              \
        patomic_il_increment_##width(                                       \
            obj,                                                            \
            order,                                                          \
            ret                                                             \
        );                                                                  \
    }                                                                       \
    static PATOMIC_FORCE_INLINE void                                        \
    patomic_opimpl_inc_##name(                                              \
        volatile void *obj                                                  \
   vis(_,int order)                                                         \
    )                                                                       \
    {                                                                       \
        patomic_uint##width##_t val;                                        \
        /* assertion in called function */                                  \
        patomic_opimpl_fetch_inc_##name(                                    \
            obj                                                             \
       vis(_,order)                                                         \
            ,&val                                                           \
        );                                                                  \
    }                                                                       \
    static PATOMIC_FORCE_INLINE void                                        \
    patomic_opimpl_fetch_dec_##name(                                        \
        volatile void *obj                                                  \
   vis(_,int order)                                                         \
        , void *ret                                                         \
    )                                                                       \
    {                                                                       \
        assert(patomic_is_valid_order(order));                              \
        patomic_il_decrement_##width(                                       \
            obj,                                                            \
            order,                                                          \
            ret                                                             \
        );                                                                  \
    }                                                                       \
    static PATOMIC_FORCE_INLINE void                                        \
    patomic_opimpl_dec_##name(                                              \
        volatile void *obj                                                  \
   vis(_,int order)                                                         \
    )                                                                       \
    {                                                                       \
        patomic_uint##width##_t val;                                        \
        /* assertion in called function */                                  \
        patomic_opimpl_fetch_dec_##name(                                    \
            obj                                                             \
       vis(_,order)                                                         \
            ,&val                                                           \
        );                                                                  \
    }                                                                       \
    static PATOMIC_FORCE_INLINE void                                        \
    patomic_opimpl_fetch_neg_##name(                                        \
        volatile void *obj                                                  \
   vis(_,int order)                                                         \
        ,void *ret                                                          \
    )                                                                       \
    {                                                                       \
        /* declarations */                                                  \
        patomic_uint##width##_t expected;                                   \
        patomic_uint##width##_t desired;                                    \
        int succ;                                                           \
        int fail;                                                           \
        /* assertion */                                                     \
        assert(patomic_is_valid_order(order));                              \
        /* setup memory orders */                                           \
        succ = order;                                                       \
        fail = patomic_cmpxchg_fail_order(order);                           \
        /* load initial value */                                            \
        patomic_il_load_##width(                                            \
            obj,                                                            \
            fail,                                                           \
            &expected                                                       \
        );                                                                  \
        /* cas loop */                                                      \
        do {                                                                \
            /* negation - msvc only supports 2's complement */              \
            desired = PATOMIC_UINT_NEG(width, expected);                    \
        }                                                                   \
        while (!patomic_il_compare_exchange_##width(                        \
            obj,                                                            \
            &expected,                                                      \
            &desired,                                                       \
            succ,                                                           \
            fail                                                            \
        ));                                                                 \
        PATOMIC_IGNORE_UNUSED(memcpy(ret, &expected, sizeof desired));      \
    }                                                                       \
    static PATOMIC_FORCE_INLINE void                                        \
    patomic_opimpl_neg_##name(                                              \
        volatile void *obj                                                  \
   vis(_,int order)                                                         \
    )                                                                       \
    {                                                                       \
        patomic_uint##width##_t val;                                        \
        /* assertion in called function */                                  \
        patomic_opimpl_fetch_neg_##name(                                    \
            obj                                                             \
       vis(_,order)                                                         \
            ,&val                                                           \
        );                                                                  \
    }                                                                       \
    static patomic_##opsk##_arithmetic_t                                    \
    patomic_ops_arithmetic_create_##name(void)                              \
    {                                                                       \
        static const patomic_##opsk##_arithmetic_t patomic_ari_NULL;        \
        patomic_##opsk##_arithmetic_t pao = patomic_ari_NULL;               \
        pao.fp_add = patomic_opimpl_add_##name;                             \
        pao.fp_sub = patomic_opimpl_sub_##name;                             \
        pao.fp_inc = patomic_opimpl_inc_##name;                             \
        pao.fp_dec = patomic_opimpl_dec_##name;                             \
        pao.fp_neg = patomic_opimpl_neg_##name;                             \
        pao.fp_fetch_add = patomic_opimpl_fetch_add_##name;                 \
        pao.fp_fetch_sub = patomic_opimpl_fetch_sub_##name;                 \
        pao.fp_fetch_inc = patomic_opimpl_fetch_inc_##name;                 \
        pao.fp_fetch_dec = patomic_opimpl_fetch_dec_##name;                 \
        pao.fp_fetch_neg = patomic_opimpl_fetch_neg_##name;                 \
        return pao;                                                         \
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
#define PATOMIC_DEFINE_OPS_CREATE_CA(width, name, order, vis, inv, opsk) \
    /* no store in consume/acquire */                                    \
    PATOMIC_DEFINE_LOAD_OPS(width, name, order, vis)                     \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(width, name, order, vis, inv, opsk)   \
    PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(width, name, order, vis, opsk)    \
    PATOMIC_DEFINE_BIN_OPS_CREATE(width, name, order, vis, opsk)         \
    PATOMIC_DEFINE_ARI_OPS_CREATE(width, name, order, vis, opsk)         \
    static patomic_##opsk##_t                                            \
    patomic_ops_create_##name(void)                                      \
    {                                                                    \
        patomic_##opsk##_t pao;                                          \
        pao.fp_store = NULL;                                             \
        pao.fp_load = patomic_opimpl_load_##name;                        \
        pao.xchg_ops = patomic_ops_xchg_create_##name();                 \
        pao.bitwise_ops = patomic_ops_bitwise_create_##name();           \
        pao.binary_ops = patomic_ops_binary_create_##name();             \
        pao.unsigned_ops = patomic_ops_arithmetic_create_##name();       \
        pao.signed_ops = pao.unsigned_ops;                               \
        return pao;                                                      \
    }

#define PATOMIC_DEFINE_OPS_CREATE_R(width, name, order, vis, inv, opsk) \
    PATOMIC_DEFINE_STORE_OPS(width, name, order, vis)                   \
    /* no load in release */                                            \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(width, name, order, vis, inv, opsk)  \
    PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(width, name, order, vis, opsk)   \
    PATOMIC_DEFINE_BIN_OPS_CREATE(width, name, order, vis, opsk)        \
    PATOMIC_DEFINE_ARI_OPS_CREATE(width, name, order, vis, opsk)        \
    static patomic_##opsk##_t                                           \
    patomic_ops_create_##name(void)                                     \
    {                                                                   \
        patomic_##opsk##_t pao;                                         \
        pao.fp_store = patomic_opimpl_store_##name;                     \
        pao.fp_load = NULL;                                             \
        pao.xchg_ops = patomic_ops_xchg_create_##name();                \
        pao.bitwise_ops = patomic_ops_bitwise_create_##name();          \
        pao.binary_ops = patomic_ops_binary_create_##name();            \
        pao.unsigned_ops = patomic_ops_arithmetic_create_##name();      \
        pao.signed_ops = pao.unsigned_ops;                              \
        return pao;                                                     \
    }

#define PATOMIC_DEFINE_OPS_CREATE_AR(width, name, order, vis, inv, opsk) \
    /* no store/load in acq_rel */                                       \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(width, name, order, vis, inv, opsk)   \
    PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(width, name, order, vis, opsk)    \
    PATOMIC_DEFINE_BIN_OPS_CREATE(width, name, order, vis, opsk)         \
    PATOMIC_DEFINE_ARI_OPS_CREATE(width, name, order, vis, opsk)         \
    static patomic_##opsk##_t                                            \
    patomic_ops_create_##name(void)                                      \
    {                                                                    \
        patomic_##opsk##_t pao;                                          \
        pao.fp_store = NULL;                                             \
        pao.fp_load = NULL;                                              \
        pao.xchg_ops = patomic_ops_xchg_create_##name();                 \
        pao.bitwise_ops = patomic_ops_bitwise_create_##name();           \
        pao.binary_ops = patomic_ops_binary_create_##name();             \
        pao.unsigned_ops = patomic_ops_arithmetic_create_##name();       \
        pao.signed_ops = pao.unsigned_ops;                               \
        return pao;                                                      \
    }

#define PATOMIC_DEFINE_OPS_CREATE_RSC(width, name, order, vis, inv, opsk) \
    PATOMIC_DEFINE_STORE_OPS(width, name, order, vis)                     \
    PATOMIC_DEFINE_LOAD_OPS(width, name, order, vis)                      \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(width, name, order, vis, inv, opsk)    \
    PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(width, name, order, vis, opsk)     \
    PATOMIC_DEFINE_BIN_OPS_CREATE(width, name, order, vis, opsk)          \
    PATOMIC_DEFINE_ARI_OPS_CREATE(width, name, order, vis, opsk)          \
    static patomic_##opsk##_t                                             \
    patomic_ops_create_##name(void)                                       \
    {                                                                     \
        patomic_##opsk##_t pao;                                           \
        pao.fp_store = patomic_opimpl_store_##name;                       \
        pao.fp_load = patomic_opimpl_load_##name;                         \
        pao.xchg_ops = patomic_ops_xchg_create_##name();                  \
        pao.bitwise_ops = patomic_ops_bitwise_create_##name();            \
        pao.binary_ops = patomic_ops_binary_create_##name();              \
        pao.unsigned_ops = patomic_ops_arithmetic_create_##name();        \
        pao.signed_ops = pao.unsigned_ops;                                \
        return pao;                                                       \
    }

#define PATOMIC_DEFINE_OPS_CREATE_ALL(width)                            \
    PATOMIC_DEFINE_OPS_CREATE_CA(                                       \
        width, width##_acquire, patomic_ACQUIRE, HIDE_P, SHOW, ops \
    )                                                                   \
    PATOMIC_DEFINE_OPS_CREATE_R(                                        \
        width, width##_release, patomic_RELEASE, HIDE_P, SHOW, ops \
    )                                                                   \
    PATOMIC_DEFINE_OPS_CREATE_AR(                                       \
        width, width##_acq_rel, patomic_ACQ_REL, HIDE_P, SHOW, ops \
    )                                                                   \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                      \
        width, width##_relaxed, patomic_RELAXED, HIDE_P, SHOW, ops \
    )                                                                   \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                      \
        width, width##_seq_cst, patomic_SEQ_CST, HIDE_P, SHOW, ops \
    )                                                                   \
    /* all ops are in RSC */                                            \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                      \
        width, width##_explicit, order, SHOW_P, HIDE, ops_explicit      \
    )

PATOMIC_DEFINE_OPS_CREATE_ALL(8)
PATOMIC_DEFINE_OPS_CREATE_ALL(16)
PATOMIC_DEFINE_OPS_CREATE_ALL(32)
PATOMIC_DEFINE_OPS_CREATE_ALL(64)
#if defined(_M_X64)||defined(_M_AMD64) || defined(_M_ARM64)
    PATOMIC_DEFINE_OPS_CREATE_ALL(128)
#endif


static const patomic_ops_t patomic_ops_NULL;
static const patomic_ops_explicit_t patomic_ops_explicit_NULL;


#define PATOMIC_SET_RET(width, order, ret)                                    \
    {switch (order)                                                           \
    {                                                                         \
        case patomic_RELAXED: (ret) = patomic_ops_create_##width##_relaxed(); \
                              break;                                          \
        case patomic_CONSUME:                                                 \
        case patomic_ACQUIRE: (ret) = patomic_ops_create_##width##_acquire(); \
                              break;                                          \
        case patomic_RELEASE: (ret) = patomic_ops_create_##width##_release(); \
                              break;                                          \
        case patomic_ACQ_REL: (ret) = patomic_ops_create_##width##_acq_rel(); \
                              break;                                          \
        case patomic_SEQ_CST: (ret) = patomic_ops_create_##width##_seq_cst(); \
                              break;                                          \
        default: (ret) = patomic_ops_NULL;                                    \
    }}

static patomic_ops_t
patomic_create_ops(
    size_t byte_width,
    patomic_memory_order_t order
)
{
    patomic_ops_t ret;
    assert(patomic_is_valid_order((int) order));

    switch (byte_width)
    {
#if defined(_M_X64)||defined(_M_AMD64) || defined(_M_ARM64)
        case 16: PATOMIC_SET_RET(128, order, ret) break;
#endif
        case 8: PATOMIC_SET_RET(64, order, ret) break;
        case 4: PATOMIC_SET_RET(32, order, ret) break;
        case 2: PATOMIC_SET_RET(16, order, ret) break;
        case 1: PATOMIC_SET_RET(8, order, ret) break;
        default: ret = patomic_ops_NULL;
    }

    /* patomic_il_load_128 is NOT const safe */
    if (byte_width == 16)
    {
        ret.fp_load = NULL;
        ret.bitwise_ops.fp_test = NULL;
    }

    return ret;
}

static patomic_ops_explicit_t
patomic_create_ops_explicit(
    size_t byte_width
)
{
    patomic_ops_explicit_t ret;

    switch (byte_width)
    {
#if defined(_M_X64)||defined(_M_AMD64) || defined(_M_ARM64)
        case 16: ret = patomic_ops_create_128_explicit(); break;
#endif
        case 8: ret = patomic_ops_create_64_explicit(); break;
        case 4: ret = patomic_ops_create_32_explicit(); break;
        case 2: ret = patomic_ops_create_16_explicit(); break;
        case 1: ret = patomic_ops_create_8_explicit(); break;
        default: ret = patomic_ops_explicit_NULL;
    }

    /* patomic_il_load_128 is NOT const safe */
    if (byte_width == 16)
    {
        ret.fp_load = NULL;
        ret.bitwise_ops.fp_test = NULL;
    }

    return ret;
}

static patomic_align_t
patomic_create_align(
    size_t byte_width
)
{
    patomic_align_t pat;

    switch (byte_width)
    {
#if defined(_M_X64)||defined(_M_AMD64) || defined(_M_ARM64)
        case 16:
#endif
        case 8:
        case 4:
        case 2: pat.recommended = byte_width; break;
        case 1:
        default: pat.recommended = 1;
    }

    pat.minimum = pat.recommended;
    pat.size_within = 0;
    return pat;
}

patomic_t
patomic_impl_create_msvc(
    size_t byte_width,
    patomic_memory_order_t order,
    int options
)
{
    patomic_t ret;
    PATOMIC_IGNORE_UNUSED(options);
    ret.ops = patomic_create_ops(byte_width, order);
    ret.align = patomic_create_align(byte_width);
    /* patomic_il_load_128 is NOT const safe */
    if (byte_width == 16)
    {
        assert(ret.ops.fp_load == NULL);
        assert(ret.ops.bitwise_ops.fp_test == NULL);
    }
    return ret;
}

patomic_explicit_t
patomic_impl_create_explicit_msvc(
    size_t byte_width,
    int options
)
{
    patomic_explicit_t ret;
    PATOMIC_IGNORE_UNUSED(options);
    ret.ops = patomic_create_ops_explicit(byte_width);
    ret.align = patomic_create_align(byte_width);
    /* patomic_il_load_128 is NOT const safe */
    if (byte_width == 16)
    {
        assert(ret.ops.fp_load == NULL);
        assert(ret.ops.bitwise_ops.fp_test == NULL);
    }
    return ret;
}

#else

patomic_t patomic_impl_create_msvc(
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
patomic_impl_create_explicit_msvc(
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

#endif  /* defined(PATOMIC_DEFINE_IL) */
