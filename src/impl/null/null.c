#include "null.h"

#include <patomic/patomic.h>
#include <patomic/macros/ignore_unused.h>

static const patomic_t patomic_NULL;
static const patomic_explicit_t patomic_explicit_NULL;

patomic_t
patomic_impl_create_null(
    size_t byte_width,
    patomic_memory_order_t order,
    int options
)
{
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(order);
    PATOMIC_IGNORE_UNUSED(options);
    return patomic_NULL;
}

patomic_explicit_t
patomic_impl_create_explicit_null(
    size_t byte_width,
    int options
)
{
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(options);
    return patomic_explicit_NULL;
}
