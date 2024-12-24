// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_test_set(
    std::size_t width,
    std::size_t align,
    const std::function<int(
        void *object,
        int offset
    )>& fp_test_set
)
{
    // start with all bits unset
    test::generic_integer object { width, align, false };
    test::generic_integer object_exp { width, align, false };

    // go through all bit offsets
    for (int i = 0; i < static_cast<int>(object.bit_width()); ++i)
    {
        // 0 -> 1
        object_exp.inv_at(i);
        ASSERT_EQ(0, fp_test_set(object, i));
        ASSERT_EQ(object, object_exp);

        // 1 -> 1
        ASSERT_EQ(1, fp_test_set(object, i));
        ASSERT_EQ(object, object_exp);

        // reset
        object.store_zero();
        object_exp = object;
    }

    // start with all bits set
    object.store_zero();
    object.inv();
    object_exp = object;

    // go through all bit offsets
    for (int i = 0; i < static_cast<int>(object.bit_width()); ++i)
    {
        // 1 -> 1
        ASSERT_EQ(1, fp_test_set(object, i));
        ASSERT_EQ(object, object_exp);

        // 0 -> 1
        object.inv_at(i);
        ASSERT_EQ(0, fp_test_set(object, i));
        ASSERT_EQ(object, object_exp);
    }
}

}  // namespace


#define SKIP_NULL_OP_FP_TEST_SET(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).bitwise_ops.fp_test_set, "test_set")


/// @brief Check that the non-atomic logic of implicit test_set works correctly.
TEST_P(BtLogicImplicit, fp_test_set)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TEST_SET(p.id, m_ops);

    // wrap operation
    const auto fp_test_set = [&](void *object, int offset) -> int {
        return m_ops.bitwise_ops.fp_test_set(object, offset);
    };

    // test
    test_test_set(p.width, m_align.recommended, fp_test_set);
}


/// @brief Check that the non-atomic logic of explicit test_set works correctly.
TEST_P(BtLogicExplicit, fp_test_set)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TEST_SET(p.id, m_ops);

    // wrap operation
    const auto fp_test_set = [&](void *object, int offset) -> int {
        return m_ops.bitwise_ops.fp_test_set(object, offset, p.order);
    };

    // test
    test_test_set(p.width, m_align.recommended, fp_test_set);
}


/// @brief Check that the non-atomic logic of transaction test_set works correctly.
TEST_P(BtLogicTransaction, fp_test_set)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TEST_SET(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.bitwise_ops.fp_test_set);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.bitwise_ops.fp_test_set);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config.width = width;

        // wrap operation
        const auto fp_test_set = [&](void *object, int offset) -> int {
            patomic_transaction_result_t result {};
            int bit = m_ops.bitwise_ops.fp_test_set(object, offset, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
            return bit;
        };

        // test
        TEST_TSX(m_config, test_set);
    }
}
