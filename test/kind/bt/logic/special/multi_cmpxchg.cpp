#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>

#include <algorithm>
#include <vector>


namespace
{


void
store_zero(test::generic_integer& gi)
{
    gi.store_zero();
}

void
store_inv_zero(test::generic_integer& gi)
{
    gi.store_zero();
    gi.inv();
}

void
store_one(test::generic_integer& gi)
{
    gi.store_zero();
    gi.inc();
}

void
store_min(test::generic_integer& gi)
{
    gi.store_min();
}

void
store_max(test::generic_integer& gi)
{
    gi.store_max();
}


void
test_multi_cmpxchg(
    std::size_t width,
    std::size_t align,
    const std::function<int(
        patomic_transaction_cmpxchg_t *cxs,
        std::size_t cxs_len
    )>& fp_multi_cmpxchg
)
{
    // set of states
    std::vector<std::function<void(test::generic_integer&)>> states {
        store_zero,
        store_inv_zero,
        store_one,
        store_min,
        store_max
    };

    // as many cmpxchg as there are states
    std::vector<test::generic_cmpxchg> cxs {
        states.size(), { width, align, false }
    };

    // wrap operation
    const auto fp_wrapped = [&](std::vector<test::generic_cmpxchg>& cxs_vec) -> int {
        std::vector<patomic_transaction_cmpxchg_t> cxs;
        cxs.reserve(cxs_vec.size());
        for (auto& cx : cxs_vec)
        {
            cxs.push_back(cx);
        }
        return fp_multi_cmpxchg(cxs.data(), cxs.size());
    };

    // test all cxs sizes (from 0 to size())
    for (std::size_t len = 0; len <= cxs.size(); ++len)
    {
        // transfer through all states
        for (std::size_t count = 0; count < states.size(); ++count)
        {
            // set object to current state
            for (std::size_t j = 0; j < cxs.size(); ++j)
            {
                states.at(j)(cxs.at(j).object);
            }

            // set desired to next state
            std::rotate(states.begin(), states.begin() + 1, states.end());
            for (std::size_t j = 0; j < cxs.size(); ++j)
            {
                states.at(j)(cxs.at(j).desired);
            }

            // copy len objects to use
            std::vector<test::generic_cmpxchg> cxs_copy;
            cxs_copy.reserve(len);
            for (std::size_t j = 0; j < len; ++j)
            {
                cxs_copy.push_back(cxs.at(j));
            }

            // test with only len object
            DO_TEST_TSX_MULTI_CMPXCHG(cxs_copy, fp_wrapped);
        }
    }
}


}  // namespace


#define SKIP_NULL_OP_FP_MULTI_CMPXCHG(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).special_ops.fp_multi_cmpxchg, "multi_cmpxchg")


/// @brief Check that the non-atomic logic of transaction multi_cmpxchg works correctly.
TEST_P(BtLogicTransaction, fp_multi_cmpxchg)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_MULTI_CMPXCHG(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO_WFB(m_ops.special_ops.fp_multi_cmpxchg, {}, {});

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config_wfb.width = width;

        // wrap operation
        const auto fp_multi_cmpxchg = [&](patomic_transaction_cmpxchg_t *cxs, std::size_t cxs_len) -> int {
            patomic_transaction_result_wfb_t result {};
            int ok = m_ops.special_ops.fp_multi_cmpxchg(cxs, cxs_len, m_config_wfb, &result);
            ADD_FAILURE_TSX_SUCCESS_WFB(m_config_wfb, result);
            return ok;
        };

        // test
        test_multi_cmpxchg(width, 1u, fp_multi_cmpxchg);
    }
}
