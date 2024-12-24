// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

#include <test/common/skip.hpp>

#include <test/suite/bt_logic.hpp>


// testy rather than test so that gtest doesn't think it's a test
#define SKIP_NULL_OP_FP_TESTY(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).flag_ops.fp_test, "test")


/// @brief Check that the non-atomic logic of transaction test works correctly.
TEST_P(BtLogicTransaction, flag_fp_test)
{
    // check pre-condition
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_TESTY(p.id, m_ops);

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
