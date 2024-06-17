#include <patomic/types/feature_check.h>

#include <test/common/make_ops.hpp>
#include <test/common/math.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <bitset>
#include <set>
#include <string>
#include <type_traits>
#include <vector>


/// @brief Test fixture.
class BtTypesFeatureCheckLeaf : public testing::Test
{
public:
    const std::vector<patomic_opkind_t> solo_opkinds {
        patomic_opkind_NONE,
        patomic_opkind_LOAD,
        patomic_opkind_STORE,
        patomic_opkind_EXCHANGE,
        patomic_opkind_CMPXCHG_WEAK,
        patomic_opkind_CMPXCHG_STRONG,
        patomic_opkind_TEST,
        patomic_opkind_TEST_SET,
        patomic_opkind_TEST_RESET,
        patomic_opkind_TEST_COMPL,
        patomic_opkind_CLEAR,
        patomic_opkind_OR,
        patomic_opkind_XOR,
        patomic_opkind_AND,
        patomic_opkind_NOT,
        patomic_opkind_ADD,
        patomic_opkind_SUB,
        patomic_opkind_INC,
        patomic_opkind_DEC,
        patomic_opkind_NEG,
        patomic_opkind_DOUBLE_CMPXCHG,
        patomic_opkind_MULTI_CMPXCHG,
        patomic_opkind_GENERIC,
        patomic_opkind_GENERIC_WFB,
        patomic_opkind_TBEGIN,
        patomic_opkind_TABORT,
        patomic_opkind_TCOMMIT,
        patomic_opkind_TTEST
    };

    const std::vector<patomic_opkind_t> combined_opkinds {
        patomic_opkinds_LDST,
        patomic_opkinds_XCHG,
        patomic_opkinds_BIT,
        patomic_opkinds_BIN,
        patomic_opkinds_ARI,
        patomic_opkinds_TSPEC,
        patomic_opkinds_TFLAG,
        patomic_opkinds_TRAW
    };
};

/// @brief Templated test fixture.
template <class T>
class BtTypesFeatureCheckLeafT : public testing::Test
{
public:
    static constexpr test::ops_domain domain = T::value;
    using OpsTypes = test::ops_types<domain>;
};

using BtTypesFeatureCheckLeafT_Types = ::testing::Types<
    std::integral_constant<test::ops_domain, test::ops_domain::IMPLICIT>,
    std::integral_constant<test::ops_domain, test::ops_domain::EXPLICIT>,
    std::integral_constant<test::ops_domain, test::ops_domain::TRANSACTION>
>;

namespace
{

/// @brief Helper type for templated test fixture.
class TTestHelper
{
public:
    template <class T>
    static std::string
    GetName(int)
    {
        auto name = test::to_string(T::value);
        std::transform(name.begin(), name.end(), name.begin(),
                       [](unsigned char c) noexcept -> char {
           return static_cast<char>(std::tolower(c));
       });
        return name;
    }

    static unsigned int
    check_leaf(const patomic_ops_t& ops, patomic_opcat_t opcat,
               unsigned int opkinds) noexcept
    {
        return patomic_feature_check_leaf(&ops, opcat, opkinds);
    }

    static unsigned int
    check_leaf(const patomic_ops_explicit_t& ops, patomic_opcat_t opcat,
               unsigned int opkinds) noexcept
    {
        return patomic_feature_check_leaf_explicit(&ops, opcat, opkinds);
    }

    static unsigned int
    check_leaf(const patomic_ops_transaction_t& ops, patomic_opcat_t opcat,
               unsigned int opkinds) noexcept
    {
        return patomic_feature_check_leaf_transaction(&ops, opcat, opkinds);
    }
};

}  // namespace

TYPED_TEST_SUITE(
    BtTypesFeatureCheckLeafT,
    BtTypesFeatureCheckLeafT_Types,
    TTestHelper
);

/// @brief Test fixture for death tests.
class BtTypesFeatureCheckLeaf_DeathTest : public testing::Test
{};


/// @brief All "opkind" opkinds have exactly zero or one bits set.
TEST_F(BtTypesFeatureCheckLeaf, all_opkind_have_zero_or_one_bits_set)
{
    // test
    for (const patomic_opkind_t opkind : solo_opkinds)
    {
        if (opkind == patomic_opkind_NONE)
        {
            EXPECT_EQ(0, opkind);
        }
        else
        {
            EXPECT_TRUE(test::is_positive_pow2(static_cast<int>(opkind)));
        }
    }
}

/// @brief All "opkinds" opkinds have multiple bits set.
TEST_F(BtTypesFeatureCheckLeaf, all_opkinds_have_multiple_bits_set)
{
    // test
    for (const int opkinds : combined_opkinds)
    {
        EXPECT_GT(opkinds, 0);
        EXPECT_FALSE(test::is_positive_pow2(opkinds));
    }
}

/// @brief All "opkinds" opkinds have expected combination of bits.
TEST_F(BtTypesFeatureCheckLeaf, all_opkinds_have_expected_bits)
{
    // setup
    // values
    constexpr auto expected_ldst = patomic_opkind_LOAD | patomic_opkind_STORE;
    constexpr auto expected_xchg =
        patomic_opkind_EXCHANGE     |
        patomic_opkind_CMPXCHG_WEAK |
        patomic_opkind_CMPXCHG_STRONG;
    constexpr auto expected_bit =
        patomic_opkind_TEST       |
        patomic_opkind_TEST_COMPL |
        patomic_opkind_TEST_SET   |
        patomic_opkind_TEST_RESET;
    constexpr auto expected_bin =
        patomic_opkind_OR  |
        patomic_opkind_XOR |
        patomic_opkind_AND |
        patomic_opkind_NOT;
    constexpr auto expected_ari =
        patomic_opkind_ADD |
        patomic_opkind_SUB |
        patomic_opkind_INC |
        patomic_opkind_DEC |
        patomic_opkind_NEG;
    constexpr auto expected_tspec =
        patomic_opkind_DOUBLE_CMPXCHG |
        patomic_opkind_MULTI_CMPXCHG  |
        patomic_opkind_GENERIC        |
        patomic_opkind_GENERIC_WFB;
    constexpr auto expected_tflag =
        patomic_opkind_TEST     |
        patomic_opkind_TEST_SET |
        patomic_opkind_CLEAR;
    constexpr auto expected_traw =
        patomic_opkind_TBEGIN  |
        patomic_opkind_TABORT  |
        patomic_opkind_TCOMMIT |
        patomic_opkind_TTEST;
    // sets of values
    const std::vector<int> expected_vec {
        expected_ldst,
        expected_xchg,
        expected_bit,
        expected_bin,
        expected_ari,
        expected_tspec,
        expected_tflag,
        expected_traw
    };
    const std::set<int> expected_set {
        expected_vec.begin(), expected_vec.end()
    };
    const std::set<int> actual_set {
        combined_opkinds.begin(), combined_opkinds.end()
    };

    // test
    // checks that all values are expected
    EXPECT_EQ(expected_set, actual_set);
    // checks that each value is assigned the correct variable
    EXPECT_EQ(patomic_opkinds_LDST, expected_ldst);
    EXPECT_EQ(patomic_opkinds_XCHG, expected_xchg);
    EXPECT_EQ(patomic_opkinds_BIT, expected_bit);
    EXPECT_EQ(patomic_opkinds_BIN, expected_bin);
    EXPECT_EQ(patomic_opkinds_ARI, expected_ari);
    EXPECT_EQ(patomic_opkinds_TSPEC, expected_tspec);
    EXPECT_EQ(patomic_opkinds_TFLAG, expected_tflag);
    EXPECT_EQ(patomic_opkinds_TRAW, expected_traw);
    // can't check set sizes in case two opcats have the same value
    EXPECT_EQ(expected_vec.size(), combined_opkinds.size());
}

/// @brief All "opkinds" opkinds consist only of known "opkind" opkind bits.
TEST_F(BtTypesFeatureCheckLeaf, all_opkinds_only_contain_known_opkind_bits)
{
    // setup
    for (unsigned int opkinds : combined_opkinds)
    {
        for (const unsigned int opkind : solo_opkinds)
        {
            opkinds &= ~opkind;
        }

        // test
        EXPECT_EQ(0, opkinds);
    }
}

/// @brief Calling check_leaf with all LDST function pointers set in
///        patomic_ops*_t unsets exactly the bits in patomic_opkinds_LDST.
TYPED_TEST(BtTypesFeatureCheckLeafT, check_leaf_full_ldst_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::base_t ops {};
    ops.fp_load = test::make_ops_all_nonnull<TestFixture::domain>().fp_load;
    ops.fp_store = test::make_ops_all_nonnull<TestFixture::domain>().fp_store;
    constexpr unsigned int input_opkinds = ~0;
    const unsigned int set_opkinds = patomic_opkinds_LDST;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result =
        TTestHelper::check_leaf(ops, patomic_opcat_LDST, input_opkinds);
    EXPECT_EQ(expected_result, actual_result);
}

/// @brief Calling check_leaf with all combinations of LDST function pointers
///        set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtTypesFeatureCheckLeafT, check_leaf_ldst_bits_match_expected)
{}

/// @brief Calling check_leaf with all XCHG function pointers set in
///        patomic_ops*_t unsets exactly the bits in patomic_opkinds_XCHG.
TYPED_TEST(BtTypesFeatureCheckLeafT, check_leaf_full_xchg_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::base_t ops {};
    ops.xchg_ops = test::make_ops_all_nonnull<TestFixture::domain>().xchg_ops;
    constexpr unsigned int input_opkinds = ~0;
    const unsigned int set_opkinds = patomic_opkinds_XCHG;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result =
        TTestHelper::check_leaf(ops, patomic_opcat_XCHG, input_opkinds);
    EXPECT_EQ(expected_result, actual_result);
}

/// @brief Calling check_leaf with all BIT function pointers set in
///        patomic_ops*_t unsets exactly the bits in patomic_opkinds_BIT.
TYPED_TEST(BtTypesFeatureCheckLeafT, check_leaf_full_bitwise_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::base_t ops {};
    ops.bitwise_ops = test::make_ops_all_nonnull<TestFixture::domain>().bitwise_ops;
    constexpr unsigned int input_opkinds = ~0;
    const unsigned int set_opkinds = patomic_opkinds_BIT;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result =
        TTestHelper::check_leaf(ops, patomic_opcat_BIT, input_opkinds);
    EXPECT_EQ(expected_result, actual_result);
}

/// @brief Calling check_leaf with all BIN(_V/F) function pointers set in
///        patomic_ops*_t unsets exactly the bits in patomic_opkinds_BIN.
TYPED_TEST(BtTypesFeatureCheckLeafT, check_leaf_full_binary_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::base_t ops {};
    ops.binary_ops = test::make_ops_all_nonnull<TestFixture::domain>().binary_ops;
    constexpr unsigned int input_opkinds = ~0;
    const unsigned int set_opkinds = patomic_opkinds_BIN;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result_void =
        TTestHelper::check_leaf(ops, patomic_opcat_BIN_V, input_opkinds);
    const std::bitset<bit_width> actual_result_fetch =
        TTestHelper::check_leaf(ops, patomic_opcat_BIN_F, input_opkinds);
    EXPECT_EQ(expected_result, actual_result_void);
    EXPECT_EQ(expected_result, actual_result_fetch);
}

/// @brief Calling check_leaf with all ARI(_V/F) function pointers set in
///        patomic_ops*_t unsets exactly the bits in patomic_opkinds_ARI.
TYPED_TEST(BtTypesFeatureCheckLeafT, check_leaf_full_arithmetic_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::base_t ops {};
    ops.arithmetic_ops = test::make_ops_all_nonnull<TestFixture::domain>().arithmetic_ops;
    constexpr unsigned int input_opkinds = ~0;
    const unsigned int set_opkinds = patomic_opkinds_ARI;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result_void =
        TTestHelper::check_leaf(ops, patomic_opcat_ARI_V, input_opkinds);
    const std::bitset<bit_width> actual_result_fetch =
        TTestHelper::check_leaf(ops, patomic_opcat_ARI_F, input_opkinds);
    EXPECT_EQ(expected_result, actual_result_void);
    EXPECT_EQ(expected_result, actual_result_fetch);
}

/// @brief Calling check_leaf with all TSPEC function pointers set in
///        patomic_ops_transaction_t unsets exactly the bits in
///        patomic_opkinds_TSPEC.
TEST_F(BtTypesFeatureCheckLeaf, check_leaf_full_special_bits_match_expected)
{
    // setup
    constexpr auto domain = test::ops_domain::TRANSACTION;
    patomic_ops_transaction_t ops {};
    ops.special_ops = test::make_ops_all_nonnull<domain>().special_ops;
    constexpr unsigned int input_opkinds = ~0;
    const unsigned int set_opkinds = patomic_opkinds_TSPEC;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result =
        TTestHelper::check_leaf(ops, patomic_opcat_TSPEC, input_opkinds);
    EXPECT_EQ(expected_result, actual_result);
}

/// @brief Calling check_leaf with all TFLAG function pointers set in
///        patomic_ops_transaction_t unsets exactly the bits in
///        patomic_opkinds_TFLAG.
TEST_F(BtTypesFeatureCheckLeaf, check_leaf_full_flag_bits_match_expected)
{
    // setup
    constexpr auto domain = test::ops_domain::TRANSACTION;
    patomic_ops_transaction_t ops {};
    ops.flag_ops = test::make_ops_all_nonnull<domain>().flag_ops;
    constexpr unsigned int input_opkinds = ~0;
    const unsigned int set_opkinds = patomic_opkinds_TFLAG;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result =
        TTestHelper::check_leaf(ops, patomic_opcat_TFLAG, input_opkinds);
    EXPECT_EQ(expected_result, actual_result);
}

/// @brief Calling check_leaf with all TRAW function pointers set in
///        patomic_ops_transaction_t unsets exactly the bits in
///        patomic_opkinds_TRAW.
TEST_F(BtTypesFeatureCheckLeaf, check_leaf_full_raw_bits_match_expected)
{
    // setup
    constexpr auto domain = test::ops_domain::TRANSACTION;
    patomic_ops_transaction_t ops {};
    ops.raw_ops = test::make_ops_all_nonnull<domain>().raw_ops;
    constexpr unsigned int input_opkinds = ~0;
    const unsigned int set_opkinds = patomic_opkinds_TRAW;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result =
        TTestHelper::check_leaf(ops, patomic_opcat_TRAW, input_opkinds);
    EXPECT_EQ(expected_result, actual_result);
}


// TODO: death