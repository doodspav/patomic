#include <patomic/patomic.h>

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>

#include "impl/register.h"


static const patomic_impl_t *
patomic_find_impl(
    patomic_impl_t const *begin,
    patomic_impl_t const *const end,
    patomic_impl_id_t id
)
{
    assert(begin != NULL);
    assert(end != NULL);
    assert(begin <= end);

    while (begin != end)
    {
        if (begin->id == id) { return begin; }
        else { ++begin; }
    }
    return begin;
}

/* very simple bubble-sort implementation to replace qsort */
static void
patomic_sort_impls(
    patomic_impl_t *const begin,
    patomic_impl_t const *const end
)
{
    ptrdiff_t size, outer_idx, inner_idx;
    int is_swapped, cmp_gt;
    patomic_impl_t temp;

    assert(begin != NULL);
    assert(end != NULL);
    assert(begin <= end);

    size = end - begin;
    if (size == 0) { return; }

    /* i = [0..n-1] */
    for (outer_idx = 0; outer_idx < (size - 1); ++outer_idx)
    {
        is_swapped = 0;
        /* j = [0..n-i-1] */
        for (inner_idx = 0; inner_idx < (size - outer_idx - 1); ++inner_idx)
        {
            assert(inner_idx < (size - 1));
            cmp_gt = begin[inner_idx].id > begin[inner_idx + 1].id;
            if (cmp_gt)
            {
                /* swap */
                temp = begin[inner_idx];
                begin[inner_idx] = begin[inner_idx + 1];
                begin[inner_idx + 1] = temp;
                is_swapped = 1;
            }
        }
        if (!is_swapped) { break; }
    }
}

/* copies contents of patomic_impl_register into given array */
/* array MUST have size of PATOMIC_IMPL_REGISTER_SIZE */
/* order of contents is determined by argv, argc, and options */
/* return: number of impls copied over */
static int
patomic_fill_impl_array(
    patomic_impl_t *const begin,
    patomic_impl_t const *const end,
    va_list argv,
    int argc,
    int options
)
{
    int i;
    patomic_impl_id_t id;
    patomic_impl_t const *tmp;
    patomic_impl_t const *tmp2;

    patomic_impl_t *it = begin;
    patomic_impl_t const *const impls_begin = &patomic_impl_register[0];
    patomic_impl_t const *const impls_end = \
        impls_begin + PATOMIC_IMPL_REGISTER_SIZE;

    assert(begin != NULL);
    assert(end != NULL);
    assert(begin <= end);
    assert(argc >= 0);
    assert(PATOMIC_IMPL_REGISTER_SIZE == (size_t) (end - begin));
    assert(PATOMIC_IMPL_REGISTER_SIZE == (size_t) (impls_end - impls_begin));

    /* copy over impls found using impl_id values from argv */
    /* ignore (or assert on) duplicate and invalid impl_id values */
    for (i = 0; i < argc; ++i)
    {
        /* get impl from impl_id */
        id = va_arg(argv, patomic_impl_id_t);
        tmp = patomic_find_impl(impls_begin, impls_end, id);
        /* check valid id (i.e. impl with that id exists) */
        assert(tmp != impls_end);
        if (tmp == impls_end) { continue; }
        /* check duplicate id (i.e. did we already copy it over) */
        tmp2 = patomic_find_impl(begin, it, id);
        assert(tmp2 == it);
        if (tmp2 != it) { continue; }
        /* copy over unique valid impl */
        *it++ = *tmp;
    }

    /* keep track of how many argv impls we actually copied over */
    argc = (int) (it - begin);

    /* sort argv impls copied over if prioritising them */
    if (options & (int) patomic_options_PRIORITISE_ARG_IDS)
    {
        patomic_sort_impls(begin, it);
    }

    /* return if we ignore non argv impls */
    if (options & (int) patomic_options_IGNORE_NON_ARG_IDS)
    {
        return argc;
    }

    /* copy the rest of the impls */
    for (tmp = impls_begin; tmp < impls_end; ++tmp)
    {
        /* check it's not already copied over */
        tmp2 = patomic_find_impl(begin, it, tmp->id);
        if (tmp2 == it)
        {
            *it++ = *tmp;
        }
    }
    assert(it == end);

    /* sort impls again (not including argv impls if they were prioritised) */
    if (options & (int) patomic_options_PRIORITISE_ARG_IDS)
    {
        patomic_sort_impls(begin + argc, end);
    }
    else { patomic_sort_impls(begin, end); }

    return (int) PATOMIC_IMPL_REGISTER_SIZE;
}

static int
patomic_is_pow2(
    size_t val
)
{
    if (val == 0) { return 0; }
    else { return (val & (val - 1)) == 0; }
}


#define PATOMIC_COPY_IF_NULL(pa, pb, m) \
    do { if ((pa)->m == NULL) { (pa)->m = (pb)->m; } } while (0)

#define PATOMIC_CCOPY_IF_NULL(i, pa, pb, m) \
    do { if ((pa)->m == NULL) { (pa)->m = (pb)->m; ++(i); } } while (0)

#define PATOMIC_SET_MAX(pa, pb, m) \
    do { if ((pa)->m < (pb)->m) { (pa)->m = (pb)->m; } } while (0)

#define PATOMIC_SET_MIN(pa, pb, m) \
    do { if ((pa)->m > (pb)->m) { (pa)->m = (pb)->m; } } while (0)

#define PATOMIC_SET_MIN_NZ(pa, pb, m) \
    do { if ((pa)->m > (pb)->m && (pb)->m != 0) {(pa)->m = (pb)->m; } } while (0)


/* IET: implicit, explicit, transaction */
#define PATOMIC_COPY_COUNT_IET_OPS(dst, src, i)                             \
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

/* T: transaction */
#define PATOMIC_COPY_COUNT_T_OPS(dst, src, i)                                  \
    do {                                                                       \
        /* special */                                                          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.special_ops.fp_double_cmpxchg_weak); \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.special_ops.fp_double_cmpxchg_strong); \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.special_ops.fp_n_cmpxchg_weak); \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.special_ops.fp_n_cmpxchg_strong); \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.special_ops.fp_generic);        \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.special_ops.fp_generic_wfb);    \
        /* flag */                                                             \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.flag_ops.fp_test);              \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.flag_ops.fp_test_set);          \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.flag_ops.fp_clear);             \
        /* raw */                                                              \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.raw_ops.fp_tbegin);             \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.raw_ops.fp_tabort);             \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.raw_ops.fp_tcommit);            \
        PATOMIC_CCOPY_IF_NULL(i, dst, src, ops.raw_ops.fp_ttest);              \
    }                                                                          \
    while (0)

/* T: transaction */
#define PATOMIC_COPY_T_EXTRAS(dst, src)                                \
    do {                                                               \
        /* recommended */                                              \
        PATOMIC_SET_MIN_NZ(dst, src, recommended.max_rmw_memory);      \
        PATOMIC_SET_MIN_NZ(dst, src, recommended.max_load_memory);     \
        PATOMIC_SET_MAX(dst, src, recommended.min_rmw_attempts);       \
        PATOMIC_SET_MAX(dst, src, recommended.min_load_attempts);      \
        /* sstring */                                                  \
        /* transaction implementations are all hardware based */       \
        /* two implementations on the same hardware is unlikely */     \
        assert(!((dst)->sstring.required && (src)->sstring.required)); \
        if ((src)->sstring.required)                                   \
        {                                                              \
            PATOMIC_COPY_IF_NULL(dst, src, sstring.fp_memcpy);         \
            PATOMIC_COPY_IF_NULL(dst, src, sstring.fp_memmove);        \
            PATOMIC_COPY_IF_NULL(dst, src, sstring.fp_memset);         \
            PATOMIC_COPY_IF_NULL(dst, src, sstring.fp_memcmp);         \
        }                                                              \
    }                                                                  \
    while (0)

#define PATOMIC_COPY_ALIGN(dst, src)                              \
    do {                                                          \
        PATOMIC_SET_MAX(dst, src, align.recommended);             \
        PATOMIC_SET_MIN(dst, src, align.minimum);                 \
        PATOMIC_SET_MIN(dst, src, align.size_within);             \
    }                                                             \
    while (0)


/* T: transaction */
#define PATOMIC_T_OPS_CHECKS(obj)                                      \
    do {                                                               \
        /* either tbegin/commit/test are all available, or none are */ \
        /* tabort is exempt from this rule but: */                     \
        /* tabort cannot be present without the rest */                \
        unsigned int flags = patomic_opkind_TBEGIN  |                  \
                             patomic_opkind_TCOMMIT |                  \
                             patomic_opkind_TTEST;                     \
        unsigned int result = patomic_feature_check_leaf_transaction(  \
            &((obj)->ops),                                             \
            patomic_opcat_RAW,                                         \
            flags | patomic_opkind_TABORT                              \
        );                                                             \
        assert((result == patomic_opkind_TABORT)  /* all but TABORT */ \
               || (result == 0)  /* all (including TABORT) */          \
               || (result == flags));  /* none */                      \
        /* set flags to all transaction opcats except RAW */           \
        /* since we just check RAW ops above */                        \
        flags = patomic_opcat_TRANSACTION;                             \
        flags &= ~((unsigned int) patomic_opcat_RAW);                  \
        /* if they're available, all other ops MUST be available */    \
        if (result == 0)                                               \
        {                                                              \
            assert(patomic_feature_check_all_transaction(              \
                &((obj)->ops),                                         \
                flags                                                  \
            ));                                                        \
        }                                                              \
        /* otherwise, all other ops MUST NOT be available */           \
        else                                                           \
        {                                                              \
            assert(flags == patomic_feature_check_any_transaction(     \
                &((obj)->ops),                                         \
                flags                                                  \
            ));                                                        \
        }                                                              \
    }                                                                  \
    while (0)

/* T: transaction */
#define PATOMIC_T_EXTRAS_CHECKS(obj)                                       \
    do {                                                                   \
        /* if we have ops, no extras can be 0 or NULL */                   \
        /* sstring.required may or may not be set */                       \
        /* otherwise, everything must be 0 or NULL (including required) */ \
        int ops_set = (obj)->ops.raw_ops.fp_tbegin != NULL;                \
        /* recommended */                                                  \
        assert(ops_set == ((obj)->recommended.max_rmw_memory != 0));       \
        assert(ops_set == ((obj)->recommended.max_load_memory != 0));      \
        assert(ops_set == ((obj)->recommended.min_rmw_attempts != 0));     \
        assert(ops_set == ((obj)->recommended.min_load_attempts != 0));    \
        /* sstring */                                                      \
        assert(ops_set == ((obj)->sstring.fp_memcpy != NULL));             \
        assert(ops_set == ((obj)->sstring.fp_memmove != NULL));            \
        assert(ops_set == ((obj)->sstring.fp_memset != NULL));             \
        assert(ops_set == ((obj)->sstring.fp_memcmp != NULL));             \
        /* special case for sstring.required */                            \
        if (!ops_set) { assert((obj)->sstring.required == 0); }            \
    }                                                                      \
    while (0)

#define PATOMIC_ALIGN_CHECKS(obj)                                 \
    do {                                                          \
        assert(patomic_is_pow2((obj)->align.recommended));        \
        assert(patomic_is_pow2((obj)->align.minimum));            \
        assert((obj)->align.recommended >= (obj)->align.minimum); \
    }                                                             \
    while (0)


static void
patomic_combine(
    patomic_t *const dst,
    patomic_t const *const src
)
{
    int i = 0;

    PATOMIC_ALIGN_CHECKS(src);

    PATOMIC_COPY_COUNT_IET_OPS(dst, src, i);

    if (i != 0) { PATOMIC_COPY_ALIGN(dst, src); }
}

static void
patomic_combine_explicit(
    patomic_explicit_t *const dst,
    patomic_explicit_t const *const src
)
{
    int i = 0;

    PATOMIC_ALIGN_CHECKS(src);

    PATOMIC_COPY_COUNT_IET_OPS(dst, src, i);

    if (i != 0) { PATOMIC_COPY_ALIGN(dst, src); }
}

static void
patomic_combine_transaction(
    patomic_transaction_t *const dst,
    patomic_transaction_t const *const src
)
{
    int i = 0;

    PATOMIC_T_OPS_CHECKS(dst);
    PATOMIC_T_EXTRAS_CHECKS(dst);
    PATOMIC_ALIGN_CHECKS(dst);

    PATOMIC_COPY_COUNT_IET_OPS(dst, src, i);
    PATOMIC_COPY_COUNT_T_OPS(dst, src, i);

    if (i != 0)
    {
        PATOMIC_COPY_T_EXTRAS(dst, src);
        PATOMIC_COPY_ALIGN(dst, src);
    }
}


#define SHOW(x) x
#define HIDE(x)

#define SHOW_P(x, y) x,
#define HIDE_P(x, y)

/* fn: e, e_explicit, e_transaction */
/* type: _, _explicit_, _transaction_ */
#define PATOMIC_DEFINE_CREATE(fn, type, vis_wp, vis_op, vis_o)        \
    patomic##type##t                                                  \
    patomic_creat##fn(                                                \
 vis_wp(size_t byte_width, _)                                         \
 vis_op(patomic_memory_order_t order, _)                              \
        int options,                                                  \
        int impl_id_argc,                                             \
        ...                                                           \
    )                                                                 \
    {                                                                 \
        static const patomic##type##t zeroed;                         \
                                                                      \
        patomic##type##t ret, tmp;                                    \
        patomic_impl_t reg_copy[PATOMIC_IMPL_REGISTER_SIZE];          \
        va_list impl_id_argv;                                         \
        int i, impl_count;                                            \
                                                                      \
        va_start(impl_id_argv, impl_id_argc);                         \
  vis_o(assert(patomic_is_valid_order((int) order));)                 \
        impl_count = patomic_fill_impl_array(                         \
            &reg_copy[0],                                             \
            &reg_copy[0] + PATOMIC_IMPL_REGISTER_SIZE,                \
            impl_id_argv,                                             \
            impl_id_argc,                                             \
            options                                                   \
        );                                                            \
        va_end(impl_id_argv);                                         \
                                                                      \
        ret = zeroed;                                                 \
        ret.align.recommended = 1;                                    \
        ret.align.minimum = 1;                                        \
                                                                      \
        for (i = 0; i < impl_count; ++i)                              \
        {                                                             \
            assert(reg_copy[i].fp_creat##fn != NULL);                 \
            tmp = reg_copy[i].fp_creat##fn(                           \
         vis_wp(byte_width,_)                                         \
         vis_op(order,_)                                              \
                options                                               \
            );                                                        \
            patomic_combin##fn(&ret, &tmp);                           \
        }                                                             \
                                                                      \
        return ret;                                                   \
    }

PATOMIC_DEFINE_CREATE(e, _, SHOW_P, SHOW_P, SHOW)
PATOMIC_DEFINE_CREATE(e_explicit, _explicit_, SHOW_P, HIDE_P, HIDE)
PATOMIC_DEFINE_CREATE(e_transaction, _transaction_, HIDE_P, HIDE_P, HIDE)


#define PATOMIC_DEFINE_NONNULL_COUNT(cntk, type)            \
    int                                                     \
    patomic_nonnull_ops_##cntk(                             \
        type const *const p                                 \
    )                                                       \
    {                                                       \
        int i = 0;                                          \
                                                            \
        /* base */                                          \
        if (p->fp_store != NULL) { ++i; }                   \
        if (p->fp_load != NULL) { ++i; }                    \
        /* xchg */                                          \
        if (p->xchg_ops.fp_exchange != NULL) { ++i; }       \
        if (p->xchg_ops.fp_cmpxchg_weak != NULL) { ++i; }   \
        if (p->xchg_ops.fp_cmpxchg_strong != NULL) { ++i; } \
        /* bitwise */                                       \
        if (p->bitwise_ops.fp_test != NULL) { ++i; }        \
        if (p->bitwise_ops.fp_test_compl != NULL) { ++i; }  \
        if (p->bitwise_ops.fp_test_set != NULL) { ++i; }    \
        if (p->bitwise_ops.fp_test_reset != NULL) { ++i; }  \
        /* binary */                                        \
        if (p->binary_ops.fp_or != NULL) { ++i; }           \
        if (p->binary_ops.fp_xor != NULL) { ++i; }          \
        if (p->binary_ops.fp_and != NULL) { ++i; }          \
        if (p->binary_ops.fp_not != NULL) { ++i; }          \
        if (p->binary_ops.fp_fetch_or != NULL) { ++i; }     \
        if (p->binary_ops.fp_fetch_xor != NULL) { ++i; }    \
        if (p->binary_ops.fp_fetch_and != NULL) { ++i; }    \
        if (p->binary_ops.fp_fetch_not != NULL) { ++i; }    \
        /* signed */                                        \
        if (p->signed_ops.fp_add != NULL) { ++i; }          \
        if (p->signed_ops.fp_sub != NULL) { ++i; }          \
        if (p->signed_ops.fp_inc != NULL) { ++i; }          \
        if (p->signed_ops.fp_dec != NULL) { ++i; }          \
        if (p->signed_ops.fp_neg != NULL) { ++i; }          \
        if (p->signed_ops.fp_fetch_add != NULL) { ++i; }    \
        if (p->signed_ops.fp_fetch_sub != NULL) { ++i; }    \
        if (p->signed_ops.fp_fetch_inc != NULL) { ++i; }    \
        if (p->signed_ops.fp_fetch_dec != NULL) { ++i; }    \
        if (p->signed_ops.fp_fetch_neg != NULL) { ++i; }    \
        /* unsigned */                                      \
        if (p->unsigned_ops.fp_add != NULL) { ++i; }        \
        if (p->unsigned_ops.fp_sub != NULL) { ++i; }        \
        if (p->unsigned_ops.fp_inc != NULL) { ++i; }        \
        if (p->unsigned_ops.fp_dec != NULL) { ++i; }        \
        if (p->unsigned_ops.fp_neg != NULL) { ++i; }        \
        if (p->unsigned_ops.fp_fetch_add != NULL) { ++i; }  \
        if (p->unsigned_ops.fp_fetch_sub != NULL) { ++i; }  \
        if (p->unsigned_ops.fp_fetch_inc != NULL) { ++i; }  \
        if (p->unsigned_ops.fp_fetch_dec != NULL) { ++i; }  \
        if (p->unsigned_ops.fp_fetch_neg != NULL) { ++i; }  \
                                                            \
        return i;                                           \
    }

PATOMIC_DEFINE_NONNULL_COUNT(count, patomic_ops_t)
PATOMIC_DEFINE_NONNULL_COUNT(count_explicit, patomic_ops_explicit_t)
