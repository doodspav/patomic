#include "null.h"

#define PATOMIC_IGNORE_UNUSED(expr) ((void)(expr))


patomic_t
patomic_impl_create_null(
    size_t byte_width,
    patomic_memory_order_t order,
    unsigned int opts
)
{
    /* zero all fields */
    patomic_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(order);
    PATOMIC_IGNORE_UNUSED(opts);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}


patomic_explicit_t
patomic_impl_create_explicit_null(
    size_t byte_width,
    unsigned int opts
)
{
    /* zero all fields */
    patomic_explicit_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(opts);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}


patomic_transaction_t
patomic_impl_create_transaction_null(
    unsigned int opts
)
{
    /* zero all fields */
    patomic_transaction_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(opts);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}
