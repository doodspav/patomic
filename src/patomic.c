#include <patomic/patomic.h>

#include <patomic/internal/align.h>
#include <patomic/internal/combine.h>
#include <patomic/internal/feature_check.h>

#include <patomic/stdlib/sort.h>

#include "impl/register.h"


static int
compare_implicit(
    const void *const lhs_void,
    const void *const rhs_void
)
{
    /* convert to non-void types */
    const patomic_t lhs = *(const patomic_t *const) lhs_void;
    const patomic_t rhs = *(const patomic_t *const) rhs_void;

    /* defer to internal comparison function */
    return patomic_internal_compare_align(lhs.align, rhs.align);
}


static int
compare_explicit(
    const void *const lhs_void,
    const void *const rhs_void
)
{
    /* convert to non-void types */
    const patomic_explicit_t lhs = *(const patomic_explicit_t *const) lhs_void;
    const patomic_explicit_t rhs = *(const patomic_explicit_t *const) rhs_void;

    /* defer to internal comparison function */
    return patomic_internal_compare_align(lhs.align, rhs.align);
}


patomic_t
patomic_create(
    const size_t byte_width,
    const patomic_memory_order_t order,
    const unsigned int opts,
    const unsigned int kinds,
    const unsigned long ids
)
{
    /* declare variables */
    const unsigned int opcats = ~0u;
    patomic_t ret;
    patomic_t objs[PATOMIC_IMPL_REGISTER_SIZE];
    patomic_t *begin = objs;
    patomic_t *end   = objs;
    size_t i;

    /* fill array with implementations */
    for (i = 0; i < PATOMIC_IMPL_REGISTER_SIZE; ++i)
    {
        if ( ((unsigned long) patomic_impl_register[i].id & ids) &&
             ((unsigned int)  patomic_impl_register[i].kind & kinds))
        {
            /* only add to array if some operation is supported */
            *end = patomic_impl_register[i].fp_create(byte_width, order, opts);
            if (opcats != patomic_internal_feature_check_any(&end->ops, opcats))
            {
                ++end;
            }
        }
    }

    /* sort implementations by alignment */
    patomic_array_sort(
        begin,
        (size_t) (end - begin),
        sizeof(patomic_t),
        &compare_implicit
    );

    /* combine implementations */
    ret = patomic_impl_create_null(byte_width, order, opts);
    for (; begin != end; ++begin)
    {
        patomic_internal_combine(&ret, begin);
    }

    return ret;
}


patomic_explicit_t
patomic_create_explicit(
    const size_t byte_width,
    const unsigned int opts,
    const unsigned int kinds,
    const unsigned long ids
)
{
    /* declare variables */
    const unsigned int opcats = ~0u;
    patomic_explicit_t ret;
    patomic_explicit_t objs[PATOMIC_IMPL_REGISTER_SIZE];
    patomic_explicit_t *begin = objs;
    patomic_explicit_t *end   = objs;
    size_t i;

    /* fill array with implementations */
    for (i = 0; i < PATOMIC_IMPL_REGISTER_SIZE; ++i)
    {
        if ( ((unsigned long) patomic_impl_register[i].id & ids) &&
             ((unsigned int)  patomic_impl_register[i].kind & kinds))
        {
            /* only add to array if some operation is supported */
            *end = patomic_impl_register[i].fp_create_explicit(byte_width, opts);
            if (opcats != patomic_internal_feature_check_any_explicit(&end->ops, opcats))
            {
                ++end;
            }
        }
    }

    /* sort implementations by alignment */
    patomic_array_sort(
        begin,
        (size_t) (end - begin),
        sizeof(patomic_explicit_t),
        &compare_explicit
    );

    /* combine implementations */
    ret = patomic_impl_create_explicit_null(byte_width, opts);
    for (; begin != end; ++begin)
    {
        patomic_internal_combine_explicit(&ret, begin);
    }

    return ret;
}


patomic_transaction_t
patomic_create_transaction(
    const unsigned int opts,
    const unsigned int kinds,
    const unsigned long ids
)
{
    /* declare variables */
    const unsigned int opcats = ~0u;
    patomic_kind_t last_kind = patomic_kind_UNKN;
    patomic_transaction_t ret;
    patomic_transaction_t objs[PATOMIC_IMPL_REGISTER_SIZE];
    patomic_transaction_t *begin = objs;
    patomic_transaction_t *end   = objs;
    size_t i;

    /* fill array with implementations */
    for (i = 0; i < PATOMIC_IMPL_REGISTER_SIZE; ++i)
    {
        if( ((unsigned long) patomic_impl_register[i].id & ids) &&
            ((unsigned int)  patomic_impl_register[i].kind & kinds))
        {
            /* only add to array if some operation is supported */
            ret = patomic_impl_register[i].fp_create_transaction(opts);
            if(opcats != patomic_internal_feature_check_any_transaction(&ret.ops, opcats))
            {
                /* ignore previous implementations if current one has a better kind */
                if (patomic_impl_register[i].kind > last_kind)
                {
                    end = objs;
                }
                last_kind = patomic_impl_register[i].kind;
                *end++ = ret;
            }
        }
    }

    /* get first implementation available */
    if (begin != end)
    {
        return objs[0];
    }
    else
    {
        return patomic_impl_create_transaction_null(opts);
    }
}
