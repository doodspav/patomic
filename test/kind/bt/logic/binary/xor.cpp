#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_bin_fetch_xor(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument,
        void *ret
    )>& fp_fetch_xor
)
{
    // start with all bits unset
    test::generic_integer object { width, align, false };
    test::generic_integer argument { width, align, false };
    test::generic_integer ret { width, align, false };
    test::generic_integer object_old { width, align, false };
    test::generic_integer object_exp { width, align, false };
    test::generic_integer argument_old { width, align, false };

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        for (std::size_t j = 0; j < object.bit_width(); ++j)
        {
            // set up params
            object.inv_at(i);
            object_old = object;
            argument.inv_at(j);
            argument_old = argument;

            // set up expected
            object_exp.store_zero();
            if (i != j)
            {
                object_exp.inv_at(i);
                object_exp.inv_at(j);
            }

            // test
            fp_fetch_xor(object, argument, ret);
            ASSERT_EQ(object, object_exp);
            ASSERT_EQ(argument, argument_old);
            ASSERT_EQ(ret, object_old);

            // reset
            argument.inv_at(j);
            object = argument;
        }
    }

    // start with all bits set
    object.inv();
    argument.inv();

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        for (std::size_t j = 0; j < object.bit_width(); ++j)
        {
            // set up params
            object.inv_at(i);
            object_old = object;
            argument.inv_at(j);
            argument_old = argument;

            // set up expected
            object_exp.store_zero();
            if (i != j)
            {
                object_exp.inv_at(i);
                object_exp.inv_at(j);
            }

            // test
            fp_fetch_xor(object, argument, ret);
            ASSERT_EQ(object, object_exp);
            ASSERT_EQ(argument, argument_old);
            ASSERT_EQ(ret, object_old);

            // reset
            argument.inv_at(j);
            object = argument;
        }
    }
}

void
test_bin_xor(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument
    )>& fp_xor
)
{
    // wrap as fetch
    auto fetch_xor = [&](void *object, const void *argument, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_xor(object, argument);
    };

    // defer to fetch variant
    return test_bin_fetch_xor(width, align, fetch_xor);
}

}  // namespace
