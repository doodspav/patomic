#ifndef PATOMIC_PATOMIC_H
#define PATOMIC_PATOMIC_H

#include <stddef.h>

#include <patomic/types/align.h>
#include <patomic/types/ids.h>
#include <patomic/types/ops.h>
#include <patomic/types/options.h>
#include <patomic/types/memory_order.h>
#include <patomic/types/transaction.h>

#include <patomic/patomic_export.h>
#include <patomic/patomic_version.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    patomic_ops_t ops;
    patomic_align_t align;
} patomic_t;

typedef struct {
    patomic_ops_explicit_t ops;
    patomic_align_t align;
} patomic_explicit_t;

typedef struct {
    patomic_ops_transaction_t ops;
    patomic_align_t align;
    patomic_transaction_recommended_t recommended;
    patomic_transaction_safe_string_t sstring;
} patomic_transaction_t;


PATOMIC_EXPORT patomic_t
patomic_create(
    size_t byte_width,
    patomic_memory_order_t order,
    int options,
    int impl_id_argc,
    ...
);

PATOMIC_EXPORT patomic_explicit_t
patomic_create_explicit(
    size_t byte_width,
    int options,
    int impl_id_argc,
    ...
);

PATOMIC_EXPORT patomic_transaction_t
patomic_create_transaction(
    int options,
    int impl_id_argc,
    ...
);


/* WARNING: will be removed in version 1.0.0 */
PATOMIC_DEPRECATED_EXPORT int
patomic_nonnull_ops_count(
    patomic_ops_t const *const ops
);

/* WARNING: will be removed in version 1.0.0 */
PATOMIC_DEPRECATED_EXPORT int
patomic_nonnull_ops_count_explicit(
    patomic_ops_explicit_t const *const ops
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_PATOMIC_H */
