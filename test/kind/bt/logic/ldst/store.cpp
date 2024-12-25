// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_store(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *desired
    )>& fp_store
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer desired { width, align, false };
    // old
    test::generic_integer desired_old { width, align, false };

#define DO_TEST_STORE()             \
    desired_old = desired;          \
    fp_store(object, desired);      \
    ASSERT_EQ(object, desired);     \
    ASSERT_EQ(desired, desired_old)

    // 0 -> ~0
    desired.inv();
    DO_TEST_STORE();

    // ~0 -> 1
    desired.store_zero();
    desired.inc();
    DO_TEST_STORE();

    // 1 -> max
    desired.store_max();
    DO_TEST_STORE();

    // max -> min
    desired.store_min();
    DO_TEST_STORE();

    // min -> 0
    desired.store_zero();
    DO_TEST_STORE();
}

}  // namespace


#define SKIP_NULL_OP_FP_STORE(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).fp_store, "store")


/// @brief Check that the non-atomic logic of implicit store works correctly.
TEST_P(BtLogicImplicitStore, fp_store)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_STORE(p.id, m_ops);

    // wrap operation
    const auto fp_store = [&](void *object, const void *desired) -> void {
        return m_ops.fp_store(object, desired);
    };

    // test
    test_store(p.width, m_align.recommended, fp_store);
}


/// @brief Check that the non-atomic logic of explicit store works correctly.
TEST_P(BtLogicExplicitStore, fp_store)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_STORE(p.id, m_ops);

    // wrap operation
    const auto fp_store = [&](void *object, const void *desired) -> void {
        return m_ops.fp_store(object, desired, p.order);
    };

    // test
    test_store(p.width, m_align.recommended, fp_store);
}


/// @brief Check that the non-atomic logic of transaction store works correctly.
TEST_P(BtLogicTransaction, fp_store)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_STORE(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.fp_store);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.fp_store);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config.width = width;

        // wrap operation
        const auto fp_store = [=](void *object, const void *desired) -> void {
            patomic_transaction_result_t result {};
            m_ops.fp_store(object, desired, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, store);
    }
}
