#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_bit_test(
    std::size_t width,
    std::size_t align,
    const std::function<int(
        const void *object,
        int offset
    )>& fp_test
)
{
    // create integers
    int bit;
    // params
    test::generic_integer object { width, align, false };
    // old
    test::generic_integer object_old { width, align, false };

    // todo: test
}

}  // namespace
