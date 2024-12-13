#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{
    
void
test_cmpxchg_strong(
    std::size_t width,
    std::size_t align,
    const std::function<int(
        void *object,
        void *expected,
        const void *desired
    )>& fp_cmpxchg_strong
)
{
    // create integers
    int ok;
    // params
    test::generic_integer object { width, align, false };
    test::generic_integer expected { width, align, false };
    test::generic_integer desired { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };
    test::generic_integer expected_old { width, align, false };
    test::generic_integer desired_old { width, align, false };

#define DO_TEST_CMPXCHG_STRONG()                       \
    expected = object;                                 \
    expected.inv();                                    \
    object_old = object;                               \
    desired_old = desired;                             \
    ok = fp_cmpxchg_strong(object, expected, desired); \
    ASSERT_FALSE(ok);                                  \
    ASSERT_EQ(object, object_old);                     \
    ASSERT_EQ(expected, object);                       \
    ASSERT_EQ(desired, desired_old);                   \
    expected_old = expected;                           \
    ok = fp_cmpxchg_strong(object, expected, desired); \
    ASSERT_TRUE(ok);                                   \
    ASSERT_EQ(object, desired);                        \
    ASSERT_EQ(expected, expected_old);                 \
    ASSERT_EQ(desired, desired_old)

    // 0 -> ~0
    desired.inv();
    DO_TEST_CMPXCHG_STRONG();

    // ~0 -> 1
    desired.store_zero();
    desired.inc();
    DO_TEST_CMPXCHG_STRONG();

    // 1 -> max
    desired.store_max();
    DO_TEST_CMPXCHG_STRONG();

    // max -> min
    desired.store_min();
    DO_TEST_CMPXCHG_STRONG();

    // min -> 0
    desired.store_zero();
    DO_TEST_CMPXCHG_STRONG();
}

}  // namespace
