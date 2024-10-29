#include "std.h"

#include <patomic/config.h>

#include <patomic/macros/ignore_unused.h>


#if 0  /* PATOMIC_HAS_STD_ATOMIC */



#else  /* PATOMIC_HAS_STD_ATOMIC */


patomic_t
patomic_impl_create_std(
    const size_t byte_width,
    const patomic_memory_order_t order,
    const unsigned int options
)
{
    /* zero all fields */
    patomic_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(order);
    PATOMIC_IGNORE_UNUSED(options);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}


patomic_explicit_t
patomic_impl_create_explicit_std(
    const size_t byte_width,
    const unsigned int options
)
{
    /* zero all fields */
    patomic_explicit_t impl = {0};

    /* ignore all parameters */
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(options);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}


#endif  /* PATOMIC_HAS_STD_ATOMIC */


patomic_transaction_t
patomic_impl_create_transaction_std(
    const unsigned int options
)
{
    /* zero all fields */
    patomic_transaction_t impl = {0};

    /* ignore parameters */
    PATOMIC_IGNORE_UNUSED(options);

    /* set a valid minimal alignment */
    impl.align.recommended = 1;
    impl.align.minimum = 1;

    /* return */
    return impl;
}
