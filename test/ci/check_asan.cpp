#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>


class CiAsan : public testing::Test
{};


TEST_F(CiAsan, UseAfterFree)
{
    int *p = new int(5);
    volatile int _ = *p;
    delete p;

    volatile int val;
    EXPECT_NONFATAL_FAILURE(val = *p, "asan");
}

TEST_F(CiAsan, HeapBufferOverflow)
{
    int *arr = new int[100]{};
    int index = 1;

    volatile int val;
    EXPECT_NONFATAL_FAILURE(val = arr[100 + index], "asan");

    delete[] arr;
}

TEST_F(CiAsan, StackBufferOverflow)
{
    int arr[100]{};
    int index = 1;

    volatile int val;
    EXPECT_NONFATAL_FAILURE(val = arr[100 + index], "asan");
}

TEST_F(CiAsan, StaticBufferOverflow)
{
    static int arr[100]{};
    int index = 1;

    volatile int val;
    EXPECT_NONFATAL_FAILURE(val = arr[100 + index], "asan");
}
