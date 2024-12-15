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


/// @brief Test single transaction with tbegin + tcommit.
TEST_P(BtLogicTransaction, raw_single_tbegin_tcommit)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TBEGIN_TCOMMIT(p.id, m_ops);

    // alias but do not wrap operations
    auto fp_tbegin = m_ops.raw_ops.fp_tbegin;
    auto fp_tcommit = m_ops.raw_ops.fp_tcommit;

    // setup
    patomic_transaction_result_t result {};
    patomic_transaction_padded_flag_holder_abi_unstable_t holder {};
    patomic_transaction_flag_t *flag = &holder.flag;

    // test
    for (unsigned long i = 0; i < m_config.attempts; ++i)
    {
        ++result.attempts_made;
        result.status = fp_tbegin();
        if (result.status == 0)
        {
            static_cast<void>(*flag);
            fp_tcommit();
            break;
        }
    }

    // check
    ADD_FAILURE_TSX_SUCCESS(m_config, result);
}


/// @brief Test single transaction with tbegin + tcommit + ttest.
TEST_P(BtLogicTransaction, raw_single_tbegin_tcommit_ttest)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TBEGIN_TCOMMIT(p.id, m_ops);
    SKIP_NULL_OP_FP_TTESTY(p.id, m_ops);

    // alias but do not wrap operations
    auto fp_tbegin = m_ops.raw_ops.fp_tbegin;
    auto fp_tcommit = m_ops.raw_ops.fp_tcommit;
    auto fp_ttest = m_ops.raw_ops.fp_ttest;

    // setup
    patomic_transaction_result_t result {};
    patomic_transaction_padded_flag_holder_abi_unstable_t holder {};
    patomic_transaction_flag_t *flag = &holder.flag;
    int test = 0;
    ASSERT_EQ(0, fp_ttest());

    // run
    for (unsigned long i = 0; i < m_config.attempts; ++i)
    {
        ++result.attempts_made;
        result.status = fp_tbegin();
        if (result.status == 0)
        {
            static_cast<void>(*flag);
            test = fp_ttest();
            fp_tcommit();
            break;
        }
    }

    // test
    ADD_FAILURE_TSX_SUCCESS(m_config, result);
    ASSERT_EQ(0, fp_ttest());
    if (m_ops.raw_ops.fp_tdepth != nullptr)
    {
        // guaranteed to be interchangeable with tdepth
        ASSERT_EQ(1, test);
    }
    else
    {
        // just guaranteed to be non-zero
        ASSERT_NE(0, test);
    }
}


/// @brief Test single transaction with tbegin + tcommit + tdepth.
TEST_P(BtLogicTransaction, raw_single_tbegin_tcommit_tdepth)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TBEGIN_TCOMMIT(p.id, m_ops);
    SKIP_NULL_OP_FP_TDEPTH(p.id, m_ops);

    // alias but do not wrap operations
    auto fp_tbegin = m_ops.raw_ops.fp_tbegin;
    auto fp_tcommit = m_ops.raw_ops.fp_tcommit;
    auto fp_tdepth = m_ops.raw_ops.fp_tdepth;

    // setup
    patomic_transaction_result_t result {};
    patomic_transaction_padded_flag_holder_abi_unstable_t holder {};
    patomic_transaction_flag_t *flag = &holder.flag;
    int depth = 0;
    ASSERT_EQ(0, fp_tdepth());

    // run
    for (unsigned long i = 0; i < m_config.attempts; ++i)
    {
        ++result.attempts_made;
        result.status = fp_tbegin();
        if (result.status == 0)
        {
            static_cast<void>(*flag);
            depth = fp_tdepth();
            fp_tcommit();
            break;
        }
    }

    // test
    ADD_FAILURE_TSX_SUCCESS(m_config, result);
    ASSERT_EQ(0, fp_tdepth());
    ASSERT_EQ(1, depth);
}


/// @brief Test single transaction with tbegin + tcommit + tabort_all.
TEST_P(BtLogicTransaction, raw_single_tbegin_tcommit_tabort_all)
{
    // check pre-condition
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TBEGIN_TCOMMIT(p.id, m_ops);
    SKIP_NULL_OP_FP_TABORT_ALL(p.id, m_ops);

    // alias but do not wrap operations
    auto fp_tbegin = m_ops.raw_ops.fp_tbegin;
    auto fp_tcommit = m_ops.raw_ops.fp_tcommit;
    auto fp_tabort_all = m_ops.raw_ops.fp_tabort_all;

    // setup
    patomic_transaction_padded_flag_holder_abi_unstable_t holder {};
    patomic_transaction_flag_t *flag = &holder.flag;

    // go through all possible abort reasons
    constexpr int reason_count = (1 << 8);
    for (int r = 0; r < reason_count; ++r)
    {
        // run without transaction, should be no-op
        // use a different reason here to below in case it makes a difference
        // it shouldn't make a difference (hence the test)
        fp_tabort_all(static_cast<unsigned char>(reason_count - r));

        // run
        patomic_transaction_result_t result {};
        for (unsigned long i = 0; i < m_config.attempts; ++i)
        {
            ++result.attempts_made;
            result.status = fp_tbegin();
            if (result.status == 0)
            {
                static_cast<void>(*flag);
                fp_tabort_all(static_cast<unsigned char>(r));
                fp_tcommit();
            }

            // test
            auto exit_code = PATOMIC_TRANSACTION_STATUS_ABORT_REASON(result.status);
            auto reason = PATOMIC_TRANSACTION_STATUS_ABORT_REASON(result.status);
            if (exit_code == patomic_TABORT_EXPLICIT)
            {
                ASSERT_EQ(r, reason);
                break;
            }
        }

        // test
        auto exit_code = PATOMIC_TRANSACTION_STATUS_EXIT_CODE(result.status);
        ASSERT_EQ(patomic_TABORT_EXPLICIT, exit_code);
        auto exit_info = PATOMIC_TRANSACTION_STATUS_EXIT_INFO(result.status);
        ASSERT_FALSE(exit_info & patomic_TINFO_NESTED);
    }
}


/// @brief Test single transaction with tbegin + tcommit + tabort_single.
TEST_P(BtLogicTransaction, raw_single_tbegin_tcommit_tabort_single)
{
    // check pre-condition
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TBEGIN_TCOMMIT(p.id, m_ops);
    SKIP_NULL_OP_FP_TABORT_SINGLE(p.id, m_ops);

    // alias but do not wrap operations
    auto fp_tbegin = m_ops.raw_ops.fp_tbegin;
    auto fp_tcommit = m_ops.raw_ops.fp_tcommit;
    auto fp_tabort_single = m_ops.raw_ops.fp_tabort_single;

    // setup
    patomic_transaction_padded_flag_holder_abi_unstable_t holder {};
    patomic_transaction_flag_t *flag = &holder.flag;

    // go through all possible abort reasons
    constexpr int reason_count = (1 << 8);
    for (int r = 0; r < reason_count; ++r)
    {
        // run without transaction, should be no-op
        // use a different reason here to below in case it makes a difference
        // it shouldn't make a difference (hence the test)
        fp_tabort_single(static_cast<unsigned char>(reason_count - r), 0);

        // run
        patomic_transaction_result_t result {};
        for (unsigned long i = 0; i < m_config.attempts; ++i)
        {
            ++result.attempts_made;
            result.status = fp_tbegin();
            if (result.status == 0)
            {
                static_cast<void>(*flag);
                fp_tabort_single(static_cast<unsigned char>(r), 0);
                fp_tcommit();
            }

            // test
            auto exit_code = PATOMIC_TRANSACTION_STATUS_ABORT_REASON(result.status);
            auto info = PATOMIC_TRANSACTION_STATUS_EXIT_INFO(result.status);
            auto reason = PATOMIC_TRANSACTION_STATUS_ABORT_REASON(result.status);
            if (exit_code == patomic_TABORT_EXPLICIT)
            {
                ASSERT_EQ(r, reason);
                ASSERT_FALSE(info & patomic_TINFO_NESTED);
                break;
            }
        }

        // test
        auto exit_code = PATOMIC_TRANSACTION_STATUS_EXIT_CODE(result.status);
        ASSERT_EQ(patomic_TABORT_EXPLICIT, exit_code);
        auto exit_info = PATOMIC_TRANSACTION_STATUS_EXIT_INFO(result.status);
        ASSERT_FALSE(exit_info & patomic_TINFO_NESTED);
    }
}
