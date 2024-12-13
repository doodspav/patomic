#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_bin_fetch_not(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        void *ret
    )>& fp_fetch_not
)
{
    // todo
}

void
test_bin_not(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object
    )>& fp_not
)
{
    // wrap as fetch
    auto fetch_not = [&](void *object, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_not(object);
    };

    // defer to fetch variant
    return test_bin_fetch_not(width, align, fetch_not);
}

}  // namespace
