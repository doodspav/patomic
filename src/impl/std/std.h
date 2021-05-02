#ifndef PATOMIC_IMPL_STD_H
#define PATOMIC_IMPL_STD_H

#include <stddef.h>

#include <patomic/types/ops.h>
#include <patomic/types/memory_order.h>

patomic_ops_t
patomic_impl_create_ops_std(size_t, patomic_memory_order_t);

patomic_ops_explicit_t
patomic_impl_create_ops_explicit_std(size_t);

#endif  /* !PATOMIC_IMPL_STD_H */
