#include <patomic/patomic.h>

#include <test/common/death.hpp>
#include <test/common/generic_int.hpp>
#include <test/common/params.hpp>

#include <gtest/gtest.h>


/// @brief Test fixture.
class BtDeathInvalidOffset : public testing::Test
{};


/// @brief Check that using an offset which exceeds the bit width on bitwise
///        operations is asserted.
TEST_F(BtDeathInvalidOffset, exceeds_bit_width)
{
    // go through all params
    const auto params = test::ParamsImplicit::combinations();
    for (const auto& param : params)
    {
        // add trace
        SCOPED_TRACE(param.as_test_suffix());

        // create implicit object
        auto pai = patomic_create(
            param.width,
            param.order,
            param.options,
            patomic_kinds_ALL,
            param.id
        );

        // create explicit object
        auto pae = patomic_create_explicit(
            param.width,
            param.options,
            patomic_kinds_ALL,
            param.id
        );

        // create suitably aligned "atomic objects"
        test::generic_integer pi { param.width, pai.align.recommended, false };
        test::generic_integer pe { param.width, pae.align.recommended, false };

        // attempt to call operations if non-null with offset that exceeds the bit width
        const auto offset = static_cast<int>(param.width * CHAR_BIT);

        // implicit
        ASSERT_DEATH_IF_NON_NULL(pai.ops.bitwise_ops.fp_test, pi, offset);
        ASSERT_DEATH_IF_NON_NULL(pai.ops.bitwise_ops.fp_test_compl, pi, offset);
        ASSERT_DEATH_IF_NON_NULL(pai.ops.bitwise_ops.fp_test_set, pi, offset);
        ASSERT_DEATH_IF_NON_NULL(pai.ops.bitwise_ops.fp_test_reset, pi, offset);

        // explicit
        if (patomic_is_valid_load_order(param.order))
        {
            ASSERT_DEATH_IF_NON_NULL(pae.ops.bitwise_ops.fp_test, pe, offset, param.order);
        }
        ASSERT_DEATH_IF_NON_NULL(pae.ops.bitwise_ops.fp_test_compl, pe, offset, param.order);
        ASSERT_DEATH_IF_NON_NULL(pae.ops.bitwise_ops.fp_test_set, pe, offset, param.order);
        ASSERT_DEATH_IF_NON_NULL(pae.ops.bitwise_ops.fp_test_reset, pe, offset, param.order);
    }
}


/// @brief Check that using a negative offset on bitwise operations is
///        asserted.
TEST_F(BtDeathInvalidOffset, negative)
{
    // go through all params
    const auto params = test::ParamsImplicit::combinations();
    for (const auto& param : params)
    {
        // add trace
        SCOPED_TRACE(param.as_test_suffix());

        // create implicit object
        auto pai = patomic_create(
            param.width,
            param.order,
            param.options,
            patomic_kinds_ALL,
            param.id
        );

        // create explicit object
        auto pae = patomic_create_explicit(
            param.width,
            param.options,
            patomic_kinds_ALL,
            param.id
        );

        // create suitably aligned "atomic objects"
        test::generic_integer pi { param.width, pai.align.recommended, false };
        test::generic_integer pe { param.width, pae.align.recommended, false };

        // attempt to call operations if non-null with a negative offset
        constexpr int offset = -1;

        // implicit
        ASSERT_DEATH_IF_NON_NULL(pai.ops.bitwise_ops.fp_test, pi, offset);
        ASSERT_DEATH_IF_NON_NULL(pai.ops.bitwise_ops.fp_test_compl, pi, offset);
        ASSERT_DEATH_IF_NON_NULL(pai.ops.bitwise_ops.fp_test_set, pi, offset);
        ASSERT_DEATH_IF_NON_NULL(pai.ops.bitwise_ops.fp_test_reset, pi, offset);

        // explicit
        if (patomic_is_valid_load_order(param.order))
        {
            ASSERT_DEATH_IF_NON_NULL(pae.ops.bitwise_ops.fp_test, pe, offset, param.order);
        }
        ASSERT_DEATH_IF_NON_NULL(pae.ops.bitwise_ops.fp_test_compl, pe, offset, param.order);
        ASSERT_DEATH_IF_NON_NULL(pae.ops.bitwise_ops.fp_test_set, pe, offset, param.order);
        ASSERT_DEATH_IF_NON_NULL(pae.ops.bitwise_ops.fp_test_reset, pe, offset, param.order);
    }
}
