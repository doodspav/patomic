#ifndef PATOMIC_IMPL_GNU_H
#define PATOMIC_IMPL_GNU_H

#include <stddef.h>

#include <patomic/patomic.h>
#include <patomic/patomic_export.h>

PATOMIC_NO_EXPORT patomic_t
patomic_impl_create_gnu(size_t, patomic_memory_order_t, unsigned int);

PATOMIC_NO_EXPORT patomic_explicit_t
patomic_impl_create_explicit_gnu(size_t, unsigned int);

PATOMIC_NO_EXPORT patomic_transaction_t
patomic_impl_create_transaction_gnu(unsigned int);

#endif  /* !PATOMIC_IMPL_GNU_H */
