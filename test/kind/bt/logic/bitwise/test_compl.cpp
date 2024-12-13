#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_bit_test_compl(
    std::size_t width,
    std::size_t align,
    const std::function<int(
        void *object,
        int offset
    )>& fp_test_compl
)
{
    // start with all bits unset
    test::generic_integer object { width, align, false };
    test::generic_integer object_exp { width, align, false };

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        // 0 -> 1
        object_exp.inv_at(i);
        ASSERT_EQ(0, fp_test_compl(object, i));
        ASSERT_EQ(object, object_exp);

        // 1 -> 0
        object_exp.inv_at(i);
        ASSERT_EQ(1, fp_test_compl(object, i));
        ASSERT_EQ(object, object_exp);
    }

    // start with all bits set
    object.store_zero();
    object.inv();
    object_exp = object;

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        // 1 -> 0
        object_exp.inv_at(i);
        ASSERT_EQ(1, fp_test_compl(object, i));
        ASSERT_EQ(object, object_exp);

        // 0 -> 1
        object_exp.inv_at(i);
        ASSERT_EQ(0, fp_test_compl(object, i));
        ASSERT_EQ(object, object_exp);
    }
}

}  // namespace
