// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/skip.hpp>

#include <test/suite/bt_logic.hpp>


#define SKIP_NULL_OP_FP_CLEAR(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).flag_ops.fp_clear, "clear")


/// @brief Check that the non-atomic logic of transaction clear works correctly.
TEST_P(BtLogicTransaction, flag_fp_clear)
{
    // check pre-condition
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_CLEAR(p.id, m_ops);

    // wrap operation
    const auto fp_clear = [=](patomic_transaction_flag_t& flag) -> void {
        m_ops.flag_ops.fp_clear(&flag);
    };

    // setup
    patomic_transaction_flag_t flag {};

    // clear: unset -> unset
    fp_clear(flag);
    ASSERT_EQ(0, flag);

    // clear: set -> unset
    flag = 1;
    fp_clear(flag);
    ASSERT_EQ(0, flag);
}
