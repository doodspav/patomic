#include <patomic/types/feature_check.h>

#include <test/common/math.hpp>
#include <test/common/make_ops.hpp>

#include <gtest/gtest.h>

#include <bitset>
#include <string>
#include <type_traits>
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

/// @brief Templated test fixture.
template <class T>
class BtTypesFeatureCheckAnyAllT : public testing::Test
{
public:
    static constexpr test::ops_domain domain = T::value;
    using OpsTypes = test::ops_types<domain>;
};

using BtTypesFeatureCheckAnyAllT_Types = ::testing::Types<
    std::integral_constant<test::ops_domain, test::ops_domain::IMPLICIT>,
    std::integral_constant<test::ops_domain, test::ops_domain::EXPLICIT>,
    std::integral_constant<test::ops_domain, test::ops_domain::TRANSACTION>
>;

/// @brief Helper type for templated test fixture.
class TTestHelper
{
public:
    template <class T>
    static std::string
    GetName(int);

    template <>
    std::string
    GetName<std::integral_constant<test::ops_domain, test::ops_domain::IMPLICIT>>(int)
    {
        return "implicit";
    }

    template <>
    std::string
    GetName<std::integral_constant<test::ops_domain, test::ops_domain::EXPLICIT>>(int)
    {
        return "explicit";
    }

    template <>
    std::string
    GetName<std::integral_constant<test::ops_domain, test::ops_domain::TRANSACTION>>(int)
    {
        return "transaction";
    }

    static unsigned int
    check_any(const patomic_ops_t& ops, unsigned int opcats) noexcept
    {
        return patomic_feature_check_any(&ops, opcats);
    }

    static unsigned int
    check_any(const patomic_ops_explicit_t& ops, unsigned int opcats) noexcept
    {
        return patomic_feature_check_any_explicit(&ops, opcats);
    }

    static unsigned int
    check_any(const patomic_ops_transaction_t& ops, unsigned int opcats) noexcept
    {
        return patomic_feature_check_any_transaction(&ops, opcats);
    }

    static unsigned int
    check_all(const patomic_ops_t& ops, unsigned int opcats) noexcept
    {
        return patomic_feature_check_all(&ops, opcats);
    }

    static unsigned int
    check_all(const patomic_ops_explicit_t& ops, unsigned int opcats) noexcept
    {
        return patomic_feature_check_all_explicit(&ops, opcats);
    }

    static unsigned int
    check_all(const patomic_ops_transaction_t& ops, unsigned int opcats) noexcept
    {
        return patomic_feature_check_all_transaction(&ops, opcats);
    }
};

TYPED_TEST_SUITE(
    BtTypesFeatureCheckAnyAllT,
    BtTypesFeatureCheckAnyAllT_Types,
    TTestHelper
);


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

/// @brief Calling check_any with all combinations of LDST function pointers
///        set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtTypesFeatureCheckAnyAllT, check_any_ldst_bits_match_expected)
{
    // setup
    for (const auto& ldst : test::make_ops_ldst_combinations<TestFixture::domain>())
    {
        constexpr unsigned int input_opcats = ~0;
        const unsigned int set_opcats = ldst.any ? patomic_opcat_LDST : 0;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_any(ldst.ops, input_opcats);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_all with all combinations of LDST function pointers
///        set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtTypesFeatureCheckAnyAllT, check_all_ldst_bits_match_expected)
{
    // setup
    for (const auto& ldst : test::make_ops_ldst_combinations<TestFixture::domain>())
    {
        constexpr unsigned int input_opcats = ~0;
        const unsigned int set_opcats = ldst.all ? patomic_opcat_LDST : 0;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_all(ldst.ops, input_opcats);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_any with all combinations of XCHG function pointers
///        set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtTypesFeatureCheckAnyAllT, check_any_xchg_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::base_t ops {};
    for (const auto& xchg : test::make_ops_xchg_combinations<TestFixture::domain>())
    {
        ops.xchg_ops = xchg.ops;
        constexpr unsigned int input_opcats = ~0;
        const unsigned int set_opcats = xchg.any ? patomic_opcat_XCHG : 0;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_any(ops, input_opcats);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_all with all combinations of XCHG function pointers
///        set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtTypesFeatureCheckAnyAllT, check_all_xchg_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::base_t ops {};
    for (const auto& xchg : test::make_ops_xchg_combinations<TestFixture::domain>())
    {
        ops.xchg_ops = xchg.ops;
        constexpr unsigned int input_opcats = ~0;
        const unsigned int set_opcats = xchg.all ? patomic_opcat_XCHG : 0;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_all(ops, input_opcats);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_any with all combinations of BIT function pointers
///        set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtTypesFeatureCheckAnyAllT, check_any_bitwise_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::base_t ops {};
    for (const auto& bitwise : test::make_ops_bitwise_combinations<TestFixture::domain>())
    {
        ops.bitwise_ops = bitwise.ops;
        constexpr unsigned int input_opcats = ~0;
        const unsigned int set_opcats = bitwise.any ? patomic_opcat_BIT : 0;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_any(ops, input_opcats);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_all with all combinations of BIT function pointers
///        set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtTypesFeatureCheckAnyAllT, check_all_bitwise_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::base_t ops {};
    for (const auto& bitwise : test::make_ops_bitwise_combinations<TestFixture::domain>())
    {
        ops.bitwise_ops = bitwise.ops;
        constexpr unsigned int input_opcats = ~0;
        const unsigned int set_opcats = bitwise.all ? patomic_opcat_BIT : 0;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_all(ops, input_opcats);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_any with all combinations of BIN(_V/F) function
///        pointers set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtTypesFeatureCheckAnyAllT, check_any_binary_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::base_t ops {};
    for (const auto& binary : test::make_ops_binary_combinations<TestFixture::domain>())
    {
        ops.binary_ops = binary.ops;
        constexpr unsigned int input_opcats = ~0;
        const unsigned int set_opcats =
            (binary.any_void ? patomic_opcat_BIN_V : 0) |
            (binary.any_fetch ? patomic_opcat_BIN_F : 0);
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_any(ops, input_opcats);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_all with all combinations of BIN(_V/F) function
///        pointers set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtTypesFeatureCheckAnyAllT, check_all_binary_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::base_t ops {};
    for (const auto& binary : test::make_ops_binary_combinations<TestFixture::domain>())
    {
        ops.binary_ops = binary.ops;
        constexpr unsigned int input_opcats = ~0;
        const unsigned int set_opcats =
            (binary.all_void ? patomic_opcat_BIN_V : 0) |
            (binary.all_fetch ? patomic_opcat_BIN_F : 0);
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~set_opcats;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_all(ops, input_opcats);
        EXPECT_EQ(expected_result, actual_result);
    }
}
