// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{
    
void
test_cmpxchg_strong(
    std::size_t width,
    std::size_t align,
    const std::function<int(
        void *object,
        void *expected,
        const void *desired
    )>& fp_cmpxchg_strong
)
{
    // create integers
    int ok;
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer expected { width, align, false };
    test::generic_integer desired { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };
    test::generic_integer expected_old { width, align, false };
    test::generic_integer desired_old { width, align, false };

#define DO_TEST_CMPXCHG_STRONG()                       \
    expected = object;                                 \
    expected.inv();                                    \
    object_old = object;                               \
    desired_old = desired;                             \
    ok = fp_cmpxchg_strong(object, expected, desired); \
    ASSERT_FALSE(ok);                                  \
    ASSERT_EQ(object, object_old);                     \
    ASSERT_EQ(expected, object);                       \
    ASSERT_EQ(desired, desired_old);                   \
    expected_old = expected;                           \
    ok = fp_cmpxchg_strong(object, expected, desired); \
    ASSERT_TRUE(ok);                                   \
    ASSERT_EQ(object, desired);                        \
    ASSERT_EQ(expected, expected_old);                 \
    ASSERT_EQ(desired, desired_old)

    // 0 -> ~0
    desired.inv();
    DO_TEST_CMPXCHG_STRONG();

    // ~0 -> 1
    desired.store_zero();
    desired.inc();
    DO_TEST_CMPXCHG_STRONG();

    // 1 -> max
    desired.store_max();
    DO_TEST_CMPXCHG_STRONG();

    // max -> min
    desired.store_min();
    DO_TEST_CMPXCHG_STRONG();

    // min -> 0
    desired.store_zero();
    DO_TEST_CMPXCHG_STRONG();
}

}  // namespace


#define SKIP_NULL_OP_FP_CMPXCHG_STRONG(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).xchg_ops.fp_cmpxchg_strong, "cmpxchg_strong")


/// @brief Check that the non-atomic logic of implicit cmpxchg_strong works correctly.
TEST_P(BtLogicImplicit, fp_cmpxchg_strong)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_CMPXCHG_STRONG(p.id, m_ops);

    // wrap operation
    const auto fp_cmpxchg_strong = [&](void *object, void *expected, const void *desired) -> int {
        return m_ops.xchg_ops.fp_cmpxchg_strong(object, expected, desired);
    };

    // test
    test_cmpxchg_strong(p.width, m_align.recommended, fp_cmpxchg_strong);
}


/// @brief Check that the non-atomic logic of explicit cmpxchg_strong works correctly.
TEST_P(BtLogicExplicit, fp_cmpxchg_strong)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_CMPXCHG_STRONG(p.id, m_ops);

    // wrap operation
    const auto fp_cmpxchg_strong = [&](void *object, void *expected, const void *desired) -> int {
        auto fail = PATOMIC_CMPXCHG_FAIL_ORDER(p.order);
        return m_ops.xchg_ops.fp_cmpxchg_strong(object, expected, desired, p.order, fail);
    };

    // test
    test_cmpxchg_strong(p.width, m_align.recommended, fp_cmpxchg_strong);
}


/// @brief Check that the non-atomic logic of transaction cmpxchg_strong works correctly.
TEST_P(BtLogicTransaction, fp_cmpxchg_strong)
{
    // check pre-conditions
    ASSERT_EQ(m_ops.xchg_ops.fp_cmpxchg_strong, nullptr);
}
