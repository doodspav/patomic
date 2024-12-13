#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_fetch_sub(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument,
        void *ret
    )>& fp_fetch_sub
)
{
    // create integers
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer argument { width, align, false };
    test::generic_integer ret { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };
    test::generic_integer object_exp { width, align, false };
    test::generic_integer argument_old { width, align, false };

#define DO_TEST_FETCH_SUB()              \
    object_old = object;                 \
    argument_old = argument;             \
    object_exp = object;                 \
    object_exp.sub(argument);            \
    fp_fetch_sub(object, argument, ret); \
    ASSERT_EQ(object, object_exp);       \
    ASSERT_EQ(argument, argument_old);   \
    ASSERT_EQ(ret, object_old)

    // 0 - 0 -> 0
    DO_TEST_FETCH_SUB();

    // 0 - 1 -> max
    argument.inc();
    DO_TEST_FETCH_SUB();

    // 0 - 2 -> (max - 1)
    object.store_zero();
    argument.inc();
    DO_TEST_FETCH_SUB();

    // 0 - max -> 1
    object.store_zero();
    argument.store_max();
    DO_TEST_FETCH_SUB();

    // 1 - 0 -> 1
    argument.store_zero();
    DO_TEST_FETCH_SUB();

    // 1 - 1 -> 0
    argument.inc();
    DO_TEST_FETCH_SUB();

    // 1 - 2 -> max
    object.inc();
    argument.inc();
    DO_TEST_FETCH_SUB();

    // 1 - max -> 2
    object.store_zero();
    object.inc();
    argument.store_max();
    DO_TEST_FETCH_SUB();

    // max - 0 -> max
    object.store_max();
    argument.store_zero();
    DO_TEST_FETCH_SUB();

    // max - 1 -> (max - 1)
    argument.inc();
    DO_TEST_FETCH_SUB();

    // max - 2 -> (max - 2)
    object.store_max();
    argument.inc();
    DO_TEST_FETCH_SUB();

    // max - max -> 0
    object.store_max();
    argument.store_max();
    DO_TEST_FETCH_SUB();
}

void
test_sub(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument
    )>& fp_sub
)
{
    // wrap as fetch
    auto fetch_sub = [&](void *object, const void *argument, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_sub(object, argument);
    };

    // defer to fetch variant
    return test_fetch_sub(width, align, fetch_sub);
    }

}  // namespace
