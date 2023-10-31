#include <gtest/gtest.h>

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


class CiAsanDeathTest : public testing::Test
{};


TEST_F(CiAsanDeathTest, UseAfterFree)
{
#if !PATOMIC_CI_ASAN
    GTEST_SKIP() << "Address Sanitizer is not enabled";
#else
    auto fn = []() noexcept -> void {
        int *p = new int(5);
        volatile int val = *p;
        delete p;

        volatile int _ = *p;  // <-- use after free (intentional)
    };
    EXPECT_DEATH(fn(), ".*AddressSanitizer: heap-use-after-free.*");
#endif
}

TEST_F(CiAsanDeathTest, HeapBufferOverflow)
{
#if !PATOMIC_CI_ASAN
    GTEST_SKIP() << "Address Sanitizer is not enabled";
#else
    auto fn = []() noexcept -> void {
        int *arr = new int[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)

        delete[] arr;
    };
    EXPECT_DEATH(fn(), ".*AddressSanitizer: heap-buffer-overflow.*");
#endif
}

TEST_F(CiAsanDeathTest, StackBufferOverflow)
{
#if !PATOMIC_CI_ASAN
    GTEST_SKIP() << "Address Sanitizer is not enabled";
#else
    auto fn = []() noexcept -> void {
        int arr[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)
    };
    EXPECT_DEATH(fn(), ".*AddressSanitizer: stack-buffer-overflow.*");
#endif
}

TEST_F(CiAsanDeathTest, GlobalBufferOverflow)
{
#if !PATOMIC_CI_ASAN
    GTEST_SKIP() << "Address Sanitizer is not enabled";
#else
    auto fn = []() noexcept -> void {
        static int arr[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)
    };
    EXPECT_DEATH(fn(), ".*AddressSanitizer: global-buffer-overflow.*");
#endif
}
