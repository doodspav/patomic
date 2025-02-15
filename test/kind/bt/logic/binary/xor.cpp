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
test_fetch_xor(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument,
        void *ret
    )>& fp_fetch_xor
)
{
    // start with all bits unset
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer argument { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };
    test::generic_integer object_exp { width, align, false };
    test::generic_integer argument_old { width, align, false };

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        SCOPED_TRACE("index: " + std::to_string(i));

        // set up params
        object.inv_at(i);
        object_old = object;

        // set up expected
        object_exp = object;

        // test
        fp_fetch_xor(object, argument, ret);
        ASSERT_EQ(object, object_exp);
        ASSERT_EQ(argument, argument_old);
        ASSERT_EQ(ret, object_old);

        // test backwards
        object = object_old;
        fp_fetch_xor(argument, object, ret);
        ASSERT_EQ(argument, object_exp);
        ASSERT_EQ(object, object_old);
        ASSERT_EQ(ret, argument_old);

        // reset
        argument = argument_old;
        object = argument;
    }

    // start with all bits set
    object.inv();
    argument.inv();
    argument_old = argument;

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        SCOPED_TRACE("index: " + std::to_string(i));

        // set up params
        object.inv_at(i);
        object_old = object;

        // set up expected
        object_exp = object;
        object_exp.inv();

        // test
        fp_fetch_xor(object, argument, ret);
        ASSERT_EQ(object, object_exp);
        ASSERT_EQ(argument, argument_old);
        ASSERT_EQ(ret, object_old);

        // test backwards
        object = object_old;
        fp_fetch_xor(argument, object, ret);
        ASSERT_EQ(argument, object_exp);
        ASSERT_EQ(object, object_old);
        ASSERT_EQ(ret, argument_old);

        // reset
        argument = argument_old;
        object = argument;
    }
}

void
test_xor(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument
    )>& fp_xor
)
{
    // wrap as fetch
    auto fetch_xor = [&](void *object, const void *argument, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_xor(object, argument);
    };

    // defer to fetch variant
    return test_fetch_xor(width, align, fetch_xor);
}

}  // namespace


#define SKIP_NULL_OP_FP_XOR(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).binary_ops.fp_xor, "xor")

#define SKIP_NULL_OP_FP_FETCH_XOR(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).binary_ops.fp_fetch_xor, "fetch_xor")


/// @brief Check that the non-atomic logic of implicit xor works correctly.
TEST_P(BtLogicImplicit, fp_xor)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_XOR(p.id, m_ops);

    // wrap operation
    const auto fp_xor = [&](void *object, const void *argument) -> void {
        return m_ops.binary_ops.fp_xor(object, argument);
    };

    // test
    test_xor(p.width, m_align.recommended, fp_xor);
}

/// @brief Check that the non-atomic logic of implicit fetch_xor works correctly.
TEST_P(BtLogicImplicit, fp_fetch_xor)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_XOR(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_xor = [&](void *object, const void *argument, void *ret) -> void {
        return m_ops.binary_ops.fp_fetch_xor(object, argument, ret);
    };

    // test
    test_fetch_xor(p.width, m_align.recommended, fp_fetch_xor);
}


/// @brief Check that the non-atomic logic of explicit xor works correctly.
TEST_P(BtLogicExplicit, fp_xor)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_XOR(p.id, m_ops);

    // wrap operation
    const auto fp_xor = [&](void *object, const void *argument) -> void {
        return m_ops.binary_ops.fp_xor(object, argument, p.order);
    };

    // test
    test_xor(p.width, m_align.recommended, fp_xor);
}

/// @brief Check that the non-atomic logic of explicit fetch_xor works correctly.
TEST_P(BtLogicExplicit, fp_fetch_xor)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_XOR(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_xor = [&](void *object, const void *argument, void *ret) -> void {
        return m_ops.binary_ops.fp_fetch_xor(object, argument, p.order, ret);
    };

    // test
    test_fetch_xor(p.width, m_align.recommended, fp_fetch_xor);
}


/// @brief Check that the non-atomic logic of transaction xor works correctly.
TEST_P(BtLogicTransaction, fp_xor)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_XOR(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.binary_ops.fp_xor);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.binary_ops.fp_xor);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE("width: " + std::to_string(width));
        m_config.width = width;

        // wrap operation
        const auto fp_xor = [&](void *object, const void *argument) -> void {
            patomic_transaction_result_t result {};
            m_ops.binary_ops.fp_xor(object, argument, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, xor);
    }
}

/// @brief Check that the non-atomic logic of transaction fetch_xor works correctly.
TEST_P(BtLogicTransaction, fp_fetch_xor)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_XOR(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.binary_ops.fp_fetch_xor);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.binary_ops.fp_fetch_xor);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE("width: " + std::to_string(width));
        m_config.width = width;

        // wrap operation
        const auto fp_fetch_xor = [&](void *object, const void *argument, void *ret) -> void {
            patomic_transaction_result_t result {};
            m_ops.binary_ops.fp_fetch_xor(object, argument, ret, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, fetch_xor);
    }
}
