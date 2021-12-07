#ifndef PATOMIC_IMPL_NULL_H
#define PATOMIC_IMPL_NULL_H

#include <stddef.h>

#include <patomic/patomic.h>

patomic_t
patomic_impl_create_null(size_t, patomic_memory_order_t, int);

patomic_explicit_t
patomic_impl_create_explicit_null(size_t, int);

patomic_transaction_t
patomic_impl_create_transaction_null(int);

#endif  /* !PATOMIC_IMPL_NULL_H */
