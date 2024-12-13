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
    // todo
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
