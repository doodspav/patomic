#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>


class StAsan : public testing::Test
{};


TEST_F(StAsan, UseAfterFree)
{
#if PATOMIC_HAS_ASAN
    EXPECT_FATAL_FAILURE({
        int *p = new int(5);
        volatile int val = *p;
        delete p;

        volatile int _ = *p;  // <-- use after free (intentional)
    }, "asan");
#endif
}

TEST_F(StAsan, HeapBufferOverflow)
{
#if PATOMIC_HAS_ASAN
    EXPECT_FATAL_FAILURE({
        int *arr = new int[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)

        delete[] arr;
    }, "asan");
#endif
}

TEST_F(StAsan, StackBufferOverflow)
{
#if PATOMIC_HAS_ASAN
#fdef __clang__
    EXPECT_NONFATAL_FAILURE({
#endif
    EXPECT_FATAL_FAILURE({
        int arr[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)
    }, "asan");
#ifdef __clang__
    }, "Actual: 2");  // this is also caught by ubsan on clang
#endif
#endif
}

TEST_F(StAsan, GlobalBufferOverflow)
{
#if PATOMIC_HAS_ASAN
#ifdef __clang__
    EXPECT_NONFATAL_FAILURE({
#endif
    EXPECT_FATAL_FAILURE({
        static int arr[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)
    }, "asan");
#ifdef __clang__
    }, "Actual: 2");  // this is also caught by ubsan on clang
#endif
#endif
}
