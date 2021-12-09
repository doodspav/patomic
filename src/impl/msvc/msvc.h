#ifndef PATOMIC_IMPL_MSVC_H
#define PATOMIC_IMPL_MSVC_H

#include <stddef.h>

#include <patomic/patomic.h>
#include <patomic/patomic_export.h>

PATOMIC_NO_EXPORT patomic_t
patomic_impl_create_msvc(size_t, patomic_memory_order_t, int);

PATOMIC_NO_EXPORT patomic_explicit_t
patomic_impl_create_explicit_msvc(size_t, int);

PATOMIC_NO_EXPORT patomic_transaction_t
patomic_impl_create_transaction_msvc(int);

#endif  /* !PATOMIC_IMPL_MSVC_H */
