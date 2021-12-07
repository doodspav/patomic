#ifndef PATOMIC_IMPL_MSVC_H
#define PATOMIC_IMPL_MSVC_H

#include <stddef.h>

#include <patomic/patomic.h>

patomic_t
patomic_impl_create_msvc(size_t, patomic_memory_order_t, int);

patomic_explicit_t
patomic_impl_create_explicit_msvc(size_t, int);

patomic_transaction_t
patomic_impl_create_transaction_msvc(int);

#endif  /* !PATOMIC_IMPL_MSVC_H */
