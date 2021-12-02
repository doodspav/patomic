#include "tsx.h"

#include <patomic/patomic.h>
#include <patomic/macros/ignore_unused.h>

static const patomic_t patomic_NULL;
static const patomic_explicit_t patomic_explicit_NULL;
static const patomic_transaction_t patomic_transaction_NULL;

patomic_t
patomic_impl_create_tsx(
    size_t byte_width,
    patomic_memory_order_t order,
    int options
)
{
    patomic_t ret;
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(order);
    PATOMIC_IGNORE_UNUSED(options);
    ret = patomic_NULL;
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}

patomic_explicit_t
patomic_impl_create_explicit_tsx(
    size_t byte_width,
    int options
)
{
    patomic_explicit_t ret;
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(options);
    ret = patomic_explicit_NULL;
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}

patomic_transaction_t
patomic_impl_create_transaction_tsx(
    int options
)
{
    patomic_transaction_t ret;
    PATOMIC_IGNORE_UNUSED(options);
    ret = patomic_transaction_NULL;
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}
