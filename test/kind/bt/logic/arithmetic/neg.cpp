#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_fetch_neg(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        void *ret
    )>& fp_fetch_neg
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };

#define DO_TEST_FETCH_NEG()        \
    object_old = object;           \
    fp_fetch_neg(object, ret);     \
    ASSERT_EQ(ret, object_old);    \
    object_old.neg();              \
    ASSERT_EQ(object, object_old)

    // -0 -> 0
    DO_TEST_FETCH_NEG();

    // -1 -> max
    object.inc();
    DO_TEST_FETCH_NEG();

    // -max -> 1
    DO_TEST_FETCH_NEG();

    // -2 -> (max - 1)
    object.inc();
    DO_TEST_FETCH_NEG();

    // -(max - 1) -> 2
    DO_TEST_FETCH_NEG();
}

void
test_neg(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object
    )>& fp_neg
)
{
    // wrap as fetch
    auto fetch_neg = [&](void *object, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_neg(object);
    };

    // defer to fetch variant
    return test_fetch_neg(width, align, fetch_neg);
}

}  // namespace


#define SKIP_NULL_OP_FP_NEG(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).arithmetic_ops.fp_neg, "neg")

#define SKIP_NULL_OP_FP_FETCH_NEG(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).arithmetic_ops.fp_fetch_neg, "fetch_neg")


/// @brief Check that the non-atomic logic of implicit neg works correctly.
TEST_P(BtLogicImplicit, fp_neg)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_NEG(p.id, m_ops);

    // wrap operation
    const auto fp_neg = [&](void *object) -> void {
        return m_ops.arithmetic_ops.fp_neg(object);
    };

    // test
    test_neg(p.width, m_align.recommended, fp_neg);
}

/// @brief Check that the non-atomic logic of implicit fetch_neg works correctly.
TEST_P(BtLogicImplicit, fp_fetch_neg)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_NEG(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_neg = [&](void *object, void *ret) -> void {
        return m_ops.arithmetic_ops.fp_fetch_neg(object, ret);
    };

    // test
    test_fetch_neg(p.width, m_align.recommended, fp_fetch_neg);
}


/// @brief Check that the non-atomic logic of explicit neg works correctly.
TEST_P(BtLogicExplicit, fp_neg)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_NEG(p.id, m_ops);

    // wrap operation
    const auto fp_neg = [&](void *object) -> void {
        return m_ops.arithmetic_ops.fp_neg(object, p.order);
    };

    // test
    test_neg(p.width, m_align.recommended, fp_neg);
}

/// @brief Check that the non-atomic logic of explicit fetch_neg works correctly.
TEST_P(BtLogicExplicit, fp_fetch_neg)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_NEG(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_neg = [&](void *object, void *ret) -> void {
        return m_ops.arithmetic_ops.fp_fetch_neg(object, p.order, ret);
    };

    // test
    test_fetch_neg(p.width, m_align.recommended, fp_fetch_neg);
}


/// @brief Check that the non-atomic logic of transaction neg works correctly.
TEST_P(BtLogicTransaction, fp_neg)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_NEG(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.arithmetic_ops.fp_neg, nullptr);

    // go through all widths
    for (std::size_t width : p.widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config.width = width;

        // wrap operation
        const auto fp_neg = [&](void *object) -> void {
            patomic_transaction_result_t result {};
            m_ops.arithmetic_ops.fp_neg(object, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        test_neg(width, 1u, fp_neg);
    }
}

/// @brief Check that the non-atomic logic of transaction fetch_neg works correctly.
TEST_P(BtLogicTransaction, fp_fetch_neg)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_NEG(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.arithmetic_ops.fp_fetch_neg, nullptr, nullptr);

    // go through all widths
    for (std::size_t width : p.widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config.width = width;

        // wrap operation
        const auto fp_fetch_neg = [&](void *object, void *ret) -> void {
            patomic_transaction_result_t result {};
            m_ops.arithmetic_ops.fp_fetch_neg(object, ret, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        test_fetch_neg(width, 1u, fp_fetch_neg);
    }
}
