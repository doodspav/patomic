// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_generic(
    const std::function<void(
        void (* fn)(void *),
        void *ctx
    )>& fp_generic
)
{
    // setup params
    const auto fn = [](void *ctx) -> void {
        auto *ptr = reinterpret_cast<int *>(ctx);
        ++*ptr;
    };
    int ctx = 0;

    // test
    fp_generic(fn, &ctx);
    ASSERT_EQ(1, ctx);
}

}  // namespace


#define SKIP_NULL_OP_FP_GENERIC(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).special_ops.fp_generic, "generic")


/// @brief Check that the non-atomic logic of transaction generic works correctly.
TEST_P(BtLogicTransaction, fp_generic)
{
    // check pre-condition
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_GENERIC(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.special_ops.fp_generic);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.special_ops.fp_generic);

    // make sure width is non-zero
    m_config.width = 1;

    // wrap operation
    const auto fp_generic = [&](void (* fn)(void *), void *ctx) -> void {
        patomic_transaction_result_t result {};
        m_ops.special_ops.fp_generic(fn, ctx, m_config, &result);
        ADD_FAILURE_TSX_SUCCESS(m_config, result);
    };

    // test
    test_generic(fp_generic);
}
