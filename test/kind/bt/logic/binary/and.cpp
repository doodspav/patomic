#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>
#include <test/common/transaction.hpp>

#include <test/suite/bt_logic.hpp>

#include <cstring>


namespace
{

void
test_fetch_and(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument,
        void *ret
    )>& fp_fetch_and
)
{
    // start with all bits unset
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer argument { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };
    test::generic_integer object_exp { width, align, false };
    test::generic_integer argument_old { width, align, false };

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        for (std::size_t j = 0; j < object.bit_width(); ++j)
        {
            // set up params
            object.inv_at(i);
            object_old = object;
            argument.inv_at(j);
            argument_old = argument;

            // set up expected
            object_exp.store_zero();
            if (i == j)
            {
                object_exp.inv_at(i);
            }

            // test
            fp_fetch_and(object, argument, ret);
            ASSERT_EQ(object, object_exp);
            ASSERT_EQ(argument, argument_old);
            ASSERT_EQ(ret, object_old);

            // reset
            argument.inv_at(j);
            object = argument;
        }
    }

    // start with all bits set
    object.inv();
    argument.inv();

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        for (std::size_t j = 0; j < object.bit_width(); ++j)
        {
            // set up params
            object.inv_at(i);
            object_old = object;
            argument.inv_at(j);
            argument_old = argument;

            // set up expected
            object_exp = object;
            if (i != j)
            {
                object_exp.inv_at(j);
            }

            // test
            fp_fetch_and(object, argument, ret);
            ASSERT_EQ(object, object_exp);
            ASSERT_EQ(argument, argument_old);
            ASSERT_EQ(ret, object_old);

            // reset
            argument.inv_at(j);
            object = argument;
        }
    }
}

void
test_and(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument
    )>& fp_and
)
{
    // wrap as fetch
    auto fetch_and = [&](void *object, const void *argument, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_and(object, argument);
    };

    // defer to fetch variant
    return test_fetch_and(width, align, fetch_and);
}

}  // namespace


#define SKIP_NULL_OP_FP_AND(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).binary_ops.fp_and, "and")

#define SKIP_NULL_OP_FP_FETCH_AND(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).binary_ops.fp_fetch_and, "fetch_and")


/// @brief Check that the non-atomic logic of implicit and works correctly.
TEST_P(BtLogicImplicit, fp_and)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_AND(p.id, m_ops);

    // wrap operation
    const auto fp_and = [&](void *object, const void *argument) -> void {
        return m_ops.binary_ops.fp_and(object, argument);
    };

    // test
    test_and(p.width, m_align.recommended, fp_and);
}

/// @brief Check that the non-atomic logic of implicit fetch_and works correctly.
TEST_P(BtLogicImplicit, fp_fetch_and)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_AND(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_and = [&](void *object, const void *argument, void *ret) -> void {
        return m_ops.binary_ops.fp_fetch_and(object, argument, ret);
    };

    // test
    test_fetch_and(p.width, m_align.recommended, fp_fetch_and);
}


/// @brief Check that the non-atomic logic of explicit and works correctly.
TEST_P(BtLogicExplicit, fp_and)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_AND(p.id, m_ops);

    // wrap operation
    const auto fp_and = [&](void *object, const void *argument) -> void {
        return m_ops.binary_ops.fp_and(object, argument, p.order);
    };

    // test
    test_and(p.width, m_align.recommended, fp_and);
}

/// @brief Check that the non-atomic logic of explicit fetch_and works correctly.
TEST_P(BtLogicExplicit, fp_fetch_and)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_AND(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_and = [&](void *object, const void *argument, void *ret) -> void {
        return m_ops.binary_ops.fp_fetch_and(object, argument, p.order, ret);
    };

    // test
    test_fetch_and(p.width, m_align.recommended, fp_fetch_and);
}


/// @brief Check that the non-atomic logic of transaction and works correctly.
TEST_P(BtLogicTransaction, fp_and)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_AND(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.binary_ops.fp_and);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.binary_ops.fp_and);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config.width = width;

        // wrap operation
        const auto fp_and = [&](void *object, const void *argument) -> void {
            patomic_transaction_result_t result {};
            m_ops.binary_ops.fp_and(object, argument, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, and);
    }
}

/// @brief Check that the non-atomic logic of transaction fetch_and works correctly.
TEST_P(BtLogicTransaction, fp_fetch_and)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_AND(p.id, m_ops);

    // test zero
    ASSERT_TSX_ZERO(m_ops.binary_ops.fp_fetch_and);

    // test flag set
    ASSERT_TSX_FLAG_SET(m_ops.binary_ops.fp_fetch_and);

    // go through all widths
    for (std::size_t width : m_widths)
    {
        // setup
        SCOPED_TRACE(width);
        m_config.width = width;

        // wrap operation
        const auto fp_fetch_and = [&](void *object, const void *argument, void *ret) -> void {
            patomic_transaction_result_t result {};
            m_ops.binary_ops.fp_fetch_and(object, argument, ret, m_config, &result);
            ADD_FAILURE_TSX_SUCCESS(m_config, result);
        };

        // test
        TEST_TSX(m_config, fetch_and);
    }
}
