#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_exchange(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *desired,
        void *ret
    )>& fp_exchange
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer desired { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };
    test::generic_integer desired_old { width, align, false };

#define DO_TEST_EXCHANGE()             \
    ASSERT_NE(object, ret);            \
    object_old = object;               \
    desired_old = desired;             \
    fp_exchange(object, desired, ret); \
    ASSERT_EQ(object, desired);        \
    ASSERT_EQ(desired, desired_old);   \
    ASSERT_EQ(ret, object_old)

    // set ret to non-zero
    ret.inc();

    // 0 -> ~0
    desired.inv();
    DO_TEST_EXCHANGE();

    // ~0 -> 1
    desired.store_zero();
    desired.inc();
    DO_TEST_EXCHANGE();

    // 1 -> max
    desired.store_max();
    DO_TEST_EXCHANGE();

    // max -> min
    desired.store_min();
    DO_TEST_EXCHANGE();

    // min -> 0
    desired.store_zero();
    DO_TEST_EXCHANGE();
}

}  // namespace


#define SKIP_NULL_OP_FP_EXCHANGE(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).xchg_ops.fp_exchange, "exchange")


/// @brief Check that the non-atomic logic of implicit exchange works correctly.
TEST_P(BtLogicImplicit, fp_exchange)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_EXCHANGE(p.id, m_ops);

    // wrap operation
    const auto fp_exchange = [&](void *object, const void *desired, void *ret) -> void {
        m_ops.xchg_ops.fp_exchange(object, desired, ret);
    };

    // test
    test_exchange(p.width, m_align.recommended, fp_exchange);
}


/// @brief Check that the non-atomic logic of explicit exchange works correctly.
TEST_P(BtLogicExplicit, fp_exchange)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_EXCHANGE(p.id, m_ops);

    // wrap operation
    const auto fp_exchange = [&](void *object, const void *desired, void *ret) -> void {
        m_ops.xchg_ops.fp_exchange(object, desired, p.order, ret);
    };

    // test
    test_exchange(p.width, m_align.recommended, fp_exchange);
}


/// @brief Check that the non-atomic logic of transaction exchange works correctly.
TEST_P(BtLogicTransaction, fp_exchange)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_EXCHANGE(p.id, m_ops);

    // go through all widths
    for (std::size_t width : p.widths)
    {
        // setup
        m_config.width = width;

        // wrap operation
        const auto fp_exchange = [&](void *object, const void *desired, void *ret) -> void {
            m_ops.xchg_ops.fp_exchange(object, desired, ret, m_config, nullptr);
        };

        // test
        test_exchange(width, 1u, fp_exchange);
    }
}

