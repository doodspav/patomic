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


/// @brief
///   Helper type for nested transactions.
struct tsx_setup
{
    patomic_transaction_result_t result {};
    patomic_transaction_padded_flag_holder_abi_unstable_t holder {};

    const patomic_transaction_flag_t *
    flag() const noexcept
    {
        return &holder.flag;
    }
};


/// @brief Test nested transaction with tbegin + tcommit.
TEST_P(BtLogicTransaction, raw_nested_tbegin_tcommit)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TBEGIN_TCOMMIT(p.id, m_ops);

    // alias but do not wrap operations
    auto fp_tbegin = m_ops.raw_ops.fp_tbegin;
    auto fp_tcommit = m_ops.raw_ops.fp_tcommit;

    // setup
    tsx_setup outer {};
    tsx_setup inner {};
    inner.result.status = patomic_TABORT_UNKNOWN;

    // outer test
    for (unsigned long i = 0; i < m_config.attempts; ++i)
    {
        ++outer.result.attempts_made;
        outer.result.status = fp_tbegin();
        if (outer.result.status == 0)
        {
            static_cast<void>(*outer.flag());

            // inner test
            for (unsigned long j = 0; j < m_config.attempts; ++j)
            {
                ++inner.result.attempts_made;
                inner.result.status = fp_tbegin();
                if (inner.result.status == 0)
                {
                    // inner commit
                    static_cast<void>(*inner.flag());
                    fp_tcommit();
                    break;
                }
            }

            // outer commit
            fp_tcommit();
            break;
        }
    }

    // check
    ADD_FAILURE_TSX_SUCCESS(m_config, outer.result);
    ADD_FAILURE_TSX_SUCCESS(m_config, inner.result);
}


/// @brief Test nested transaction with tbegin + tcommit + ttest.
TEST_P(BtLogicTransaction, raw_nested_tbegin_tcommit_ttest)
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
    tsx_setup outer {};
    tsx_setup inner {};
    inner.result.status = patomic_TABORT_UNKNOWN;
    int outer_test = 0;
    int inner_test = 0;
    ASSERT_EQ(0, fp_ttest());

    // outer test
    for (unsigned long i = 0; i < m_config.attempts; ++i)
    {
        ++outer.result.attempts_made;
        outer.result.status = fp_tbegin();
        if (outer.result.status == 0)
        {
            static_cast<void>(*outer.flag());

            // inner test
            for (unsigned long j = 0; j < m_config.attempts; ++j)
            {
                ++inner.result.attempts_made;
                inner.result.status = fp_tbegin();
                if (inner.result.status == 0)
                {
                    // inner commit
                    static_cast<void>(*inner.flag());
                    inner_test = fp_ttest();
                    fp_tcommit();
                    break;
                }
            }

            // outer commit
            outer_test = fp_ttest();
            fp_tcommit();
            break;
        }
    }

    // check
    ADD_FAILURE_TSX_SUCCESS(m_config, outer.result);
    ADD_FAILURE_TSX_SUCCESS(m_config, inner.result);
    ASSERT_EQ(0, fp_ttest());
    if (m_ops.raw_ops.fp_tdepth != nullptr)
    {
        // guaranteed to be interchangeable with tdepth
        ASSERT_EQ(1, outer_test);
        ASSERT_EQ(2, inner_test);
    }
    else
    {
        // just guaranteed to be non-zero
        ASSERT_NE(0, outer_test);
        ASSERT_NE(0, inner_test);
    }
}


/// @brief Test nested transaction with tbegin + tcommit + tdepth.
TEST_P(BtLogicTransaction, raw_nested_tbegin_tcommit_tdepth)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TBEGIN_TCOMMIT(p.id, m_ops);
    SKIP_NULL_OP_FP_TTESTY(p.id, m_ops);

    // alias but do not wrap operations
    auto fp_tbegin = m_ops.raw_ops.fp_tbegin;
    auto fp_tcommit = m_ops.raw_ops.fp_tcommit;
    auto fp_tdepth = m_ops.raw_ops.fp_tdepth;

    // setup
    tsx_setup outer {};
    tsx_setup inner {};
    inner.result.status = patomic_TABORT_UNKNOWN;
    int outer_depth = 0;
    int inner_depth = 0;
    ASSERT_EQ(0, fp_tdepth());

    // outer test
    for (unsigned long i = 0; i < m_config.attempts; ++i)
    {
        ++outer.result.attempts_made;
        outer.result.status = fp_tbegin();
        if (outer.result.status == 0)
        {
            static_cast<void>(*outer.flag());

            // inner test
            for (unsigned long j = 0; j < m_config.attempts; ++j)
            {
                ++inner.result.attempts_made;
                inner.result.status = fp_tbegin();
                if (inner.result.status == 0)
                {
                    // inner commit
                    static_cast<void>(*inner.flag());
                    inner_depth = fp_tdepth();
                    fp_tcommit();
                    break;
                }
            }

            // outer commit
            outer_depth = fp_tdepth();
            fp_tcommit();
            break;
        }
    }

    // check
    ADD_FAILURE_TSX_SUCCESS(m_config, outer.result);
    ADD_FAILURE_TSX_SUCCESS(m_config, inner.result);
    ASSERT_EQ(0, fp_tdepth());
    ASSERT_EQ(1, outer_depth);
    ASSERT_EQ(2, inner_depth);
}


/// @brief Test nested transaction with tbegin + tcommit + tabort_all.
TEST_P(BtLogicTransaction, raw_nested_tbegin_tcommit_tabort_all)
{}


/// @brief Test nested transaction with tbegin + tcommit + tabort_single.
TEST_P(BtLogicTransaction, raw_nested_tbegin_tcommit_tabort_single)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TBEGIN_TCOMMIT(p.id, m_ops);
    SKIP_NULL_OP_FP_TABORT_SINGLE(p.id, m_ops);

    // alias but do not wrap operations
    auto fp_tbegin = m_ops.raw_ops.fp_tbegin;
    auto fp_tcommit = m_ops.raw_ops.fp_tcommit;
    auto fp_tabort_single = m_ops.raw_ops.fp_tabort_single;

    // go through all possible abort reasons
    constexpr int reason_count = (1 << 8);
    for (int r = 0; r < reason_count; ++r)
    {
        // run without transaction, should be no-op
        // use a different reason here to below in case it makes a difference
        // it shouldn't make a difference (hence the test)
        fp_tabort_single(static_cast<unsigned char>(reason_count - r - 1), 0);

        // setup
        tsx_setup outer {};
        tsx_setup inner {};
        inner.result.status = patomic_TABORT_UNKNOWN;

        // outer run
        for (unsigned long i = 0; i < m_config.attempts; ++i)
        {
            ++outer.result.attempts_made;
            outer.result.status = fp_tbegin();
            if (outer.result.status == 0)
            {
                static_cast<void>(*outer.flag());

                // inner run
                for (unsigned long j = 0; m_config.attempts; ++j)
                {
                    ++inner.result.attempts_made;
                    inner.result.status = fp_tbegin();
                    if (inner.result.status == 0)
                    {
                        // inner commit with tabort
                        static_cast<void>(*inner.flag());
                        fp_tabort_single(static_cast<unsigned char>(r), 0);
                        fp_tcommit();
                    }

                    // inner test
                    auto inner_exit_code = PATOMIC_TRANSACTION_STATUS_EXIT_CODE(inner.result.status);
                    if (inner_exit_code == patomic_TABORT_EXPLICIT)
                    {
                        break;
                    }
                }

                // outer commit
                fp_tcommit();
                break;
            }

            // outer test (test inner again)
            auto inner_exit_code = PATOMIC_TRANSACTION_STATUS_EXIT_CODE(inner.result.status);
            if (inner_exit_code == patomic_TABORT_EXPLICIT)
            {
                break;
            }
        }

        // test
        ADD_FAILURE_TSX_SUCCESS(m_config, outer.result);
        auto inner_exit_code = patomic_transaction_status_exit_code(inner.result.status);
        auto inner_exit_info = patomic_transaction_status_exit_info(inner.result.status);
        auto inner_reason = patomic_transaction_status_abort_reason(inner.result.status);
        ASSERT_EQ(inner_exit_code, patomic_TABORT_EXPLICIT);
        ASSERT_FALSE(inner_exit_info & patomic_TINFO_NESTED);
        ASSERT_EQ(inner_reason, r);
    }
}
