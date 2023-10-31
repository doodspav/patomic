#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>

#include <climits>
#include <limits>

extern "C" {

/// @brief Called by asan when an error is detected.
/// @note  Asan may still abort after calling this function.
void
__asan_on_error()
{
    GTEST_FAIL() << "Encountered an address sanitizer (asan) error";
}

/// @brief Called by tsan when an error is detected.
/// @note  Tsan may still abort after calling this function.
void
__tsan_on_report()
{
    GTEST_FAIL() << "Encountered a thread sanitizer (tsan) error";
}

/// @brief Called by ubsan when an error is detected .
/// @note  Ubsan may still abort after calling this function.
void
__ubsan_on_report()
{
    GTEST_FAIL() << "Encountered an undefined behaviour sanitizer (ubsan) error";
}

}  // extern "C"


class CiUbsanDeathTest : public testing::Test
{};


TEST_F(CiUbsanDeathTest, ShiftExponentTooLarge)
{
#if !PATOMIC_CI_UBSAN
    GTEST_SKIP() << "Undefined Behaviour Sanitizer is not enabled";
#else
    EXPECT_FATAL_FAILURE({
        volatile int _ = sizeof(int) * CHAR_BIT;
        int x = 10;
        int e = _;

        _ = x << e;  // <-- shift exponent too large (intentional)
    }, "ubsan");
#endif
}

TEST_F(CiUbsanDeathTest, SignedIntegerOverflow)
{
#if !PATOMIC_CI_UBSAN
    GTEST_SKIP() << "Undefined Behaviour Sanitizer is not enabled";
#else
    EXPECT_FATAL_FAILURE({
        volatile int _ = 5;
        int x = _;

        x += INT_MAX;  // <-- signed integer overflow (intentional)

        _ = x;
    }, "ubsan");
#endif
}

TEST_F(CiUbsanDeathTest, FloatCastOverflow)
{
#if !PATOMIC_CI_UBSAN
    GTEST_SKIP() << "Undefined Behaviour Sanitizer is not enabled";
#else
    EXPECT_FATAL_FAILURE({
        volatile auto _ = std::numeric_limits<double>::max();
        int x;

        x = static_cast<int>(_);  // <-- float cast overflow (intentional)

        _ = x;
    }, "ubsan");
#endif
}
