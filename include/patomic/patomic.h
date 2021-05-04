#ifndef PATOMIC_PATOMIC_H
#define PATOMIC_PATOMIC_H

#include <stddef.h>

#include <patomic/types/align.h>
#include <patomic/types/ids.h>
#include <patomic/types/ops.h>
#include <patomic/types/options.h>
#include <patomic/types/memory_order.h>

#include <patomic/patomic_export.h>

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


#ifdef __cplusplus
}
#endif

#endif  /* !PATOMIC_PATOMIC_H */
