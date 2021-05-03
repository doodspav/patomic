#ifndef PATOMIC_REGISTER_H
#define PATOMIC_REGISTER_H

#include <stddef.h>

#include <patomic/impl/ids.h>
#include <patomic/types/ops.h>
#include <patomic/types/memory_order.h>

#include "null/null.h"
#include "std/std.h"

/*
 * REGISTER STRUCT
 *
 * - id: implementation id from ids.h
 * - fp_create_ops: (byte_width, order, options)
 * - fp_create_explicit_ops: (order, options)
 *
 * NOTE: - id doesn't HAVE to be provided from ids.h, but if it
 *         isn't, it won't be explicitly selectable by a user
 *       - the behaviour is UNDEFINED if two ids have the same value
 */
typedef struct {
    int id;
    patomic_ops_t (* fp_create_ops) (size_t, patomic_memory_order_t, int);
    patomic_ops_explicit_t (* fp_create_ops_explicit) (size_t, int);
} patomic_impl_register_t;

/*
 * REGISTER ARRAY
 *
 * - there is NO REQUIREMENT that id matches index
 * - nor that NULL must be the final element
 */
static const patomic_impl_register_t
patomic_impl_register[] = {
    {
        patomic_impl_id_NULL,
        patomic_impl_create_ops_null,
        patomic_impl_create_ops_explicit_null
    },
    {
        patomic_impl_id_STD,
        patomic_impl_create_ops_std,
        patomic_impl_create_ops_explicit_std
    }
};

#endif  /* !PATOMIC_REGISTER_H */
