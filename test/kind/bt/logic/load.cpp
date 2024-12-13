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
    // base is here to make sure fp_load doesn't modify object
    test::generic_integer base { width, align, false };
    test::generic_integer object { width, align, false };
    test::generic_integer ret {width, align, false };

    // set ret to non-zero
    ret.inc();

    // 0
    fp_load(object, ret);
    EXPECT_EQ(base, object);
    EXPECT_EQ(object, ret);

    // ~0
    object.inv();
    base = object;
    fp_load(object, ret);
    EXPECT_EQ(base, object);
    EXPECT_EQ(object, ret);

    // 1
    object.store_zero();
    object.inc();
    base = object;
    fp_load(object, ret);
    EXPECT_EQ(base, object);
    EXPECT_EQ(object, ret);

    // max
    object.store_max();
    base = object;
    fp_load(object, ret);
    EXPECT_EQ(base, object);
    EXPECT_EQ(object, ret);

    // min
    object.store_min();
    base = object;
    fp_load(object, ret);
    EXPECT_EQ(base, object);
    EXPECT_EQ(object, ret);
}

}  // namespace
