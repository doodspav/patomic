#include <patomic/patomic.h>

#include <test/common/generic_int.hpp>
#include <test/common/name.hpp>
#include <test/common/skip.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_store(
    std::size_t width,
    std::size_t align,
    const std::function<void(void *, const void *)>& fp_store
)
{
    // create integers
    // base is here to make sure fp_store doesn't modify desired
    test::generic_integer base { width, align, false };
    test::generic_integer object { width, align, false };
    test::generic_integer desired { width, align, false };

    // 0 -> ~0
    desired.inv();
    base = desired;
    fp_store(object.data(), desired.data());
    EXPECT_EQ(base, desired);
    EXPECT_EQ(object, desired);

    // ~0 -> 1
    desired.store_zero();
    desired.inc();
    base = desired;
    fp_store(object, desired);
    EXPECT_EQ(base, desired);
    EXPECT_EQ(object, desired);

    // 1 -> max
    desired.store_max();
    base = desired;
    fp_store(object, desired);
    EXPECT_EQ(base, desired);
    EXPECT_EQ(object, desired);

    // max -> min
    desired.store_min();
    base = desired;
    fp_store(object, desired);
    EXPECT_EQ(base, desired);
    EXPECT_EQ(object, desired);

    // min -> 0
    desired.store_zero();
    base = desired;
    fp_store(object, desired);
    EXPECT_EQ(base, desired);
    EXPECT_EQ(object, desired);
}

}  // namespace


#define SKIP_NULL_OP_FP_STORE(ops) \
    SKIP_NULL_OP_FP((ops).fp_store, "store")


/// @brief Check that implicit store's non-atomic logic works correctly.
TEST_P(BtLogicImplicit, store)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NON_STORE_ORDER(p.order);
    SKIP_NULL_OP_FP_STORE(m_ops);

    // wrap store operation
    const auto fp_store = [&](void *object, const void *desired) -> void {
        m_ops.fp_store(object, desired);
    };

    // test implicit store
    test_store(p.width, m_align.recommended, fp_store);
}


/// @brief Check that explicit store's non-atomic logic works correctly.
TEST_P(BtLogicExplicit, store)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NON_STORE_ORDER(p.order);
    SKIP_NULL_OP_FP_STORE(m_ops);

    // wrap store operation
    const auto fp_store = [&](void *object, const void *desired) -> void {
        m_ops.fp_store(object, desired, p.order);
    };

    // test explicit store
    test_store(p.width, m_align.recommended, fp_store);
}


/// @brief Check that transaction store's non-atomic logic works correctly.
TEST_P(BtLogicTransaction, store)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_STORE(m_ops);
    // TODO: skip if recommended is 0?

    // go through all widths
    for (std::size_t width : p.widths)
    {
        // wrap store operation
        const auto fp_store = [&](void *object, const void *desired) -> void {
            // TODO: set stuff up properly
            m_ops.fp_store(object, desired, {}, nullptr);
        };

        // test transaction store
        test_store(width, 1u, fp_store);
    }
}
