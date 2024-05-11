#ifndef PATOMIC_IMPL_NULL_H
#define PATOMIC_IMPL_NULL_H

#include <patomic/patomic.h>

#include <stddef.h>


/**
 * @addtogroup impl.null
 *
 * @brief
 *   No operations are supported.
 *
 * @param byte_width
 *   Value is ignored.
 *
 * @param order
 *   Value is ignored.
 *
 * @param opts
 *   Value is ignored.
 *
 * @return
 *   Implementation where no operations are supported and alignment requirements
 *   are the minimum possible.
 */
patomic_t
patomic_impl_create_null(
    size_t byte_width,
    patomic_memory_order_t order,
    unsigned int opts
);


/**
 * @addtogroup impl.null
 *
 * @brief
 *   No operations are supported.
 *
 * @param byte_width
 *   Value is ignored.
 *
 * @param opts
 *   Value is ignored.
 *
 * @return
 *   Implementation where no operations are supported and alignment requirements
 *   are the minimum possible.
 */
patomic_explicit_t
patomic_impl_create_explicit_null(
    size_t byte_width,
    unsigned int opts
);


/**
 * @addtogroup impl.null
 *
 * @brief
 *   No operations are supported.
 *
 * @param opts
 *   Value is ignored.
 *
 * @return
 *   Implementation where no operations are supported and alignment requirements
 *   are the minimum possible.
 */
patomic_transaction_t
patomic_impl_create_transaction_null(
    unsigned int opts
);


#endif  /* PATOMIC_IMPL_NULL_H */
