#include <test/common/generic_int.hpp>

#include <test/suite/bt_logic.hpp>


namespace
{

void
test_fetch_sub(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument,
        void *ret
    )>& fp_fetch_sub
)
{
    // todo
}

void
test_sub(
    std::size_t width,
    std::size_t align,
    const std::function<void(
        void *object,
        const void *argument
    )>& fp_sub
)
{
    // wrap as fetch
    auto fetch_sub = [&](void *object, const void *argument, void *ret) -> void {
        std::memcpy(ret, object, width);
        return fp_sub(object, argument);
    };

    // defer to fetch variant
    return test_fetch_sub(width, align, fetch_sub);
    }

}  // namespace
