#include <test/common/generic_int.hpp>
#include <test/common/skip.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_bin_fetch_or(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument,
        void *ret
    )>& fp_fetch_or
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
            object_exp = object;
            if (i != j)
            {
                object_exp.inv_at(j);
            }

            // test
            fp_fetch_or(object, argument, ret);
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
                object_exp.inv_at(i);
            }

            // test
            fp_fetch_or(object, argument, ret);
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
test_bin_or(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument
    )>& fp_or
)
{
    // wrap as fetch
    auto fetch_or = [&](void *object, const void *argument, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_or(object, argument);
    };

    // defer to fetch variant
    return test_bin_fetch_or(width, align, fetch_or);
}

}  // namespace


#define SKIP_NULL_OP_FP_OR(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).binary_ops.fp_or, "or")

#define SKIP_NULL_OP_FP_FETCH_OR(id, ops) \
    SKIP_NULL_OP_FP(id, (ops).binary_ops.fp_fetch_or, "fetch_or")


/// @brief Check that the non-atomic logic of implicit or works correctly.
TEST_P(BtLogicImplicit, fp_or)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_OR(p.id, m_ops);

    // wrap operation
    const auto fp_or = [&](void *object, const void *argument) -> void {
        return m_ops.binary_ops.fp_or(object, argument);
    };

    // test
    test_bin_or(p.width, m_align.recommended, fp_or);
}

/// @brief Check that the non-atomic logic of implicit fetch_or works correctly.
TEST_P(BtLogicImplicit, fp_fetch_or)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_OR(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_or = [&](void *object, const void *argument, void *ret) -> void {
        return m_ops.binary_ops.fp_fetch_or(object, argument, ret);
    };

    // test
    test_bin_fetch_or(p.width, m_align.recommended, fp_fetch_or);
}


/// @brief Check that the non-atomic logic of explicit or works correctly.
TEST_P(BtLogicExplicit, fp_or)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_OR(p.id, m_ops);

    // wrap operation
    const auto fp_or = [&](void *object, const void *argument) -> void {
        return m_ops.binary_ops.fp_or(object, argument, p.order);
    };

    // test
    test_bin_or(p.width, m_align.recommended, fp_or);
}

/// @brief Check that the non-atomic logic of explicit fetch_or works correctly.
TEST_P(BtLogicExplicit, fp_fetch_or)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_OR(p.id, m_ops);

    // wrap operation
    const auto fp_fetch_or = [&](void *object, const void *argument, void *ret) -> void {
        return m_ops.binary_ops.fp_fetch_or(object, argument, p.order, ret);
    };

    // test
    test_bin_fetch_or(p.width, m_align.recommended, fp_fetch_or);
}


/// @brief Check that the non-atomic logic of transaction or works correctly.
TEST_P(BtLogicTransaction, fp_or)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_OR(p.id, m_ops);

    // go through all widths
    for (std::size_t width : p.widths)
    {
        // setup
        m_config.width = width;

        // wrap operation
        const auto fp_or = [&](void *object, const void *argument) -> void {
            return m_ops.binary_ops.fp_or(object, argument, m_config, nullptr);
        };

        // test
        test_bin_or(width, 1u, fp_or);
    }
}

/// @brief Check that the non-atomic logic of transaction fetch_or works correctly.
TEST_P(BtLogicTransaction, fp_fetch_or)
{
    // check pre-conditions
    const auto& p = GetParam();
    SKIP_NULL_OP_FP_FETCH_OR(p.id, m_ops);

    // go through all widths
    for (std::size_t width : p.widths)
    {
        // setup
        m_config.width = width;

        // wrap operation
        const auto fp_fetch_or = [&](void *object, const void *argument, void *ret) -> void {
            return m_ops.binary_ops.fp_fetch_or(object, argument, ret, m_config, nullptr);
        };

        // test
        test_bin_fetch_or(width, 1u, fp_fetch_or);
    }
}
