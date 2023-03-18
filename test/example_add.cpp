#include <gtest/gtest.h>
#include <patomic/patomic.h>


TEST(suite, name)
{
    ASSERT_EQ(5, patomic_example_add(2, 3));
}