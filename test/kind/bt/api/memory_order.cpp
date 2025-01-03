// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <patomic/api/memory_order.h>

#include <test/common/support.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <vector>


/// @brief Test fixture.
class BtApiMemoryOrder : public testing::Test
{
public:
    
    /// @brief Return set of invalid memory orders.
    static const std::vector<int>&
    invalid_orders()
    {
        static const std::vector<int> orders {
            -1, -10, 10,
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max(),
        };
        return orders;
    }

    /// @brief Return set of all valid memory orders.
    static const std::vector<patomic_memory_order_t>&
    valid_orders()
    {
        static const auto orders = test::supported_orders();
        return orders;
    };
    
    /// @brief Return set of all memory orders valid for store operations.
    static const std::vector<patomic_memory_order_t>&
    store_orders()
    {
        static const auto orders = test::supported_orders_store();
        return orders;
    }
    
    /// @brief Return set of all memory orders valid for load operations.
    static const std::vector<patomic_memory_order_t>& 
    load_orders()
    {
        static const auto orders = test::supported_orders_load();
        return orders;
    };
    
    /// @brief Return set of all valid memory orders which are not valid for
    ///        store operations.
    static const std::vector<patomic_memory_order_t>&
    non_store_orders() 
    {
        static const auto get_orders = []() -> std::vector<patomic_memory_order_t> {
            using Bt = BtApiMemoryOrder;
            std::vector<patomic_memory_order_t> orders;
            for (auto order : test::supported_orders())
            {
                if (!Bt::contains(test::supported_orders_store(), order))
                {
                    orders.push_back(order);
                }
            }
            return orders;
        };
        static const auto orders = get_orders();
        return orders;
    }
    
    /// @brief Return set of all valid memory orders which are not valid for
    ///        load operations.
    static const std::vector<patomic_memory_order_t>&
    non_load_orders()
    {
        static const auto get_orders = []() -> std::vector<patomic_memory_order_t> {
            using Bt = BtApiMemoryOrder;
            std::vector<patomic_memory_order_t> orders;
            for (auto order : test::supported_orders())
            {
                if (!Bt::contains(test::supported_orders_load(), order))
                {
                    orders.push_back(order);
                }
            }
            return orders;
        };
        static const auto orders = get_orders();
        return orders;
    }

    /// @brief Check if a vector of memory orders contains a memory order.
    static bool
    contains(const std::vector<patomic_memory_order_t>& orders,
             const patomic_memory_order_t order) noexcept
    {
        const auto it = std::find(std::begin(orders), std::end(orders), order);
        return it != std::end(orders);
    }
};


/// @brief Valid orders are allowed by patomic_is_valid_order.
TEST_F(BtApiMemoryOrder, is_valid_order_allows_all_valid_orders)
{
    // test orders on function and macro
    for (const patomic_memory_order_t order : valid_orders())
    {
        EXPECT_EQ(1, patomic_is_valid_order(order));
        EXPECT_EQ(1, PATOMIC_IS_VALID_ORDER(order));
    }
}

/// @brief Invalid orders are rejected by patomic_is_valid_order.
TEST_F(BtApiMemoryOrder, is_valid_order_rejects_invalid_orders)
{
    // test orders on function and macro
    for (const int order : invalid_orders())
    {
        EXPECT_EQ(0, patomic_is_valid_order(order));
        EXPECT_EQ(0, PATOMIC_IS_VALID_ORDER(order));
    }
}

/// @brief Valid store orders are allowed by patomic_is_valid_store_order.
TEST_F(BtApiMemoryOrder, is_valid_store_order_allows_all_valid_store_orders)
{
    // test orders on function and macro
    for (const patomic_memory_order_t order : store_orders())
    {
        EXPECT_EQ(1, patomic_is_valid_store_order(order));
        EXPECT_EQ(1, PATOMIC_IS_VALID_STORE_ORDER(order));
    }
}

/// @brief Invalid store orders are rejected by patomic_is_valid_store_order.
TEST_F(BtApiMemoryOrder, is_valid_store_order_rejects_invalid_store_orders)
{
    // orders to test
    std::vector<int> bad_orders = invalid_orders();
    for (const int order : non_store_orders())
    {
        bad_orders.push_back(order);
    }

    // test orders on function and macro
    for (const int order : bad_orders)
    {
        EXPECT_EQ(0, patomic_is_valid_store_order(order));
        EXPECT_EQ(0, PATOMIC_IS_VALID_STORE_ORDER(order));
    }
}

/// @brief Valid load orders are allowed by patomic_is_valid_load_order.
TEST_F(BtApiMemoryOrder, is_valid_load_order_allows_all_valid_load_orders)
{
    // test orders on function and macro
    for (const patomic_memory_order_t order : load_orders())
    {
        EXPECT_EQ(1, patomic_is_valid_load_order(order));
        EXPECT_EQ(1, PATOMIC_IS_VALID_LOAD_ORDER(order));
    }
}

/// @brief Invalid load orders are rejected by patomic_is_valid_load_order.
TEST_F(BtApiMemoryOrder, is_valid_load_order_rejects_invalid_load_orders)
{
    // orders to test
    std::vector<int> bad_orders = invalid_orders();
    for (const int order : non_load_orders())
    {
        bad_orders.push_back(order);
    }

    // test orders on function and macro
    for (const int order : bad_orders)
    {
        EXPECT_EQ(0, patomic_is_valid_load_order(order));
        EXPECT_EQ(0, PATOMIC_IS_VALID_LOAD_ORDER(order));
    }
}

/// @brief Valid succ-fail order pairs are allowed by patomic_is_valid_fail_order.
TEST_F(BtApiMemoryOrder, is_valid_fail_order_allows_all_valid_pairs)
{
    // go through all combinations of orders
    for (const patomic_memory_order_t succ : valid_orders())
    {
        for (const patomic_memory_order_t fail : valid_orders())
        {
            // skip non-load fail orders or fail > succ
            if (fail > succ || contains(non_load_orders(), fail))
            {
                continue;
            }

            // test orders on function and macro
            EXPECT_EQ(1, patomic_is_valid_fail_order(succ, fail));
            EXPECT_EQ(1, PATOMIC_IS_VALID_FAIL_ORDER(succ, fail));
        }
    }
}

/// @brief Succ order less than fail order is rejected by patomic_is_valid_fail_order.
TEST_F(BtApiMemoryOrder, is_valid_fail_order_rejects_succ_lt_fail)
{
    // setup iterators
    const auto begin = std::begin(valid_orders());
    const auto end = std::end(valid_orders());

    // ensure orders are sorted
    ASSERT_TRUE(std::is_sorted(begin, end));

    // go through all order pairs where one order is smaller than the other
    for (auto fail_it = std::next(begin); fail_it < end; ++fail_it)
    {
        for (auto succ_it = begin; succ_it < fail_it; ++succ_it)
        {
            // test orders on function and macro
            EXPECT_EQ(0, patomic_is_valid_fail_order(*succ_it, *fail_it));
            EXPECT_EQ(0, PATOMIC_IS_VALID_FAIL_ORDER(*succ_it, *fail_it));
        }
    }
}

/// @brief Invalid succ order is rejected by patomic_is_valid_fail_order.
TEST_F(BtApiMemoryOrder, is_valid_fail_order_rejects_invalid_succ_order)
{
    // go through all combinations of orders
    for (const int succ : invalid_orders())
    {
        for (const patomic_memory_order_t fail : valid_orders())
        {
            // test orders on function and macro
            EXPECT_EQ(0, patomic_is_valid_fail_order(succ, fail));
            EXPECT_EQ(0, PATOMIC_IS_VALID_FAIL_ORDER(succ, fail));
        }
    }
}

/// @brief Invalid fail order is rejected by patomic_is_valid_fail_order.
TEST_F(BtApiMemoryOrder, is_valid_fail_order_rejects_invalid_fail_order)
{
    // fail is also invalid if it's a non-load order
    std::vector<int> bad_orders = invalid_orders();
    for (const int order : non_load_orders())
    {
        bad_orders.push_back(order);
    }

    // go through all combinations of orders
    for (const int fail : bad_orders)
    {
        for (const patomic_memory_order_t succ : valid_orders())
        {
            // test orders on function and macro
            EXPECT_EQ(0, patomic_is_valid_fail_order(succ, fail));
            EXPECT_EQ(0, PATOMIC_IS_VALID_FAIL_ORDER(succ, fail));
        }
    }
}

/// @brief Fail order is created from valid succ order by patomic_cmpxchg_fail_order.
TEST_F(BtApiMemoryOrder, cmpxchg_fail_order_converts_valid_succ_order)
{
    // test load orders on function and macro, stay the same
    for (const patomic_memory_order_t order : load_orders())
    {
        EXPECT_EQ(order, patomic_cmpxchg_fail_order(order));
        EXPECT_EQ(order, PATOMIC_CMPXCHG_FAIL_ORDER(order));
    }

    // test non-load orders on function and macro, are converted
    for (const patomic_memory_order_t order : non_load_orders())
    {
        EXPECT_EQ(patomic_ACQUIRE, patomic_cmpxchg_fail_order(order));
        EXPECT_EQ(patomic_ACQUIRE, PATOMIC_CMPXCHG_FAIL_ORDER(order));
    }
}

/// @brief Invalid succ order is returned directly by patomic_cmpxchg_fail_order.
TEST_F(BtApiMemoryOrder, cmpxchg_fail_order_returns_invalid_succ_order)
{
    // test orders on function and macro
    for (const int order : invalid_orders())
    {
        EXPECT_EQ(order, patomic_cmpxchg_fail_order(order));
        EXPECT_EQ(order, PATOMIC_CMPXCHG_FAIL_ORDER(order));
    }
}
