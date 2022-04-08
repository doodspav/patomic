#include "null.h"

#include <patomic/patomic.h>
#include <patomic/macros/ignore_unused.h>


patomic_t
patomic_impl_create_null(
    size_t byte_width,
    patomic_memory_order_t order,
    unsigned int options
)
{
    patomic_t ret = {0};
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(order);
    PATOMIC_IGNORE_UNUSED(options);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}

patomic_explicit_t
patomic_impl_create_explicit_null(
    size_t byte_width,
    unsigned int options
)
{
    patomic_explicit_t ret = {0};
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(options);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}

patomic_transaction_t
patomic_impl_create_transaction_null(
    unsigned int options
)
{
    patomic_transaction_t ret = {0};
    PATOMIC_IGNORE_UNUSED(options);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}
