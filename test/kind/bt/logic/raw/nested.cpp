#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


#define SKIP_NULL_OP_FP_TBEGIN_TCOMMIT(id, ops)             \
    SKIP_NULL_OP_FP(id, (ops).raw_ops.fp_tbegin, "tbegin"); \
    SKIP_NULL_OP_FP(id, (ops).raw_ops.fp_tcommit, "tcommit")

#define SKIP_NULL_OP_FP_TABORT_ALL(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).raw_ops.fp_tabort_all, "tabort_all")

#define SKIP_NULL_OP_FP_TABORT_SINGLE(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).raw_ops.fp_tabort_single, "tabort_single")

// testy rather than test so that gtest doesn't think it's a test
#define SKIP_NULL_OP_FP_TTESTY(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).raw_ops.fp_ttest, "ttest")

#define SKIP_NULL_OP_FP_TDEPTH(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).raw_ops.fp_tdepth, "tdepth")


/// @brief Test nested transaction with tbegin + tcommit.
TEST_P(BtLogicTransaction, raw_nested_tbegin_tcommit)
{
    // check pre-conditions
    const auto &p = GetParam();
    SKIP_NULL_OP_FP_TBEGIN_TCOMMIT(p.id, m_ops);

    // alias but do not wrap operations
    auto fp_tbegin = m_ops.raw_ops.fp_tbegin;
    auto fp_tcommit = m_ops.raw_ops.fp_tcommit;

    // outer setup
    patomic_transaction_result_t result_outer {};
    patomic_transaction_padded_flag_holder_abi_unstable_t holder {};
    patomic_transaction_flag_t *flag = &holder.flag;

    // outer test
    for (unsigned long i = 0; i < m_config.attempts; ++i)
    {
        ++result_outer.attempts_made;
        result_outer.status = fp_tbegin();
        if (result_outer.status == 0)
        {
            static_cast<void>(*flag);

            // inner setup
            patomic_transaction_result_t result_inner {};
            patomic_transaction_padded_flag_holder_abi_unstable_t holder_inner {};
            patomic_transaction_flag_t *flag_inner = &holder_inner.flag;

            // inner test
            for (unsigned long j = 0; j < m_config.attempts; ++j)
            {
                ++result_inner.attempts_made;
                result_inner.status = fp_tbegin();
                if (result_inner.status == 0)
                {
                    static_cast<void>(*flag_inner);
                }
            }

            // outer commit
            fp_tcommit();
            break;
        }
    }
}


/// @brief Test nested transaction with tbegin + tcommit + ttest.
TEST_P(BtLogicTransaction, raw_nested_tbegin_tcommit_ttest)
{}


/// @brief Test nested transaction with tbegin + tcommit + tdepth.
TEST_P(BtLogicTransaction, raw_nested_tbegin_tcommit_tdepth)
{}


/// @brief Test nested transaction with tbegin + tcommit + tabort_all.
TEST_P(BtLogicTransaction, raw_nested_tbegin_tcommit_tabort_all)
{}


/// @brief Test nested transaction with tbegin + tcommit + tabort_single.
TEST_P(BtLogicTransaction, raw_nested_tbegin_tcommit_tabort_single)
{}
