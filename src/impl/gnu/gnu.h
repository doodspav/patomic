#ifndef PATOMIC_IMPL_GNU_H
#define PATOMIC_IMPL_GNU_H

#include <stddef.h>

#include <patomic/patomic.h>

patomic_t
patomic_impl_create_gnu(size_t, patomic_memory_order_t, int);

patomic_explicit_t
patomic_impl_create_explicit_gnu(size_t, int);

patomic_transaction_t
patomic_impl_create_transaction_gnu(int);

#endif  /* !PATOMIC_IMPL_GNU_H */
