#include <test/common/skip.hpp>

#include <test/suite/bt_logic.hpp>


#define SKIP_NULL_OP_FP_TEST_SET(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).flag_ops.fp_test_set, "test_set")


/// @brief Check that the non-atomic logic of transaction test_set works correctly.
TEST_P(BtLogicTransaction, flag_fp_test_set)
{
    // check pre-condition
    const auto &p = GetParam();
    SKIP_NULL_OP_FP_TEST_SET(p.id, m_ops);

    // wrap operation
    const auto fp_test_set = [=](patomic_transaction_flag_t &flag) -> int {
        return m_ops.flag_ops.fp_test_set(&flag);
    };

    // setup
    patomic_transaction_flag_t flag{};

    // test_set: unset -> set
    ASSERT_EQ(0, fp_test_set(flag));
    ASSERT_EQ(1, flag);

    // test_set: set -> set
    ASSERT_EQ(1, fp_test_set(flag));
    ASSERT_EQ(1, flag);
}
