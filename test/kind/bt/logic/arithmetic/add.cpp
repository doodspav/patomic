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
test_fetch_add(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument,
        void *ret
    )>& fp_fetch_add
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer argument { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };
    test::generic_integer object_exp { width, align, false };
    test::generic_integer argument_old { width, align, false };

    // since addition is commutative, all tests are done with swapped arguments
    //   as well as normal ordering
    // object and argument will have the values as if only normal ordering was
    //   done
#define DO_TEST_FETCH_ADD()              \
    object_old = object;                 \
    argument_old = argument;             \
    object_exp = object;                 \
    object_exp.add(argument);            \
    fp_fetch_add(object, argument, ret); \
    ASSERT_EQ(object, object_exp);       \
    ASSERT_EQ(argument, argument_old);   \
    ASSERT_EQ(ret, object_old);          \
    object = object_old;                 \
    object_old.swap(argument_old);       \
    ret.inc();                           \
    fp_fetch_add(argument, object, ret); \
    ASSERT_EQ(argument, object_exp);     \
    ASSERT_EQ(object, argument_old);     \
    ASSERT_EQ(ret, object_old);          \
    object.swap(argument)

    // 0 + 0 -> 0
    DO_TEST_FETCH_ADD();

    // 0 + 1 -> 1
    argument.inc();
    DO_TEST_FETCH_ADD();

    // 0 + 2 -> 2
    object.store_zero();
    argument.inc();
    DO_TEST_FETCH_ADD();

    // 0 + max -> max
    object.store_zero();
    argument.store_max();
    DO_TEST_FETCH_ADD();

    // 1 + 1 -> 2
    object.store_zero();
    object.inc();
    argument = object;
    DO_TEST_FETCH_ADD();

    // 1 + 2 -> 3
    object.swap(argument);
    DO_TEST_FETCH_ADD();

    // 1 + max -> 0
    object.store_zero();
    object.inc();
    argument.store_max();
    DO_TEST_FETCH_ADD();

    // max + 2 -> 1
    object.store_max();
    argument.store_zero();
    argument.inc();
    argument.inc();
    DO_TEST_FETCH_ADD();

    // max + max -> (max - 1)
    object.store_max();
    argument.store_max();
    DO_TEST_FETCH_ADD();

    // (max - 1) + 0 -> (max - 1)
    argument.store_zero();
    DO_TEST_FETCH_ADD();

    // (max - 1) + 1 -> max
    argument.inc();
    DO_TEST_FETCH_ADD();

    // (max - 1) + 2 -> 0
    object.dec();
    argument.inc();
    DO_TEST_FETCH_ADD();

    // (max - 1) + max -> (max - 2)
    object.store_max();
    object.dec();
    argument.store_max();
    DO_TEST_FETCH_ADD();
}

void
test_add(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument
    )>& fp_add
)
{
    // wrap as fetch
    auto fetch_add = [&](void *object, const void *argument, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_add(object, argument);
    };

    // defer to fetch variant
    return test_fetch_add(width, align, fetch_add);
}

}  // namespace


#define SKIP_NULL_OP_FP_ADD(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).arithmetic_ops.fp_add, "add")

#define SKIP_NULL_OP_FP_FETCH_ADD(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).arithmetic_ops.fp_fetch_add, "fetch_add")


/// @brief Check that the non-atomic logic of implicit add works correctly.
TEST_P(BtLogicImplicit, fp_add)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_ADD(p.id, m_ops);

    // wrap operation
    const auto fp_add = [&](void *object, const void *argument) -> void {
        return m_ops.arithmetic_ops.fp_add(object, argument);
    };

    // test
    test_add(p.width, m_align.recommended, fp_add);
}

/// @brief Check that the non-atomic logic of implicit fetch_add works correctly.
TEST_P(BtLogicImplicit, fp_fetch_add)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_ADD(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_add = [&](void *object, const void *argument, void *ret) -> void {
        return m_ops.arithmetic_ops.fp_fetch_add(object, argument, ret);
    };

    // test
    test_fetch_add(p.width, m_align.recommended, fp_fetch_add);
}


/// @brief Check that the non-atomic logic of explicit add works correctly.
TEST_P(BtLogicExplicit, fp_add)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_ADD(p.id, m_ops);

    // wrap operation
    const auto fp_add = [&](void *object, const void *argument) -> void {
        return m_ops.arithmetic_ops.fp_add(object, argument, p.order);
    };

    // test
    test_add(p.width, m_align.recommended, fp_add);
}

/// @brief Check that the non-atomic logic of explicit fetch_add works correctly.
TEST_P(BtLogicExplicit, fp_fetch_add)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_ADD(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_add = [&](void *object, const void *argument, void *ret) -> void {
        return m_ops.arithmetic_ops.fp_fetch_add(object, argument, p.order, ret);
    };

    // test
    test_fetch_add(p.width, m_align.recommended, fp_fetch_add);
}


/// @brief Check that the non-atomic logic of transaction add works correctly.
TEST_P(BtLogicTransaction, fp_add)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_ADD(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.arithmetic_ops.fp_add);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.arithmetic_ops.fp_add);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE("width: " + std::to_string(width));
        m_config.width = width;

        // wrap operation
        const auto fp_add = [&](void *object, const void *argument) -> void {
            patomic_transaction_result_t result {};
            m_ops.arithmetic_ops.fp_add(object, argument, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, add);
    }
}

/// @brief Check that the non-atomic logic of transaction fetch_add works correctly.
TEST_P(BtLogicTransaction, fp_fetch_add)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_ADD(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.arithmetic_ops.fp_fetch_add);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.arithmetic_ops.fp_fetch_add);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE("width: " + std::to_string(width));
        m_config.width = width;

        // wrap operation
        const auto fp_fetch_add = [&](void *object, const void *argument, void *ret) -> void {
            patomic_transaction_result_t result {};
            m_ops.arithmetic_ops.fp_fetch_add(object, argument, ret, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, fetch_add);
    }
}
