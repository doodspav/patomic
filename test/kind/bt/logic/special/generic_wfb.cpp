// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_generic_wfb(
    patomic_transaction_config_wfb_t config,
    patomic_opsig_transaction_generic_wfb_t fp_generic_wfb
)
{
    // setup params
    struct ctx_t {
        int first;
        int second;
        patomic_transaction_result_t result;
        patomic_transaction_result_wfb_t result_wfb;
    };
    const auto fn = [](void *ctx) -> void {
        auto *ptr = reinterpret_cast<ctx_t *>(ctx);
        ++ptr->first;
    };
    const auto fallback_fn = [](void *ctx) -> void {
        auto *ptr = reinterpret_cast<ctx_t *>(ctx);
        ++ptr->second;
        ptr->result.status = ptr->result_wfb.status;
        ptr->result.attempts_made = ptr->result_wfb.attempts_made;
    };

    // test primary path
    ctx_t ctx {};
    int ok = fp_generic_wfb(fn, &ctx, fallback_fn, &ctx, config, &ctx.result_wfb);
    ADD_FAILURE_TSX_SUCCESS_WFB(config, ctx.result_wfb);
    ASSERT_EQ(ok, 1);
    ASSERT_EQ(1, ctx.first);
    ASSERT_EQ(0, ctx.second);

    // test fallback path
    ctx = ctx_t {};
    config.attempts = 0;
    ok = fp_generic_wfb(fn, &ctx, fallback_fn, &ctx, config, &ctx.result_wfb);
    ADD_FAILURE_TSX_SUCCESS_WFB(config, ctx.result_wfb);
    ASSERT_EQ(ok, 0);
    ASSERT_EQ(0, ctx.first);
    ASSERT_EQ(1, ctx.second);
    ASSERT_EQ(ctx.result.status, ctx.result_wfb.status);
    ASSERT_EQ(ctx.result.attempts_made, ctx.result_wfb.attempts_made);
}

}  // namespace


#define SKIP_NULL_OP_FP_GENERIC_WFB(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).special_ops.fp_generic_wfb, "generic_wfb")


/// @brief Check that the non-atomic logic of transaction generic_wfb works correctly.
TEST_P(BtLogicTransaction, fp_generic_wfb)
{
    // check pre-condition
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_GENERIC_WFB(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO_WFB(m_ops.special_ops.fp_generic_wfb);

    // test flag set
    ASSERT_TSX_FLAG_SET_WFB(m_ops.special_ops.fp_generic_wfb);

    // make sure width is non-zero
    m_config_wfb.width = 1;

    // test
    test_generic_wfb(m_config_wfb, m_ops.special_ops.fp_generic_wfb);
}
