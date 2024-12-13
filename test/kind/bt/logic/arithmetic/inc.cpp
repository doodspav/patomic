#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_fetch_inc(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        void *ret
    )>& fp_fetch_inc
)
{
    // todo
}

void
test_inc(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object
    )>& fp_inc
)
{
    // wrap as fetch
    auto fetch_inc = [&](void *object, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_inc(object);
    };

    // defer to fetch variant
    return test_fetch_inc(width, align, fetch_inc);
}

}  // namespace
