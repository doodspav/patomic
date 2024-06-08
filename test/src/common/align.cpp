#include <test/common/align.hpp>

namespace test
{


void *
aligned_pointer(
    void *const buf_ptr, const size_t buf_size, const size_t align,
    const size_t size
) noexcept
{
    const void *cc_ptr = buf_ptr;
    cc_ptr = aligned_pointer(cc_ptr, buf_size, align, size);
    return const_cast<void *>(cc_ptr);
}


const void *
aligned_pointer(
    const void *const buf_ptr, const size_t buf_size, const size_t align,
    const size_t size
) noexcept
{
    // special case for zero alignment
    if (align == 0)
    {
        return (buf_size >= size) ? buf_ptr : nullptr;
    }

    // TODO
    return nullptr;
}


}  // namespace test
