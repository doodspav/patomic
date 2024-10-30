#include "std.h"

#include <patomic/config.h>

#include <patomic/macros/ignore_unused.h>


#if PATOMIC_HAS_ATOMIC && PATOMIC_HAS_STDATOMIC_H && PATOMIC_HAS_IR_TWOS_COMPL


#include <patomic/wrapped/direct.h>

#include <stdatomic.h>
#include <stddef.h>


/*
 * BASE:
 * - store (direct)
 * - load  (direct)
 */
#define do_store_explicit(type, obj, des, order) \
    atomic_store_explicit(obj, des, order)
#define do_load_explicit(type, obj, order, res) \
    res = atomic_load_explicit(obj, order)

#define PATOMIC_DEFINE_STORE_OP(type, name, vis_p, order) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(               \
        _Atomic(type), type,                              \
        patomic_opimpl_store_##name,                      \
        vis_p, order,                                     \
        do_store_explicit                                 \
    )

#define PATOMIC_DEFINE_LOAD_OP(type, name, vis_p, order) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(               \
        _Atomic(type), type,                             \
        patomic_opimpl_load_##name,                      \
        vis_p, order,                                    \
        do_load_explicit                                 \
    )


/*
 * XCHG:
 * - exchange       (direct)
 * - cmpxchg_weak   (direct)
 * - cmpxchg_strong (direct)
 */
#define do_exchange_explicit(type, obj, des, order, res) \
    res = atomic_exchange_explicit(obj, des, order)
#define do_cmpxchg_weak(type, obj, exp, des, succ, fail, ok) \
    ok = atomic_compare_exchange_weak_explicit(obj, &exp, des, succ, fail)
#define do_cmpxchg_strong(type, obj, exp, des, succ, fail, ok) \
    ok = atomic_compare_exchange_strong_explicit(obj, &exp, des, succ, fail)

#define PATOMIC_DEFINE_XCHG_OPS_CREATE(type, name, vis_p, inv, order, ops) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(                             \
        _Atomic(type), type,                                               \
        patomic_opimpl_exchange_##name,                                    \
        vis_p, order,                                                      \
        do_exchange_explicit                                               \
    )                                                                      \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(                              \
        _Atomic(type), type,                                               \
        patomic_opimpl_cmpxchg_weak_##name,                                \
        vis_p, inv, order,                                                 \
        do_cmpxchg_weak                                                    \
    )                                                                      \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(                              \
        _Atomic(type), type,                                               \
        patomic_opimpl_cmpxchg_strong_##name,                              \
        vis_p, inv, order,                                                 \
        do_cmpxchg_strong                                                  \
    )                                                                      \
    static patomic_##ops##_xchg_t                                          \
    patomic_ops_xchg_create_##name(void)                                   \
    {                                                                      \
        patomic_##ops##_xchg_t pao;                                        \
        pao.fp_exchange = patomic_opimpl_exchange_##name;                  \
        pao.fp_cmpxchg_weak = patomic_opimpl_cmpxchg_weak_##name;          \
        pao.fp_cmpxchg_strong = patomic_opimpl_cmpxchg_strong_##name;      \
        return pao;                                                        \
    }


/*
 * BITWISE:
 * - bit_test       (direct)
 * - bit_test_compl (no)
 * - bit_test_set   (no)
 * - bit_test_reset (no)
 */
#define do_bit_test_explicit(type, obj, offset, order, res) \
    scratch = (type) ((type) 1 << (type) offset);           \
    scratch &= atomic_load_explicit(obj, order);            \
    res = (scratch != (type) 0)

#define PATOMIC_DEFINE_BIT_TEST_OP(type, name, vis_p, order) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST(               \
        _Atomic(type), type,                                 \
        patomic_opimpl_bit_test_##name,                      \
        vis_p, order,                                        \
        do_bit_test_explicit                                 \
    )

#define PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(type, name, vis_p, order) \
    static const void * patomic_opimpl_bit_test_compl_##name = NULL; \
    static const void * patomic_opimpl_bit_test_set_##name   = NULL; \
    static const void * patomic_opimpl_bit_test_reset_##name = NULL;

/* create ops which support all memory orders */
#define PATOMIC_DEFINE_BITWISE_OPS_CREATE_NO_LOAD(type, name, vis_p, order, ops) \
    PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(type, name, vis_p, order)                 \
    static patomic_##ops##_bitwise_t                                             \
    patomic_ops_bitwise_create_##name(void)                                      \
    {                                                                            \
        patomic_##ops##_bitwise_t pao;                                           \
        pao.fp_test = NULL;  /* does not support release or acq_rel */           \
        pao.fp_test_compl = patomic_opimpl_bit_test_compl_##name;                \
        pao.fp_test_set   = patomic_opimpl_bit_test_set_##name;                  \
        pao.fp_test_reset = patomic_opimpl_bit_test_reset_##name;                \
        return pao;                                                              \
    }

/* order cannot be release or acq_rel */
#define PATOMIC_DEFINE_BITWISE_OPS_CREATE_LOAD(type, name, vis_p, order, ops) \
    PATOMIC_DEFINE_BIT_TEST_OP(type, name, vis_p, order)                      \
    PATOMIC_DEFINE_BIT_TEST_MODIFY_OPS(type, name, vis_p, order)              \
    static patomic_##ops##_bitwise_t                                          \
    patomic_ops_bitwise_create_##name(void)                                   \
    {                                                                         \
        patomic_##ops##_bitwise_t pao;                                        \
        pao.fp_test = patomic_opimpl_bit_test_##name;                         \
        pao.fp_test_compl = patomic_opimpl_bit_test_compl_##name;             \
        pao.fp_test_set   = patomic_opimpl_bit_test_set_##name;               \
        pao.fp_test_reset = patomic_opimpl_bit_test_reset_##name;             \
        return pao;                                                           \
    }


/*
 * BINARY:
 * - (fetch_)or  (direct)
 * - (fetch_)xor (direct)
 * - (fetch_)and (direct)
 * - (fetch_)not (no)
 */
#define do_void_or_explicit(type, obj, arg, order) \
    PATOMIC_IGNORE_UNUSED(atomic_fetch_or_explicit(obj, arg, order))
#define do_void_xor_explicit(type, obj, arg, order) \
    PATOMIC_IGNORE_UNUSED(atomic_fetch_xor_explicit(obj, arg, order))
#define do_void_and_explicit(type, obj, arg, order) \
    PATOMIC_IGNORE_UNUSED(atomic_fetch_and_explicit(obj, arg, order))

#define do_fetch_or_explicit(type, obj, arg, order, res) \
    res = atomic_fetch_or_explicit(obj, arg, order)
#define do_fetch_xor_explicit(type, obj, arg, order, res) \
    res = atomic_fetch_xor_explicit(obj, arg, order)
#define do_fetch_and_explicit(type, obj, arg, order, res) \
    res = atomic_fetch_and_explicit(obj, arg, order)

#define PATOMIC_DEFINE_BINARY_OPS_CREATE(type, name, vis_p, order, ops) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(                              \
        _Atomic(type), type,                                            \
        patomic_opimpl_void_or_##name,                                  \
        vis_p, order,                                                   \
        do_void_or_explicit                                             \
    )                                                                   \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(                              \
        _Atomic(type), type,                                            \
        patomic_opimpl_void_xor_##name,                                 \
        vis_p, order,                                                   \
        do_void_xor_explicit                                            \
    )                                                                   \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(                              \
        _Atomic(type), type,                                            \
        patomic_opimpl_void_and_##name,                                 \
        vis_p, order,                                                   \
        do_void_and_explicit                                            \
    )                                                                   \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(                             \
        _Atomic(type), type,                                            \
        patomic_opimpl_fetch_or_##name,                                 \
        vis_p, order,                                                   \
        do_fetch_or_explicit                                            \
    )                                                                   \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(                             \
        _Atomic(type), type,                                            \
        patomic_opimpl_fetch_xor_##name,                                \
        vis_p, order,                                                   \
        do_fetch_xor_explicit                                           \
    )                                                                   \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(                             \
        _Atomic(type), type,                                            \
        patomic_opimpl_fetch_and_##name,                                \
        vis_p, order,                                                   \
        do_fetch_and_explicit                                           \
    )                                                                   \
    static patomic_##ops##_binary_t                                     \
    patomic_ops_binary_create_##name(void)                              \
    {                                                                   \
        patomic_##ops##_binary_t pao;                                   \
        pao.fp_or  = patomic_opimpl_void_or_##name;                     \
        pao.fp_xor = patomic_opimpl_void_xor_##name;                    \
        pao.fp_and = patomic_opimpl_void_and_##name;                    \
        pao.fp_not = NULL;                                              \
        pao.fp_fetch_or  = patomic_opimpl_fetch_or_##name;              \
        pao.fp_fetch_xor = patomic_opimpl_fetch_xor_##name;             \
        pao.fp_fetch_and = patomic_opimpl_fetch_and_##name;             \
        pao.fp_fetch_not = NULL;                                        \
        return pao;                                                     \
    }


/*
 * ARITHMETIC:
 * - (fetch_)add (direct)
 * - (fetch_)sub (direct)
 * - (fetch_)inc (direct)
 * - (fetch_)dec (direct)
 * - (fetch_)neg (no)
 */
#define do_void_add_explicit(type, obj, arg, order) \
    PATOMIC_IGNORE_UNUSED(atomic_fetch_add_explicit(obj, arg, order))
#define do_void_sub_explicit(type, obj, arg, order) \
    PATOMIC_IGNORE_UNUSED(atomic_fetch_sub_explicit(obj, arg, order))
#define do_void_inc_explicit(type, obj, order) \
    PATOMIC_IGNORE_UNUSED(atomic_fetch_add_explicit(obj, (type) 1, order))
#define do_void_dec_explicit(type, obj, order) \
    PATOMIC_IGNORE_UNUSED(atomic_fetch_sub_explicit(obj, (type) 1, order))

#define do_fetch_add_explicit(type, obj, arg, order, res) \
    res = atomic_fetch_add_explicit(obj, arg, order)
#define do_fetch_sub_explicit(type, obj, arg, order, res) \
    res = atomic_fetch_sub_explicit(obj, arg, order)
#define do_fetch_inc_explicit(type, obj, order, res) \
    res = atomic_fetch_add_explicit(obj, (type) 1, order)
#define do_fetch_dec_explicit(type, obj, order, res) \
    res = atomic_fetch_sub_explicit(obj, (type) 1, order)

#define PATOMIC_DEFINE_ARITHMETIC_OPS_CREATE(type, name, vis_p, order, ops) \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(                                  \
        _Atomic(type), type,                                                \
        patomic_opimpl_void_add_##name,                                     \
        vis_p, order,                                                       \
        do_void_add_explicit                                                \
    )                                                                       \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(                                  \
        _Atomic(type), type,                                                \
        patomic_opimpl_void_sub_##name,                                     \
        vis_p, order,                                                       \
        do_void_sub_explicit                                                \
    )                                                                       \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(                            \
        _Atomic(type), type,                                                \
        patomic_opimpl_void_inc_##name,                                     \
        vis_p, order,                                                       \
        do_void_inc_explicit                                                \
    )                                                                       \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(                            \
        _Atomic(type), type,                                                \
        patomic_opimpl_void_dec_##name,                                     \
        vis_p, order,                                                       \
        do_void_dec_explicit                                                \
    )                                                                       \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(                                 \
        _Atomic(type), type,                                                \
        patomic_opimpl_fetch_add_##name,                                    \
        vis_p, order,                                                       \
        do_fetch_add_explicit                                               \
    )                                                                       \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(                                 \
        _Atomic(type), type,                                                \
        patomic_opimpl_fetch_sub_##name,                                    \
        vis_p, order,                                                       \
        do_fetch_sub_explicit                                               \
    )                                                                       \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(                           \
        _Atomic(type), type,                                                \
        patomic_opimpl_fetch_inc_##name,                                    \
        vis_p, order,                                                       \
        do_fetch_inc_explicit                                               \
    )                                                                       \
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(                           \
        _Atomic(type), type,                                                \
        patomic_opimpl_fetch_dec_##name,                                    \
        vis_p, order,                                                       \
        do_fetch_dec_explicit                                               \
    )                                                                       \
    static patomic_##ops##_arithmetic_t                                     \
    patomic_ops_arithmetic_create_##name(void)                              \
    {                                                                       \
        patomic_##ops##_arithmetic_t pao;                                   \
        pao.fp_add = patomic_opimpl_void_add_##name;                        \
        pao.fp_sub = patomic_opimpl_void_sub_##name;                        \
        pao.fp_inc = patomic_opimpl_void_inc_##name;                        \
        pao.fp_dec = patomic_opimpl_void_dec_##name;                        \
        pao.fp_neg = NULL;                                                  \
        pao.fp_fetch_add = patomic_opimpl_fetch_add_##name;                 \
        pao.fp_fetch_sub = patomic_opimpl_fetch_sub_##name;                 \
        pao.fp_fetch_inc = patomic_opimpl_fetch_inc_##name;                 \
        pao.fp_fetch_dec = patomic_opimpl_fetch_dec_##name;                 \
        pao.fp_fetch_neg = NULL;                                            \
        return pao;                                                         \
    }


/*
 * CREATE STRUCTS
 *
 * Implicit:
 * - ca:  { consume, acquire } (not supported by store)
 * - r:   { release } (not supported by load)
 * - ar:  { acq_rel } (not supported by store or load)
 * - rsc:{ relaxed, seq_cst }
 */
#define PATOMIC_DEFINE_OPS_CREATE_CA(type, name, vis_p, inv, order, ops)  \
    /* no store in consume or acquire */                                  \
    PATOMIC_DEFINE_LOAD_OP(type, name, vis_p, order)                      \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(type, name, vis_p, inv, order, ops)    \
    PATOMIC_DEFINE_BITWISE_OPS_CREATE_LOAD(type, name, vis_p, order, ops) \
    PATOMIC_DEFINE_BINARY_OPS_CREATE(type, name, vis_p, order, ops)       \
    PATOMIC_DEFINE_ARITHMETIC_OPS_CREATE(type, name, vis_p, order, ops)   \
    static patomic_##ops##_t                                              \
    patomic_ops_create_##name(void)                                       \
    {                                                                     \
        patomic_##ops##_t pao;                                            \
        pao.fp_store = NULL;                                              \
        pao.fp_load = patomic_opimpl_load_##name;                         \
        pao.xchg_ops = patomic_ops_xchg_create_##name();                  \
        pao.bitwise_ops = patomic_ops_bitwise_create_##name();            \
        pao.binary_ops = patomic_ops_binary_create_##name();              \
        pao.arithmetic_ops = patomic_ops_arithmetic_create_##name();      \
        return pao;                                                       \
    }

#define PATOMIC_DEFINE_OPS_CREATE_R(type, name, vis_p, inv, order, ops)      \
    PATOMIC_DEFINE_STORE_OP(type, name, vis_p, order)                        \
    /* no load in release */                                                 \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(type, name, vis_p, inv, order, ops)       \
    PATOMIC_DEFINE_BITWISE_OPS_CREATE_NO_LOAD(type, name, vis_p, order, ops) \
    PATOMIC_DEFINE_BINARY_OPS_CREATE(type, name, vis_p, order, ops)          \
    PATOMIC_DEFINE_ARITHMETIC_OPS_CREATE(type, name, vis_p, order, ops)      \
    static patomic_##ops##_t                                                 \
    patomic_ops_create_##name(void)                                          \
    {                                                                        \
        patomic_##ops##_t pao;                                               \
        pao.fp_store = patomic_opimpl_store_##name;                          \
        pao.fp_load = NULL;                                                  \
        pao.xchg_ops = patomic_ops_xchg_create_##name();                     \
        pao.bitwise_ops = patomic_ops_bitwise_create_##name();               \
        pao.binary_ops = patomic_ops_binary_create_##name();                 \
        pao.arithmetic_ops = patomic_ops_arithmetic_create_##name();         \
        return pao;                                                          \
    }

#define PATOMIC_DEFINE_OPS_CREATE_AR(type, name, vis_p, inv, order, ops)     \
    /* no store or load in acq_rel */                                        \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(type, name, vis_p, inv, order, ops)       \
    PATOMIC_DEFINE_BITWISE_OPS_CREATE_NO_LOAD(type, name, vis_p, order, ops) \
    PATOMIC_DEFINE_BINARY_OPS_CREATE(type, name, vis_p, order, ops)          \
    PATOMIC_DEFINE_ARITHMETIC_OPS_CREATE(type, name, vis_p, order, ops)      \
    static patomic_##ops##_t                                                 \
    patomic_ops_create_##name(void)                                          \
    {                                                                        \
        patomic_##ops##_t pao;                                               \
        pao.fp_store = NULL;                                                 \
        pao.fp_load = NULL;                                                  \
        pao.xchg_ops = patomic_ops_xchg_create_##name();                     \
        pao.bitwise_ops = patomic_ops_bitwise_create_##name();               \
        pao.binary_ops = patomic_ops_binary_create_##name();                 \
        pao.arithmetic_ops = patomic_ops_arithmetic_create_##name();         \
        return pao;                                                          \
    }

#define PATOMIC_DEFINE_OPS_CREATE_RSC(type, name, vis_p, inv, order, ops) \
    PATOMIC_DEFINE_STORE_OP(type, name, vis_p, order)                     \
    PATOMIC_DEFINE_LOAD_OP(type, name, vis_p, order)                      \
    PATOMIC_DEFINE_XCHG_OPS_CREATE(type, name, vis_p, inv, order, ops)    \
    PATOMIC_DEFINE_BITWISE_OPS_CREATE_LOAD(type, name, vis_p, order, ops) \
    PATOMIC_DEFINE_BINARY_OPS_CREATE(type, name, vis_p, order, ops)       \
    PATOMIC_DEFINE_ARITHMETIC_OPS_CREATE(type, name, vis_p, order, ops)   \
    static patomic_##ops##_t                                              \
    patomic_ops_create_##name(void)                                       \
    {                                                                     \
        patomic_##ops##_t pao;                                            \
        pao.fp_store = patomic_opimpl_store_##name;                       \
        pao.fp_load = patomic_opimpl_load_##name;                         \
        pao.xchg_ops = patomic_ops_xchg_create_##name();                  \
        pao.bitwise_ops = patomic_ops_bitwise_create_##name();            \
        pao.binary_ops = patomic_ops_binary_create_##name();              \
        pao.arithmetic_ops = patomic_ops_arithmetic_create_##name();      \
        return pao;                                                       \
    }

#define PATOMIC_DEFINE_OPS_CREATE_ALL(type, name)                     \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                    \
        type, name##_relaxed, HIDE_P, SHOW, memory_order_relaxed, ops \
    )                                                                 \
    /* consume is not supported, we just use acquire */               \
    PATOMIC_DEFINE_OPS_CREATE_CA(                                     \
        type, name##_acquire, HIDE_P, SHOW, memory_order_acquire, ops \
    )                                                                 \
    PATOMIC_DEFINE_OPS_CREATE_R(                                      \
        type, name##_release, HIDE_P, SHOW, memory_order_release, ops \
    )                                                                 \
    PATOMIC_DEFINE_OPS_CREATE_AR(                                     \
        type, name##_acq_rel, HIDE_P, SHOW, memory_order_acq_rel, ops \
    )                                                                 \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                    \
        type, name##_seq_cst, HIDE_P, SHOW, memory_order_seq_cst, ops \
    )                                                                 \
    PATOMIC_DEFINE_OPS_CREATE_RSC(                                    \
        type, name##_explicit, SHOW_P, HIDE, order, ops_explicit      \
    )


#else  /* PATOMIC_HAS_ATOMIC && PATOMIC_HAS_STDATOMIC_H && PATOMIC_HAS_IR_TWOS_COMPL */


patomic_t
patomic_impl_create_std(
    const size_t byte_width,
    const patomic_memory_order_t order,
    const unsigned int options
)
{
    /* zero all fields */
    patomic_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(order);
    PATOMIC_IGNORE_UNUSED(options);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}


patomic_explicit_t
patomic_impl_create_explicit_std(
    const size_t byte_width,
    const unsigned int options
)
{
    /* zero all fields */
    patomic_explicit_t impl = {0};

    /* ignore all parameters */
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(options);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}


#endif  /* PATOMIC_HAS_ATOMIC && PATOMIC_HAS_STDATOMIC_H && PATOMIC_HAS_IR_TWOS_COMPL */


patomic_transaction_t
patomic_impl_create_transaction_std(
    const unsigned int options
)
{
    /* zero all fields */
    patomic_transaction_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(options);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}
