#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_fetch_neg(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        void *ret
    )>& fp_fetch_neg
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };

#define DO_TEST_FETCH_NEG()        \
    object_old = object;           \
    fp_fetch_neg(object, ret);     \
    ASSERT_EQ(ret, object_old);    \
    object_old.neg();              \
    ASSERT_EQ(object, object_old)

    // -0 -> 0
    DO_TEST_FETCH_NEG();

    // -1 -> max
    object.inc();
    DO_TEST_FETCH_NEG();

    // -max -> 1
    DO_TEST_FETCH_NEG();

    // -2 -> (max - 1)
    object.inc();
    DO_TEST_FETCH_NEG();

    // -(max - 1) -> 2
    DO_TEST_FETCH_NEG();
}

void
test_neg(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object
    )>& fp_neg
)
{
    // wrap as fetch
    auto fetch_neg = [&](void *object, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_neg(object);
    };

    // defer to fetch variant
    return test_fetch_neg(width, align, fetch_neg);
}

}  // namespace
