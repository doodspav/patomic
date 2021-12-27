#ifndef PATOMIC_IMPL_GNU_SYNC_H
#define PATOMIC_IMPL_GNU_SYNC_H

#include <patomic/patomic.h>
#include <patomic/patomic_config.h>


#if PATOMIC_HAVE_GNU_SYNC


#include <stddef.h>

#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>
#include <patomic/macros/likely_unlikely.h>

#include <patomic/stdlib/assert.h>
#include <patomic/stdlib/limits.h>
#include <patomic/stdlib/stdalign.h>
#include <patomic/stdlib/stdint.h>

#include <patomic/wrapped/cmpxchg.h>
#include <patomic/wrapped/direct.h>
#include <patomic/wrapped/fetch.h>


#if !PATOMIC_HAVE_IR_TWOS_COMPL
    #error "GNU is not known to support non twos-complement architectures"
#endif


/*
 * FUNCTION/STRUCT DEFINE MACRO PARAMS
 *
 * - type: operand type (e.g. int)
 * - name: 'explicit' or memory order (e.g. acquire)
 * - order: memory order value (ignored)
 * - vis_p: order parameter visibility (e.g. SHOW_P)
 * - inv: the inverse of vis_p, but for anything (e.g. HIDE, not HIDE_P)
 * - opsk: return type ops kind (e.g. ops_explicit)
 * - min: minimum value of arithmetic type (e.g. INT_MIN)
 */


/*
 * BASE:
 * - store (y)
 * - load (n)
 */
#define patomic_do_cmpxchg_we(t,i,y,p,e,d,s,f,r)  \
    scratch = __sync_val_compare_and_swap(p,e,d); \
    r = (scratch == e); e = scratch;              \
    PATOMIC_IGNORE_UNUSED(s);                     \
    PATOMIC_IGNORE_UNUSED(f)

#define PATOMIC_DEFINE_STORE_OPS(type, name, order, vis_p) \
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(               \
        0, 0, patomic_do_cmpxchg_we,                       \
        type, type,                                        \
        patomic_opimpl_store_##name,                       \
        order, vis_p                                       \
    )


/*
 * XCHG:
 * - exchange (y)
 * - compare_exchange_weak (y)
 * - compare_exchange_strong (y)
 */
#define patomic_do_cmpxchg_se patomic_do_cmpxchg_we

#define PATOMIC_DEFINE_XCHG_OPS_CREATE(type, name, order, vis_p, inv, opsk) \
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(                             \
        0, 0, patomic_do_cmpxchg_we,                                        \
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
 * - bit_test (n)
 * - bit_test_compl (y)
 * - bit_test_set (y)
 * - bit_test_reset (y)
 *
 * WARNING:
 * - type MUST be an unsigned integer type
 */
#define patomic_do_cmp_eqz(t,i,y,a,r) r = ((a) == (t)0)
#define patomic_do_ip_nth_bit_mask(t,i,y,m,n) m = (t)((t)1u << (t)(n));
#define patomic_do_ip_bit_or(t,i,y,a,b) a |= b
#define patomic_do_ip_bit_xor(t,i,y,a,b) a ^= b
#define patomic_do_ip_bit_and(t,i,y,a,b) a &= b
#define patomic_do_ip_bit_not(t,i,y,a) a = (t)~a

#define PATOMIC_DEFINE_BIT_OPS_CREATE(type, name, order, vis_p, opsk) \
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_COMPL(                 \
        0, 0, patomic_do_cmpxchg_we, patomic_do_cmp_eqz,              \
        patomic_do_ip_nth_bit_mask,                                   \
        patomic_do_ip_bit_and, patomic_do_ip_bit_xor,                 \
        type, type,                                                   \
        patomic_opimpl_bit_test_compl_##name,                         \
        order, vis_p                                                  \
    )                                                                 \
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_SET(                   \
        0, 0, patomic_do_cmpxchg_we, patomic_do_cmp_eqz,              \
        patomic_do_ip_nth_bit_mask,                                   \
        patomic_do_ip_bit_and, patomic_do_ip_bit_or,                  \
        type, type,                                                   \
        patomic_opimpl_bit_test_set_##name,                           \
        order, vis_p                                                  \
    )                                                                 \
    PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_RESET(                 \
        0, 0, patomic_do_cmpxchg_we, patomic_do_cmp_eqz,              \
        patomic_do_ip_nth_bit_mask,                                   \
        patomic_do_ip_bit_and, patomic_do_ip_bit_not,                 \
        type, type,                                                   \
        patomic_opimpl_bit_test_reset_##name,                         \
        order, vis_p                                                  \
    )                                                                 \
    static patomic_##opsk##_bitwise_t                                 \
    patomic_ops_bitwise_create_##name(void)                           \
    {                                                                 \
        patomic_##opsk##_bitwise_t pao;                               \
        pao.fp_test = NULL;                                           \
        pao.fp_test_compl = patomic_opimpl_bit_test_compl_##name;     \
        pao.fp_test_set = patomic_opimpl_bit_test_set_##name;         \
        pao.fp_test_reset = patomic_opimpl_bit_test_reset_##name;     \
        return pao;                                                   \
    }


/*
 * BINARY:
 * - (fetch_)or (y)
 * - (fetch_)xor (y)
 * - (fetch_)and (y)
 * - (fetch_)not (y)
 */
#define patomic_do_or_fe(t,i,y,p,a,o,r) r = __sync_fetch_and_or(p,a)
#define patomic_do_xor_fe(t,i,y,p,a,o,r) r = __sync_fetch_and_xor(p,a)
#define patomic_do_and_fe(t,i,y,p,a,o,r) r = __sync_fetch_and_and(p,a)

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
#define patomic_do_add_fe(t,i,y,m,p,a,o,r) r = __sync_fetch_and_add(p,a)
#define patomic_do_sub_fe(t,i,y,m,p,a,o,r) r = __sync_fetch_and_sub(p,a)
#define patomic_do_inc_fe(t,i,y,m,p,o,r) r = __sync_fetch_and_add(p,(t)1u)
#define patomic_do_dec_fe(t,i,y,m,p,o,r) r = __sync_fetch_and_sub(p,(t)1u)
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
 * - SC: __sync ignores memory orders in favour of seq_cst
 */
#define PATOMIC_DEFINE_OPS_CREATE_SC(type, name, order, vis_p, inv, opsk, min)        \
    PATOMIC_DEFINE_STORE_OPS(type##_unsigned, u##name, order, vis_p)                  \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(type##_unsigned, u##name, order, vis_p, inv, opsk) \
    PATOMIC_DEFINE_BIT_OPS_CREATE(type##_unsigned, u##name, order, vis_p, opsk)       \
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


#define PATOMIC_DEFINE_OPS_CREATE_ALL(type, name, min)                \
    PATOMIC_DEFINE_OPS_CREATE_SC(                                     \
        type, name##_seq_cst, patomic_SEQ_CST, HIDE_P, SHOW, ops, min \
    )                                                                 \
    PATOMIC_DEFINE_OPS_CREATE_SC(                                     \
        type, name##_explicit, order, SHOW_P, HIDE, ops_explicit, min \
    )


#undef PATOMIC_IMPL_GNU_SYNC_TYPE_1
#undef PATOMIC_IMPL_GNU_SYNC_NAME_1
#if PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_1
    PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_char, char, SCHAR_MIN)
    #define PATOMIC_IMPL_GNU_SYNC_TYPE_1 patomic_char_signed
    #define PATOMIC_IMPL_GNU_SYNC_NAME_1 char
#endif

#undef PATOMIC_IMPL_GNU_SYNC_TYPE_2
#undef PATOMIC_IMPL_GNU_SYNC_NAME_2
#if PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_2
    #if PATOMIC_SIZEOF_SHORT == 2
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_short, short, SHRT_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_2 patomic_short_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_2 short
    #elif PATOMIC_SIZEOF_INT == 2
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_int, int, INT_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_2 patomic_int_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_2 int
    #elif PATOMIC_SIZEOF_LONG == 2
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_long, long, LONG_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_2 patomic_long_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_2 long
    #elif ( PATOMIC_SIZEOF_LLONG == 2 && \
            PATOMIC_STDINT_HAVE_LLONG && \
            defined(PATOMIC_LLONG_MIN) )
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_llong, llong, PATOMIC_LLONG_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_2 patomic_llong_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_2 llong
    #elif ( PATOMIC_SIZEOF_I128 == 2 && \
            PATOMIC_STDINT_HAVE_I128 && \
            defined(PATOMIC_INT128_MIN) )
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_i128, 128, PATOMIC_INT128_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_2 patomic_i128_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_2 128
    #endif
#endif

#undef PATOMIC_IMPL_GNU_SYNC_TYPE_4
#undef PATOMIC_IMPL_GNU_SYNC_NAME_4
#if PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_4
    #if PATOMIC_SIZEOF_SHORT == 4
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_short, short, SHRT_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_4 patomic_short_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_4 short
    #elif PATOMIC_SIZEOF_INT == 4
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_int, int, INT_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_4 patomic_int_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_4 int
    #elif PATOMIC_SIZEOF_LONG == 4
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_long, long, LONG_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_4 patomic_long_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_4 long
    #elif ( PATOMIC_SIZEOF_LLONG == 4 && \
            PATOMIC_STDINT_HAVE_LLONG && \
            defined(PATOMIC_LLONG_MIN) )
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_llong, llong, PATOMIC_LLONG_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_4 patomic_llong_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_4 llong
    #elif ( PATOMIC_SIZEOF_I128 == 4 && \
            PATOMIC_STDINT_HAVE_I128 && \
            defined(PATOMIC_INT128_MIN) )
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_i128, 128, PATOMIC_INT128_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_4 patomic_i128_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_4 128
    #endif
#endif

#undef PATOMIC_IMPL_GNU_SYNC_TYPE_8
#undef PATOMIC_IMPL_GNU_SYNC_NAME_8
#if PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_8
    #if PATOMIC_SIZEOF_SHORT == 8
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_short, short, SHRT_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_8 patomic_short_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_8 short
    #elif PATOMIC_SIZEOF_INT == 8
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_int, int, INT_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_8 patomic_int_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_8 int
    #elif PATOMIC_SIZEOF_LONG == 8
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_long, long, LONG_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_8 patomic_long_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_8 long
    #elif ( PATOMIC_SIZEOF_LLONG == 8 && \
            PATOMIC_STDINT_HAVE_LLONG && \
            defined(PATOMIC_LLONG_MIN) )
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_llong, llong, PATOMIC_LLONG_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_8 patomic_llong_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_8 llong
    #elif ( PATOMIC_SIZEOF_I128 == 8 && \
            PATOMIC_STDINT_HAVE_I128 && \
            defined(PATOMIC_INT128_MIN) )
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_i128, 128, PATOMIC_INT128_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_8 patomic_i128_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_8 128
    #endif
#endif

#undef PATOMIC_IMPL_GNU_SYNC_TYPE_16
#undef PATOMIC_IMPL_GNU_SYNC_NAME_16
#if PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_16
    #if PATOMIC_SIZEOF_SHORT == 16
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_short, short, SHRT_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_16 patomic_short_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_16 short
    #elif PATOMIC_SIZEOF_INT == 16
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_int, int, INT_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_16 patomic_int_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_16 int
    #elif PATOMIC_SIZEOF_LONG == 16
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_long, long, LONG_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_16 patomic_long_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_16 long
    #elif ( PATOMIC_SIZEOF_LLONG == 16 && \
            PATOMIC_STDINT_HAVE_LLONG &&  \
            defined(PATOMIC_LLONG_MIN) )
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_llong, llong, PATOMIC_LLONG_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_16 patomic_llong_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_16 llong
    #elif ( PATOMIC_SIZEOF_I128 == 16 && \
            PATOMIC_STDINT_HAVE_I128 &&  \
            defined(PATOMIC_INT128_MIN) )
        PATOMIC_DEFINE_OPS_CREATE_ALL(patomic_i128, 128, PATOMIC_INT128_MIN)
        #define PATOMIC_IMPL_GNU_SYNC_TYPE_16 patomic_i128_signed
        #define PATOMIC_IMPL_GNU_SYNC_NAME_16 128
    #endif
#endif


#define PATOMIC_SET_RET_(type, name, width, ret)       \
        ((width) == sizeof(type))                      \
    {                                                  \
        (ret) = patomic_ops_create_##name##_seq_cst(); \
    }
#define PATOMIC_SET_RET(type, name, width, ret) \
    PATOMIC_SET_RET_(type, name, width, ret)

#define PATOMIC_SET_RET_EXPLICIT_(type, name, width, ret) \
        ((width) == sizeof(type))                         \
    {                                                     \
        (ret) = patomic_ops_create_##name##_explicit();   \
    }
#define PATOMIC_SET_RET_EXPLICIT(type, name, width, ret) \
    PATOMIC_SET_RET_EXPLICIT_(type, name, width, ret)

#define PATOMIC_SET_ALIGN_(type, width, member) \
        ((width) == sizeof(type))               \
    {                                           \
        (member) = patomic_alignof_type(type);  \
    }
#define PATOMIC_SET_ALIGN(type, width, member) \
    PATOMIC_SET_ALIGN_(type, width, member)


static patomic_ops_t
patomic_create_ops(
    size_t byte_width,
    patomic_memory_order_t order
)
{
    patomic_ops_t ret = {0};
    patomic_assert_always(patomic_is_valid_order((int) order));
    PATOMIC_IGNORE_UNUSED(order);

    if (byte_width == 0) { return ret; }
#if defined(PATOMIC_IMPL_GNU_SYNC_TYPE_1)
    else if PATOMIC_SET_RET(
        PATOMIC_IMPL_GNU_SYNC_TYPE_1,
        PATOMIC_IMPL_GNU_SYNC_NAME_1,
        byte_width, ret
    )
#endif
#if defined(PATOMIC_IMPL_GNU_SYNC_TYPE_2)
    else if PATOMIC_SET_RET(
        PATOMIC_IMPL_GNU_SYNC_TYPE_2,
        PATOMIC_IMPL_GNU_SYNC_NAME_2,
        byte_width, ret
    )
#endif
#if defined(PATOMIC_IMPL_GNU_SYNC_TYPE_4)
    else if PATOMIC_SET_RET(
        PATOMIC_IMPL_GNU_SYNC_TYPE_4,
        PATOMIC_IMPL_GNU_SYNC_NAME_4,
        byte_width, ret
    )
#endif
#if defined(PATOMIC_IMPL_GNU_SYNC_TYPE_8)
    else if PATOMIC_SET_RET(
        PATOMIC_IMPL_GNU_SYNC_TYPE_8,
        PATOMIC_IMPL_GNU_SYNC_NAME_8,
        byte_width, ret
    )
#endif
#if defined(PATOMIC_IMPL_GNU_SYNC_TYPE_16)
    else if PATOMIC_SET_RET(
        PATOMIC_IMPL_GNU_SYNC_TYPE_16,
        PATOMIC_IMPL_GNU_SYNC_NAME_16,
        byte_width, ret
    )
#endif

    return ret;
}

static patomic_ops_explicit_t
patomic_create_ops_explicit(
    size_t byte_width
)
{
    patomic_ops_explicit_t ret = {0};

    if (byte_width == 0) { return ret; }
#if defined(PATOMIC_IMPL_GNU_SYNC_TYPE_1)
    else if PATOMIC_SET_RET_EXPLICIT(
        PATOMIC_IMPL_GNU_SYNC_TYPE_1,
        PATOMIC_IMPL_GNU_SYNC_NAME_1,
        byte_width, ret
    )
#endif
#if defined(PATOMIC_IMPL_GNU_SYNC_TYPE_2)
    else if PATOMIC_SET_RET_EXPLICIT(
        PATOMIC_IMPL_GNU_SYNC_TYPE_2,
        PATOMIC_IMPL_GNU_SYNC_NAME_2,
        byte_width, ret
    )
#endif
#if defined(PATOMIC_IMPL_GNU_SYNC_TYPE_4)
    else if PATOMIC_SET_RET_EXPLICIT(
        PATOMIC_IMPL_GNU_SYNC_TYPE_4,
        PATOMIC_IMPL_GNU_SYNC_NAME_4,
        byte_width, ret
    )
#endif
#if defined(PATOMIC_IMPL_GNU_SYNC_TYPE_8)
    else if PATOMIC_SET_RET_EXPLICIT(
        PATOMIC_IMPL_GNU_SYNC_TYPE_8,
        PATOMIC_IMPL_GNU_SYNC_NAME_8,
        byte_width, ret
    )
#endif
#if defined(PATOMIC_IMPL_GNU_SYNC_TYPE_16)
    else if PATOMIC_SET_RET_EXPLICIT(
        PATOMIC_IMPL_GNU_SYNC_TYPE_16,
        PATOMIC_IMPL_GNU_SYNC_NAME_16,
        byte_width, ret
    )
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
#if PATOMIC_STDINT_HAVE_LLONG
    else if PATOMIC_SET_ALIGN(patomic_llong_signed, byte_width, ret.recommended)
#endif
#if PATOMIC_STDINT_HAVE_I128
    else if PATOMIC_SET_ALIGN(patomic_i128_signed, byte_width, ret.recommended)
#endif
    else { ret.recommended = 1; }

    ret.minimum = ret.recommended;
    return ret;
}


#endif  /* PATOMIC_HAVE_GNU_SYNC */

#endif  /* !PATOMIC_IMPL_GNU_SYNC_H */
