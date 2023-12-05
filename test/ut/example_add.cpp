#include <gtest/gtest.h>
#include <patomic/patomic.h>


TEST(UtSuite, Add)
{
    ASSERT_EQ(5, patomic_example_add(2, 3));
}
