#include <test/common/skip.hpp>

#include <test/suite/bt_logic.hpp>


#define SKIP_NULL_OP_FP_TEST(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).flag_ops.fp_test, "test")


TEST_P(BtLogicTransaction, fp_test_set)
{
    // check pre-condition
    const auto &p = GetParam();
    SKIP_NULL_OP_FP_TEST(p.id, m_ops);

    // wrap operation
    const auto fp_test = [=](const patomic_transaction_flag_t &flag) -> int {
        return m_ops.flag_ops.fp_test(&flag);
    };

    // setup
    patomic_transaction_flag_t flag{};

    // test: unset
    ASSERT_EQ(0, fp_test(flag));
    ASSERT_EQ(0, flag);

    // test: set
    flag = 1;
    ASSERT_EQ(1, fp_test(flag));
    ASSERT_EQ(1, flag);
}
