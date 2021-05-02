#include "null.h"

static const patomic_ops_t patomic_ops_NULL;
static const patomic_ops_explicit_t patomic_ops_explicit_NULL;

patomic_ops_t
patomic_impl_create_ops_null(
    size_t byte_width,
    patomic_memory_order_t order
)
{
    (void) byte_width;
    (void) order;
    return patomic_ops_NULL;
}

patomic_ops_explicit_t
patomic_impl_create_ops_explicit_null(
    size_t byte_width
)
{
    (void) byte_width;
    return patomic_ops_explicit_NULL;
}
