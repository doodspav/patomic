#include <patomic/api/combine.h>
#include <patomic/helper/combine_internal.h>


#define PATOMIC_SET_MAX(ops, other_ops, member) \
    do {                                        \
        if (ops->member < other_ops->member)    \
        {                                       \
            ops->member = other_ops->member;    \
        }                                       \
    }                                           \
    while (0)


#define PATOMIC_SET_MIN_NZ(ops, other_ops, member)                   \
    do {                                                             \
        if (other_ops->member != 0)                                  \
        {                                                            \
            if (ops->member == 0 || ops->member > other_ops->member) \
            {                                                        \
                ops->member = other_ops->member;                     \
            }                                                        \
        }                                                            \
    }                                                                \
    while (0)


#define PATOMIC_COMBINE_ALIGN(core, other_core)                  \
    do {                                                         \
        PATOMIC_SET_MAX(core, other_core, align.recommended);    \
        PATOMIC_SET_MAX(core, other_core, align.minimum);        \
        PATOMIC_SET_MIN_NZ(core, other_core, align.size_within); \
    }                                                            \
    while (0)


#define PATOMIC_COPY_IF_NULL(i, core, other_core, member)       \
    do {                                                        \
        if (core->member == NULL && other_core->member != NULL) \
        {                                                       \
            core->member = other_core->member;                  \
            ++i;                                                \
        }                                                       \
    }                                                           \
    while (0)


#define PATOMIC_COMBINE_OPS(i, core, other_core)                                    \
    do {                                                                            \
        /* base */                                                                  \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.fp_store);                    \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.fp_load);                     \
                                                                                    \
        /* xchg */                                                                  \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.xchg_ops.fp_exchange);        \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.xchg_ops.fp_cmpxchg_weak);    \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.xchg_ops.fp_cmpxchg_strong);  \
                                                                                    \
        /* bitwise */                                                               \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.bitwise_ops.fp_test);         \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.bitwise_ops.fp_test_compl);   \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.bitwise_ops.fp_test_set);     \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.bitwise_ops.fp_test_reset);   \
                                                                                    \
        /* binary - void */                                                         \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.binary_ops.fp_or);            \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.binary_ops.fp_xor);           \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.binary_ops.fp_and);           \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.binary_ops.fp_not);           \
                                                                                    \
        /* binary - fetch */                                                        \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.binary_ops.fp_fetch_or);      \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.binary_ops.fp_fetch_xor);     \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.binary_ops.fp_fetch_and);     \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.binary_ops.fp_fetch_not);     \
                                                                                    \
        /* arithmetic - void */                                                     \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.arithmetic_ops.fp_add);       \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.arithmetic_ops.fp_sub);       \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.arithmetic_ops.fp_inc);       \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.arithmetic_ops.fp_dec);       \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.arithmetic_ops.fp_neg);       \
                                                                                    \
        /* arithmetic - fetch */                                                    \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.arithmetic_ops.fp_fetch_add); \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.arithmetic_ops.fp_fetch_sub); \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.arithmetic_ops.fp_fetch_inc); \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.arithmetic_ops.fp_fetch_dec); \
        PATOMIC_COPY_IF_NULL(i, core, other_core, ops.arithmetic_ops.fp_fetch_neg); \
    }                                                                               \
    while (0)


void
patomic_internal_combine(
    patomic_t *const priority,
    const patomic_t *const other
)
{
    /* count how many ops were copied over */
    int copy_count = 0;
    PATOMIC_COMBINE_OPS(copy_count, priority, other);

    /* only combine alignment if at least one op was copied over */
    if (copy_count > 0)
    {
        PATOMIC_COMBINE_ALIGN(priority, other);
    }
}


void
patomic_combine(
    patomic_t *const priority,
    const patomic_t *const other
)
{
    /* defer to internal implementation */
    patomic_internal_combine(priority, other);
}


void
patomic_internal_combine_explicit(
    patomic_explicit_t *const priority,
    const patomic_explicit_t *const other
)
{
    /* count how many ops were copied over */
    int copy_count = 0;
    PATOMIC_COMBINE_OPS(copy_count, priority, other);

    /* only combine alignment if at least one op was copied over */
    if (copy_count > 0)
    {
        PATOMIC_COMBINE_ALIGN(priority, other);
    }
}


void
patomic_combine_explicit(
    patomic_explicit_t *const priority,
    const patomic_explicit_t *const other
)
{
    /* defer to internal implementation */
    patomic_internal_combine_explicit(priority, other);
}
