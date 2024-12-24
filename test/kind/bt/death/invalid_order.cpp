// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

#include <patomic/patomic.h>

#include <test/common/death.hpp>
#include <test/common/generic_int.hpp>
#include <test/common/name.hpp>
#include <test/common/params.hpp>
#include <test/common/support.hpp>

#include <gtest/gtest.h>

#include <vector>


/// @brief Test fixture.
class BtDeathInvalidOrder : public testing::Test
{};


/// @brief Check that creating an implicit patomic object with an invalid
///        order is asserted.
TEST_F(BtDeathInvalidOrder, implicit)
{
    // invalid orders to test
    const std::vector<patomic_memory_order_t> invalid_orders {
        static_cast<patomic_memory_order_t>(-1),
        static_cast<patomic_memory_order_t>(6)
    };

    // go through all params
    const auto params = test::ParamsImplicit::combinations_with(invalid_orders);
    for (const auto& param : params)
    {
        // add trace
        SCOPED_TRACE(param.as_test_suffix());

        // attempt to create object with invalid memory order
        ASSERT_DEATH({
            static_cast<void>(patomic_create(
                param.width,
                param.order,
                param.options,
                patomic_kinds_ALL,
                param.id
            ));
        }, ".*");
    }
}


/// @brief Check that calling an explicit atomic operation with an invalid
///        order is asserted.
TEST_F(BtDeathInvalidOrder, explicit_any)
{
    // invalid orders to test
    const std::vector<patomic_memory_order_t> invalid_orders {
        static_cast<patomic_memory_order_t>(-1),
        static_cast<patomic_memory_order_t>(6)
    };

    // go through all params
    const auto params = test::ParamsImplicit::combinations_with(invalid_orders);
    for (const auto& param : params)
    {
        // add trace
        SCOPED_TRACE(param.as_test_suffix());

        // get operations
        const auto pao = patomic_create_explicit(
            param.width,
            param.options,
            patomic_kinds_ALL,
            param.id
        );
        const auto& ops = pao.ops;

        // create suitable aligned "atomic object"
        test::generic_integer p { param.width, pao.align.recommended, false };

        // attempt to call operations if non-null with invalid memory order

        // load/store
        ASSERT_DEATH_IF_NON_NULL(ops.fp_store, p, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.fp_load, p, param.order, p);

        // xchg
        ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_exchange, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_cmpxchg_weak, p, p, p, param.order, patomic_RELAXED);
        ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_cmpxchg_strong, p, p, p, param.order, patomic_RELAXED);
        ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_cmpxchg_weak, p, p, p, patomic_SEQ_CST, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_cmpxchg_strong, p, p, p, patomic_SEQ_CST, param.order);

        // bitwise
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test, p, 0, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test_compl, p, 0, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test_set, p, 0, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test_reset, p, 0, param.order);

        // binary
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_or, p, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_xor, p, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_and, p, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_not, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_or, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_xor, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_and, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_not, p, param.order, p);

        // arithmetic
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_add, p, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_sub, p, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_inc, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_dec, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_neg, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_add, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_sub, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_inc, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_dec, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_neg, p, param.order, p);
    }
}

/// @brief Check that calling an explicit atomic cmpxchg operation with valid
///        success and fail orders but fail order isn't valid for the given
///        success is asserted.
TEST_F(BtDeathInvalidOrder, explicit_fail)
{
    // go through all params
    const auto params = test::ParamsImplicit::combinations_with(test::supported_orders());
    for (const auto& param : params)
    {
        // invalid orders to test
        std::vector<patomic_memory_order_t> invalid_orders {};
        for (auto order : test::supported_orders())
        {

            if (!patomic_is_valid_fail_order(param.order, order))
            {
                invalid_orders.push_back(order);
            }
        }

        // go through all invalid orders
        for (auto fail_order : invalid_orders)
        {
            // add trace
            SCOPED_TRACE("fail order: " + test::name_order(fail_order));
            SCOPED_TRACE(param.as_test_suffix());

            // get operations
            const auto pao = patomic_create_explicit(
                    param.width,
                    param.options,
                    patomic_kinds_ALL,
                    param.id
            );
            const auto& ops = pao.ops;

            // create suitable aligned "atomic object"
            test::generic_integer p { param.width, pao.align.recommended, false };

            // attempt to call cmpxchg operations if non-null with invalid fail order
            ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_cmpxchg_weak, p, p, p, param.order, fail_order);
            ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_cmpxchg_strong, p, p, p, param.order, fail_order);
        }
    }
}


/// @brief Check that calling an explicit atomic store operation with an
///        invalid store order that is otherwise a valid order is asserted.
TEST_F(BtDeathInvalidOrder, explicit_store)
{
    // invalid orders to test
    std::vector<patomic_memory_order_t> invalid_orders {};
    for (auto order : test::supported_orders())
    {
        if (!patomic_is_valid_store_order(order))
        {
            invalid_orders.push_back(order);
        }
    }

    // go through all params
    const auto params = test::ParamsImplicit::combinations_with(invalid_orders);
    for (const auto& param : params)
    {
        // add trace
        SCOPED_TRACE(param.as_test_suffix());

        // get operations
        const auto pao = patomic_create_explicit(
            param.width,
            param.options,
            patomic_kinds_ALL,
            param.id
        );
        const auto& ops = pao.ops;

        // create suitable aligned "atomic object"
        test::generic_integer p { param.width, pao.align.recommended, false };

        // attempt to call store operations if non-null with invalid memory order
        ASSERT_DEATH_IF_NON_NULL(ops.fp_store, p, p, param.order);
    }
}


/// @brief Check that calling an explicit atomic load operation with an invalid
///        load order that is otherwise a valid order is asserted.
TEST_F(BtDeathInvalidOrder, explicit_load)
{
    // invalid orders to test
    std::vector<patomic_memory_order_t> invalid_orders {};
    for (auto order : test::supported_orders())
    {
        if (!patomic_is_valid_load_order(order))
        {
            invalid_orders.push_back(order);
        }
    }

    // go through all params
    const auto params = test::ParamsImplicit::combinations_with(invalid_orders);
    for (const auto& param : params)
    {
        // add trace
        SCOPED_TRACE(param.as_test_suffix());

        // get operations
        const auto pao = patomic_create_explicit(
            param.width,
            param.options,
            patomic_kinds_ALL,
            param.id
        );
        const auto& ops = pao.ops;

        // create suitable aligned "atomic object"
        test::generic_integer p { param.width, pao.align.recommended, false };

        // attempt to call load operations if non-null with invalid memory order
        ASSERT_DEATH_IF_NON_NULL(ops.fp_load, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test, p, 0, param.order);
    }
}
