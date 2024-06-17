#include <patomic/api/options.h>

#include <test/common/math.hpp>

#include <gtest/gtest.h>

#include <vector>


/// @brief Test fixture.
class BtApiOptions : public testing::Test
{
public:
    const std::vector<patomic_option_t> solo_options {
        patomic_option_NONE
    };

    const std::vector<patomic_option_t> combined_options {
    };
};


/// @brief All "option" options have exactly one bit set, except for
///        patomic_option_NONE which has zero bits set.
TEST_F(BtApiOptions, all_option_have_zero_or_one_bits_set)
{
    // test
    for (const patomic_option_t option : solo_options)
    {
        if (option == patomic_option_NONE)
        {
            EXPECT_EQ(0, option);
        }
        else
        {
            EXPECT_TRUE(test::is_positive_pow2(static_cast<int>(option)));
        }
    }
}

/// @brief All "options" options have multiple bits set.
TEST_F(BtApiOptions, all_options_have_multiple_bits_set)
{
    // test
    for (const int options : combined_options)
    {
        EXPECT_GT(options, 0);
        EXPECT_FALSE(test::is_positive_pow2(options));
    }
}

/// @brief All "options" options have expected combinations of bits.
TEST_F(BtApiOptions, all_options_have_expected_bits0)
{
    // test (no combined options yet)
    EXPECT_TRUE(combined_options.empty());
}

/// @brief All "options" options consist only of known "option" option bits.
TEST_F(BtApiOptions, all_options_only_contain_known_option_bits)
{
    // test (no combined options yet)
    EXPECT_TRUE(combined_options.empty());
}
