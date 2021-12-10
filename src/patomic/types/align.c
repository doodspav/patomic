#include <patomic/types/align.h>
#include <patomic/types/intptr.h>


size_t
patomic_cache_line_size(void)
{
    return (size_t) PATOMIC_MAX_CACHE_LINE_SIZE;
}


/* checks if x is a power of 2 */
#define PATOMIC_IS_POW2(x) (((x) != 0) && (((x) & ((x) - 1u)) == 0))

/* (x % y) assuming y is a power of 2 */
#define PATOMIC_MOD_POW2(x, y) ((x) & ((y) - 1u))

/* (x % y) with a conditionally fast case if y is a power of 2 */
#define PATOMIC_MOD_CPOW2(x, y) \
    (PATOMIC_IS_POW2(y) ? PATOMIC_MOD_POW2(x, y) : ((x) % (y)))


int
patomic_align_meets_recommended(
    const volatile void *ptr,
    patomic_align_t align
)
{
    patomic_uintptr_t addr = (patomic_uintptr_t) ptr;
    return (PATOMIC_MOD_POW2(addr, align.recommended) == 0);
}

int
patomic_align_meets_minimum(
    const volatile void *ptr,
    patomic_align_t align,
    size_t width
)
{
    patomic_uintptr_t addr = (patomic_uintptr_t) ptr;
    if (PATOMIC_MOD_POW2(addr, align.minimum) == 0)
    {
        if (align.size_within == 0) { return 1; }
        addr = (patomic_uintptr_t) PATOMIC_MOD_CPOW2(addr, align.size_within);
        return (addr + width) <= align.size_within;
    }
    else { return 0; }
}
