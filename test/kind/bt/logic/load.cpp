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

#define DO_TEST()              \
    object_old = object;       \
    fp_load(object, ret);      \
    ASSERT_EQ(object, ret);    \
    ASSERT_EQ(ret, object_old)

    // set ret to non-zero
    ret.inc();

    // 0
    DO_TEST();

    // ~0
    object.inv();
    DO_TEST();

    // 1
    object.store_zero();
    object.inc();
    DO_TEST();

    // max
    object.store_max();
    DO_TEST();

    // min
    object.store_min();
    DO_TEST();
}

}  // namespace
