#include <patomic/stdlib/stdalign.h>
#include <patomic/stdlib/stdint.h>

size_t
__patomic_alignment_from_size(
    size_t size
)
{
    size_t align = 1u;
    size_t bit_offset = 0u;
    while (size >>= 1u) { ++bit_offset; }
    align <<= bit_offset;
    /* alignof should not be smaller than sizeof */
    if (align < size) { align <<= 1u; }
    return align;
}

int
__patomic_is_aligned(
    const volatile void *ptr,
    size_t align
)
{
    patomic_uintptr_t addr = (patomic_uintptr_t) ptr;
    addr &= (align - 1u);
    return addr == 0;
}
