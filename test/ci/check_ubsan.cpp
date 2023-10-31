#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>

#include <climits>
#include <limits>


class CiUbsanDeathTest : public testing::Test
{};


#if PATOMIC_CI_UBSAN

TEST_F(CiUbsanDeathTest, NotEnabled)
{
    GTEST_SKIP() << "Running in CI but Undefined Behaviour Sanitizer is not enabled";
}

#else

TEST_F(CiUbsanDeathTest, ShiftExponentTooLarge)
{
    EXPECT_FATAL_FAILURE({
        volatile int _ = sizeof(int) * CHAR_BIT;
        int x = 10;
        int e = _;

        _ = x << e;  // <-- shift exponent too large (intentional)
    }, "ubsan");
}

TEST_F(CiUbsanDeathTest, SignedIntegerOverflow)
{
    EXPECT_FATAL_FAILURE({
        volatile int _ = 5;
        int x = _;

        x += INT_MAX;  // <-- signed integer overflow (intentional)

        _ = x;
    }, "ubsan");
}

TEST_F(CiUbsanDeathTest, FloatCastOverflow)
{
    EXPECT_FATAL_FAILURE({
        volatile auto _ = std::numeric_limits<double>::max();
        int x;

        x = static_cast<int>(_);  // <-- float cast overflow (intentional)

        _ = x;
    }, "ubsan");
}

#endif  // PATOMIC_CI_UBSAN
