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
test_fetch_dec(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        void *ret
    )>& fp_fetch_dec
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };

#define DO_TEST_FETCH_DEC()        \
    object_old = object;           \
    fp_fetch_dec(object, ret);     \
    ASSERT_EQ(ret, object_old);    \
    object_old.dec();              \
    ASSERT_EQ(object, object_old)

    // --2 -> 1
    object.inc();
    object.inc();
    DO_TEST_FETCH_DEC();

    // --1 -> 0
    DO_TEST_FETCH_DEC();

    // --0 -> max
    DO_TEST_FETCH_DEC();

    // --max -> (max - 1)
    DO_TEST_FETCH_DEC();
}

void
test_dec(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object
    )>& fp_dec
)
{
    // wrap as fetch
    auto fetch_dec = [&](void *object, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_dec(object);
    };

    // defer to fetch variant
    return test_fetch_dec(width, align, fetch_dec);
}

}  // namespace


#define SKIP_NULL_OP_FP_DEC(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).arithmetic_ops.fp_dec, "dec")

#define SKIP_NULL_OP_FP_FETCH_DEC(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).arithmetic_ops.fp_fetch_dec, "fetch_dec")


/// @brief Check that the non-atomic logic of implicit dec works correctly.
TEST_P(BtLogicImplicit, fp_dec)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_DEC(p.id, m_ops);

    // wrap operation
    const auto fp_dec = [&](void *object) -> void {
        return m_ops.arithmetic_ops.fp_dec(object);
    };

    // test
    test_dec(p.width, m_align.recommended, fp_dec);
}

/// @brief Check that the non-atomic logic of implicit fetch_dec works correctly.
TEST_P(BtLogicImplicit, fp_fetch_dec)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_DEC(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_dec = [&](void *object, void *ret) -> void {
        return m_ops.arithmetic_ops.fp_fetch_dec(object, ret);
    };

    // test
    test_fetch_dec(p.width, m_align.recommended, fp_fetch_dec);
}


/// @brief Check that the non-atomic logic of explicit dec works correctly.
TEST_P(BtLogicExplicit, fp_dec)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_DEC(p.id, m_ops);

    // wrap operation
    const auto fp_dec = [&](void *object) -> void {
        return m_ops.arithmetic_ops.fp_dec(object, p.order);
    };

    // test
    test_dec(p.width, m_align.recommended, fp_dec);
}

/// @brief Check that the non-atomic logic of explicit fetch_dec works correctly.
TEST_P(BtLogicExplicit, fp_fetch_dec)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_DEC(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_dec = [&](void *object, void *ret) -> void {
        return m_ops.arithmetic_ops.fp_fetch_dec(object, p.order, ret);
    };

    // test
    test_fetch_dec(p.width, m_align.recommended, fp_fetch_dec);
}


/// @brief Check that the non-atomic logic of transaction dec works correctly.
TEST_P(BtLogicTransaction, fp_dec)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_DEC(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.arithmetic_ops.fp_dec);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.arithmetic_ops.fp_dec);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE("width: " + std::to_string(width));
        m_config.width = width;

        // wrap operation
        const auto fp_dec = [&](void *object) -> void {
            patomic_transaction_result_t result {};
            m_ops.arithmetic_ops.fp_dec(object, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, dec);
    }
}

/// @brief Check that the non-atomic logic of transaction fetch_dec works correctly.
TEST_P(BtLogicTransaction, fp_fetch_dec)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_DEC(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.arithmetic_ops.fp_fetch_dec);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.arithmetic_ops.fp_fetch_dec);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE("width: " + std::to_string(width));
        m_config.width = width;

        // wrap operation
        const auto fp_fetch_dec = [&](void *object, void *ret) -> void {
            patomic_transaction_result_t result {};
            m_ops.arithmetic_ops.fp_fetch_dec(object, ret, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, fetch_dec);
    }
}
