#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_exchange(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *desired,
        void *ret
    )>& fp_exchange
)
{
    // create integers
    // base is here to make sure fp_exchange doesn't modify desired
    test::generic_integer object { width, align, false };
    test::generic_integer desired { width, align, false };
    test::generic_integer ret { width, align, false };
    test::generic_integer object_old { width, align, false };
    test::generic_integer desired_old { width, align, false };

#define DO_TEST()                      \
    desired_old = desired;             \
    object_old = object;               \
    fp_exchange(object, desired, ret); \
    ASSERT_EQ(object, desired);        \
    ASSERT_EQ(desired, desired_old);   \
    ASSERT_EQ(ret, object_old)

    // set ret to non-zero
    ret.inc();

    // 0 -> ~0
    desired.inv();
    DO_TEST();

    // ~0 -> 1
    desired.store_zero();
    desired.inc();
    DO_TEST();

    // 1 -> max
    desired.store_max();
    DO_TEST();

    // max -> min
    desired.store_min();
    DO_TEST();

    // min -> 0
    desired.store_zero();
    DO_TEST();
}

}  // namespace
