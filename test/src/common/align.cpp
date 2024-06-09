#include <test/common/align.hpp>

#include <cstdint>

namespace test
{


void *
aligned_pointer(
    void *const buf_ptr, const std::size_t buf_size, const std::size_t align,
    const std::size_t size
) noexcept
{
    // defer to const implementation
    const void *cc_ptr = buf_ptr;
    cc_ptr = aligned_pointer(cc_ptr, buf_size, align, size);
    return const_cast<void *>(cc_ptr);
}


const void *
aligned_pointer(
    const void *const buf_ptr, const std::size_t buf_size,
    const std::size_t align, const std::size_t size
) noexcept
{
    // special case for zero alignment
    if (align == 0)
    {
        return (buf_size >= size) ? buf_ptr : nullptr;
    }

    // calculate the offset to the next aligned address
    const auto raw_addr = reinterpret_cast<std::uintptr_t>(buf_ptr);
    const std::size_t remainder = raw_addr % align;
    const std::size_t offset = (align - remainder) % align;

    // check that there is enough room in the buffer
    if (buf_size < offset + size)
    {
        return nullptr;
    }

    // return aligned pointer
    return static_cast<const char *>(buf_ptr) + offset;
}


std::size_t
runtime_alignof(const void *ptr) noexcept
{
    // get raw address
    auto raw_addr = reinterpret_cast<std::uintptr_t>(ptr);

    // handle special case of zero (would cause infinite loop below)
    std::size_t alignment = 1;
    if (raw_addr == 0)
    {
        // return minimal alignment
        return alignment;
    }

    // manually perform std::countr_zero from C++20 (a.k.a. __builtin_clz)
    while ((raw_addr & 1) == 0)
    {
        raw_addr >>= 1;
        alignment <<= 1;
    }
    return alignment;
}


}  // namespace test
