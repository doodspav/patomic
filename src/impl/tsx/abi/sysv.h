#ifndef PATOMIC_IMPL_TSX_ABI_SYSV_H
#define PATOMIC_IMPL_TSX_ABI_SYSV_H

#include <patomic/patomic_config.h>

#include "types.h"

#if PATOMIC_HAVE_SYSV_ABI_ATTR && \
    PATOMIC_HAVE_X86           && \
    !defined(PATOMIC_TSX_ABI)

#define PATOMIC_TSX_ABI __attribute__((sysv_abi))

static patomic_tsx_funcs_t
patomic_tsx_get_funcs(void)
{
    static patomic_tsx_funcs_t funcs_NULL;
    return funcs_NULL;
}

#endif  /* !(PATOMIC_HAVE_SYSV_ABI_ATTR && PATOMIC_HAVE_X86) */

#endif  /* !PATOMIC_IMPL_TSX_ABI_SYSV_H */
