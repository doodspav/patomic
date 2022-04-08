#include <patomic/patomic.h>
#include <patomic/stdlib/assert.h>

#include <stddef.h>

#include "impl/register.h"


#define PATOMIC_CCOPY_IF_NULL(i, pa, pb, m)                                    \
    do { if ((pa)->m == NULL) { (pa)->m = (pb)->m; i += ((pb)->m != NULL); } } \
    while (0)

#define PATOMIC_SET_MAX(pa, pb, m) \
    do { if ((pa)->m < (pb)->m) { (pa)->m = (pb)->m; } } while (0)

#define PATOMIC_SET_MIN_NZ(pa, pb, m)                                    \
    do { if ((pa)->m > (pb)->m && (pb)->m != 0) { (pa)->m = (pb)->m; } } \
    while (0)


/* IE: implicit, explicit */
#define PATOMIC_CCOPY_IE_OPS(dst, src, i)                                   \
    do {                                                                    \
        /* base */                                                          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.fp_store);                   \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.fp_load);                    \
        /* xchg */                                                          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.xchg_ops.fp_exchange);       \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.xchg_ops.fp_cmpxchg_weak);   \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.xchg_ops.fp_cmpxchg_strong); \
        /* bitwise */                                                       \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.bitwise_ops.fp_test);        \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.bitwise_ops.fp_test_compl);  \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.bitwise_ops.fp_test_set);    \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.bitwise_ops.fp_test_reset);  \
        /* binary */                                                        \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.binary_ops.fp_or);           \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.binary_ops.fp_xor);          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.binary_ops.fp_and);          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.binary_ops.fp_not);          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.binary_ops.fp_fetch_or);     \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.binary_ops.fp_fetch_xor);    \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.binary_ops.fp_fetch_and);    \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.binary_ops.fp_fetch_not);    \
        /* signed */                                                        \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.signed_ops.fp_add);          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.signed_ops.fp_sub);          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.signed_ops.fp_inc);          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.signed_ops.fp_dec);          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.signed_ops.fp_neg);          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.signed_ops.fp_fetch_add);    \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.signed_ops.fp_fetch_sub);    \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.signed_ops.fp_fetch_inc);    \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.signed_ops.fp_fetch_dec);    \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.signed_ops.fp_fetch_neg);    \
        /* unsigned */                                                      \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_add);        \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_sub);        \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_inc);        \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_dec);        \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_neg);        \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_fetch_add);  \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_fetch_sub);  \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_fetch_inc);  \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_fetch_dec);  \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_fetch_neg);  \
    }                                                                       \
    while (0)

#define PATOMIC_COPY_ALIGN(dst, src)                     \
    do {                                                 \
        PATOMIC_SET_MAX(dst, src, align.recommended);    \
        PATOMIC_SET_MAX(dst, src, align.minimum);        \
        PATOMIC_SET_MIN_NZ(dst, src, align.size_within); \
    }                                                    \
    while (0)


void
patomic_combine(
    patomic_t *const priority,
    patomic_t const *const other
)
{
    int i = 0;
    PATOMIC_CCOPY_IE_OPS(priority, other, i);
    if (i != 0) { PATOMIC_COPY_ALIGN(priority, other); }
}

void
patomic_combine_explicit(
    patomic_explicit_t *const priority,
    patomic_explicit_t const *const other
)
{
    int i = 0;
    PATOMIC_CCOPY_IE_OPS(priority, other, i);
    if (i != 0) { PATOMIC_COPY_ALIGN(priority, other); }
}


patomic_t
patomic_create(
    size_t byte_width,
    patomic_memory_order_t order,
    unsigned int opts,
    unsigned int kinds,
    unsigned int ids
)
{
    patomic_t objs[PATOMIC_IMPL_REGISTER_SIZE];
    patomic_t *begin = objs;
    patomic_t *end   = objs;
    size_t i;

    for (i = 0; i < PATOMIC_IMPL_REGISTER_SIZE; ++i)
    {
        if ( (patomic_impl_register[i].id & ids) &&
             (patomic_impl_register[i].kind & kinds))
        {
            *end++ = patomic_impl_register[i].fp_create(
                byte_width,
                order,
                opts
            );
        }
    }

    /* TODO: sort objs by alignment */

    patomic_t ret = patomic_impl_create_null(byte_width, order, opts);
    for (; begin != end; ++begin) { patomic_combine(&ret, begin); }

    return ret;
}

PATOMIC_EXPORT patomic_explicit_t
patomic_create_explicit(
    size_t byte_width,
    unsigned int opts,
    unsigned int kinds,
    unsigned int ids
)
{
    patomic_explicit_t objs[PATOMIC_IMPL_REGISTER_SIZE];
    patomic_explicit_t *begin = objs;
    patomic_explicit_t *end   = objs;
    size_t i;

    for (i = 0; i < PATOMIC_IMPL_REGISTER_SIZE; ++i)
    {
        if ( (patomic_impl_register[i].id & ids) &&
             (patomic_impl_register[i].kind & kinds))
        {
            *end++ = patomic_impl_register[i].fp_create_explicit(
                byte_width,
                opts
            );
        }
    }

    /* TODO: sort objs by alignment */

    patomic_explicit_t ret = patomic_impl_create_explicit_null(byte_width, opts);
    for (; begin != end; ++begin) { patomic_combine_explicit(&ret, begin); }

    return ret;
}

PATOMIC_EXPORT patomic_transaction_t
patomic_create_transaction(
    unsigned int opts,
    unsigned int kinds,
    unsigned int ids
)
{
    patomic_transaction_t ret;
    patomic_impl_t impls[PATOMIC_IMPL_REGISTER_SIZE];
    patomic_impl_t *begin = impls;
    patomic_impl_t *end   = impls;
    size_t i;

    for (i = 0; i < PATOMIC_IMPL_REGISTER_SIZE; ++i)
    {
        if ( (patomic_impl_register[i].id & ids) &&
             (patomic_impl_register[i].kind & kinds))
        {
            *end++ = patomic_impl_register[i];
        }
    }

    /* TODO: sort impls by kind */

    for (; begin != end; ++begin)
    {
        /* any impl supporting any ops should support tbegin */
        /* TODO: do a more thorough check */
        ret = begin->fp_create_transaction(opts);
        if (ret.ops.raw_ops.fp_tbegin != NULL) { return ret; }
    }

    return patomic_impl_create_transaction_null(opts);
}
