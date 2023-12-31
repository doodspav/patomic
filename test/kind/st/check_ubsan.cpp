#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>

#include <climits>
#include <limits>


class StUbsanDeathTest : public testing::Test
{};


TEST_F(StUbsanDeathTest, ShiftExponentTooLarge)
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

TEST_F(StUbsanDeathTest, SignedIntegerOverflow)
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

TEST_F(StUbsanDeathTest, FloatCastOverflow)
{
#if PATOMIC_HAS_UBSAN
    EXPECT_FATAL_FAILURE({
        volatile auto _ = std::numeric_limits<double>::max();
        int x;

        x = static_cast<int>(_);  // <-- float cast overflow (intentional)

        _ = x;
    }, "ubsan");
#endif
}
