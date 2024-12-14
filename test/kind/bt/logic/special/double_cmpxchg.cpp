#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>

#include <vector>


namespace
{

void
test_double_cmpxchg(
    std::size_t width,
    std::size_t align,
    const std::function<int(
        patomic_transaction_cmpxchg_t cxa,
        patomic_transaction_cmpxchg_t cxb
    )>& fp_double_cmpxchg
)
{
    // create cmpxchg objects
    std::vector<test::generic_cmpxchg> cxs {
        { width, align, false },
        { width, align, false }
    };
    auto& cxa = cxs[0];
    auto& cxb = cxs[1];

    // wrap operation
    const auto fp_wrapped = [&](std::vector<test::generic_cmpxchg>& cxs_vec) -> int {
        return fp_double_cmpxchg(cxs_vec.at(0), cxs_vec.at(1));
    };

    // (0, ~0) -> (~0, 1)
    cxa.desired.inv();
    cxb.desired.inc();
    DO_TEST_MULTI_CMPXCHG(cxs, fp_wrapped);

    // (~0, 1) -> (1, max)
    cxa.desired.store_zero();
    cxa.desired.inc();
    cxb.desired.store_max();
    DO_TEST_MULTI_CMPXCHG(cxs, fp_wrapped);

    // (1, max) -> (max, min)
    cxa.desired.store_max();
    cxb.desired.store_min();
    DO_TEST_MULTI_CMPXCHG(cxs, fp_wrapped);

    // (max, min) -> (min, 0)
    cxa.desired.store_min();
    cxb.desired.store_zero();
    DO_TEST_MULTI_CMPXCHG(cxs, fp_wrapped);

    // (min, 0) -> (0, 1)
    cxa.desired.store_zero();
    cxb.desired.inc();
    DO_TEST_MULTI_CMPXCHG(cxs, fp_wrapped);

    // (0, 0) -> (1, 1)
    cxa.desired.inc();
    cxb.desired = cxa.desired;
    DO_TEST_MULTI_CMPXCHG(cxs, fp_wrapped);
}

}  // namespace


#define SKIP_NULL_OP_FP_DOUBLE_CMPXCHG(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).special_ops.fp_double_cmpxchg, "double_cmpxchg")


/// @brief Check that the non-atomic logic of transaction double_cmpxchg works correctly.
TEST_P(BtLogicTransaction, fp_double_cmpxchg)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_DOUBLE_CMPXCHG(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO_WFB(m_ops.special_ops.fp_double_cmpxchg, {}, {});

    // go through all widths
    for (std::size_t width : p.widths)
    {
        // setup
        m_config_wfb.width = width;

        // wrap operation
        using cmpxchg_t = patomic_transaction_cmpxchg_t;
        const auto fp_double_cmpxchg = [&](cmpxchg_t cxa, cmpxchg_t cxb) -> int {
            patomic_transaction_result_wfb_t result {};
            int ok = m_ops.special_ops.fp_double_cmpxchg(cxa, cxb, m_config_wfb, &result);
            ADD_FAILURE_TSX_SUCCESS_WFB(m_config_wfb, result);
            return ok;
        };

        // test
        test_double_cmpxchg(width, 1u, fp_double_cmpxchg);
    }
}
