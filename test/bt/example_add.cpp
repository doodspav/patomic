#include <gtest/gtest.h>
#include <patomic/patomic.h>

#include <iostream>


TEST(BtSuite, Add)
{
    ASSERT_EQ(6, patomic_example_add(2, 3));
}
