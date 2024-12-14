#include <test/common/skip.hpp>

#include <test/suite/bt_logic.hpp>


#define SKIP_NULL_FLAG_OPS(id, ops)                              \
    SKIP_NULL_OP_FP(id, (ops).flag_ops.fp_test, "test");         \
    SKIP_NULL_OP_FP(id, (ops).flag_ops.fp_test_set, "test_set"); \
    SKIP_NULL_OP_FP(id, (ops).flag_ops.fp_clear, "clear")


TEST_P(BtLogicTransaction, fp_flag)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_FLAG_OPS(p.id, m_ops);

    // wrap ops (useful for catching const-qualifier issues)
    const auto fp_test = [=](const patomic_transaction_flag_t& flag) -> int {
        return m_ops.flag_ops.fp_test(&flag);
    };
    const auto fp_test_set = [=](patomic_transaction_flag_t& flag) -> int {
        return m_ops.flag_ops.fp_test_set(&flag);
    };
    const auto fp_clear = [=](patomic_transaction_flag_t& flag) -> void {
        m_ops.flag_ops.fp_clear(&flag);
    };

    // setup
    patomic_transaction_flag_t flag {};

    // test: unset
    ASSERT_EQ(0, fp_test(flag));
    ASSERT_EQ(0, flag);

    // test_set: unset -> set
    ASSERT_EQ(0, fp_test_set(flag));
    ASSERT_EQ(1, flag);

    // test: set
    ASSERT_EQ(1, fp_test(flag));
    ASSERT_EQ(1, flag);

    // test_set: set -> set
    ASSERT_EQ(1, fp_test_set(flag));
    ASSERT_EQ(1, flag);

    // test: set
    ASSERT_EQ(1, fp_test(flag));
    ASSERT_EQ(1, flag);

    // clear: set -> unset
    fp_clear(flag);
    ASSERT_EQ(0, flag);

    // test: unset
    ASSERT_EQ(0, fp_test(flag));
    ASSERT_EQ(0, flag);

    // clear: unset -> unset
    fp_clear(flag);
    ASSERT_EQ(0, flag);

    // test: unset
    ASSERT_EQ(0, fp_test(flag));
    ASSERT_EQ(0, flag);
}
