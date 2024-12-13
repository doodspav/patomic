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
    test::generic_integer base { width, align, false };
    test::generic_integer object { width, align, false };
    test::generic_integer desired { width, align, false };
    test::generic_integer ret { width, align, false };

    // TODO: test
}

}  // namespace
