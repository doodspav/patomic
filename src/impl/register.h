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
    patomic_id_t id;
    patomic_kind_t kind;
    patomic_t (* fp_create) (size_t, patomic_memory_order_t, unsigned int);
    patomic_explicit_t (* fp_create_explicit) (size_t, unsigned int);
    patomic_transaction_t (* fp_create_transaction) (unsigned int);
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
        patomic_id_NULL,
        patomic_kind_UNKN,
        patomic_impl_create_null,
        patomic_impl_create_explicit_null,
        patomic_impl_create_transaction_null
    }
    ,{
        patomic_id_STD,
        patomic_kind_BLTN,
        patomic_impl_create_std,
        patomic_impl_create_explicit_std,
        patomic_impl_create_transaction_std
    }
    ,{
        patomic_id_MSVC,
        patomic_kind_ASM,
        patomic_impl_create_msvc,
        patomic_impl_create_explicit_msvc,
        patomic_impl_create_transaction_msvc
    }
    ,{
        patomic_id_TSX,
        patomic_kind_ASM,
        patomic_impl_create_tsx,
        patomic_impl_create_explicit_tsx,
        patomic_impl_create_transaction_tsx
    }
    ,{
        patomic_id_GNU,
        patomic_kind_BLTN,
        patomic_impl_create_gnu,
        patomic_impl_create_explicit_gnu,
        patomic_impl_create_transaction_gnu
    }
};

#define PATOMIC_IMPL_REGISTER_SIZE \
    ((sizeof patomic_impl_register) / sizeof(patomic_impl_t))

#endif  /* !PATOMIC_REGISTER_H */
