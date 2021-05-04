#ifndef PATOMIC_REGISTER_H
#define PATOMIC_REGISTER_H

#include <stddef.h>

#include <patomic/patomic.h>

#include "null/null.h"
#include "std/std.h"

/*
 * REGISTER STRUCT
 *
 * - id: implementation id from ids.h
 * - fp_create: (byte_width, order, options)
 * - fp_create_explicit: (order, options)
 *
 * NOTE: - id doesn't HAVE to be provided from ids.h, but if it
 *         isn't, it won't be explicitly selectable by a user
 *       - the behaviour is UNDEFINED if two ids have the same value
 */
typedef struct {
    int id;
    patomic_t (* fp_create) (size_t, patomic_memory_order_t, int);
    patomic_explicit_t (* fp_create_explicit) (size_t, int);
} patomic_impl_t;

/*
 * REGISTER ARRAY
 *
 * - there is NO REQUIREMENT that id matches index
 * - nor that NULL must be the final element
 */
static const patomic_impl_t
patomic_impl_register[] = {
    {
        patomic_impl_id_NULL,
        patomic_impl_create_null,
        patomic_impl_create_explicit_null
    },
    {
        patomic_impl_id_STD,
        patomic_impl_create_std,
        patomic_impl_create_explicit_std
    }
};

#endif  /* !PATOMIC_REGISTER_H */
