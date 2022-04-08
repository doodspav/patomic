#include <patomic/types/ids.h>

#include "../../impl/register.h"


unsigned int
patomic_get_ids(
    unsigned int kinds
)
{
    patomic_impl_t const *begin = patomic_impl_register;
    patomic_impl_t const *const end = begin + PATOMIC_IMPL_REGISTER_SIZE;
    unsigned int ids = 0;

    for (; begin != end; ++begin)
    {
        unsigned int kind = (unsigned int) begin->kind;
        unsigned int id = (unsigned int) begin->id;
        if (kind & kinds) { ids |= id; }
    }

    return ids;
}

patomic_kind_t
patomic_get_kind(
    patomic_id_t id
)
{
    patomic_impl_t const *begin = patomic_impl_register;
    patomic_impl_t const *const end = begin + PATOMIC_IMPL_REGISTER_SIZE;

    for (; begin != end; ++begin) {
        if (begin->id == id) { return begin->kind; }
    }

    return patomic_kind_UNKN;
}
