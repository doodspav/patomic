#include <gtest/gtest.h>

#include <climits>
#include <limits>


class CiUbsanDeathTest : public testing::Test
{};


#if PATOMIC_CI_UBSAN

TEST_F(CiUbsanDeathTest, IndexOutOfBounds)
{
    auto fn = []() noexcept -> void {
        int arr[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- index out of bounds (intentional)
    };
    EXPECT_DEATH(fn(),
        ".*index \\d+ out of bounds for type.*"
        ".*UndefinedBehaviorSanitizer: undefined-behavior.*"
    );
}

TEST_F(CiUbsanDeathTest, ShiftExponentTooLarge)
{
    auto fn = []() noexcept -> void {
        volatile int _ = 10;
        int x = _;

        x << sizeof(int) * CHAR_BIT;  // <-- shift exponent too large (intentional)

        _ = x;
    };
    EXPECT_DEATH(fn(),
        ".*shift exponent \\d+ is too large for \\d+-bit type.*"
        ".*UndefinedBehaviorSanitizer: undefined-behavior.*"
    );
}

TEST_F(CiUbsanDeathTest, SignedIntegerOverflow)
{
    auto fn = []() noexcept -> void {
        volatile int _ = 5;
        int x = _;

        x += INT_MAX;  // <-- signed integer overflow (intentional)

        _ = x;
    };
    EXPECT_DEATH(fn(),
        ".*signed integer overflow: .* cannot be represented in type.*"
        ".*UndefinedBehaviorSanitizer: undefined-behavior.*"
    );
}

TEST_F(CiUbsanDeathTest, FloatCastOverflow)
{
    auto fn = []() noexcept -> void {
        volatile auto _ = std::numeric_limits<double>::max();
        int x;

        x = static_cast<int>(_);  // <-- float cast overflow (intentional)

        _ = x;
    };
    EXPECT_DEATH(fn(),
        ".*is outside the range of representable values of type.*"
        ".*UndefinedBehaviorSanitizer: undefined-behavior.*"
    );
}

#endif  // PATOMIC_CI_UBSAN
