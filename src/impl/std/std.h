#ifndef PATOMIC_IMPL_STD_H
#define PATOMIC_IMPL_STD_H

#include <patomic/patomic.h>


/**
 * @addtogroup impl.std
 *
 * @brief
 *   Support for operations depends on the availability of C11 atomics. If one
 *   operation is supported for a given width, all operations are supported for
 *   that width.
 *
 * @note
 *   Bitwise operations are implemented as a cmpxchg loop.
 *
 * @param byte_width
 *   The width of an object to operate on.
 *
 * @param order
 *   The minimum memory order to perform the operation with.
 *
 * @param options
 *   Value is currently unused.
 *
 * @return
 *   Implementation where operations are as C11 atomic operations would be.
 */
patomic_t
patomic_impl_create_std(
    size_t byte_width,
    patomic_memory_order_t order,
    unsigned int options
);


/**
 * @addtogroup impl.std
 *
 * @brief
 *   Support for operations depends on the availability of C11 atomics. If one
 *   operation is supported for a given width, all operations are supported for
 *   that width.
 *
 * @note
 *   Bitwise operations are implemented as a cmpxchg loop.
 *
 * @param byte_width
 *   The width of an object to operate on.
 *
 * @param options
 *   Value is currently unused.
 *
 * @return
 *   Implementation where operations are as C11 atomic operations would be.
 */
patomic_explicit_t
patomic_impl_create_explicit_std(
    size_t byte_width,
    unsigned int options
);


/**
 * @addtogroup impl.std
 *
 * @brief
 *   No operations are supported here, since C11 does not provide transactional
 *   operations.
 *
 * @param options
 *   Value is ignored.
 *
 * @return
 *   Implementation where no operations are supported and alignment requirements
 *   are the minimum possible.
 */
patomic_transaction_t
patomic_impl_create_transaction_std(
    unsigned int options
);


#endif  /* PATOMIC_IMPL_STD_H */
