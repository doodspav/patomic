#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_bin_fetch_not(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        void *ret
    )>& fp_fetch_not
)
{
    // start with all bits unset
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        // set up object
        object.inv_at(i);
        object_old = object;

        // test
        fp_fetch_not(object, ret);
        ASSERT_EQ(ret, object_old);
        object.inv();
        ASSERT_EQ(object, object_old);

        // reset
        object.inv_at(i);
    }

    // start with all bits set
    object.inv();

    // go through all bit offsets
    for (std::size_t i = 0; i < object.bit_width(); ++i)
    {
        // set up object
        object.inv_at(i);
        object_old = object;

        // test
        fp_fetch_not(object, ret);
        ASSERT_EQ(ret, object_old);
        object.inv();
        ASSERT_EQ(object, object_old);

        // reset
        object.inv_at(i);
    }
}

void
test_bin_not(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object
    )>& fp_not
)
{
    // wrap as fetch
    auto fetch_not = [&](void *object, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_not(object);
    };

    // defer to fetch variant
    return test_bin_fetch_not(width, align, fetch_not);
}

}  // namespace
