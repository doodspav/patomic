// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_load(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        const void *object,
        void *ret
    )>& fp_load
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };

#define DO_TEST_LOAD()         \
    ASSERT_NE(object, ret);    \
    object_old = object;       \
    fp_load(object, ret);      \
    ASSERT_EQ(object, ret);    \
    ASSERT_EQ(ret, object_old)

    // set ret to non-zero
    ret.inc();

    // 0
    DO_TEST_LOAD();

    // ~0
    object.inv();
    DO_TEST_LOAD();

    // 1
    object.store_zero();
    object.inc();
    DO_TEST_LOAD();

    // max
    object.store_max();
    DO_TEST_LOAD();

    // min
    object.store_min();
    DO_TEST_LOAD();
}

}  // namespace


#define SKIP_NULL_OP_FP_LOAD(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).fp_load, "load")


/// @brief Check that the non-atomic logic of implicit load works correctly.
TEST_P(BtLogicImplicitLoad, fp_load)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_LOAD(p.id, m_ops);

    // wrap operation
    const auto fp_load = [&](const void *object, void *ret) -> void {
        return m_ops.fp_load(object, ret);
    };

    // test
    test_load(p.width, m_align.recommended, fp_load);
}


/// @brief Check that the non-atomic logic of explicit load works correctly.
TEST_P(BtLogicExplicitLoad, fp_load)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_LOAD(p.id, m_ops);

    // wrap operation
    const auto fp_load = [&](const void *object, void *ret) -> void {
        return m_ops.fp_load(object, p.order, ret);
    };

    // test
    test_load(p.width, m_align.recommended, fp_load);
}


/// @brief Check that the non-atomic logic of transaction load works correctly.
TEST_P(BtLogicTransaction, fp_load)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_LOAD(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.fp_load);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.fp_load);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE("width: " + std::to_string(width));
        m_config.width = width;

        // wrap operation
        const auto fp_load = [&](const void *object, void *ret) -> void {
            patomic_transaction_result_t result {};
            m_ops.fp_load(object, ret, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, load);
    }
}
