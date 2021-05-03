#include "null.h"

#include <patomic/macros/ignore_unused.h>

static const patomic_ops_t patomic_ops_NULL;
static const patomic_ops_explicit_t patomic_ops_explicit_NULL;

patomic_ops_t
patomic_impl_create_ops_null(
    size_t byte_width,
    patomic_memory_order_t order,
    int options
)
{
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(order);
    PATOMIC_IGNORE_UNUSED(options);
    return patomic_ops_NULL;
}

patomic_ops_explicit_t
patomic_impl_create_ops_explicit_null(
    size_t byte_width,
    int options
)
{
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(options);
    return patomic_ops_explicit_NULL;
}
