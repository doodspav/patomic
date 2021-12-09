#include <patomic/types/align.h>
#include <patomic/types/intptr.h>


size_t
patomic_cache_line_size(void)
{
    return (size_t) PATOMIC_MAX_CACHE_LINE_SIZE;
}

int
patomic_align_meets_recommended(
    const volatile void *ptr,
    patomic_align_t align
)
{
    patomic_uintptr_t addr = (patomic_uintptr_t) ptr;
    return (addr % align.recommended == 0);
}

int
patomic_align_meets_minimum(
    const volatile void *ptr,
    patomic_align_t align,
    size_t width
)
{
    patomic_uintptr_t addr = (patomic_uintptr_t) ptr;
    if (addr % align.minimum == 0)
    {
        addr %= align.size_within;
        return (addr + width) <= align.size_within;
    }
    else { return 0; }
}
