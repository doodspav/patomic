#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_fetch_add(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument,
        void *ret
    )>& fp_fetch_add
)
{
    // todo
}

void
test_add(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument
    )>& fp_add
)
{
    // wrap as fetch
    auto fetch_add = [&](void *object, const void *argument, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_add(object, argument);
    };

    // defer to fetch variant
    return test_fetch_add(width, align, fetch_add);
}

}  // namespace
