#include <gtest/gtest.h>
#include <patomic/patomic.h>


TEST(BtSuite, Sub)
{
    GTEST_SKIP();
    ASSERT_EQ(5, patomic_example_add(2, 3));
}
