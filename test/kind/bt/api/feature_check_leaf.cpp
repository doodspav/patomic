#include <patomic/api/feature_check.h>

#include <test/common/make_ops.hpp>
#include <test/common/math.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <bitset>
#include <set>
#include <string>
#include <type_traits>
#include <vector>


#ifdef _MSC_VER
    #define KilledBySignal(_sig) ExitedWithCode(3)
#endif


/// @brief Test fixture.
class BtApiFeatureCheckLeaf : public testing::Test
{};

/// @brief Templated test fixture.
template <class T>
class BtApiFeatureCheckLeafT : public testing::Test
{
public:
    static constexpr test::ops_domain domain = T::value;
    using OpsTypes = test::ops_types<domain>;
};

/// @brief Templated test fixture for death tests.
template <class T>
class BtApiFeatureCheckLeafT_DeathTest : public testing::Test
{
public:
    static constexpr test::ops_domain domain = T::value;
    using OpsTypes = test::ops_types<domain>;
};

using BtApiFeatureCheckLeafT_Types = ::testing::Types<
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
    BtApiFeatureCheckLeafT,
    BtApiFeatureCheckLeafT_Types,
    TTestHelper
);

TYPED_TEST_SUITE(
    BtApiFeatureCheckLeafT_DeathTest,
    BtApiFeatureCheckLeafT_Types,
    TTestHelper
);


/// @brief All "opkind" opkinds have exactly one bit set, except for
///        patomic_opkind_NONE which has zero bits set.
TEST_F(BtApiFeatureCheckLeaf, all_opkind_have_zero_or_one_bits_set)
{
    // test
    for (const patomic_opkind_t opkind : test::make_opkinds_all_solo())
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
TEST_F(BtApiFeatureCheckLeaf, all_opkinds_have_multiple_bits_set)
{
    // test
    for (const int opkinds : test::make_opkinds_all_combined())
    {
        EXPECT_GT(opkinds, 0);
        EXPECT_FALSE(test::is_positive_pow2(opkinds));
    }
}

/// @brief All "opkinds" opkinds have expected combination of bits.
TEST_F(BtApiFeatureCheckLeaf, all_opkinds_have_expected_bits)
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
    const auto actual_vec = test::make_opkinds_all_combined();
    const std::set<int> expected_set {
        expected_vec.begin(), expected_vec.end()
    };
    const std::set<int> actual_set {
        actual_vec.begin(), actual_vec.end()
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
    EXPECT_EQ(expected_vec.size(), actual_vec.size());
}

/// @brief All "opkinds" opkinds consist only of known "opkind" opkind bits.
TEST_F(BtApiFeatureCheckLeaf, all_opkinds_only_contain_known_opkind_bits)
{
    // setup
    for (unsigned int opkinds : test::make_opkinds_all_combined())
    {
        for (const unsigned int opkind : test::make_opkinds_all_solo())
        {
            opkinds &= ~opkind;
        }

        // test
        EXPECT_EQ(0, opkinds);
    }
}

/// @brief Calling check_leaf with invalid opkind bits for any valid opcat does
///        not unset the invalid opkind bits.
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_ignores_invalid_opkind_bits)
{
    // setup
    const auto ops = test::make_ops_all_nonnull<TestFixture::domain>();
    unsigned int invalid_opkind = ~0u;
    for (const unsigned int valid_opkind : test::make_opkinds_all_solo())
    {
        invalid_opkind &= ~valid_opkind;
    }
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = invalid_opkind;

    // test
    for (const patomic_opcat_t opcat : test::make_opcats_all_solo())
    {
        if (opcat == 0)
        {
            continue;
        }

        ASSERT_TRUE(test::is_positive_pow2(static_cast<unsigned int>(opcat)));
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_leaf(ops, opcat, invalid_opkind);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_leaf with a valid opkind with an opcat which does not
///        apply to the domain does not unset any opkind bits.
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_unused_opcat_ignores_all_opkind_bits)
{
    // setup
    const auto ops = test::make_ops_all_nonnull<TestFixture::domain>();
    const auto opcat_vec = test::make_opcats_all_solo();
    const std::set<patomic_opcat_t> opcat_set {
        opcat_vec.begin(), opcat_vec.end()
    };
    unsigned int opcat = 1;
    while (opcat != 0 && opcat_set.count(static_cast<patomic_opcat_t>(opcat)) != 0)
    {
        opcat <<= 1;
    }
    constexpr unsigned int input_opkinds = ~0u;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = input_opkinds;
    
    // test
    if (opcat != 0)
    {
        ASSERT_TRUE(test::is_positive_pow2(opcat));
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_leaf(ops, static_cast<patomic_opcat_t>(opcat), input_opkinds);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_leaf with all LDST function pointers set in
///        patomic_ops*_t unsets exactly the bits in patomic_opkinds_LDST.
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_full_ldst_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::ldst_t ops {};
    ops.fp_load = test::make_ops_all_nonnull<TestFixture::domain>().fp_load;
    ops.fp_store = test::make_ops_all_nonnull<TestFixture::domain>().fp_store;
    constexpr unsigned int input_opkinds = ~0u;
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
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_ldst_bits_match_expected)
{
    // setup
    for (const auto& ldst : test::make_ops_ldst_combinations<TestFixture::domain>())
    {
        constexpr unsigned int input_opkinds = ~0u;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~ldst.opkinds;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_leaf(ldst.ops, patomic_opcat_LDST, input_opkinds);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_leaf with all XCHG function pointers set in
///        patomic_ops*_t unsets exactly the bits in patomic_opkinds_XCHG.
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_full_xchg_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::ldst_t ops {};
    ops.xchg_ops = test::make_ops_all_nonnull<TestFixture::domain>().xchg_ops;
    constexpr unsigned int input_opkinds = ~0u;
    const unsigned int set_opkinds = patomic_opkinds_XCHG;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result =
        TTestHelper::check_leaf(ops, patomic_opcat_XCHG, input_opkinds);
    EXPECT_EQ(expected_result, actual_result);
}

/// @brief Calling check_leaf with all combinations of XCHG function pointers
///        set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_xchg_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::ldst_t ops {};
    for (const auto& xchg : test::make_ops_xchg_combinations<TestFixture::domain>())
    {
        ops.xchg_ops = xchg.ops;
        constexpr unsigned int input_opkinds = ~0u;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~xchg.opkinds;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_leaf(ops, patomic_opcat_XCHG, input_opkinds);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_leaf with all BIT function pointers set in
///        patomic_ops*_t unsets exactly the bits in patomic_opkinds_BIT.
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_full_bitwise_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::ldst_t ops {};
    ops.bitwise_ops = test::make_ops_all_nonnull<TestFixture::domain>().bitwise_ops;
    constexpr unsigned int input_opkinds = ~0u;
    const unsigned int set_opkinds = patomic_opkinds_BIT;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result =
        TTestHelper::check_leaf(ops, patomic_opcat_BIT, input_opkinds);
    EXPECT_EQ(expected_result, actual_result);
}

/// @brief Calling check_leaf with all combinations of BIT function pointers
///        set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_bitwise_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::ldst_t ops {};
    for (const auto& bitwise : test::make_ops_bitwise_combinations<TestFixture::domain>())
    {
        ops.bitwise_ops = bitwise.ops;
        constexpr unsigned int input_opkinds = ~0u;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~bitwise.opkinds;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_leaf(ops, patomic_opcat_BIT, input_opkinds);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_leaf with all BIN(_V/F) function pointers set in
///        patomic_ops*_t unsets exactly the bits in patomic_opkinds_BIN.
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_full_binary_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::ldst_t ops {};
    ops.binary_ops = test::make_ops_all_nonnull<TestFixture::domain>().binary_ops;
    constexpr unsigned int input_opkinds = ~0u;
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

/// @brief Calling check_leaf with all combinations of BIN(_V/F) function
///        pointers set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_binary_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::ldst_t ops {};
    for (const auto& binary : test::make_ops_binary_combinations<TestFixture::domain>())
    {
        ops.binary_ops = binary.ops;
        constexpr unsigned int input_opkinds = ~0u;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result_void = ~binary.opkinds_void;
        const std::bitset<bit_width> expected_result_fetch = ~binary.opkinds_fetch;

        // test
        const std::bitset<bit_width> actual_result_void =
            TTestHelper::check_leaf(ops, patomic_opcat_BIN_V, input_opkinds);
        const std::bitset<bit_width> actual_result_fetch =
            TTestHelper::check_leaf(ops, patomic_opcat_BIN_F, input_opkinds);
        EXPECT_EQ(expected_result_void, actual_result_void);
        EXPECT_EQ(expected_result_fetch, actual_result_fetch);
    }
}

/// @brief Calling check_leaf with all ARI(_V/F) function pointers set in
///        patomic_ops*_t unsets exactly the bits in patomic_opkinds_ARI.
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_full_arithmetic_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::ldst_t ops {};
    ops.arithmetic_ops = test::make_ops_all_nonnull<TestFixture::domain>().arithmetic_ops;
    constexpr unsigned int input_opkinds = ~0u;
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

/// @brief Calling check_leaf with all combinations of ARI(_V/F) function
///        pointers set in patomic_ops*_t unsets the correct bits.
TYPED_TEST(BtApiFeatureCheckLeafT, check_leaf_arithmetic_bits_match_expected)
{
    // setup
    typename TestFixture::OpsTypes::ldst_t ops {};
    for (const auto& arithmetic : test::make_ops_arithmetic_combinations<TestFixture::domain>())
    {
        ops.arithmetic_ops = arithmetic.ops;
        constexpr unsigned int input_opkinds = ~0u;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result_void = ~arithmetic.opkinds_void;
        const std::bitset<bit_width> expected_result_fetch = ~arithmetic.opkinds_fetch;

        // test
        const std::bitset<bit_width> actual_result_void =
            TTestHelper::check_leaf(ops, patomic_opcat_ARI_V, input_opkinds);
        const std::bitset<bit_width> actual_result_fetch =
            TTestHelper::check_leaf(ops, patomic_opcat_ARI_F, input_opkinds);
        EXPECT_EQ(expected_result_void, actual_result_void);
        EXPECT_EQ(expected_result_fetch, actual_result_fetch);
    }
}

/// @brief Calling check_leaf with all TSPEC function pointers set in
///        patomic_ops_transaction_t unsets exactly the bits in
///        patomic_opkinds_TSPEC.
TEST_F(BtApiFeatureCheckLeaf, check_leaf_full_special_bits_match_expected)
{
    // setup
    constexpr auto domain = test::ops_domain::TRANSACTION;
    patomic_ops_transaction_t ops {};
    ops.special_ops = test::make_ops_all_nonnull<domain>().special_ops;
    constexpr unsigned int input_opkinds = ~0u;
    const unsigned int set_opkinds = patomic_opkinds_TSPEC;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result =
        TTestHelper::check_leaf(ops, patomic_opcat_TSPEC, input_opkinds);
    EXPECT_EQ(expected_result, actual_result);
}

/// @brief Calling check_leaf with all combinations of TSPEC function pointers
///        set in patomic_ops_transaction_t unsets the correct bits.
TEST_F(BtApiFeatureCheckLeaf, check_leaf_special_bits_match_expected)
{
    // setup
    patomic_ops_transaction_t ops {};
    for (const auto& special : test::make_ops_special_combinations_transaction())
    {
        ops.special_ops = special.ops;
        constexpr unsigned int input_opkinds = ~0u;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~special.opkinds;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_leaf(ops, patomic_opcat_TSPEC, input_opkinds);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_leaf with all TFLAG function pointers set in
///        patomic_ops_transaction_t unsets exactly the bits in
///        patomic_opkinds_TFLAG.
TEST_F(BtApiFeatureCheckLeaf, check_leaf_full_flag_bits_match_expected)
{
    // setup
    constexpr auto domain = test::ops_domain::TRANSACTION;
    patomic_ops_transaction_t ops {};
    ops.flag_ops = test::make_ops_all_nonnull<domain>().flag_ops;
    constexpr unsigned int input_opkinds = ~0u;
    const unsigned int set_opkinds = patomic_opkinds_TFLAG;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result =
        TTestHelper::check_leaf(ops, patomic_opcat_TFLAG, input_opkinds);
    EXPECT_EQ(expected_result, actual_result);
}

/// @brief Calling check_leaf with all combinations of TFLAG function pointers
///        set in patomic_ops_transaction_t unsets the correct bits.
TEST_F(BtApiFeatureCheckLeaf, check_leaf_flag_bits_match_expected)
{
    // setup
    patomic_ops_transaction_t ops {};
    for (const auto& flag : test::make_ops_flag_combinations_transaction())
    {
        ops.flag_ops = flag.ops;
        constexpr unsigned int input_opkinds = ~0u;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~flag.opkinds;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_leaf(ops, patomic_opcat_TFLAG, input_opkinds);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_leaf with all TRAW function pointers set in
///        patomic_ops_transaction_t unsets exactly the bits in
///        patomic_opkinds_TRAW.
TEST_F(BtApiFeatureCheckLeaf, check_leaf_full_raw_bits_match_expected)
{
    // setup
    constexpr auto domain = test::ops_domain::TRANSACTION;
    patomic_ops_transaction_t ops {};
    ops.raw_ops = test::make_ops_all_nonnull<domain>().raw_ops;
    constexpr unsigned int input_opkinds = ~0u;
    const unsigned int set_opkinds = patomic_opkinds_TRAW;
    constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
    const std::bitset<bit_width> expected_result = ~set_opkinds;

    // test
    const std::bitset<bit_width> actual_result =
        TTestHelper::check_leaf(ops, patomic_opcat_TRAW, input_opkinds);
    EXPECT_EQ(expected_result, actual_result);
}

/// @brief Calling check_leaf with all combinations of TRAW function pointers
///        set in patomic_ops_transaction_t unsets the correct bits.
TEST_F(BtApiFeatureCheckLeaf, check_leaf_raw_bits_match_expected)
{
    // setup
    patomic_ops_transaction_t ops {};
    for (const auto& raw : test::make_ops_raw_combinations_transaction())
    {
        ops.raw_ops = raw.ops;
        constexpr unsigned int input_opkinds = ~0u;
        constexpr auto bit_width = sizeof(unsigned int) * CHAR_BIT;
        const std::bitset<bit_width> expected_result = ~raw.opkinds;

        // test
        const std::bitset<bit_width> actual_result =
            TTestHelper::check_leaf(ops, patomic_opcat_TRAW, input_opkinds);
        EXPECT_EQ(expected_result, actual_result);
    }
}

/// @brief Calling check_leaf with an opcat value which has no bits set is
///        fatally asserted.
TYPED_TEST(BtApiFeatureCheckLeafT_DeathTest, check_leaf_asserts_on_zero_bit_opcat)
{
    // setup
    const auto ops = test::make_ops_all_nonnull<TestFixture::domain>();
    constexpr auto zero_bit_opcat = static_cast<patomic_opcat_t>(0);

    // test
    EXPECT_EQ(0, zero_bit_opcat);
    EXPECT_EXIT(
        TTestHelper::check_leaf(ops, zero_bit_opcat, 0u),
        testing::KilledBySignal(SIGABRT),
        ".*"
    );
}

/// @brief Calling check_leaf with an opcat value which has more than one bit
///        set is fatally asserted.
TYPED_TEST(BtApiFeatureCheckLeafT_DeathTest, check_leaf_asserts_on_multi_bit_opcat)
{
    // setup
    const auto ops = test::make_ops_all_nonnull<TestFixture::domain>();
    constexpr auto multi_valid_bit_opcat = TestFixture::OpsTypes::full_opcat;
    constexpr auto multi_invalid_bit_opcat = ~multi_valid_bit_opcat;
    constexpr auto multi_mixed_bit_opcat =
        multi_valid_bit_opcat | multi_invalid_bit_opcat;
    constexpr unsigned int multi_opcats[] {
        multi_valid_bit_opcat,
        multi_invalid_bit_opcat,
        multi_mixed_bit_opcat
    };

    // test
    for (const unsigned int multi_opcat : multi_opcats)
    {
        EXPECT_NE(0, multi_opcat);
        EXPECT_FALSE(test::is_positive_pow2(multi_opcat));
        EXPECT_EXIT(
            TTestHelper::check_leaf(
                ops, static_cast<patomic_opcat_t>(multi_opcat), 0u),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}
