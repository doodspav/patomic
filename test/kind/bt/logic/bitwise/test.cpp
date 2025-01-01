// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_test(
    std::size_t width,
    std::size_t align,
    const std::function<int(
        const void *object,
        int offset
    )>& fp_test
)
{
    // start with all bits unset
    test::generic_integer object { width, align, false };
    test::generic_integer object_exp { width, align, false };

    // go through all bit offsets
    for (int i = 0; i < static_cast<int>(object.bit_width()); ++i)
    {
        SCOPED_TRACE(i);

        // 1
        object.inv_at(i);
        object_exp = object;
        ASSERT_EQ(1, fp_test(object, i));
        ASSERT_EQ(object, object_exp);

        // 0
        object.inv_at(i);
        object_exp = object;
        ASSERT_EQ(0, fp_test(object, i));
        ASSERT_EQ(object, object_exp);
    }

    // start with all bits set
    object.store_zero();
    object.inv();

    // go through all bit offsets
    for (int i = 0; i < static_cast<int>(object.bit_width()); ++i)
    {
        SCOPED_TRACE(i);

        // 0
        object.inv_at(i);
        object_exp = object;
        ASSERT_EQ(0, fp_test(object, i));
        ASSERT_EQ(object, object_exp);

        // 1
        object.inv_at(i);
        object_exp = object;
        ASSERT_EQ(1, fp_test(object, i));
        ASSERT_EQ(object, object_exp);
    }
}

}  // namespace


// testy rather than test so that gtest doesn't think it's a test
#define SKIP_NULL_OP_FP_TESTY(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).bitwise_ops.fp_test, "test")


/// @brief Check that the non-atomic logic of implicit test works correctly.
TEST_P(BtLogicImplicitLoad, fp_test)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TESTY(p.id, m_ops);

    // wrap operation
    const auto fp_test = [&](const void *object, int offset) -> int {
        return m_ops.bitwise_ops.fp_test(object, offset);
    };

    // test
    test_test(p.width, m_align.recommended, fp_test);
}


/// @brief Check that the non-atomic logic of explicit test works correctly.
TEST_P(BtLogicExplicitLoad, fp_test)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TESTY(p.id, m_ops);

    // wrap operation
    const auto fp_test = [&](const void *object, int offset) -> int {
        return m_ops.bitwise_ops.fp_test(object, offset, p.order);
    };

    // test
    test_test(p.width, m_align.recommended, fp_test);
}


/// @brief Check that the non-atomic logic of transaction test works correctly.
TEST_P(BtLogicTransaction, fp_test)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TESTY(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.bitwise_ops.fp_test);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.bitwise_ops.fp_test);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config.width = width;

        // wrap operation
        const auto fp_test = [&](const void *object, int offset) -> int {
            patomic_transaction_result_t result {};
            int bit = m_ops.bitwise_ops.fp_test(object, offset, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
            return bit;
        };

        // test
        TEST_TSX(m_config, test);
    }
}
