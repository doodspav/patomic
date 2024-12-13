#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_bin_fetch_xor(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *arg,
        void *ret
    )>& fp_fetch_xor
)
{
    // todo
}

void
test_bin_xor(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *arg
    )>& fp_xor
)
{
    // wrap as fetch
    auto fetch_xor = [&](void *object, const void *arg, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_xor(object, arg);
    };

    // defer to fetch variant
    return test_bin_fetch_xor(width, align, fetch_xor);
}

}  // namespace
