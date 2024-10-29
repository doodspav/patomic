#include <patomic/stdlib/stdalign.h>
#include <patomic/stdlib/stdint.h>


size_t
patomic_alignment_from_size(
    const size_t size
)
{
    /* early check for zero size */
    if (size == 0)
    {
        return 0;
    }

    /* return largest power of 2 which divides size */
    return size & -size;
}


int
patomic_is_aligned(
    const volatile void *const ptr,
    const size_t alignment
)
{
    /* check that our address is a multiple of the alignment */
    patomic_intptr_unsigned_t address = (patomic_intptr_unsigned_t) ptr;
    address &= (alignment - 1u);
    return address == 0;
}
