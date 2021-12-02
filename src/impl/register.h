#ifndef PATOMIC_REGISTER_H
#define PATOMIC_REGISTER_H

#include <stddef.h>

#include <patomic/patomic.h>

#include "null/null.h"
#include "std/std.h"
#include "msvc/msvc.h"
#include "tsx/tsx.h"
#include "gnu/gnu.h"

/*
 * REGISTER STRUCT
 *
 * - id: implementation id from ids.h
 * - fp_create: (byte_width, order, options)
 * - fp_create_explicit: (byte_width, options)
 * - fp_create_transaction: (options)
 */
typedef struct {
    patomic_impl_id_t id;
    patomic_t (* fp_create) (size_t, patomic_memory_order_t, int);
    patomic_explicit_t (* fp_create_explicit) (size_t, int);
    patomic_transaction_t (* fp_create_transaction) (int);
} patomic_impl_t;

/*
 * REGISTER ARRAY
 *
 * - two ids MUST NOT have the same value
 * - there is NO REQUIREMENT that id matches index
 * - nor that NULL must be the final element
 */
static const patomic_impl_t
patomic_impl_register[] = {
    {
        patomic_impl_id_NULL,
        patomic_impl_create_null,
        patomic_impl_create_explicit_null,
        patomic_impl_create_transaction_null
    }
    ,{
        patomic_impl_id_STD,
        patomic_impl_create_std,
        patomic_impl_create_explicit_std,
        patomic_impl_create_transaction_std
    }
    ,{
        patomic_impl_id_MSVC,
        patomic_impl_create_msvc,
        patomic_impl_create_explicit_msvc,
        patomic_impl_create_transaction_msvc
    }
    ,{
        patomic_impl_id_TSX,
        patomic_impl_create_tsx,
        patomic_impl_create_explicit_tsx,
        patomic_impl_create_transaction_tsx
    }
    ,{
        patomic_impl_id_GNU,
        patomic_impl_create_gnu,
        patomic_impl_create_explicit_gnu,
        patomic_impl_create_transaction_gnu
    }
};

#endif  /* !PATOMIC_REGISTER_H */
