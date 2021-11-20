#ifndef PATOMIC_IMPL_TSX_ABI_ABI_H
#define PATOMIC_IMPL_TSX_ABI_ABI_H

#include "types.h"

#ifndef PATOMIC_TSX_ABI
#include "sysv.h"
#endif

#ifndef PATOMIC_TSX_ABI
#define PATOMIC_TSX_ABI
static patomic_tsx_funcs_t
patomic_tsx_get_funcs(void)
{
    static patomic_tsx_funcs_t funcs_NULL;
    return funcs_NULL;
}
#endif

#endif  /* !PATOMIC_IMPL_TSX_ABI_ABI_H */
