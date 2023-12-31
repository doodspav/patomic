#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>


class StAsan : public testing::Test
{};

/*
TEST_F(StAsan, UseAfterFree)
{
#if PATOMIC_HAS_ASAN
    EXPECT_NONFATAL_FAILURE({
        int *p = new int(5);
        volatile int val = *p;
        delete p;

        volatile int _ = *p;
    }, "asan");
#endif
}

TEST_F(StAsan, HeapBufferOverflow)
{
#if PATOMIC_HAS_ASAN
    auto fn = []() noexcept -> void {
        int *arr = new int[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)

        delete[] arr;
    };
    EXPECT_DEATH(fn(), ".*AddressSanitizer: heap-buffer-overflow.*");
#endif
}

TEST_F(StAsan, StackBufferOverflow)
{
#if PATOMIC_HAS_ASAN
    auto fn = []() noexcept -> void {
        int arr[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)
    };
    EXPECT_DEATH(fn(), ".*AddressSanitizer: stack-buffer-overflow.*");
#endif
}

TEST_F(StAsan, GlobalBufferOverflow)
{
#if PATOMIC_HAS_ASAN
    auto fn = []() noexcept -> void {
        static int arr[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)
    };
    EXPECT_DEATH(fn(), ".*AddressSanitizer: global-buffer-overflow.*");
#endif
}
*/