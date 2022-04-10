#include <patomic/types/ids.h>

#include <patomic/macros/force_inline.h>
#include <patomic/stdlib/assert.h>

#include "../../impl/register.h"


PATOMIC_FORCE_INLINE static int
patomic_is_pow2_or_zero(
    unsigned long val
)
{
    return (val & (val - 1ul)) == 0ul;
}


unsigned long
patomic_get_ids(
    unsigned int kinds
)
{
    patomic_impl_t const *begin = patomic_impl_register;
    patomic_impl_t const *const end = begin + PATOMIC_IMPL_REGISTER_SIZE;
    unsigned long ids = 0ul;

    for (; begin != end; ++begin)
    {
        unsigned int kind = (unsigned int) begin->kind;
        unsigned long id = (unsigned long) begin->id;
        if (kind & kinds) { ids |= id; }
    }

    return ids;
}

unsigned int
patomic_get_kind(
    unsigned long id
)
{
    patomic_impl_t const *begin = patomic_impl_register;
    patomic_impl_t const *const end = begin + PATOMIC_IMPL_REGISTER_SIZE;

    patomic_assert_always(patomic_is_pow2_or_zero(id));

    for (; begin != end; ++begin) {
        if (begin->id == id) { return (unsigned int) begin->kind; }
    }

    return patomic_kind_UNKN;
}
