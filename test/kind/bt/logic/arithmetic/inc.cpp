#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_fetch_inc(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        void *ret
    )>& fp_fetch_inc
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };

#define DO_TEST_FETCH_INC()        \
    object_old = object;           \
    fp_fetch_inc(object, ret);     \
    ASSERT_EQ(ret, object_old);    \
    object_old.inc();              \
    ASSERT_EQ(object, object_old)

    // ++(max - 1) -> max
    object.dec();
    object.dec();
    DO_TEST_FETCH_INC();

    // ++max -> 0
    DO_TEST_FETCH_INC();

    // ++0 -> 1
    DO_TEST_FETCH_INC();

    // ++1 -> 2
    DO_TEST_FETCH_INC();
}

void
test_inc(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object
    )>& fp_inc
)
{
    // wrap as fetch
    auto fetch_inc = [&](void *object, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_inc(object);
    };

    // defer to fetch variant
    return test_fetch_inc(width, align, fetch_inc);
}

}  // namespace


#define SKIP_NULL_OP_FP_INC(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).arithmetic_ops.fp_inc, "inc")

#define SKIP_NULL_OP_FP_FETCH_INC(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).arithmetic_ops.fp_fetch_inc, "fetch_inc")


/// @brief Check that the non-atomic logic of implicit inc works correctly.
TEST_P(BtLogicImplicit, fp_inc)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_INC(p.id, m_ops);

    // wrap operation
    const auto fp_inc = [&](void *object) -> void {
        return m_ops.arithmetic_ops.fp_inc(object);
    };

    // test
    test_inc(p.width, m_align.recommended, fp_inc);
}

/// @brief Check that the non-atomic logic of implicit fetch_inc works correctly.
TEST_P(BtLogicImplicit, fp_fetch_inc)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_INC(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_inc = [&](void *object, void *ret) -> void {
        return m_ops.arithmetic_ops.fp_fetch_inc(object, ret);
    };

    // test
    test_fetch_inc(p.width, m_align.recommended, fp_fetch_inc);
}


/// @brief Check that the non-atomic logic of explicit inc works correctly.
TEST_P(BtLogicExplicit, fp_inc)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_INC(p.id, m_ops);

    // wrap operation
    const auto fp_inc = [&](void *object) -> void {
        return m_ops.arithmetic_ops.fp_inc(object, p.order);
    };

    // test
    test_inc(p.width, m_align.recommended, fp_inc);
}

/// @brief Check that the non-atomic logic of explicit fetch_inc works correctly.
TEST_P(BtLogicExplicit, fp_fetch_inc)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_INC(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_inc = [&](void *object, void *ret) -> void {
        return m_ops.arithmetic_ops.fp_fetch_inc(object, p.order, ret);
    };

    // test
    test_fetch_inc(p.width, m_align.recommended, fp_fetch_inc);
}


/// @brief Check that the non-atomic logic of transaction inc works correctly.
TEST_P(BtLogicTransaction, fp_inc)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_INC(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.arithmetic_ops.fp_inc, nullptr);

    // go through all widths
    for (std::size_t width : p.widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config.width = width;

        // wrap operation
        const auto fp_inc = [&](void *object) -> void {
            patomic_transaction_result_t result {};
            m_ops.arithmetic_ops.fp_inc(object, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        test_inc(width, 1u, fp_inc);
    }
}

/// @brief Check that the non-atomic logic of transaction fetch_inc works correctly.
TEST_P(BtLogicTransaction, fp_fetch_inc)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_INC(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.arithmetic_ops.fp_fetch_inc, nullptr, nullptr);

    // go through all widths
    for (std::size_t width : p.widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config.width = width;

        // wrap operation
        const auto fp_fetch_inc = [&](void *object, void *ret) -> void {
            patomic_transaction_result_t result {};
            m_ops.arithmetic_ops.fp_fetch_inc(object, ret, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        test_fetch_inc(width, 1u, fp_fetch_inc);
    }
}
