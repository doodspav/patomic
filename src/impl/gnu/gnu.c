#include "gnu.h"

#include <patomic/patomic.h>
#include <patomic/patomic_config.h>

#include <patomic/macros/ignore_unused.h>


#if PATOMIC_HAVE_GNU_ATOMIC
    #include "gnu_atomic.h"
#elif PATOMIC_HAVE_GNU_SYNC
    #include "gnu_sync.h"
#endif


patomic_t
patomic_impl_create_gnu(
        size_t byte_width,
        patomic_memory_order_t order,
        int options
)
{
    patomic_t ret = {0};
    PATOMIC_IGNORE_UNUSED(options);
#if PATOMIC_HAVE_GNU_ATOMIC || PATOMIC_HAVE_GNU_SYNC
    ret.ops = patomic_create_ops(byte_width, order);
    ret.align = patomic_create_align(byte_width);
#else
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(order);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
#endif
    return ret;
}

patomic_explicit_t
patomic_impl_create_explicit_gnu(
        size_t byte_width,
        int options
)
{
    patomic_explicit_t ret = {0};
    PATOMIC_IGNORE_UNUSED(options);
#if PATOMIC_HAVE_GNU_ATOMIC || PATOMIC_HAVE_GNU_SYNC
    ret.ops = patomic_create_ops_explicit(byte_width);
    ret.align = patomic_create_align(byte_width);
#else
    PATOMIC_IGNORE_UNUSED(byte_width);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
#endif
    return ret;
}

patomic_transaction_t
patomic_impl_create_transaction_gnu(
    int options
)
{
    patomic_transaction_t ret = {0};
    PATOMIC_IGNORE_UNUSED(options);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}
