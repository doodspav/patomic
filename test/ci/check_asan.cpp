#include <gtest/gtest.h>


class CiAsanDeathTest : public testing::Test
{};


#if PATOMIC_CI_ASAN

TEST_F(CiAsanDeathTest, UseAfterFree)
{
    auto fn = []() noexcept -> void {
        int *p = new int(5);
        volatile int val = *p;
        delete p;

        volatile int _ = *p;  // <-- use after free (intentional)
    };
    EXPECT_DEATH(fn(), ".*AddressSanitizer: heap-use-after-free.*");
}

TEST_F(CiAsanDeathTest, HeapBufferOverflow)
{
    auto fn = []() noexcept -> void {
        int *arr = new int[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)

        delete[] arr;
    };
    EXPECT_DEATH(fn(), ".*AddressSanitizer: heap-buffer-overflow.*");
}

TEST_F(CiAsanDeathTest, StackBufferOverflow)
{
    auto fn = []() noexcept -> void {
        int arr[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)
    };
    EXPECT_DEATH(fn(), ".*AddressSanitizer: stack-buffer-overflow.*");
}

TEST_F(CiAsanDeathTest, GlobalBufferOverflow)
{
    auto fn = []() noexcept -> void {
        static int arr[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)
    };
    EXPECT_DEATH(fn(), ".*AddressSanitizer: global-buffer-overflow.*");
}

#endif  // PATOMIC_CI_ASAN
