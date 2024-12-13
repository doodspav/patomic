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
    const std::function<void(
        void *object,
        const void *desired
    )>& fp_store
)
{
    // create integers
    test::generic_integer object { width, align, false };
    test::generic_integer desired { width, align, false };
    test::generic_integer desired_old { width, align, false };

#define DO_TEST()                   \
    desired_old = desired;          \
    fp_store(object, desired);      \
    ASSERT_EQ(object, desired);     \
    ASSERT_EQ(desired, desired_old)

    // 0 -> ~0
    desired.inv();
    DO_TEST();

    // ~0 -> 1
    desired.store_zero();
    desired.inc();
    DO_TEST();

    // 1 -> max
    desired.store_max();
    DO_TEST();

    // max -> min
    desired.store_min();
    DO_TEST();

    // min -> 0
    desired.store_zero();
    DO_TEST();
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
