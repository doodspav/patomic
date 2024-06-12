#include <patomic/types/align.h>

#include <patomic/stdlib/math.h>
#include <patomic/stdlib/stdint.h>


size_t
patomic_cache_line_size(void)
{
    return PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE;
}


int
patomic_align_meets_recommended(
    const volatile void *const ptr,
    const patomic_align_t align
)
{
    /* declarations */
    patomic_intptr_unsigned_t addr = (patomic_intptr_unsigned_t) ptr;

    /* check that recommended alignment is valid */
    if(!patomic_unsigned_is_pow2(align.recommended))
    {
        return 0;
    }

    /* check that addr pointer is aligned to recommended alignment */
    return patomic_unsigned_mod_pow2(addr, align.recommended) == 0;
}


int
patomic_align_meets_minimum(
    const volatile void *const ptr,
    const patomic_align_t align,
    const size_t width
)
{
    /* declarations */
    patomic_intptr_unsigned_t addr = (patomic_intptr_unsigned_t) ptr;

    /* check that minimum alignment is valid */
    if(!patomic_unsigned_is_pow2(align.minimum))
    {
        return 0;
    }

    /* check that addr ptr is aligned to minimum alignment */
    if(patomic_unsigned_mod_pow2(addr, align.minimum) == 0)
    {
        /* check if minimum alignment is always valid */
        if (align.size_within == 0)
        {
            return 1;
        }

        /* make addr value less than size_within */
        addr = (patomic_intptr_unsigned_t) \
            patomic_unsigned_mod(addr, align.size_within);

        /* check that buffer starting at addr doesn't extend past size_within */
        return (addr + width) <= align.size_within;
    }

    /* addr ptr is not aligned to minimum alignment */
    return 0;
}
