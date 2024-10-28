#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>

#include <climits>
#include <limits>


/// @brief Test fixture.
class StCheckUbsan : public testing::Test
{};


/// @brief Check that ubsan catches shifting by more than the bit width.
TEST_F(StCheckUbsan, ShiftExponentTooLarge)
{
#if PATOMIC_HAS_UBSAN
    EXPECT_FATAL_FAILURE({
        volatile int _ = sizeof(int) * CHAR_BIT;
        int x = 10;
        int e = _;

        _ = x << e;  // <-- shift exponent too large (intentional)
    }, "ubsan");
#endif
}

/// @brief Check that ubsan catches signed integer overflow.
TEST_F(StCheckUbsan, SignedIntegerOverflow)
{
#if PATOMIC_HAS_UBSAN
    EXPECT_FATAL_FAILURE({
        volatile int _ = 5;
        int x = _;

        x += INT_MAX;  // <-- signed integer overflow (intentional)

        _ = x;
    }, "ubsan");
#endif
}

/// @brief Check that ubsan catches overflow from casting float to integer.
TEST_F(StCheckUbsan, FloatCastOverflow)
{
#if PATOMIC_HAS_UBSAN && defined(__clang__)
    EXPECT_FATAL_FAILURE({
        volatile auto _ = std::numeric_limits<double>::max();
        int x;

        x = static_cast<int>(_);  // <-- float cast overflow (intentional)

        _ = x;
    }, "ubsan");
#endif
}
