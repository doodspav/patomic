#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_bit_test_reset(
    std::size_t width,
    std::size_t align,
    const std::function<int(
        void *object,
        int offset
    )>& fp_test_reset
)
{
    // start with all bits unset
    test::generic_integer object { width, align, false };
    test::generic_integer object_exp { width, align, false };

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        // 1 -> 0
        object.inv_at(i);
        ASSERT_EQ(1, fp_test_reset(object, i));
        ASSERT_EQ(object, object_exp);

        // 0 -> 0
        ASSERT_EQ(0, fp_test_reset(object, i));
        ASSERT_EQ(object, object_exp);
    }

    // start with all bits set
    object.inv();
    object_exp = object;

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        // 1 -> 0
        object_exp.inv_at(i);
        ASSERT_EQ(1, fp_test_reset(object, i));
        ASSERT_EQ(object, object_exp);

        // 0 -> 0
        ASSERT_EQ(0, fp_test_reset(object, i));
        ASSERT_EQ(object, object_exp);

        // reset
        object.inv_at(i);
        object_exp = object;
    }
}

}  // namespace


#define SKIP_NULL_OP_FP_TEST_RESET(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).bitwise_ops.fp_test_reset, "test_reset")


/// @brief Check that the non-atomic logic of implicit test_reset works correctly.
TEST_P(BtLogicImplicit, fp_test_reset)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TEST_RESET(p.id, m_ops);

    // wrap operation
    const auto fp_test_reset = [&](void *object, int offset) -> int {
        return m_ops.bitwise_ops.fp_test_reset(object, offset);
    };

    // test
    test_bit_test_reset(p.width, m_align.recommended, fp_test_reset);
}


/// @brief Check that the non-atomic logic of explicit test_reset works correctly.
TEST_P(BtLogicExplicit, fp_test_reset)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TEST_RESET(p.id, m_ops);

    // wrap operation
    const auto fp_test_reset = [&](void *object, int offset) -> int {
        return m_ops.bitwise_ops.fp_test_reset(object, offset, p.order);
    };

    // test
    test_bit_test_reset(p.width, m_align.recommended, fp_test_reset);
}


/// @brief Check that the non-atomic logic of transaction test_reset works correctly.
TEST_P(BtLogicTransaction, fp_test_reset)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TEST_RESET(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.bitwise_ops.fp_test_reset, nullptr, 0);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config.width = width;

        // wrap operation
        const auto fp_test_reset = [&](void *object, int offset) -> int {
            patomic_transaction_result_t result {};
            int bit = m_ops.bitwise_ops.fp_test_reset(object, offset, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
            return bit;
        };

        // test
        test_bit_test_reset(width, 1u, fp_test_reset);
    }
}

