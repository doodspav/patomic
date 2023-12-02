#include <gtest/gtest.h>
#include <patomic/patomic.h>

#include <iostream>


TEST(BtSuite, Add)
{
    ASSERT_EQ(5, patomic_example_add(2, 3));
    std::cout << "<sender>John</sender>" << std::endl;
    std::cerr << "<sender>John</sender>" << std::endl;
}
