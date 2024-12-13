#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_bin_fetch_or(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *arg,
        void *ret
    )>& fp_fetch_or
)
{
    // todo
}

void
test_bin_or(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *arg
    )>& fp_or
)
{
    // wrap as fetch
    auto fetch_or = [&](void *object, const void *arg, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_or(object, arg);
    };

    // defer to fetch variant
    return test_bin_fetch_or(width, align, fetch_or);
}

}  // namespace
