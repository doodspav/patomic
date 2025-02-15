// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>

#include <cstring>


namespace
{

void
test_fetch_not(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        void *ret
    )>& fp_fetch_not
)
{
    // start with all bits unset
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        SCOPED_TRACE("index: " + std::to_string(i));

        // set up object
        object.inv_at(i);
        object_old = object;

        // test
        fp_fetch_not(object, ret);
        ASSERT_EQ(ret, object_old);
        object.inv();
        ASSERT_EQ(object, object_old);

        // reset
        object.inv_at(i);
    }

    // start with all bits set
    object.inv();

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        SCOPED_TRACE("index: " + std::to_string(i));

        // set up object
        object.inv_at(i);
        object_old = object;

        // test
        fp_fetch_not(object, ret);
        ASSERT_EQ(ret, object_old);
        object.inv();
        ASSERT_EQ(object, object_old);

        // reset
        object.inv_at(i);
    }
}

void
test_not(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object
    )>& fp_not
)
{
    // wrap as fetch
    auto fetch_not = [&](void *object, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_not(object);
    };

    // defer to fetch variant
    return test_fetch_not(width, align, fetch_not);
}

}  // namespace

#define SKIP_NULL_OP_FP_NOT(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).binary_ops.fp_not, "not")

#define SKIP_NULL_OP_FP_FETCH_NOT(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).binary_ops.fp_fetch_not, "fetch_not")


/// @brief Check that the non-atomic logic of implicit not works correctly.
TEST_P(BtLogicImplicit, fp_not)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_NOT(p.id, m_ops);

    // wrap operation
    const auto fp_not = [&](void *object) -> void {
        return m_ops.binary_ops.fp_not(object);
    };

    // test
    test_not(p.width, m_align.recommended, fp_not);
}

/// @brief Check that the non-atomic logic of implicit fetch_not works correctly.
TEST_P(BtLogicImplicit, fp_fetch_not)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_NOT(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_not = [&](void *object, void *ret) -> void {
        return m_ops.binary_ops.fp_fetch_not(object, ret);
    };

    // test
    test_fetch_not(p.width, m_align.recommended, fp_fetch_not);
}


/// @brief Check that the non-atomic logic of explicit not works correctly.
TEST_P(BtLogicExplicit, fp_not)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_NOT(p.id, m_ops);

    // wrap operation
    const auto fp_not = [&](void *object) -> void {
        return m_ops.binary_ops.fp_not(object, p.order);
    };

    // test
    test_not(p.width, m_align.recommended, fp_not);
}

/// @brief Check that the non-atomic logic of explicit fetch_not works correctly.
TEST_P(BtLogicExplicit, fp_fetch_not)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_NOT(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_not = [&](void *object, void *ret) -> void {
        return m_ops.binary_ops.fp_fetch_not(object, p.order, ret);
    };

    // test
    test_fetch_not(p.width, m_align.recommended, fp_fetch_not);
}


/// @brief Check that the non-atomic logic of transaction not works correctly.
TEST_P(BtLogicTransaction, fp_not)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_NOT(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.binary_ops.fp_not);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.binary_ops.fp_not);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE("width: " + std::to_string(width));
        m_config.width = width;

        // wrap operation
        const auto fp_not = [&](void *object) -> void {
            patomic_transaction_result_t result {};
            m_ops.binary_ops.fp_not(object, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, not);
    }
}

/// @brief Check that the non-atomic logic of transaction fetch_not works correctly.
TEST_P(BtLogicTransaction, fp_fetch_not)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_NOT(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.binary_ops.fp_fetch_not);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.binary_ops.fp_fetch_not);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE("width: " + std::to_string(width));
        m_config.width = width;

        // wrap operation
        const auto fp_fetch_not = [&](void *object, void *ret) -> void {
            patomic_transaction_result_t result {};
            m_ops.binary_ops.fp_fetch_not(object, ret, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, fetch_not);
    }
}
