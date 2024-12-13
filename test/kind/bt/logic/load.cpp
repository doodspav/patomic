#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_load(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        const void *object,
        void *ret
    )>& fp_load
)
{
    // create integers
    test::generic_integer object { width, align, false };
    test::generic_integer ret {width, align, false };
    test::generic_integer object_old { width, align, false };

    // set ret to non-zero
    ret.inc();

    // 0
    fp_load(object, ret);
    EXPECT_EQ(object, ret);
    EXPECT_EQ(object_old, object);

    // ~0
    object.inv();
    object_old = object;
    fp_load(object, ret);
    EXPECT_EQ(object, ret);
    EXPECT_EQ(object_old, object);

    // 1
    object.store_zero();
    object.inc();
    object_old = object;
    fp_load(object, ret);
    EXPECT_EQ(object, ret);
    EXPECT_EQ(object_old, object);

    // max
    object.store_max();
    object_old = object;
    fp_load(object, ret);
    EXPECT_EQ(object, ret);
    EXPECT_EQ(object_old, object);

    // min
    object.store_min();
    object_old = object;
    fp_load(object, ret);
    EXPECT_EQ(object, ret);
    EXPECT_EQ(object_old, object);
}

}  // namespace
