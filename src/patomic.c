#include <patomic/patomic.h>

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>

#include <patomic/types/ids.h>
#include <patomic/types/ops.h>
#include <patomic/types/options.h>
#include <patomic/types/memory_order.h>

#include "impl/register.h"

static const patomic_ops_t patomic_ops_NULL;
static const patomic_ops_explicit_t patomic_ops_explicit_NULL;

#define IMPL_REGISTER_SIZE (                                         \
    (sizeof patomic_impl_register) / sizeof(patomic_impl_register_t) \
)

#define COPY_IF_NULL(a, b, m) if ((a)->m == NULL) (a)->m = (b)->m

#define PATOMIC_DEFINE_COMBINE_OPS(opsk)                    \
    static void patomic_combine_##opsk(                     \
        patomic_##opsk##_t *dst,                            \
        const patomic_##opsk##_t *src                       \
    )                                                       \
    {                                                       \
        /* base */                                          \
        COPY_IF_NULL(dst, src, fp_store);                   \
        COPY_IF_NULL(dst, src, fp_load);                    \
        /* xchg */                                          \
        COPY_IF_NULL(dst, src, xchg_ops.fp_exchange);       \
        COPY_IF_NULL(dst, src, xchg_ops.fp_cmpxchg_weak);   \
        COPY_IF_NULL(dst, src, xchg_ops.fp_cmpxchg_strong); \
        /* bitwise */                                       \
        COPY_IF_NULL(dst, src, bitwise_ops.fp_test);        \
        COPY_IF_NULL(dst, src, bitwise_ops.fp_test_comp);   \
        COPY_IF_NULL(dst, src, bitwise_ops.fp_test_set);    \
        COPY_IF_NULL(dst, src, bitwise_ops.fp_test_reset);  \
        /* binary */                                        \
        COPY_IF_NULL(dst, src, binary_ops.fp_or);           \
        COPY_IF_NULL(dst, src, binary_ops.fp_xor);          \
        COPY_IF_NULL(dst, src, binary_ops.fp_and);          \
        COPY_IF_NULL(dst, src, binary_ops.fp_not);          \
        COPY_IF_NULL(dst, src, binary_ops.fp_fetch_or);     \
        COPY_IF_NULL(dst, src, binary_ops.fp_fetch_xor);    \
        COPY_IF_NULL(dst, src, binary_ops.fp_fetch_and);    \
        COPY_IF_NULL(dst, src, binary_ops.fp_fetch_not);    \
        /* signed */                                        \
        COPY_IF_NULL(dst, src, signed_ops.fp_add);          \
        COPY_IF_NULL(dst, src, signed_ops.fp_sub);          \
        COPY_IF_NULL(dst, src, signed_ops.fp_inc);          \
        COPY_IF_NULL(dst, src, signed_ops.fp_dec);          \
        COPY_IF_NULL(dst, src, signed_ops.fp_neg);          \
        COPY_IF_NULL(dst, src, signed_ops.fp_fetch_add);    \
        COPY_IF_NULL(dst, src, signed_ops.fp_fetch_sub);    \
        COPY_IF_NULL(dst, src, signed_ops.fp_fetch_inc);    \
        COPY_IF_NULL(dst, src, signed_ops.fp_fetch_dec);    \
        COPY_IF_NULL(dst, src, signed_ops.fp_fetch_neg);    \
        /* unsigned */                                      \
        COPY_IF_NULL(dst, src, unsigned_ops.fp_add);        \
        COPY_IF_NULL(dst, src, unsigned_ops.fp_sub);        \
        COPY_IF_NULL(dst, src, unsigned_ops.fp_inc);        \
        COPY_IF_NULL(dst, src, unsigned_ops.fp_dec);        \
        COPY_IF_NULL(dst, src, unsigned_ops.fp_neg);        \
        COPY_IF_NULL(dst, src, unsigned_ops.fp_fetch_add);  \
        COPY_IF_NULL(dst, src, unsigned_ops.fp_fetch_sub);  \
        COPY_IF_NULL(dst, src, unsigned_ops.fp_fetch_inc);  \
        COPY_IF_NULL(dst, src, unsigned_ops.fp_fetch_dec);  \
        COPY_IF_NULL(dst, src, unsigned_ops.fp_fetch_neg);  \
    }

PATOMIC_DEFINE_COMBINE_OPS(ops)
PATOMIC_DEFINE_COMBINE_OPS(ops_explicit)


static const patomic_impl_register_t*
patomic_find_impl(
    patomic_impl_register_t const *begin,
    patomic_impl_register_t const *const end,
    int impl_id
)
{
    while (begin != end)
    {
        if (begin->id == impl_id) { return begin; }
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
    const patomic_impl_register_t* x = a;
    const patomic_impl_register_t* y = b;
    return x->id < y->id;
}

static int
patomic_copy_sort_impl_register(
    patomic_impl_register_t const *const reg_begin,
    patomic_impl_register_t const *const reg_end,
    patomic_impl_register_t *reg_dst,
    va_list argv,
    int argc,
    int options
)
{
    int i;
    patomic_impl_id_t id;
    patomic_impl_register_t const *const reg_mid = reg_dst + argc;
    patomic_impl_register_t const *const reg_dst_og = reg_dst;

    assert(argc >= 0);
    assert(reg_end >= reg_begin);
    assert((size_t) argc <= (size_t) (reg_end - reg_begin));
    assert(IMPL_REGISTER_SIZE == (size_t) (reg_end - reg_begin));

    /* copy over arg impl ids */
    for (i = 0; i < argc; ++i)
    {
        id = va_arg(argv, patomic_impl_id_t);
        *reg_dst++ = *patomic_find_impl(reg_begin, reg_end, (int) id);
    }

    /* sort arg ids if prioritising them */
    reg_dst -= argc;
    if (options & (int) patomic_options_PRIORITISE_ARG_IDS)
    {
        qsort(
            reg_dst,
            (size_t) argc,
            sizeof(patomic_impl_register_t),
            patomic_compare_impl
        );
    }
    reg_dst += argc;

    /* return if we ignore the rest */
    if (options & (int) patomic_options_IGNORE_NON_ARG_IDS)
    {
        return argc;
    }

    /* copy over the rest of the impls */
    for (i = 0; i < (int) IMPL_REGISTER_SIZE; ++i)
    {
        /* not already copied */
        if (patomic_find_impl(
                reg_dst_og,
                reg_mid,
                reg_begin[i].id
            ) == reg_mid
        )
        {
            *reg_dst++ = reg_begin[i];
        }
    }

    reg_dst -= IMPL_REGISTER_SIZE;
    assert(reg_dst == reg_dst_og);

    /* sort ids again (not including arg ids if prioritised) */
    i = (int) IMPL_REGISTER_SIZE;
    if (options & (int) patomic_options_PRIORITISE_ARG_IDS)
    {
        reg_dst += argc;
        i -= argc;
    }
    qsort(
        reg_dst,
        (size_t) i,
        sizeof(patomic_impl_register_t),
        patomic_compare_impl
    );

    return (int) IMPL_REGISTER_SIZE;
}

#define SHOW(x) x
#define HIDE(x)

#define SHOW_P(x, y) ,y
#define HIDE_P(x, y)

#define PATOMIC_DEFINE_CREATE_OPS(vis_p, vis, opsk)           \
    patomic_##opsk##_t                                        \
    patomic_create_##opsk(                                    \
        size_t byte_width                                     \
 vis_p(_,patomic_memory_order_t order)                        \
        ,int options                                          \
        ,int impl_id_argc                                     \
        ,...                                                  \
    )                                                         \
    {                                                         \
        patomic_##opsk##_t ret, tmp;                          \
        patomic_impl_register_t reg_copy[IMPL_REGISTER_SIZE]; \
        va_list impl_id_argv;                                 \
        int i, impl_count;                                    \
                                                              \
        vis(assert(patomic_is_valid_order((int) order));)     \
        assert(impl_id_argc >= 0);                            \
        assert((size_t) impl_id_argc <= IMPL_REGISTER_SIZE);  \
                                                              \
        va_start(impl_id_argv, impl_id_argc);                 \
        impl_count = patomic_copy_sort_impl_register(         \
            &patomic_impl_register[0],                        \
            &patomic_impl_register[0] + IMPL_REGISTER_SIZE,   \
            reg_copy,                                         \
            impl_id_argv,                                     \
            impl_id_argc,                                     \
            options                                           \
        );                                                    \
        va_end(impl_id_argv);                                 \
                                                              \
        ret = patomic_##opsk##_NULL;                          \
        for (i = 0; i < impl_count; ++i)                      \
        {                                                     \
            assert(reg_copy[i].fp_create_##opsk != NULL);     \
            tmp = reg_copy[i].fp_create_##opsk(               \
                byte_width                                    \
         vis_p(_,order)                                       \
                ,options                                      \
            );                                                \
            patomic_combine_##opsk(&ret, &tmp);               \
        }                                                     \
        return ret;                                           \
    }

PATOMIC_DEFINE_CREATE_OPS(SHOW_P, SHOW, ops)
PATOMIC_DEFINE_CREATE_OPS(HIDE_P, HIDE, ops_explicit)
