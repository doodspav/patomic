#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_fetch_dec(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        void *ret
    )>& fp_fetch_dec
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };

#define DO_TEST_FETCH_DEC()        \
    object_old = object;           \
    fp_fetch_dec(object, ret);     \
    ASSERT_EQ(ret, object_old);    \
    object_old.dec();              \
    ASSERT_EQ(object, object_old)

    // --2 -> 1
    object.inc();
    object.inc();
    DO_TEST_FETCH_DEC();

    // --1 -> 0
    DO_TEST_FETCH_DEC();

    // --0 -> max
    DO_TEST_FETCH_DEC();

    // --max -> (max - 1)
    DO_TEST_FETCH_DEC();
}

void
test_dec(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object
    )>& fp_dec
)
{
    // wrap as fetch
    auto fetch_dec = [&](void *object, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_dec(object);
    };

    // defer to fetch variant
    return test_fetch_dec(width, align, fetch_dec);
}

}  // namespace
