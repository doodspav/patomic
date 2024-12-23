#include <patomic/patomic.h>

#include <patomic/internal/align.h>
#include <patomic/internal/combine.h>
#include <patomic/internal/feature_check.h>

#include <patomic/stdlib/assert.h>
#include <patomic/stdlib/math.h>
#include <patomic/stdlib/sort.h>

#include "impl/register.h"


#define assert_valid_alignment(align)                                           \
    patomic_assert_always(patomic_unsigned_is_pow2(align.recommended));         \
    patomic_assert_always(patomic_unsigned_is_pow2(align.minimum));             \
    patomic_assert_always(patomic_unsigned_is_pow2_or_zero(align.size_within)); \
    patomic_assert_always(align.recommended >= align.minimum)


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
    const unsigned int options,
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

    /* check memory order is valid */
    patomic_assert_always(PATOMIC_IS_VALID_ORDER(order));

    /* fill array with implementations */
    for (i = 0; i < PATOMIC_IMPL_REGISTER_SIZE; ++i)
    {
        if ( ((unsigned long) patomic_impl_register[i].id & ids) &&
             ((unsigned int)  patomic_impl_register[i].kind & kinds))
        {
            /* create implementation */
            *end = patomic_impl_register[i].fp_create(byte_width, order, options);

            /* check that alignment values are valid */
            assert_valid_alignment(end->align);

            /* only add to array if some operation is supported */
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
    ret = patomic_impl_create_null(byte_width, order, options);
    for (; begin != end; ++begin)
    {
        patomic_internal_combine(&ret, begin);
    }

    return ret;
}


patomic_explicit_t
patomic_create_explicit(
    const size_t byte_width,
    const unsigned int options,
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
            /* create implementation */
            *end = patomic_impl_register[i].fp_create_explicit(byte_width, options);

            /* check that alignment values are valid */
            assert_valid_alignment(end->align);

            /* only add to array if some operation is supported */
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
    ret = patomic_impl_create_explicit_null(byte_width, options);
    for (; begin != end; ++begin)
    {
        patomic_internal_combine_explicit(&ret, begin);
    }

    return ret;
}


patomic_transaction_t
patomic_create_transaction(
    const unsigned int options,
    const unsigned int kinds,
    const unsigned long ids
)
{
    /* declare variables */
    unsigned int opcats;
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
            ret = patomic_impl_register[i].fp_create_transaction(options);
            opcats = patomic_opcats_TRANSACTION;
            if (opcats != patomic_internal_feature_check_any_transaction(&ret.ops, opcats))
            {
                /* all flag operations must be supported if any other operation is supported */
                /* if no other operation is supported, no flag operation must be supported */
                opcats &= ~((unsigned int) patomic_opcat_TFLAG);
                if (opcats != patomic_internal_feature_check_any_transaction(&ret.ops, opcats))
                {
                    /* other operation supported -> full flag support */
                    patomic_assert_always(patomic_internal_feature_check_leaf_transaction(
                        &ret.ops, patomic_opcat_TFLAG, patomic_opkinds_TFLAG
                    ) == 0ul);
                }
                else
                {
                    /* no other operation supported -> no flag support */
                    opcats = patomic_opkinds_TFLAG;
                    patomic_assert_always(patomic_internal_feature_check_leaf_transaction(
                        &ret.ops, patomic_opcat_TFLAG, patomic_opkinds_TFLAG
                    ) == opcats);
                }

                /* tbegin and tcommit must be supported if any raw operation is supported */
                opcats = patomic_opcat_TRAW;
                if (opcats != patomic_internal_feature_check_any_transaction(&ret.ops, opcats))
                {
                    patomic_assert_always(ret.ops.raw_ops.fp_tbegin != NULL);
                    patomic_assert_always(ret.ops.raw_ops.fp_tcommit != NULL);
                }

                /* if tabort_single is supported, then tabort_all must be supported too */
                if (ret.ops.raw_ops.fp_tabort_single != NULL)
                {
                    patomic_assert_always(ret.ops.raw_ops.fp_tabort_all != NULL);
                }

                /* if tdepth is supported, ttest must have the same value */
                if (ret.ops.raw_ops.fp_tdepth != NULL)
                {
                    patomic_assert_always(
                        ret.ops.raw_ops.fp_ttest == ret.ops.raw_ops.fp_tdepth
                    );
                }

                /* not possible for cmpxchg_strong to be implemented */
                patomic_assert_always(ret.ops.xchg_ops.fp_cmpxchg_strong == NULL);

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
        return patomic_impl_create_transaction_null(options);
    }
}
