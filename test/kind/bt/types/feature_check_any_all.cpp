#include <patomic/types/feature_check.h>

#include <test/common/math.hpp>
#include <test/common/make_ops.hpp>

#include <gtest/gtest.h>

#include <bitset>
#include <vector>


/// @brief Test fixture.
class BtTypesFeatureCheckAnyAll : public testing::Test
{
public:
    const std::vector<patomic_opcat_t> solo_opcats {
        patomic_opcat_NONE,
        patomic_opcat_LDST,
        patomic_opcat_XCHG,
        patomic_opcat_BIT,
        patomic_opcat_BIN_V,
        patomic_opcat_BIN_F,
        patomic_opcat_ARI_V,
        patomic_opcat_ARI_F,
        patomic_opcat_TSPEC,
        patomic_opcat_TFLAG,
        patomic_opcat_TRAW
    };

    const std::vector<patomic_opcat_t> combined_opcats {
        patomic_opcats_BIN,
        patomic_opcats_ARI,
        patomic_opcats_IMPLICIT,
        patomic_opcats_EXPLICIT,
        patomic_opcats_TRANSACTION
    };
};

/// @brief Test fixture for tests which might take a long time to execute.
class BtTypesFeatureCheckAnyAll_SlowTest : public testing::Test
{};


/// @brief All "opcat" opcats have exactly zero or one bits set.
TEST_F(BtTypesFeatureCheckAnyAll, all_opcat_have_zero_or_one_bits_set)
{
    // test
    for (const patomic_opcat_t opcat : solo_opcats)
    {
        if (opcat == patomic_opcat_NONE)
        {
            EXPECT_EQ(0, opcat);
        }
        else
        {
            EXPECT_TRUE(test::is_positive_pow2(static_cast<int>(opcat)));
        }
    }
}

/// @brief All "opcat" values are unique.
TEST_F(BtTypesFeatureCheckAnyAll, all_opcat_are_unique)
{
    // setup
    const std::set<patomic_opcat_t> solo_opcats_set {
        solo_opcats.begin(), solo_opcats.end()
    };

    // test
    EXPECT_EQ(solo_opcats.size(), solo_opcats_set.size());
}

/// @brief All "opcats" opcats have multiple bits set.
TEST_F(BtTypesFeatureCheckAnyAll, all_opcats_have_multiple_bits_set)
{
    // test
    for (const int opcats : combined_opcats)
    {
        EXPECT_GT(opcats, 0);
        EXPECT_FALSE(test::is_positive_pow2(opcats));
    }
}

/// @brief All "opcats" opcats have expected combination of bits.
TEST_F(BtTypesFeatureCheckAnyAll, all_opcats_have_expected_bits)
{
    // setup
    // values
    constexpr auto expected_bin = patomic_opcat_BIN_V | patomic_opcat_BIN_F;
    constexpr auto expected_ari = patomic_opcat_ARI_V | patomic_opcat_ARI_F;
    constexpr auto expected_implicit =
        patomic_opcat_LDST |
        patomic_opcat_XCHG |
        patomic_opcat_BIT |
        expected_bin      |
        expected_ari;
    constexpr auto expected_explicit = expected_implicit;
    constexpr auto expected_transaction =
        expected_implicit   |
        patomic_opcat_TSPEC |
        patomic_opcat_TFLAG |
        patomic_opcat_TRAW;
    // sets of values
    const std::set<int> expected_set = {
        expected_bin,
        expected_ari,
        expected_implicit,
        expected_explicit,
        expected_transaction
    };
    const std::set<int> actual_set = {
        combined_opcats.begin(), combined_opcats.end()
    };

    // test
    // checks that all values are expected
    EXPECT_EQ(expected_set, actual_set);
    // checks that each value is assigned the correct variable
    EXPECT_EQ(patomic_opcats_BIN, expected_bin);
    EXPECT_EQ(patomic_opcats_ARI, expected_ari);
    EXPECT_EQ(patomic_opcats_IMPLICIT, expected_implicit);
    EXPECT_EQ(patomic_opcats_EXPLICIT, expected_explicit);
    EXPECT_EQ(patomic_opcats_TRANSACTION, expected_transaction);
    // can't check set sizes in case two opcats have the same value
    EXPECT_EQ(5, combined_opcats.size());
}

/// @brief Each "opcats" opcats consist only of known "opcat" opcat bits.
TEST_F(BtTypesFeatureCheckAnyAll, all_opcats_only_contain_known_opcat_bits)
{
    // setup
    for (unsigned int opcats : combined_opcats)
    {
        for (unsigned int opcat : solo_opcats)
        {
            opcats &= ~opcat;
        }

        // test
        EXPECT_EQ(0, opcats);
    }
}

/// @brief The values of patomic_opcats_IMPLICIT and patomic_opcats_EXPLICIT
///        are the same.
TEST_F(BtTypesFeatureCheckAnyAll, opcats_implicit_eq_explicit)
{
    // test
    EXPECT_EQ(patomic_opcats_IMPLICIT, patomic_opcats_EXPLICIT);
}

/// @brief The bits set in patomic_opcats_IMPLICIT and patomic_opcats_EXPLICIT
///        are all set in patomic_opcats_TRANSACTION.
TEST_F(BtTypesFeatureCheckAnyAll, opcats_implicit_explicit_subset_of_transaction)
{
    // setup
    constexpr auto masked_implicit =
        patomic_opcats_IMPLICIT & patomic_opcats_TRANSACTION;
    constexpr auto masked_explicit =
        patomic_opcats_EXPLICIT & patomic_opcats_TRANSACTION;

    // test
    EXPECT_EQ(masked_implicit, patomic_opcats_IMPLICIT);
    EXPECT_EQ(masked_explicit, patomic_opcats_EXPLICIT);
}

/// @brief Invalid opcat bits remain set in the return value of any/all feature
///        check.
TEST_F(BtTypesFeatureCheckAnyAll, check_invalid_opcats_unmodified)
{}





/// @brief Calling check_any with all combinations of function pointers set in
///        patomic_ops_t unsets the correct bits.
TEST_F(BtTypesFeatureCheckAnyAll_SlowTest, check_any_implicit_all_combinations_unset_correct_bits)
{
    // setup
    // create all combinations
    for (auto& ldst : test::make_ops_ldst_combinations_implicit())
    {
        auto& ops = ldst.ops;
        const auto ldst_any_bit = ldst.any ? patomic_opcat_LDST : 0;

    for (const auto& xchg : test::make_ops_xchg_combinations_implicit())
    {
        ops.xchg_ops = xchg.ops;
        const auto xchg_any_bit = xchg.any ? patomic_opcat_XCHG : 0;

    for (const auto& bit : test::make_ops_bitwise_combinations_implicit())
    {
        ops.bitwise_ops = bit.ops;
        const auto bit_any_bit = bit.any ? patomic_opcat_BIT : 0;

    for (const auto& bin : test::make_ops_binary_combinations_implicit())
    {
        ops.binary_ops = bin.ops;
        const auto bin_v_any_bit = bin.any_void ? patomic_opcat_BIN_V : 0;
        const auto bin_f_any_bit = bin.any_fetch ? patomic_opcat_BIN_F : 0;

    for (const auto& ari : test::make_ops_arithmetic_combinations_implicit())
    {
        ops.arithmetic_ops = ari.ops;
        const auto ari_v_any_bit = ari.any_void ? patomic_opcat_ARI_V : 0;
        const auto ari_f_any_bit = ari.any_fetch ? patomic_opcat_ARI_F : 0;

        // combine bits
        constexpr unsigned int input_opcats = ~0u;
        const unsigned int set_opcats =
            ldst_any_bit  |
            xchg_any_bit  |
            bit_any_bit   |
            bin_v_any_bit |
            bin_f_any_bit |
            ari_v_any_bit |
            ari_f_any_bit;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            patomic_feature_check_any(&ops, input_opcats);
        // assert, because if we expect and there's an error, there'll be too much output to parse
        // bitset makes error easier to understand
        ASSERT_EQ(expected_result, actual_result);

    }}}}}
}


/// @brief Calling check_all with all combinations of function pointers set in
///        patomic_ops_t unsets the correct bits.
TEST_F(BtTypesFeatureCheckAnyAll_SlowTest, check_all_implicit_all_combinations_unset_correct_bits)
{
    // setup
    // create all combinations
    for (auto& ldst : test::make_ops_ldst_combinations_implicit())
    {
        auto& ops = ldst.ops;
        const auto ldst_all_bit = ldst.all ? patomic_opcat_LDST : 0;

    for (const auto& xchg : test::make_ops_xchg_combinations_implicit())
    {
        ops.xchg_ops = xchg.ops;
        const auto xchg_all_bit = xchg.all ? patomic_opcat_XCHG : 0;

    for (const auto& bit : test::make_ops_bitwise_combinations_implicit())
    {
        ops.bitwise_ops = bit.ops;
        const auto bit_all_bit = bit.all ? patomic_opcat_BIT : 0;

    for (const auto& bin : test::make_ops_binary_combinations_implicit())
    {
        ops.binary_ops = bin.ops;
        const auto bin_v_all_bit = bin.all_void ? patomic_opcat_BIN_V : 0;
        const auto bin_f_all_bit = bin.all_fetch ? patomic_opcat_BIN_F : 0;

    for (const auto& ari : test::make_ops_arithmetic_combinations_implicit())
    {
        ops.arithmetic_ops = ari.ops;
        const auto ari_v_all_bit = ari.all_void ? patomic_opcat_ARI_V : 0;
        const auto ari_f_all_bit = ari.all_fetch ? patomic_opcat_ARI_F : 0;

        // combine bits
        constexpr unsigned int input_opcats = ~0u;
        const unsigned int set_opcats =
            ldst_all_bit  |
            xchg_all_bit  |
            bit_all_bit   |
            bin_v_all_bit |
            bin_f_all_bit |
            ari_v_all_bit |
            ari_f_all_bit;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            patomic_feature_check_all(&ops, input_opcats);
        // assert, because if we expect and there's an error, there'll be too much output to parse
        // bitset makes error easier to understand
        ASSERT_EQ(expected_result, actual_result);

    }}}}}
}


/// @brief Calling check_any with all combinations of function pointers set in
///        patomic_ops_explicit_t unsets the correct bits.
TEST_F(BtTypesFeatureCheckAnyAll_SlowTest, check_any_explicit_all_combinations_unset_correct_bits)
{
    // setup
    // create all combinations
    for (auto& ldst : test::make_ops_ldst_combinations_explicit())
    {
        auto& ops = ldst.ops;
        const auto ldst_any_bit = ldst.any ? patomic_opcat_LDST : 0;

    for (const auto& xchg : test::make_ops_xchg_combinations_explicit())
    {
        ops.xchg_ops = xchg.ops;
        const auto xchg_any_bit = xchg.any ? patomic_opcat_XCHG : 0;

    for (const auto& bit : test::make_ops_bitwise_combinations_explicit())
    {
        ops.bitwise_ops = bit.ops;
        const auto bit_any_bit = bit.any ? patomic_opcat_BIT : 0;

    for (const auto& bin : test::make_ops_binary_combinations_explicit())
    {
        ops.binary_ops = bin.ops;
        const auto bin_v_any_bit = bin.any_void ? patomic_opcat_BIN_V : 0;
        const auto bin_f_any_bit = bin.any_fetch ? patomic_opcat_BIN_F : 0;

    for (const auto& ari : test::make_ops_arithmetic_combinations_explicit())
    {
        ops.arithmetic_ops = ari.ops;
        const auto ari_v_any_bit = ari.any_void ? patomic_opcat_ARI_V : 0;
        const auto ari_f_any_bit = ari.any_fetch ? patomic_opcat_ARI_F : 0;

        // combine bits
        constexpr unsigned int input_opcats = ~0u;
        const unsigned int set_opcats =
            ldst_any_bit  |
            xchg_any_bit  |
            bit_any_bit   |
            bin_v_any_bit |
            bin_f_any_bit |
            ari_v_any_bit |
            ari_f_any_bit;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            patomic_feature_check_any_explicit(&ops, input_opcats);
        // assert, because if we expect and there's an error, there'll be too much output to parse
        // bitset makes error easier to understand
        ASSERT_EQ(expected_result, actual_result);

    }}}}}
}


/// @brief Calling check_all with all combinations of function pointers set in
///        patomic_ops_explicit_t unsets the correct bits.
TEST_F(BtTypesFeatureCheckAnyAll_SlowTest, check_all_explicit_all_combinations_unset_correct_bits)
{
    // setup
    // create all combinations
    for (auto& ldst : test::make_ops_ldst_combinations_explicit())
    {
        auto& ops = ldst.ops;
        const auto ldst_all_bit = ldst.all ? patomic_opcat_LDST : 0;

    for (const auto& xchg : test::make_ops_xchg_combinations_explicit())
    {
        ops.xchg_ops = xchg.ops;
        const auto xchg_all_bit = xchg.all ? patomic_opcat_XCHG : 0;

    for (const auto& bit : test::make_ops_bitwise_combinations_explicit())
    {
        ops.bitwise_ops = bit.ops;
        const auto bit_all_bit = bit.all ? patomic_opcat_BIT : 0;

    for (const auto& bin : test::make_ops_binary_combinations_explicit())
    {
        ops.binary_ops = bin.ops;
        const auto bin_v_all_bit = bin.all_void ? patomic_opcat_BIN_V : 0;
        const auto bin_f_all_bit = bin.all_fetch ? patomic_opcat_BIN_F : 0;

    for (const auto& ari : test::make_ops_arithmetic_combinations_explicit())
    {
        ops.arithmetic_ops = ari.ops;
        const auto ari_v_all_bit = ari.all_void ? patomic_opcat_ARI_V : 0;
        const auto ari_f_all_bit = ari.all_fetch ? patomic_opcat_ARI_F : 0;

        // combine bits
        constexpr unsigned int input_opcats = ~0u;
        const unsigned int set_opcats =
            ldst_all_bit  |
            xchg_all_bit  |
            bit_all_bit   |
            bin_v_all_bit |
            bin_f_all_bit |
            ari_v_all_bit |
            ari_f_all_bit;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            patomic_feature_check_all_explicit(&ops, input_opcats);
        // assert, because if we expect and there's an error, there'll be too much output to parse
        // bitset makes error easier to understand
        ASSERT_EQ(expected_result, actual_result);

    }}}}}
}




/*
/// @brief All bits unset for patomic_ops*_t which only supports LDST ops
///        exactly matches all bits set in patomic_opcat_LDST.
TEST_F(BtTypesFeatureCheckAnyAll, check_ldst_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports XCHG ops
///        exactly matches all bits set in patomic_opcat_XCHG.
TEST_F(BtTypesFeatureCheckAnyAll, check_xchg_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports BIT ops
///        exactly matches all bits set in patomic_opcat_BIT.
TEST_F(BtTypesFeatureCheckAnyAll, check_bit_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports BIN_V ops
///        exactly matches all bits set in patomic_opcat_BIN_V.
TEST_F(BtTypesFeatureCheckAnyAll, check_bin_v_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports BIN_F ops
///        exactly matches all bits set in patomic_opcat_BIN_F.
TEST_F(BtTypesFeatureCheckAnyAll, check_bin_f_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports ARI_V ops
///        exactly matches all bits set in patomic_opcat_ARI_V.
TEST_F(BtTypesFeatureCheckAnyAll, check_ari_v_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports ARI_F ops
///        exactly matches all bits set in patomic_opcat_ARI_F.
TEST_F(BtTypesFeatureCheckAnyAll, check_ari_f_bits_expected)
{}

/// @brief All bits unset for patomic_ops_transaction_t which only supports
///        TSPEC ops exactly matches all bits set in patomic_opcat_TSPEC.
TEST_F(BtTypesFeatureCheckAnyAll, check_tspec_bits_expected)
{}

/// @brief All bits unset for patomic_ops_transaction_t which only supports
///        TFLAG ops exactly matches all bits set in patomic_opcat_TFLAG.
TEST_F(BtTypesFeatureCheckAnyAll, check_tflag_bits_expected)
{}

/// @brief All bits unset for patomic_ops_transaction_t which only supports
///        TRAW ops exactly matches all bits set in patomic_opcat_TRAW.
TEST_F(BtTypesFeatureCheckAnyAll, check_traw_bits_expected)
{}*/


// TODO: how to test multiple ???