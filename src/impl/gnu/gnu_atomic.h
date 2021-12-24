#ifndef PATOMIC_IMPL_GNU_ATOMIC_H
#define PATOMIC_IMPL_GNU_ATOMIC_H

#include <patomic/patomic.h>
#include <patomic/patomic_config.h>


#if PATOMIC_HAVE_GNU_ATOMIC


#include <limits.h>
#include <stddef.h>

#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>
#include <patomic/macros/likely_unlikely.h>

#include <patomic/stdlib/stdint.h>

#include <patomic/wrapped/cmpxchg.h>
#include <patomic/wrapped/direct.h>
#include <patomic/wrapped/fetch.h>


#if !PATOMIC_HAVE_IR_TWOS_COMPL
    #error "GNU is not known to support non twos-complement architectures"
#endif


/* assumes twos-complement representation with same signed/unsigned range */
#if PATOMIC_STDINT_HAVE_I128 && !defined(INT128_MIN)
    #define UINT128_MAX ((patomic_i128_unsigned)((patomic_i128_signed)(-1)))
    #define INT128_MAX  ((patomic_i128_signed)(UINT128_MAX >> 1u))
    #define INT128_MIN  ((patomic_i128_signed)((-INT128_MAX) - 1))
#endif


#if PATOMIC_STDINT_HAVE_LLONG && defined(LLONG_MIN)
    #define PATOMIC_IMPL_GNU_HAVE_LLONG 1
#else
    #define PATOMIC_IMPL_GNU_HAVE_LLONG 0
#endif

#if PATOMIC_STDINT_HAVE_I128 && defined(INT128_MIN)
    #define PATOMIC_IMPL_GNU_HAVE_I128 1
#else
    #define PATOMIC_IMPL_GNU_HAVE_I128 0
#endif


/*
 * FUNCTION/STRUCT DEFINE MACRO PARAMS
 *
 * - type: operand type (e.g. int)
 * - name: 'explicit' or memory order (e.g. acquire)
 * - order: memory order value (e.g. __ATOMIC_ACQUIRE)
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
#define patomic_do_store_e(t,i,y,p,d,o) __atomic_store_n(p,d,o)
#define patomic_do_load_e(t,i,y,p,o,r) r = __atomic_load_n(p,o)

#define PATOMIC_DEFINE_STORE_OPS(type, name, order, vis_p) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(                \
        0, 0, patomic_do_store_e,                          \
        type, type,                                        \
        patomic_opimpl_store_##name,                       \
        order, vis_p                                       \
    )

#define PATOMIC_DEFINE_LOAD_OPS(type, name, order, vis_p) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(                \
        0, 0, patomic_do_load_e,                          \
        type, type,                                       \
        patomic_opimpl_load_##name,                       \
        order, vis_p                                      \
    )


/*
 * XCHG:
 * - exchange (y)
 * - compare_exchange_weak (y)
 * - compare_exchange_strong (y)
 */
#define patomic_do_exchange_e(t,i,y,p,d,o,r) r = __atomic_exchange_n(p,d,o)
#define patomic_do_cmpxchg_we(t,i,y,p,e,d,s,f,r) \
    r = (int) __atomic_compare_exchange_n(p,&e,d,1,s,f)
#define patomic_do_cmpxchg_se(t,i,y,p,e,d,s,f,r) \
    r = (int) __atomic_compare_exchange_n(p,&e,d,0,s,f)

#define PATOMIC_DEFINE_XCHG_OPS_CREATE(type, name, order, vis_p, inv, opsk) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(                              \
        0, 0, patomic_do_exchange_e,                                        \
        type, type,                                                         \
        patomic_opimpl_exchange_##name,                                     \
        order, vis_p                                                        \
    )                                                                       \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG_WEAK(                          \
        0, 0, patomic_do_cmpxchg_we,                                        \
        type, type,                                                         \
        patomic_opimpl_cmpxchg_weak_##name,                                 \
        order, vis_p, inv                                                   \
    )                                                                       \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG_STRONG(                        \
        0, 0, patomic_do_cmpxchg_se,                                        \
        type, type,                                                         \
        patomic_opimpl_cmpxchg_strong_##name,                               \
        order, vis_p, inv                                                   \
    )                                                                       \
    static patomic_##opsk##_xchg_t                                          \
    patomic_ops_xchg_create_##name(void)                                    \
    {                                                                       \
        patomic_##opsk##_xchg_t pao;                                        \
        pao.fp_exchange = patomic_opimpl_exchange_##name;                   \
        pao.fp_cmpxchg_weak = patomic_opimpl_cmpxchg_weak_##name;           \
        pao.fp_cmpxchg_strong = patomic_opimpl_cmpxchg_strong_##name;       \
        return pao;                                                         \
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
#define patomic_do_bt_e(t,i,y,p,x,o,r)                               \
    scratch = (t)((t)1u << (t)(x)); scratch &= __atomic_load_n(p,o); \
    r = (scratch != (t)0)
#define patomic_do_cmp_eqz(t,i,y,a,r) r = ((a) == (t)0)
#define patomic_do_ip_nth_bit_mask(t,i,y,m,n) m = (t)((t)1u << (t)(n));
#define patomic_do_ip_bit_or(t,i,y,a,b) a |= b
#define patomic_do_ip_bit_xor(t,i,y,a,b) a ^= b
#define patomic_do_ip_bit_and(t,i,y,a,b) a &= b
#define patomic_do_ip_bit_not(t,i,y,a) a = (t)~a

#define PATOMIC_DEFINE_BIT_TEST_OPS(type, name, order, vis_p) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST(                \
        0, 0, patomic_do_bt_e,                                \
        type, type,                                           \
        patomic_opimpl_bit_test_##name,                       \
        order, vis_p                                          \
    )

#define PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(type, name, order, vis_p) \
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_COMPL(                \
        0, 0, patomic_do_cmpxchg_we, patomic_do_cmp_eqz,             \
        patomic_do_ip_nth_bit_mask,                                  \
        patomic_do_ip_bit_and, patomic_do_ip_bit_xor,                \
        type, type,                                                  \
        patomic_opimpl_bit_test_compl_##name,                        \
        order, vis_p                                                 \
    )                                                                \
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_SET(                  \
        0, 0, patomic_do_cmpxchg_we, patomic_do_cmp_eqz,             \
        patomic_do_ip_nth_bit_mask,                                  \
        patomic_do_ip_bit_and, patomic_do_ip_bit_or,                 \
        type, type,                                                  \
        patomic_opimpl_bit_test_set_##name,                          \
        order, vis_p                                                 \
    )                                                                \
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_RESET(                \
        0, 0, patomic_do_cmpxchg_we, patomic_do_cmp_eqz,             \
        patomic_do_ip_nth_bit_mask,                                  \
        patomic_do_ip_bit_and, patomic_do_ip_bit_not,                \
        type, type,                                                  \
        patomic_opimpl_bit_test_reset_##name,                        \
        order, vis_p                                                 \
    )

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
#define patomic_do_or_fe(t,i,y,p,a,o,r) r = __atomic_fetch_or(p,a,o)
#define patomic_do_xor_fe(t,i,y,p,a,o,r) r = __atomic_fetch_xor(p,a,o)
#define patomic_do_and_fe(t,i,y,p,a,o,r) r = __atomic_fetch_and(p,a,o)

#define PATOMIC_DEFINE_BIN_OPS_CREATE(type, name, order, vis_p, opsk) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_OR(                        \
        0, 0, patomic_do_or_fe,                                       \
        type, type,                                                   \
        patomic_opimpl_fetch_or_##name,                               \
        order, vis_p                                                  \
    )                                                                 \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_XOR(                       \
        0, 0, patomic_do_xor_fe,                                      \
        type, type,                                                   \
        patomic_opimpl_fetch_xor_##name,                              \
        order, vis_p                                                  \
    )                                                                 \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_AND(                       \
        0, 0, patomic_do_and_fe,                                      \
        type, type,                                                   \
        patomic_opimpl_fetch_and_##name,                              \
        order, vis_p                                                  \
    )                                                                 \
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOT(                      \
        0, 0, patomic_do_cmpxchg_we, patomic_do_ip_bit_not,           \
        type, type,                                                   \
        patomic_opimpl_fetch_not_##name,                              \
        order, vis_p                                                  \
    )                                                                 \
    PATOMIC_WRAPPED_FETCH_DEFINE_OP_WITH_ARG(                         \
        patomic_opimpl_fetch_or_##name,                               \
        type, type,                                                   \
        patomic_opimpl_or_##name,                                     \
        vis_p                                                         \
    )                                                                 \
    PATOMIC_WRAPPED_FETCH_DEFINE_OP_WITH_ARG(                         \
        patomic_opimpl_fetch_xor_##name,                              \
        type, type,                                                   \
        patomic_opimpl_xor_##name,                                    \
        vis_p                                                         \
    )                                                                 \
    PATOMIC_WRAPPED_FETCH_DEFINE_OP_WITH_ARG(                         \
        patomic_opimpl_fetch_and_##name,                              \
        type, type,                                                   \
        patomic_opimpl_and_##name,                                    \
        vis_p                                                         \
    )                                                                 \
    PATOMIC_WRAPPED_FETCH_DEFINE_OP_NOARG(                            \
        patomic_opimpl_fetch_not_##name,                              \
        type, type,                                                   \
        patomic_opimpl_not_##name,                                    \
        vis_p                                                         \
    )                                                                 \
    static patomic_##opsk##_binary_t                                  \
    patomic_ops_binary_create_##name(void)                            \
    {                                                                 \
        patomic_##opsk##_binary_t pao;                                \
        pao.fp_or = patomic_opimpl_or_##name;                         \
        pao.fp_xor = patomic_opimpl_xor_##name;                       \
        pao.fp_and = patomic_opimpl_and_##name;                       \
        pao.fp_not = patomic_opimpl_not_##name;                       \
        pao.fp_fetch_or = patomic_opimpl_fetch_or_##name;             \
        pao.fp_fetch_xor = patomic_opimpl_fetch_xor_##name;           \
        pao.fp_fetch_and = patomic_opimpl_fetch_and_##name;           \
        pao.fp_fetch_not = patomic_opimpl_fetch_not_##name;           \
        return pao;                                                   \
    }


/*
 * ARITHMETIC:
 * - (fetch_)add (y)
 * - (fetch_)sub (y)
 * - (fetch_)inc (y)
 * - (fetch_)dec (y)
 * - (fetch_)neg (y)
 */
#define patomic_do_add_fe(t,i,y,m,p,a,o,r) r = __atomic_fetch_add(p,a,o)
#define patomic_do_sub_fe(t,i,y,m,p,a,o,r) r = __atomic_fetch_sub(p,a,o)
#define patomic_do_inc_fe(t,i,y,m,p,o,r) r = __atomic_fetch_add(p,(t)1u,o)
#define patomic_do_dec_fe(t,i,y,m,p,o,r) r = __atomic_fetch_sub(p,(t)1u,o)
#if PATOMIC_HAVE_IR_TWOS_COMPL
    #define patomic_do_ip_neg(t,i,y,m,a)                                      \
        if ( ((t)(m) == (t)0) ||             /* t is unsigned */              \
             PATOMIC_LIKELY((t)(m) != (a)) ) /* t is signed but a != T_MIN */ \
        { a = (t)(-(a)); }
#else
    #define patomic_do_ip_neg(t,i,y,m,a) a = (t)(-(a))
#endif

#define PATOMIC_DEFINE_ARI_OPS_CREATE(type, name, order, vis_p, opsk, min) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_ADD(                            \
        0, 0, patomic_do_add_fe,                                           \
        type, type,                                                        \
        patomic_opimpl_fetch_add_##name,                                   \
        order, vis_p, min                                                  \
    )                                                                      \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_SUB(                            \
        0, 0, patomic_do_sub_fe,                                           \
        type, type,                                                        \
        patomic_opimpl_fetch_sub_##name,                                   \
        order, vis_p, min                                                  \
    )                                                                      \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_INC(                            \
        0, 0, patomic_do_inc_fe,                                           \
        type, type,                                                        \
        patomic_opimpl_fetch_inc_##name,                                   \
        order, vis_p, min                                                  \
    )                                                                      \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_DEC(                            \
        0, 0, patomic_do_dec_fe,                                           \
        type, type,                                                        \
        patomic_opimpl_fetch_dec_##name,                                   \
        order, vis_p, min                                                  \
    )                                                                      \
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NEG(                           \
        0, 0, patomic_do_cmpxchg_we, patomic_do_ip_neg,                    \
        type, type,                                                        \
        patomic_opimpl_fetch_neg_##name,                                   \
        order, vis_p, min                                                  \
    )                                                                      \
    PATOMIC_WRAPPED_FETCH_DEFINE_OP_WITH_ARG(                              \
        patomic_opimpl_fetch_add_##name,                                   \
        type, type,                                                        \
        patomic_opimpl_add_##name,                                         \
        vis_p                                                              \
    )                                                                      \
    PATOMIC_WRAPPED_FETCH_DEFINE_OP_WITH_ARG(                              \
        patomic_opimpl_fetch_sub_##name,                                   \
        type, type,                                                        \
        patomic_opimpl_sub_##name,                                         \
        vis_p                                                              \
    )                                                                      \
    PATOMIC_WRAPPED_FETCH_DEFINE_OP_NOARG(                                 \
        patomic_opimpl_fetch_inc_##name,                                   \
        type, type,                                                        \
        patomic_opimpl_inc_##name,                                         \
        vis_p                                                              \
    )                                                                      \
    PATOMIC_WRAPPED_FETCH_DEFINE_OP_NOARG(                                 \
        patomic_opimpl_fetch_dec_##name,                                   \
        type, type,                                                        \
        patomic_opimpl_dec_##name,                                         \
        vis_p                                                              \
    )                                                                      \
    PATOMIC_WRAPPED_FETCH_DEFINE_OP_NOARG(                                 \
        patomic_opimpl_fetch_neg_##name,                                   \
        type, type,                                                        \
        patomic_opimpl_neg_##name,                                         \
        vis_p                                                              \
    )                                                                      \
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
#define PATOMIC_DEFINE_OPS_CREATE_CA(type, name, order, vis_p, inv, opsk, min)        \
    /* no store in consume/acquire */                                                 \
    PATOMIC_DEFINE_LOAD_OPS(type##_unsigned, u##name, order, vis_p)                   \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(type##_unsigned, u##name, order, vis_p, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(type##_unsigned, u##name, order, vis_p, opsk)  \
    PATOMIC_DEFINE_BIN_OPS_CREATE(type##_unsigned, u##name, order, vis_p, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(type##_unsigned, u##name, order, vis_p, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(type##_signed, s##name, order, vis_p, opsk, min)    \
    static patomic_##opsk##_t                                                         \
    patomic_ops_create_##name(void)                                                   \
    {                                                                                 \
        patomic_##opsk##_t pao;                                                       \
        pao.fp_store = NULL;                                                          \
        pao.fp_load = patomic_opimpl_load_u##name;                                    \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                             \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                       \
        pao.binary_ops = patomic_ops_binary_create_u##name();                         \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();                   \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                     \
        return pao;                                                                   \
    }

#define PATOMIC_DEFINE_OPS_CREATE_R(type, name, order, vis_p, inv, opsk, min)         \
    PATOMIC_DEFINE_STORE_OPS(type##_unsigned, u##name, order, vis_p)                  \
    /* no load in release */                                                          \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(type##_unsigned, u##name, order, vis_p, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_ANY(type##_unsigned, u##name, order, vis_p, opsk)   \
    PATOMIC_DEFINE_BIN_OPS_CREATE(type##_unsigned, u##name, order, vis_p, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(type##_unsigned, u##name, order, vis_p, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(type##_signed, s##name, order, vis_p, opsk, min)    \
    static patomic_##opsk##_t                                                         \
    patomic_ops_create_##name(void)                                                   \
    {                                                                                 \
        patomic_##opsk##_t pao;                                                       \
        pao.fp_store = patomic_opimpl_store_u##name;                                  \
        pao.fp_load = NULL;                                                           \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                             \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                       \
        pao.binary_ops = patomic_ops_binary_create_u##name();                         \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();                   \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                     \
        return pao;                                                                   \
    }

#define PATOMIC_DEFINE_OPS_CREATE_AR(type, name, order, vis_p, inv, opsk, min)        \
    /* no store/load in acq_rel */                                                    \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(type##_unsigned, u##name, order, vis_p, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_ANY(type##_unsigned, u##name, order, vis_p, opsk)   \
    PATOMIC_DEFINE_BIN_OPS_CREATE(type##_unsigned, u##name, order, vis_p, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(type##_unsigned, u##name, order, vis_p, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(type##_signed, s##name, order, vis_p, opsk, min)    \
    static patomic_##opsk##_t                                                         \
    patomic_ops_create_##name(void)                                                   \
    {                                                                                 \
        patomic_##opsk##_t pao;                                                       \
        pao.fp_store = NULL;                                                          \
        pao.fp_load = NULL;                                                           \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                             \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                       \
        pao.binary_ops = patomic_ops_binary_create_u##name();                         \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();                   \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                     \
        return pao;                                                                   \
    }

#define PATOMIC_DEFINE_OPS_CREATE_RSC(type, name, order, vis_p, inv, opsk, min)       \
    PATOMIC_DEFINE_STORE_OPS(type##_unsigned, u##name, order, vis_p)                  \
    PATOMIC_DEFINE_LOAD_OPS(type##_unsigned, u##name, order, vis_p)                   \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(type##_unsigned, u##name, order, vis_p, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE_NRAR(type##_unsigned, u##name, order, vis_p, opsk)  \
    PATOMIC_DEFINE_BIN_OPS_CREATE(type##_unsigned, u##name, order, vis_p, opsk)       \
    PATOMIC_DEFINE_ARI_OPS_CREATE(type##_unsigned, u##name, order, vis_p, opsk, 0u)   \
    PATOMIC_DEFINE_ARI_OPS_CREATE(type##_signed, s##name, order, vis_p, opsk, min)    \
    static patomic_##opsk##_t                                                         \
    patomic_ops_create_##name(void)                                                   \
    {                                                                                 \
        patomic_##opsk##_t pao;                                                       \
        pao.fp_store = patomic_opimpl_store_u##name;                                  \
        pao.fp_load = patomic_opimpl_load_u##name;                                    \
        pao.xchg_ops = patomic_ops_xchg_create_u##name();                             \
        pao.bitwise_ops = patomic_ops_bitwise_create_u##name();                       \
        pao.binary_ops = patomic_ops_binary_create_u##name();                         \
        pao.unsigned_ops = patomic_ops_arithmetic_create_u##name();                   \
        pao.signed_ops = patomic_ops_arithmetic_create_s##name();                     \
        return pao;                                                                   \
    }


#define PATOMIC_DEFINE_IS_LOCK_FREE(type, name)                      \
    static PATOMIC_FORCE_INLINE int                                  \
    patomic_impl_is_lock_free_##name(void)                           \
    {                                                                \
        type##_unsigned obj = {0};                                   \
        int ret = 0;                                                 \
        if (__atomic_always_lock_free(sizeof(type##_unsigned), &obj) \
            || __atomic_is_lock_free(sizeof(type##_unsigned), &obj)) \
        { ret = 1; }                                                 \
        PATOMIC_IGNORE_UNUSED(obj);                                  \
        return ret;                                                  \
    }


#define PATOMIC_DEFINE_OPS_CREATE_ALL(type, name, min)                     \
    PATOMIC_DEFINE_OPS_CREATE_CA(                                          \
        type, name##_acquire, __ATOMIC_ACQUIRE, HIDE_P, SHOW, ops, min     \
    )                                                                      \
    PATOMIC_DEFINE_OPS_CREATE_R(                                           \
        type, name##_release, __ATOMIC_RELEASE, HIDE_P, SHOW, ops, min     \
    )                                                                      \
    PATOMIC_DEFINE_OPS_CREATE_AR(                                          \
        type, name##_acq_rel, __ATOMIC_ACQ_REL, HIDE_P, SHOW, ops, min     \
    )                                                                      \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                         \
        type, name##_relaxed, __ATOMIC_RELAXED, HIDE_P, SHOW, ops, min     \
    )                                                                      \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                         \
        type, name##_seq_cst, __ATOMIC_SEQ_CST, HIDE_P, SHOW, ops, min     \
    )                                                                      \
    /* all ops are in RSC */                                               \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                         \
        type, name##_explicit, order, SHOW_P, HIDE, ops_explicit, min      \
    )                                                                      \
    PATOMIC_DEFINE_IS_LOCK_FREE(                                           \
        type, name                                                         \
    )


PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_char, char, SCHAR_MIN)
PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_short, short, SHRT_MIN)
PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_int, int, INT_MIN)
PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_long, long, LONG_MIN)
#if PATOMIC_IMPL_GNU_HAVE_LLONG
    PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_llong, llong, LLONG_MIN)
#endif
#if PATOMIC_IMPL_GNU_HAVE_I128
    PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_i128, 128, INT128_MIN)
#endif


#endif  /* PATOMIC_HAVE_GNU_ATOMIC */

#endif  /* !PATOMIC_IMPL_GNU_ATOMIC_H */
