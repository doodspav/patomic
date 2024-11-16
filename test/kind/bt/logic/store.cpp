#include <patomic/patomic.h>

#include <test/common/generic_int.hpp>
#include <test/common/params.hpp>

#include <gtest/gtest.h>


/// @brief Test fixture for implicit and explicit operations.
class BtLogicStoreImpExp : public testing::TestWithParam<test::ParamsImpExp>
{};

/// @brief Test fixture for transaction operations.
class BtLogicStoreTsx : public testing::TestWithParam<test::ParamsTsx>
{};


/// @brief Check that store's non-atomic logic works correctly.
TEST_P(BtLogicStoreImpExp, store)
{
    // patomic
    const test::ParamsImpExp& p = GetParam();
    const patomic_t pat = patomic_create(
        p.width, p.order, p.options, patomic_kinds_ALL, p.id
    );

    // check store operation is supported
    if (pat.ops.fp_store == nullptr)
    {
        GTEST_SKIP() << "Skipping; fp_store is nullptr";
    }
    const auto fp_store = pat.ops.fp_store;

    // create integers
    test::generic_integer object { p.width, pat.align.recommended, false };
    test::generic_integer desired { p.width, pat.align.recommended, false };

    // 0 -> ~0
    desired.inv();
    fp_store(object.data(), desired.data());
    EXPECT_EQ(object, desired);

    // ~0 -> 1
    desired.store_zero();
    desired.inc();
    fp_store(object, desired);
    EXPECT_EQ(object, desired);

    // 1 -> max
    desired.store_max();
    fp_store(object, desired);
    EXPECT_EQ(object, desired);

    // max -> min
    desired.store_min();
    fp_store(object, desired);
    EXPECT_EQ(object, desired);

    // min -> 0
    desired.store_zero();
    fp_store(object, desired);
    EXPECT_EQ(object, desired);
}


INSTANTIATE_TEST_SUITE_P(
    ,  // no name necessary
    BtLogicStoreImpExp,
    testing::ValuesIn(test::ParamsImpExp::combinations()),
    [](const testing::TestParamInfo<test::ParamsImpExp>& info) -> std::string {
        return info.param.as_test_suffix();
    }
);
