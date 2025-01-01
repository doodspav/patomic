/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#include "msvc.h"

#include <patomic/macros/ignore_unused.h>


#ifdef _MSC_VER

#include "ops.h"


/**
 * @param n
 *   Bit width of operation.
 *
 * @param order
 *   Memory order of operation, or 'explicit'.
 *
 * @param fp_name
 *   The function pointer member name.
 *
 * @param opimpl_name
 *   As completes 'patomic_opimpl_<opimpl_name>...'.
 *
 * @param vis_name
 *   As completes 'VIS_FP_<vis_name>...'.
 */
#define DO_ASSIGN(n, order, ops, fp_name, opimpl_name, vis_name) \
    VIS_FP_##vis_name##_##n(ops.fp_name = patomic_opimpl_##opimpl_name##_##n##_##order;)


/**
 * @param n
 *   Bit width of operation.
 *
 * @param ops
 *   Either 'ops' for implicit ops or 'ops_explicit' for explicit ops.
 *
 * @param order
 *   Memory order of operation, or 'explicit'.
 */
#define DEFINE_CREATE_OPS(n, ops, order)                                                    \
    static patomic_##ops##_t                                                                \
    patomic_ops_create_##n##_##order(void)                                                  \
    {                                                                                       \
        patomic_##ops##_t pao = {0};                                                        \
                                                                                            \
        /* ldst */                                                                          \
        DO_ASSIGN(n, order, pao, fp_store, store, STORE)                                    \
        DO_ASSIGN(n, order, pao, fp_load, load, LOAD)                                       \
                                                                                            \
        /* xchg */                                                                          \
        DO_ASSIGN(n, order, pao.xchg_ops, fp_exchange, exchange, EXCHANGE)                  \
        DO_ASSIGN(n, order, pao.xchg_ops, fp_cmpxchg_weak, cmpxchg, COMPARE_EXCHANGE)       \
        DO_ASSIGN(n, order, pao.xchg_ops, fp_cmpxchg_strong, cmpxchg, COMPARE_EXCHANGE)     \
                                                                                            \
        /* bitwise */                                                                       \
        DO_ASSIGN(n, order, pao.bitwise_ops, fp_test,       bit_test,       BIT_TEST)       \
        DO_ASSIGN(n, order, pao.bitwise_ops, fp_test_compl, bit_test_compl, BIT_TEST_COMPL) \
        DO_ASSIGN(n, order, pao.bitwise_ops, fp_test_reset, bit_test_reset, BIT_TEST_RESET) \
        DO_ASSIGN(n, order, pao.bitwise_ops, fp_test_set,   bit_test_set,   BIT_TEST_SET)   \
                                                                                            \
        /* binary */                                                                        \
        DO_ASSIGN(n, order, pao.binary_ops, fp_or,  void_or,  OR)                           \
        DO_ASSIGN(n, order, pao.binary_ops, fp_xor, void_xor, XOR)                          \
        DO_ASSIGN(n, order, pao.binary_ops, fp_and, void_and, AND)                          \
        DO_ASSIGN(n, order, pao.binary_ops, fp_not, void_not, NOT)                          \
        DO_ASSIGN(n, order, pao.binary_ops, fp_fetch_or,  fetch_or,  OR)                    \
        DO_ASSIGN(n, order, pao.binary_ops, fp_fetch_xor, fetch_xor, XOR)                   \
        DO_ASSIGN(n, order, pao.binary_ops, fp_fetch_and, fetch_and, AND)                   \
        DO_ASSIGN(n, order, pao.binary_ops, fp_fetch_not, fetch_not, NOT)                   \
                                                                                            \
        /* arithmetic */                                                                    \
        DO_ASSIGN(n, order, pao.arithmetic_ops, fp_add, void_add, ADD)                      \
        DO_ASSIGN(n, order, pao.arithmetic_ops, fp_sub, void_sub, SUB)                      \
        DO_ASSIGN(n, order, pao.arithmetic_ops, fp_inc, void_inc, INC)                      \
        DO_ASSIGN(n, order, pao.arithmetic_ops, fp_dec, void_dec, DEC)                      \
        DO_ASSIGN(n, order, pao.arithmetic_ops, fp_neg, void_neg, NEG)                      \
        DO_ASSIGN(n, order, pao.arithmetic_ops, fp_fetch_add, fetch_add, ADD)               \
        DO_ASSIGN(n, order, pao.arithmetic_ops, fp_fetch_sub, fetch_sub, SUB)               \
        DO_ASSIGN(n, order, pao.arithmetic_ops, fp_fetch_inc, fetch_inc, INC)               \
        DO_ASSIGN(n, order, pao.arithmetic_ops, fp_fetch_dec, fetch_dec, DEC)               \
        DO_ASSIGN(n, order, pao.arithmetic_ops, fp_fetch_neg, fetch_neg, NEG)               \
                                                                                            \
        /* finished */                                                                      \
        return pao;                                                                         \
    }


DEFINE_CREATE_OPS(8,   ops_explicit, explicit)
DEFINE_CREATE_OPS(16,  ops_explicit, explicit)
DEFINE_CREATE_OPS(32,  ops_explicit, explicit)
DEFINE_CREATE_OPS(64,  ops_explicit, explicit)
DEFINE_CREATE_OPS(128, ops_explicit, explicit)

DEFINE_CREATE_OPS(8,   ops, seq_cst)
DEFINE_CREATE_OPS(16,  ops, seq_cst)
DEFINE_CREATE_OPS(32,  ops, seq_cst)
DEFINE_CREATE_OPS(64,  ops, seq_cst)
DEFINE_CREATE_OPS(128, ops, seq_cst)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL

    DEFINE_CREATE_OPS(8,   ops, acquire)
    DEFINE_CREATE_OPS(16,  ops, acquire)
    DEFINE_CREATE_OPS(32,  ops, acquire)
    DEFINE_CREATE_OPS(64,  ops, acquire)
    DEFINE_CREATE_OPS(128, ops, acquire)

    DEFINE_CREATE_OPS(8,   ops, release)
    DEFINE_CREATE_OPS(16,  ops, release)
    DEFINE_CREATE_OPS(32,  ops, release)
    DEFINE_CREATE_OPS(64,  ops, release)
    DEFINE_CREATE_OPS(128, ops, release)

#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF

    DEFINE_CREATE_OPS(8,   ops, relaxed)
    DEFINE_CREATE_OPS(16,  ops, relaxed)
    DEFINE_CREATE_OPS(32,  ops, relaxed)
    DEFINE_CREATE_OPS(64,  ops, relaxed)
    DEFINE_CREATE_OPS(128, ops, relaxed)

#endif


#define DO_CASE(N, n, pao, order)                     \
    case N:                                           \
        pao.ops = patomic_ops_create_##n##_##order(); \
        pao.align.recommended = N##u;                 \
        pao.align.minimum = N##u;                     \
        pao.align.size_within = 0u;                   \
        break

#define DO_SWITCH(width, pao, order)     \
    switch (width)                       \
    {                                    \
        DO_CASE(1,  8,   pao, order);    \
        DO_CASE(2,  16,  pao, order);    \
        DO_CASE(4,  32,  pao, order);    \
        DO_CASE(8,  64,  pao, order);    \
        DO_CASE(16, 128, pao, order);    \
        default:                         \
            pao.align.recommended = 1ul; \
            pao.align.minimum = 1ul;     \
            pao.align.size_within = 0ul; \
    }


patomic_t
patomic_impl_create_msvc(
    const size_t byte_width,
    const patomic_memory_order_t order,
    const unsigned int options
)
{
    /* setup */
    patomic_t impl = {0};
    PATOMIC_IGNORE_UNUSED(options);

    /* set members */
    switch (order)
    {
#if PATOMIC_IMPL_MSVC_HAS_IL_NF
        case patomic_RELAXED:
            DO_SWITCH(byte_width, impl, relaxed)
            break;
#endif
#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
        case patomic_CONSUME:
        case patomic_ACQUIRE:
            DO_SWITCH(byte_width, impl, acquire)
            break;
        case patomic_RELEASE:
            DO_SWITCH(byte_width, impl, release)
#endif
#if !PATOMIC_IMPL_MSVC_HAS_IL_NF
        case patomic_RELAXED:
#endif
#if !PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
        case patomic_CONSUME:
        case patomic_ACQUIRE:
        case patomic_RELEASE:
#endif
        case patomic_ACQ_REL:
        case patomic_SEQ_CST:
        default:
            DO_SWITCH(byte_width, impl, seq_cst)
    }

    /* return */
    return impl;
}


patomic_explicit_t
patomic_impl_create_explicit_msvc(
    const size_t byte_width,
    const unsigned int options
)
{
    /* setup */
    patomic_explicit_t impl = {0};
    PATOMIC_IGNORE_UNUSED(options);

    /* set members */
    DO_SWITCH(byte_width, impl, explicit)

    /* return */
    return impl;
}


#else  /* !defined(_MSC_VER) */


patomic_t
patomic_impl_create_msvc(
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
patomic_impl_create_explicit_msvc(
    const size_t byte_width,
    const unsigned int options
)
{
    /* zero all fields */
    patomic_explicit_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(options);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}


#endif  /* defined(_MSC_VER) */


patomic_transaction_t
patomic_impl_create_transaction_msvc(
    const unsigned int options
)
{
    /* zero all fields */
    patomic_transaction_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(options);

    /* return */
    return impl;
}
