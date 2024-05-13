#include <patomic/types/memory_order.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <limits>


/// @brief Test fixture.
class BtTypesMemoryOrder : public testing::Test
{};


/// @brief Valid orders are allowed by patomic_is_valid_order.
TEST_F(BtTypesMemoryOrder, patomic_is_valid_order_allows_all_valid_orders)
{
    // orders to test
    std::vector<patomic_memory_order_t> valid_orders {
        patomic_RELAXED,
        patomic_CONSUME,
        patomic_ACQUIRE,
        patomic_RELEASE,
        patomic_ACQ_REL,
        patomic_SEQ_CST
    };

    // test orders on function and macro
    for (patomic_memory_order_t order : valid_orders)
    {
        EXPECT_EQ(1, patomic_is_valid_order(order));
        EXPECT_EQ(1, PATOMIC_IS_VALID_ORDER(order));
    }
}

/// @brief Invalid orders are rejected by patomic_is_valid_order.
TEST_F(BtTypesMemoryOrder, patomic_is_valid_order_rejects_invalid_orders)
{
    // orders to test
    std::vector<int> invalid_orders {
        -1, -10, 10,
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
    };

    // test orders on function and macro
    for (int order : invalid_orders)
    {
        EXPECT_EQ(0, patomic_is_valid_order(order));
        EXPECT_EQ(0, PATOMIC_IS_VALID_ORDER(order));
    }
}

/// @brief Valid store orders are allowed by patomic_is_valid_store_order.
TEST_F(BtTypesMemoryOrder, patomic_is_valid_store_order_allows_all_valid_store_orders)
{
    // orders to test
    std::vector<patomic_memory_order_t> valid_orders {
        patomic_RELAXED,
        patomic_RELEASE,
        patomic_ACQ_REL,
        patomic_SEQ_CST
    };

    // test orders on function and macro
    for (patomic_memory_order_t order : valid_orders)
    {
        EXPECT_EQ(1, patomic_is_valid_store_order(order));
        EXPECT_EQ(1, PATOMIC_IS_VALID_STORE_ORDER(order));
    }
}

/// @brief Invalid store orders are rejected by patomic_is_valid_store_order.
TEST_F(BtTypesMemoryOrder, patomic_is_valid_store_order_rejects_invalid_store_orders)
{
    // orders to test
    std::vector<int> invalid_orders {
        -1, -10, 10,
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max(),
        patomic_CONSUME,
        patomic_ACQUIRE
    };

    // test orders on function and macro
    for (int order : invalid_orders)
    {
        EXPECT_EQ(0, patomic_is_valid_store_order(order));
        EXPECT_EQ(0, PATOMIC_IS_VALID_STORE_ORDER(order));
    }
}

/// @brief Valid load orders are allowed by patomic_is_valid_load_order.
TEST_F(BtTypesMemoryOrder, patomic_is_valid_load_order_allows_all_valid_load_orders)
{
    // orders to test
    std::vector<patomic_memory_order_t> valid_orders {
        patomic_RELAXED,
        patomic_CONSUME,
        patomic_ACQUIRE,
        patomic_SEQ_CST
    };

    // test orders on function and macro
    for (patomic_memory_order_t order : valid_orders)
    {
        EXPECT_EQ(1, patomic_is_valid_load_order(order));
        EXPECT_EQ(1, PATOMIC_IS_VALID_LOAD_ORDER(order));
    }
}

/// @brief Invalid load orders are rejected by patomic_is_valid_load_order.
TEST_F(BtTypesMemoryOrder, patomic_is_valid_load_order_rejects_invalid_load_orders)
{
    // orders to test
    std::vector<int> invalid_orders {
        -1, -10, 10,
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max(),
        patomic_RELEASE,
        patomic_ACQ_REL
    };

    // test orders on function and macro
    for (int order : invalid_orders)
    {
        EXPECT_EQ(0, patomic_is_valid_load_order(order));
        EXPECT_EQ(0, PATOMIC_IS_VALID_LOAD_ORDER(order));
    }
}

/// @brief Valid succ-fail order pairs are allowed by patomic_is_valid_fail_order.
TEST_F(BtTypesMemoryOrder, patomic_is_valid_fail_order_allows_all_valid_pairs)
{
    // all valid orders
    std::vector<patomic_memory_order_t> orders {
        patomic_RELAXED,
        patomic_CONSUME,
        patomic_ACQUIRE,
        patomic_RELEASE,
        patomic_ACQ_REL,
        patomic_SEQ_CST
    };

    // go through all combinations of orders
    for (patomic_memory_order_t succ : orders)
    {
        for (patomic_memory_order_t fail : orders)
        {
            // skip non-load fail orders or fail > succ
            if ( (fail > succ)             ||
                 (fail == patomic_RELEASE) ||
                 (fail == patomic_ACQ_REL) )
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
TEST_F(BtTypesMemoryOrder, patomic_is_valid_fail_order_rejects_succ_lt_fail)
{
    // orders to test
    std::vector<patomic_memory_order_t> orders {
        patomic_RELAXED,
        patomic_CONSUME,
        patomic_ACQUIRE,
        patomic_RELEASE,
        patomic_ACQ_REL,
        patomic_SEQ_CST
    };

    // setup iterators
    const auto begin = std::begin(orders);
    const auto end = std::end(orders);

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
TEST_F(BtTypesMemoryOrder, patomic_is_valid_fail_order_rejects_invalid_succ_order)
{
    // invalid orders to test as succ order
    std::vector<int> invalid_orders {
        -1, -10, 10,
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
    };

    // valid orders to test as fail order
    // only care about if it's a valid order, not if it's a valid fail order
    std::vector<patomic_memory_order_t> valid_orders {
        patomic_RELAXED,
        patomic_CONSUME,
        patomic_ACQUIRE,
        patomic_RELEASE,
        patomic_ACQ_REL,
        patomic_SEQ_CST
    };

    // go through all combinations of orders
    for (int succ : invalid_orders)
    {
        for (patomic_memory_order_t fail : valid_orders)
        {
            // test orders on function and macro
            EXPECT_EQ(0, patomic_is_valid_fail_order(succ, fail));
            EXPECT_EQ(0, PATOMIC_IS_VALID_FAIL_ORDER(succ, fail));
        }
    }
}

/// @brief Invalid fail order is rejected by patomic_is_valid_fail_order.
TEST_F(BtTypesMemoryOrder, patomic_is_valid_fail_order_rejects_invalid_fail_order)
{
    // invalid orders to test as fail order
    std::vector<int> invalid_orders {
        -1, -10, 10,
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max(),
        patomic_RELEASE,
        patomic_ACQ_REL
    };

    // valid orders to test as succ order
    std::vector<patomic_memory_order_t> valid_orders {
        patomic_RELAXED,
        patomic_CONSUME,
        patomic_ACQUIRE,
        patomic_RELEASE,
        patomic_ACQ_REL,
        patomic_SEQ_CST
    };

    // go through all combinations of orders
    for (int fail : invalid_orders)
    {
        for (patomic_memory_order_t succ : valid_orders)
        {
            // test orders on function and macro
            EXPECT_EQ(0, patomic_is_valid_fail_order(succ, fail));
            EXPECT_EQ(0, PATOMIC_IS_VALID_FAIL_ORDER(succ, fail));
        }
    }
}

/// @brief Fail order is created from valid succ order by patomic_cmpxchg_fail_order.
TEST_F(BtTypesMemoryOrder, patomic_cmpxchg_fail_order_converts_valid_succ_order)
{
    // orders that stay the same (load orders)
    std::vector<patomic_memory_order_t> same_orders {
        patomic_RELAXED,
        patomic_CONSUME,
        patomic_ACQUIRE,
        patomic_SEQ_CST
    };

    // test load orders on function and macro
    for (patomic_memory_order_t order : same_orders)
    {
        EXPECT_EQ(order, patomic_cmpxchg_fail_order(order));
        EXPECT_EQ(order, PATOMIC_CMPXCHG_FAIL_ORDER(order));
    }

    // orders that change (non-load orders)
    std::vector<patomic_memory_order_t> diff_orders {
        patomic_RELEASE,
        patomic_ACQ_REL
    };

    // test non-load orders on function and macro
    for (patomic_memory_order_t order : diff_orders)
    {
        EXPECT_EQ(patomic_ACQUIRE, patomic_cmpxchg_fail_order(order));
        EXPECT_EQ(patomic_ACQUIRE, PATOMIC_CMPXCHG_FAIL_ORDER(order));
    }
}

/// @brief Invalid succ order is returned directly by patomic_cmpxchg_fail_order.
TEST_F(BtTypesMemoryOrder, patomic_cmpxchg_fail_order_returns_invalid_succ_order)
{
    // orders to test
    std::vector<int> invalid_orders {
        -1, -10, 10,
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
    };

    // test orders on function and macro
    for (int order : invalid_orders)
    {
        EXPECT_EQ(order, patomic_cmpxchg_fail_order(order));
        EXPECT_EQ(order, PATOMIC_CMPXCHG_FAIL_ORDER(order));
    }
}
