#include <patomic/patomic.h>

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>

#include "impl/register.h"

#define IMPL_REGISTER_SIZE (                                \
    (sizeof patomic_impl_register) / sizeof(patomic_impl_t) \
)

static const patomic_impl_t *
patomic_find_impl(
        patomic_impl_t const *begin,
        patomic_impl_t const *const end,
        int id
)
{
    while (begin != end)
    {
        if (begin->id == id) { return begin; }
        else { ++begin; }
    }
    return begin;
}

static int
patomic_compare_impl(
    const void *a,
    const void *b
)
{
    const patomic_impl_t *x = a;
    const patomic_impl_t *y = b;
    return x->id < y->id;
}

static int
patomic_copy_impl_array(
        patomic_impl_t const *const begin,
        patomic_impl_t const *const end,
        patomic_impl_t *dst,
        va_list argv,
        int argc,
        int options
)
{
    int i;
    patomic_impl_id_t id;
    patomic_impl_t const *tmp;
    patomic_impl_t const *dst_argv_end;
    patomic_impl_t const *const dst_begin = dst;

    assert(argc >= 0);
    assert(end >= begin);
    assert((size_t) argc <= (size_t) (end - begin));
    assert(IMPL_REGISTER_SIZE == (size_t) (end - begin));

    /* copy over valid argv impls */
    for (i = 0; i < argc; ++i)
    {
        id = va_arg(argv, patomic_impl_id_t);
        tmp = patomic_find_impl(begin, end, (int) id);
        assert(! ((begin != end) && (tmp == end)));
        if (tmp != end) { *dst++ = *tmp; }
    }
    dst_argv_end = dst;
    argc = (int) (dst_argv_end - dst_begin);

    /* sort argv impls if prioritising them */
    dst -= argc;
    if (options & (int) patomic_options_PRIORITISE_ARG_IDS)
    {
        qsort(
            dst,
            (size_t) argc,
            sizeof(patomic_impl_t),
            patomic_compare_impl
       );
    }
    dst += argc;

    /* return if we ignore non argv impls */
    if (options & (int) patomic_options_IGNORE_NON_ARG_IDS)
    {
        return argc;
    }

    /* copy over the rest of the impls */
    for (i = 0; i < (int) IMPL_REGISTER_SIZE; ++i)
    {
        /* check if not already copied */
        if (patomic_find_impl(dst_begin,
                              dst_argv_end,
                              begin[i].id) == dst_argv_end)
        {
            *dst++ = begin[i];
        }
    }
    dst -= IMPL_REGISTER_SIZE;
    assert(dst == dst_begin);

    /* sort impls again (not include argv impls if prioritised) */
    i = (int) IMPL_REGISTER_SIZE;
    if (options & (int) patomic_options_PRIORITISE_ARG_IDS)
    {
        dst += argc;
        i -= argc;
    }
    qsort(
        dst,
        (size_t) i,
        sizeof(patomic_impl_t),
        patomic_compare_impl
    );

    return (int) IMPL_REGISTER_SIZE;
}

#define COPY_IF_NULL(c, a, b, m) if ((a)->m == NULL) { (a)->m = (b)->m; ++(c); }

#define PATOMIC_DEFINE_COMBINE(cmbk, type)                         \
    static void patomic_##cmbk(                                    \
        type *dst,                                                 \
        type *const src                                            \
    )                                                              \
    {                                                              \
        /* counter to see if we actually copied anything */        \
        int i = 0;                                                 \
                                                                   \
        /* base */                                                 \
        COPY_IF_NULL(i, dst, src, ops.fp_store);                   \
        COPY_IF_NULL(i, dst, src, ops.fp_load);                    \
        /* xchg */                                                 \
        COPY_IF_NULL(i, dst, src, ops.xchg_ops.fp_exchange);       \
        COPY_IF_NULL(i, dst, src, ops.xchg_ops.fp_cmpxchg_weak);   \
        COPY_IF_NULL(i, dst, src, ops.xchg_ops.fp_cmpxchg_strong); \
        /* bitwise */                                              \
        COPY_IF_NULL(i, dst, src, ops.bitwise_ops.fp_test);        \
        COPY_IF_NULL(i, dst, src, ops.bitwise_ops.fp_test_comp);   \
        COPY_IF_NULL(i, dst, src, ops.bitwise_ops.fp_test_set);    \
        COPY_IF_NULL(i, dst, src, ops.bitwise_ops.fp_test_reset);  \
        /* binary */                                               \
        COPY_IF_NULL(i, dst, src, ops.binary_ops.fp_or);           \
        COPY_IF_NULL(i, dst, src, ops.binary_ops.fp_xor);          \
        COPY_IF_NULL(i, dst, src, ops.binary_ops.fp_and);          \
        COPY_IF_NULL(i, dst, src, ops.binary_ops.fp_not);          \
        COPY_IF_NULL(i, dst, src, ops.binary_ops.fp_fetch_or);     \
        COPY_IF_NULL(i, dst, src, ops.binary_ops.fp_fetch_xor);    \
        COPY_IF_NULL(i, dst, src, ops.binary_ops.fp_fetch_and);    \
        COPY_IF_NULL(i, dst, src, ops.binary_ops.fp_fetch_not);    \
        /* signed */                                               \
        COPY_IF_NULL(i, dst, src, ops.signed_ops.fp_add);          \
        COPY_IF_NULL(i, dst, src, ops.signed_ops.fp_sub);          \
        COPY_IF_NULL(i, dst, src, ops.signed_ops.fp_inc);          \
        COPY_IF_NULL(i, dst, src, ops.signed_ops.fp_dec);          \
        COPY_IF_NULL(i, dst, src, ops.signed_ops.fp_neg);          \
        COPY_IF_NULL(i, dst, src, ops.signed_ops.fp_fetch_add);    \
        COPY_IF_NULL(i, dst, src, ops.signed_ops.fp_fetch_sub);    \
        COPY_IF_NULL(i, dst, src, ops.signed_ops.fp_fetch_inc);    \
        COPY_IF_NULL(i, dst, src, ops.signed_ops.fp_fetch_dec);    \
        COPY_IF_NULL(i, dst, src, ops.signed_ops.fp_fetch_neg);    \
        /* unsigned */                                             \
        COPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_add);        \
        COPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_sub);        \
        COPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_inc);        \
        COPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_dec);        \
        COPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_neg);        \
        COPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_fetch_add);  \
        COPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_fetch_sub);  \
        COPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_fetch_inc);  \
        COPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_fetch_dec);  \
        COPY_IF_NULL(i, dst, src, ops.unsigned_ops.fp_fetch_neg);  \
                                                                   \
        /* only copy alignment if ops have been copied */          \
        if (i != 0)                                                \
        {                                                          \
            if (dst->align.recommended < src->align.recommended)   \
            {                                                      \
                dst->align.recommended = src->align.recommended;   \
            }                                                      \
            if (dst->align.minimum < src->align.minimum)           \
            {                                                      \
                dst->align.minimum = src->align.minimum;           \
                dst->align.size_within = src->align.size_within;   \
            }                                                      \
        }                                                          \
    }

PATOMIC_DEFINE_COMBINE(combine, patomic_t)
PATOMIC_DEFINE_COMBINE(combine_explicit, patomic_explicit_t)

static const patomic_t patomic_NULL;
static const patomic_explicit_t patomic_explicit_NULL;

#define SHOW(x) x
#define HIDE(x)

#define SHOW_P(x, y) ,y
#define HIDE_P(x, y)

#define PATOMIC_DEFINE_CREATE(vis_p, vis, hexp, eexp)       \
    patomic##hexp##t                                        \
    patomic_creat##eexp(                                    \
        size_t byte_width                                   \
 vis_p(_,patomic_memory_order_t order)                      \
        ,int options                                        \
        ,int impl_id_argc                                   \
        ,...                                                \
    )                                                       \
    {                                                       \
        patomic##hexp##t ret, tmp;                          \
        patomic_impl_t reg_copy[IMPL_REGISTER_SIZE];        \
        va_list impl_id_argv;                               \
        int i, impl_count;                                  \
                                                            \
        va_start(impl_id_argv, impl_id_argc);               \
    vis(assert(patomic_is_valid_order((int) order));)       \
        impl_count = patomic_copy_impl_array(               \
            &patomic_impl_register[0],                      \
            &patomic_impl_register[0] + IMPL_REGISTER_SIZE, \
            reg_copy,                                       \
            impl_id_argv,                                   \
            impl_id_argc,                                   \
            options                                         \
        );                                                  \
        va_end(impl_id_argv);                               \
                                                            \
        ret = patomic##hexp##NULL;                          \
        for (i = 0; i < impl_count; ++i)                    \
        {                                                   \
            assert(reg_copy[i].fp_creat##eexp != NULL);     \
            tmp = reg_copy[i].fp_creat##eexp(               \
                byte_width                                  \
         vis_p(_,order)                                     \
                ,options                                    \
            );                                              \
            patomic_combin##eexp(&ret, &tmp);               \
        }                                                   \
        return ret;                                         \
    }

PATOMIC_DEFINE_CREATE(SHOW_P, SHOW, _, e)
PATOMIC_DEFINE_CREATE(HIDE_P, HIDE, _explicit_, e_explicit)
