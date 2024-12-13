#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_bin_fetch_and(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *arg,
        void *ret
    )>& fp_fetch_and
)
{
    // todo
}

void
test_bin_and(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *arg
    )>& fp_and
)
{
    // wrap as fetch
    auto fetch_and = [&](void *object, const void *arg, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_and(object, arg);
    };

    // defer to fetch variant
    return test_bin_fetch_and(width, align, fetch_and);
}

}  // namespace
