#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_fetch_inc(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        void *ret
    )>& fp_fetch_inc
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };

#define DO_TEST_FETCH_INC()        \
    object_old = object;           \
    fp_fetch_inc(object, ret);     \
    ASSERT_EQ(ret, object_old);    \
    object_old.inc();              \
    ASSERT_EQ(object, object_old)

    // ++(max - 1) -> max
    object.dec();
    object.dec();
    DO_TEST_FETCH_INC();

    // ++max -> 0
    DO_TEST_FETCH_INC();

    // ++0 -> 1
    DO_TEST_FETCH_INC();

    // ++1 -> 2
    DO_TEST_FETCH_INC();
}

void
test_inc(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object
    )>& fp_inc
)
{
    // wrap as fetch
    auto fetch_inc = [&](void *object, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_inc(object);
    };

    // defer to fetch variant
    return test_fetch_inc(width, align, fetch_inc);
}

}  // namespace
