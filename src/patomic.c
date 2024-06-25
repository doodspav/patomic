#include <patomic/patomic.h>

#include <patomic/internal/align.h>
#include <patomic/internal/combine.h>

#include <patomic/stdlib/sort.h>

#include "impl/register.h"


static int
compare_implicit_align(
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
compare_explicit_align(
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
    size_t byte_width,
    patomic_memory_order_t order,
    unsigned int opts,
    unsigned int kinds,
    unsigned long ids
)
{
    /* declare variables */
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
            *end++ = patomic_impl_register[i].fp_create(
                byte_width,
                order,
                opts
            );
        }
    }

    /* sort implementations by alignment */
    patomic_array_sort(
        begin,
        i,
        sizeof(patomic_t),
        &compare_implicit_align
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
    size_t byte_width,
    unsigned int opts,
    unsigned int kinds,
    unsigned long ids
)
{
    /* declare variables */
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
            *end++ = patomic_impl_register[i].fp_create_explicit(
                byte_width,
                opts
            );
        }
    }

    /* sort implementations by alignment */
    patomic_array_sort(
        begin,
        i,
        sizeof(patomic_explicit_t),
        &compare_explicit_align
    );

    /* combine implementations */
    ret = patomic_impl_create_explicit_null(byte_width, opts);
    for (; begin != end; ++begin)
    {
        patomic_internal_combine_explicit(&ret, begin);
    }

    return ret;
}
