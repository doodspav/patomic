// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>

#include <cstring>


namespace
{

void
test_fetch_sub(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument,
        void *ret
    )>& fp_fetch_sub
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer argument { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };
    test::generic_integer argument_old { width, align, false };

#define DO_TEST_FETCH_SUB()              \
    object_old = object;                 \
    argument_old = argument;             \
    fp_fetch_sub(object, argument, ret); \
    ASSERT_EQ(ret, object_old);          \
    ASSERT_EQ(argument, argument_old);   \
    object_old.sub(argument);            \
    ASSERT_EQ(object, object_old)

    // 0 - 0 -> 0
    DO_TEST_FETCH_SUB();

    // 0 - 1 -> max
    argument.inc();
    DO_TEST_FETCH_SUB();

    // 0 - 2 -> (max - 1)
    object.store_zero();
    argument.inc();
    DO_TEST_FETCH_SUB();

    // 0 - max -> 1
    object.store_zero();
    argument.store_max();
    DO_TEST_FETCH_SUB();

    // 1 - 0 -> 1
    argument.store_zero();
    DO_TEST_FETCH_SUB();

    // 1 - 1 -> 0
    argument.inc();
    DO_TEST_FETCH_SUB();

    // 1 - 2 -> max
    object.inc();
    argument.inc();
    DO_TEST_FETCH_SUB();

    // 1 - max -> 2
    object.store_zero();
    object.inc();
    argument.store_max();
    DO_TEST_FETCH_SUB();

    // max - 0 -> max
    object.store_max();
    argument.store_zero();
    DO_TEST_FETCH_SUB();

    // max - 1 -> (max - 1)
    argument.inc();
    DO_TEST_FETCH_SUB();

    // max - 2 -> (max - 2)
    object.store_max();
    argument.inc();
    DO_TEST_FETCH_SUB();

    // max - max -> 0
    object.store_max();
    argument.store_max();
    DO_TEST_FETCH_SUB();
}

void
test_sub(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument
    )>& fp_sub
)
{
    // wrap as fetch
    auto fetch_sub = [&](void *object, const void *argument, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_sub(object, argument);
    };

    // defer to fetch variant
    return test_fetch_sub(width, align, fetch_sub);
    }

}  // namespace


#define SKIP_NULL_OP_FP_SUB(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).arithmetic_ops.fp_sub, "sub")

#define SKIP_NULL_OP_FP_FETCH_SUB(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).arithmetic_ops.fp_fetch_sub, "fetch_sub")


/// @brief Check that the non-atomic logic of implicit sub works correctly.
TEST_P(BtLogicImplicit, fp_sub)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_SUB(p.id, m_ops);

    // wrap operation
    const auto fp_sub = [&](void *object, const void *argument) -> void {
        return m_ops.arithmetic_ops.fp_sub(object, argument);
    };

    // test
    test_sub(p.width, m_align.recommended, fp_sub);
}

/// @brief Check that the non-atomic logic of implicit fetch_sub works correctly.
TEST_P(BtLogicImplicit, fp_fetch_sub)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_SUB(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_sub = [&](void *object, const void *argument, void *ret) -> void {
        return m_ops.arithmetic_ops.fp_fetch_sub(object, argument, ret);
    };

    // test
    test_fetch_sub(p.width, m_align.recommended, fp_fetch_sub);
}


/// @brief Check that the non-atomic logic of explicit sub works correctly.
TEST_P(BtLogicExplicit, fp_sub)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_SUB(p.id, m_ops);

    // wrap operation
    const auto fp_sub = [&](void *object, const void *argument) -> void {
        return m_ops.arithmetic_ops.fp_sub(object, argument, p.order);
    };

    // test
    test_sub(p.width, m_align.recommended, fp_sub);
}

/// @brief Check that the non-atomic logic of explicit fetch_sub works correctly.
TEST_P(BtLogicExplicit, fp_fetch_sub)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_SUB(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_sub = [&](void *object, const void *argument, void *ret) -> void {
        return m_ops.arithmetic_ops.fp_fetch_sub(object, argument, p.order, ret);
    };

    // test
    test_fetch_sub(p.width, m_align.recommended, fp_fetch_sub);
}


/// @brief Check that the non-atomic logic of transaction sub works correctly.
TEST_P(BtLogicTransaction, fp_sub)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_SUB(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.arithmetic_ops.fp_sub);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.arithmetic_ops.fp_sub);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE("width: " + std::to_string(width));
        m_config.width = width;

        // wrap operation
        const auto fp_sub = [&](void *object, const void *argument) -> void {
            patomic_transaction_result_t result {};
            m_ops.arithmetic_ops.fp_sub(object, argument, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, sub);
    }
}

/// @brief Check that the non-atomic logic of transaction fetch_sub works correctly.
TEST_P(BtLogicTransaction, fp_fetch_sub)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_SUB(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.arithmetic_ops.fp_fetch_sub);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.arithmetic_ops.fp_fetch_sub);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE("width: " + std::to_string(width));
        m_config.width = width;

        // wrap operation
        const auto fp_fetch_sub = [&](void *object, const void *argument, void *ret) -> void {
            patomic_transaction_result_t result {};
            m_ops.arithmetic_ops.fp_fetch_sub(object, argument, ret, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, fetch_sub);
    }
}
