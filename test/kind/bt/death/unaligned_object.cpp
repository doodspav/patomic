#include <patomic/patomic.h>

#include <test/common/death.hpp>
#include <test/common/generic_int.hpp>
#include <test/common/params.hpp>


#include <gtest/gtest.h>


/// @brief Test fixture.
class BtDeathUnalignedObject : public testing::Test
{};


/// @brief Check that calling an implicit atomic operation with an unaligned
///        atomic object is asserted.
TEST_F(BtDeathUnalignedObject, implicit)
{
    // go through all params
    const auto params = test::ParamsImplicit::combinations();
    for (const auto& param : params)
    {
        // add trace
        SCOPED_TRACE(param.as_test_suffix());
        
        // get operations
        const auto pao = patomic_create(
            param.width,
            param.order,
            param.options,
            patomic_kinds_ALL,
            param.id
        );
        const auto& ops = pao.ops;
        
        // skip if alignment is 1 (i.e. everything is aligned)
        if (pao.align.recommended == 1)
        {
            continue;
        }
        
        // create misaligned "atomic object" pointer
        test::generic_integer gi { param.width + 1, pao.align.recommended, false };
        void *p = std::next(gi.data());
        
        // attempt to call operations if non-null with misaligned object

        // load/store
        ASSERT_DEATH_IF_NON_NULL(ops.fp_store, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.fp_load, p, p);

        // xchg
        ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_exchange, p, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_cmpxchg_weak, p, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_cmpxchg_strong, p, p, p);

        // bitwise
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test, p, 0);
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test_compl, p, 0);
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test_set, p, 0);
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test_reset, p, 0);

        // binary
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_or, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_xor, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_and, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_not, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_or, p, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_xor, p, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_and, p, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_not, p, p);

        // arithmetic
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_add, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_sub, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_inc, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_dec, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_neg, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_add, p, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_sub, p, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_inc, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_dec, p, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_neg, p, p);
    }
}


/// @brief Check that calling an explicit atomic operation with an unaligned
///        atomic object is asserted.
TEST_F(BtDeathUnalignedObject, explicit)
{
    // go through all params
    const auto params = test::ParamsImplicit::combinations();
    for (const auto& param : params)
    {
        // add trace
        SCOPED_TRACE(param.as_test_suffix());

        // get operations
        const auto pao = patomic_create_explicit(
            param.width,
            param.options,
            patomic_kinds_ALL,
            param.id
        );
        const auto& ops = pao.ops;

        // skip if alignment is 1 (i.e. everything is aligned)
        if (pao.align.recommended == 1)
        {
            continue;
        }

        // create misaligned "atomic object" pointer
        test::generic_integer gi { param.width + 1, pao.align.recommended, false };
        void *p = std::next(gi.data());

        // attempt to call operations if non-null with misaligned object

        // load/store
        if (patomic_is_valid_load_order(param.order))
        {
            ASSERT_DEATH_IF_NON_NULL(ops.fp_store, p, p, param.order);
        }
        if (patomic_is_valid_store_order(param.order))
        {
            ASSERT_DEATH_IF_NON_NULL(ops.fp_load, p, param.order, p);
        }

        // xchg
        ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_exchange, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_cmpxchg_weak, p, p, p, param.order, patomic_RELAXED);
        ASSERT_DEATH_IF_NON_NULL(ops.xchg_ops.fp_cmpxchg_strong, p, p, p, param.order, patomic_RELAXED);

        // bitwise
        if (patomic_is_valid_load_order(param.order))
        {
            ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test, p, 0, param.order);
        }
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test_compl, p, 0, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test_set, p, 0, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.bitwise_ops.fp_test_reset, p, 0, param.order);

        // binary
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_or, p, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_xor, p, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_and, p, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_not, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_or, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_xor, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_and, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.binary_ops.fp_fetch_not, p, param.order, p);

        // arithmetic
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_add, p, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_sub, p, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_inc, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_dec, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_neg, p, param.order);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_add, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_sub, p, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_inc, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_dec, p, param.order, p);
        ASSERT_DEATH_IF_NON_NULL(ops.arithmetic_ops.fp_fetch_neg, p, param.order, p);
    }
}
