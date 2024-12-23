#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_cmpxchg_weak(
    std::size_t width,
    std::size_t align,
    const std::function<int(
        void *object,
        void *expected,
        const void *desired
    )>& fp_cmpxchg_weak
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

#define DO_TEST_CMPXCHG_WEAK()                                        \
    expected = object;                                                \
    expected.inv();                                                   \
    object_old = object;                                              \
    desired_old = desired;                                            \
    ok = fp_cmpxchg_weak(object, expected, desired);                  \
    ASSERT_FALSE(ok);                                                 \
    ASSERT_EQ(object, object_old);                                    \
    ASSERT_EQ(expected, object);                                      \
    ASSERT_EQ(desired, desired_old);                                  \
    for (int i = 0; !ok && i < 10'000; ++i)                           \
    {                                                                 \
        object_old = object;                                          \
        expected_old = expected;                                      \
        ok = fp_cmpxchg_weak(object, expected, desired);              \
        if (ok)                                                       \
        {                                                             \
            ASSERT_EQ(object, desired);                               \
            ASSERT_EQ(expected, expected_old);                        \
        }                                                             \
        else                                                          \
        {                                                             \
            ASSERT_EQ(object, object_old);                            \
            ASSERT_EQ(expected, object);                              \
        }                                                             \
        ASSERT_EQ(desired, desired_old);                              \
    }                                                                 \
    if (!ok)                                                          \
    {                                                                 \
        GTEST_FAIL()                                                  \
            << "Exceeded maximum cmpxchg_weak attempts, with values:" \
            << "\n\twidth         " << width                          \
            << "\n\talign         " << align                          \
            << "\n\tobject:       " << object                         \
            << "\n\texpected:     " << expected                       \
            << "\n\tdesired:      " << desired                        \
            << "\n\tobject_old:   " << object_old                     \
            << "\n\texpected_old: " << expected_old                   \
            << "\n\tdesired_old:  " << desired_old;                   \
    }                                                                 \
    REQUIRE_SEMICOLON()

    // 0 -> ~0
    desired.inv();
    DO_TEST_CMPXCHG_WEAK();

    // ~0 -> 1
    desired.store_zero();
    desired.inc();
    DO_TEST_CMPXCHG_WEAK();

    // 1 -> max
    desired.store_max();
    DO_TEST_CMPXCHG_WEAK();

    // max -> min
    desired.store_min();
    DO_TEST_CMPXCHG_WEAK();

    // min -> 0
    desired.store_zero();
    DO_TEST_CMPXCHG_WEAK();
}

}  // namespace


#define SKIP_NULL_OP_FP_CMPXCHG_WEAK(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).xchg_ops.fp_cmpxchg_weak, "cmpxchg_weak")


/// @brief Check that the non-atomic logic of implicit cmpxchg_weak works correctly.
TEST_P(BtLogicImplicit, fp_cmpxchg_weak)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_CMPXCHG_WEAK(p.id, m_ops);

    // wrap operation
    const auto fp_cmpxchg_weak = [&](void *object, void *expected, const void *desired) -> int {
        return m_ops.xchg_ops.fp_cmpxchg_weak(object, expected, desired);
    };

    // test
    test_cmpxchg_weak(p.width, m_align.recommended, fp_cmpxchg_weak);
}


/// @brief Check that the non-atomic logic of explicit cmpxchg_weak works correctly.
TEST_P(BtLogicExplicit, fp_cmpxchg_weak)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_CMPXCHG_WEAK(p.id, m_ops);

    // wrap operation
    const auto fp_cmpxchg_weak = [&](void *object, void *expected, const void *desired) -> int {
        auto fail = PATOMIC_CMPXCHG_FAIL_ORDER(p.order);
        return m_ops.xchg_ops.fp_cmpxchg_weak(object, expected, desired, p.order, fail);
    };

    // test
    test_cmpxchg_weak(p.width, m_align.recommended, fp_cmpxchg_weak);
}


/// @brief Check that the non-atomic logic of transaction cmpxchg_weak works correctly.
TEST_P(BtLogicTransaction, fp_cmpxchg_weak)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_CMPXCHG_WEAK(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO_WFB(m_ops.xchg_ops.fp_cmpxchg_weak);

    // test flag set
    ASSERT_TSX_FLAG_SET_WFB(m_ops.xchg_ops.fp_cmpxchg_weak);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config_wfb.width = width;

        // wrap operation
        const auto fp_cmpxchg_weak = [&](void *object, void *expected, const void *desired) -> int {
            patomic_transaction_result_wfb_t result {};
            int ok = m_ops.xchg_ops.fp_cmpxchg_weak(object, expected, desired, m_config_wfb, &result);
            ADD_FAILURE_TSX_SUCCESS_WFB(m_config_wfb, result);
            return ok;
        };

        // test
        TEST_TSX_WFB(m_config_wfb, cmpxchg_weak);
    }
}
