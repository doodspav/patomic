#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_fetch_neg(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        void *ret
    )>& fp_fetch_neg
)
{
    // todo
}

void
test_neg(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object
    )>& fp_neg
)
{
    // wrap as fetch
    auto fetch_neg = [&](void *object, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_neg(object);
    };

    // defer to fetch variant
    return test_fetch_neg(width, align, fetch_neg);
}

}  // namespace
