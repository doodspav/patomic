#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>


/// @brief Test fixture.
class StAsan : public testing::Test
{};


/// @brief Check that asan catches use-after-free.
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

/// @brief Check that asan catches a heap allocated buffer overflow.
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

/// @brief Check that asan catches a stack allocated buffer overflow.
TEST_F(StAsan, StackBufferOverflow)
{
#if PATOMIC_HAS_ASAN
    EXPECT_NONFATAL_FAILURE({
    EXPECT_FATAL_FAILURE({
        int arr[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)
    }, "asan");
#if defined(__clang__)
    }, "Actual: 2");  // this is also caught by ubsan on clang once more
#elif defined(__GNUG__)
    }, "Actual: 3");  // this is also caught by ubsan on gcc twice more
#endif
#endif
}

/// @brief Check that asan catches a statically allocated buffer overflow.
TEST_F(StAsan, GlobalBufferOverflow)
{
#if PATOMIC_HAS_ASAN
    EXPECT_NONFATAL_FAILURE({
    EXPECT_FATAL_FAILURE({
        static int arr[100]{};
        int i = 1;

        volatile int _ = arr[100 + i];  // <-- buffer overflow (intentional)
    }, "asan");
#if defined(__clang__)
    }, "Actual: 2");  // this is also caught by ubsan on clang once more
#elif defined(__GNUG__)
    }, "Actual: 3");  // this is also caught by ubsan on gcc twice more
#endif
#endif
}
