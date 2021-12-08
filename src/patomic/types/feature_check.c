#include <patomic/types/feature_check.h>
#include <patomic/macros/force_inline.h>
#include <patomic/stdlib/assert.h>


#define PATOMIC_UNSET_OPCAT_LDST(ops, cats, and_or) \
    do {                                            \
        if ((cats & patomic_opcat_LDST)             \
            && ((ops->fp_load != NULL) and_or       \
                (ops->fp_store != NULL)))           \
        { cats ^= patomic_opcat_LDST; }             \
    }                                               \
    while (0)

#define PATOMIC_UNSET_OPCAT_XCHG(ops, cats, and_or)            \
    do {                                                       \
        if ((cats & patomic_opcat_XCHG)                        \
            && ((ops->xchg_ops.fp_exchange != NULL)     and_or \
                (ops->xchg_ops.fp_cmpxchg_weak != NULL) and_or \
                (ops->xchg_ops.fp_cmpxchg_strong != NULL)))    \
        { cats ^= patomic_opcat_XCHG; }                        \
    }                                                          \
    while (0)

#define PATOMIC_UNSET_OPCAT_BIT(ops, cats, and_or)              \
    do {                                                        \
        if ((cats & patomic_opcat_BIT)                          \
            && ((ops->bitwise_ops.fp_test != NULL)       and_or \
                (ops->bitwise_ops.fp_test_set != NULL)   and_or \
                (ops->bitwise_ops.fp_test_reset != NULL) and_or \
                (ops->bitwise_ops.fp_test_compl != NULL)))      \
        { cats ^= patomic_opcat_BIT; }                          \
    }                                                           \
    while (0)

#define PATOMIC_UNSET_OPCAT_BIN(ops, cats, and_or, vf, fp_sep)   \
    do {                                                         \
        if ((cats & patomic_opcat_BIN_##vf)                      \
            && ((ops->binary_ops.fp##fp_sep##or != NULL)  and_or \
                (ops->binary_ops.fp##fp_sep##xor != NULL) and_or \
                (ops->binary_ops.fp##fp_sep##and != NULL) and_or \
                (ops->binary_ops.fp##fp_sep##not != NULL)))      \
        { cats ^= patomic_opcat_BIN_##vf; }                      \
    }                                                            \
    while (0)

#define PATOMIC_UNSET_OPCAT_BIN_V(ops, cats, and_or) \
    PATOMIC_UNSET_OPCAT_BIN(ops, cats, and_or, V, _)

#define PATOMIC_UNSET_OPCAT_BIN_F(ops, cats, and_or) \
    PATOMIC_UNSET_OPCAT_BIN(ops, cats, and_or, F, _fetch_)

#define PATOMIC_UNSET_OPCAT_ARI(ops, cats, and_or, vf, fp_sep, su, uns_pfx) \
    do {                                                                    \
        if ((cats & patomic_opcat_##su##ARI_##vf)                           \
            && ((ops->uns_pfx##igned_ops.fp##fp_sep##add != NULL) and_or    \
                (ops->uns_pfx##igned_ops.fp##fp_sep##sub != NULL) and_or    \
                (ops->uns_pfx##igned_ops.fp##fp_sep##inc != NULL) and_or    \
                (ops->uns_pfx##igned_ops.fp##fp_sep##dec != NULL) and_or    \
                (ops->uns_pfx##igned_ops.fp##fp_sep##neg != NULL)))         \
        { cats ^= patomic_opcat_##su##ARI_##vf; }                           \
    }                                                                       \
    while (0)

#define PATOMIC_UNSET_OPCAT_SARI_V(ops, cats, and_or) \
    PATOMIC_UNSET_OPCAT_ARI(ops, cats, and_or, V, _, S, s)

#define PATOMIC_UNSET_OPCAT_SARI_F(ops, cats, and_or) \
    PATOMIC_UNSET_OPCAT_ARI(ops, cats, and_or, F, _fetch_, S, s)

#define PATOMIC_UNSET_OPCAT_UARI_V(ops, cats, and_or) \
    PATOMIC_UNSET_OPCAT_ARI(ops, cats, and_or, V, _, U, uns)

#define PATOMIC_UNSET_OPCAT_UARI_F(ops, cats, and_or) \
    PATOMIC_UNSET_OPCAT_ARI(ops, cats, and_or, F, _fetch_, U, uns)

#define PATOMIC_UNSET_OPCAT_SPEC(ops, cats, and_or)                        \
    do {                                                                   \
        if ((cats & patomic_opcat_SPEC)                                    \
            && ((ops->special_ops.fp_double_cmpxchg_weak != NULL)   and_or \
                (ops->special_ops.fp_double_cmpxchg_strong != NULL) and_or \
                (ops->special_ops.fp_n_cmpxchg_weak != NULL)        and_or \
                (ops->special_ops.fp_n_cmpxchg_strong != NULL)      and_or \
                (ops->special_ops.fp_generic != NULL)               and_or \
                (ops->special_ops.fp_generic_wfb != NULL)))                \
        { cats ^= patomic_opcat_SPEC; }                                    \
    }                                                                      \
    while (0)

#define PATOMIC_UNSET_OPCAT_FLAG(ops, cats, and_or)        \
    do {                                                   \
        if ((cats & patomic_opcat_FLAG)                    \
            && ((ops->flag_ops.fp_test != NULL)     and_or \
                (ops->flag_ops.fp_test_set != NULL) and_or \
                (ops->flag_ops.fp_clear)))                 \
        { cats ^= patomic_opcat_FLAG; }                    \
    }                                                      \
    while (0)

#define PATOMIC_UNSET_OPCAT_RAW(ops, cats, and_or)       \
    do {                                                 \
        if ((cats & patomic_opcat_RAW)                   \
            && ((ops->raw_ops.fp_tbegin != NULL)  and_or \
                (ops->raw_ops.fp_tabort != NULL)  and_or \
                (ops->raw_ops.fp_tcommit != NULL) and_or \
                (ops->raw_ops.fp_ttest != NULL)))        \
        { cats ^= patomic_opcat_RAW; }                   \
    }                                                    \
    while (0)


unsigned int
patomic_feature_check_all(
    patomic_ops_t const *const ops,
    unsigned int opcats
)
{
    if (opcats == 0) { return opcats; }

    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, &&);

    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, &&);

    PATOMIC_UNSET_OPCAT_SARI_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_SARI_F(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_UARI_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_UARI_F(ops, opcats, &&);

    return opcats;
}

unsigned int
patomic_feature_check_all_explicit(
    patomic_ops_explicit_t const *const ops,
    unsigned int opcats
)
{
    if (opcats == 0) { return opcats; }

    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, &&);

    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, &&);

    PATOMIC_UNSET_OPCAT_SARI_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_SARI_F(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_UARI_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_UARI_F(ops, opcats, &&);

    return opcats;
}

unsigned int
patomic_feature_check_all_transaction(
    patomic_ops_transaction_t const *const ops,
    unsigned int opcats
)
{
    if (opcats == 0) { return opcats; }

    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, &&);

    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, &&);

    PATOMIC_UNSET_OPCAT_SARI_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_SARI_F(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_UARI_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_UARI_F(ops, opcats, &&);

    PATOMIC_UNSET_OPCAT_SPEC(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_FLAG(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_RAW(ops, opcats, &&);

    return opcats;
}


unsigned int
patomic_feature_check_any(
    patomic_ops_t const *const ops,
    unsigned int opcats
)
{
    if (opcats == 0) { return opcats; }

    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, ||);

    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, ||);

    PATOMIC_UNSET_OPCAT_SARI_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_SARI_F(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_UARI_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_UARI_F(ops, opcats, ||);

    return opcats;
}

unsigned int
patomic_feature_check_any_explicit(
    patomic_ops_explicit_t const *const ops,
    unsigned int opcats
)
{
    if (opcats == 0) { return opcats; }

    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, ||);

    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, ||);

    PATOMIC_UNSET_OPCAT_SARI_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_SARI_F(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_UARI_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_UARI_F(ops, opcats, ||);

    return opcats;
}

unsigned int
patomic_feature_check_any_transaction(
    patomic_ops_transaction_t const *const ops,
    unsigned int opcats
)
{
    if (opcats == 0) { return opcats; }

    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, ||);

    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, ||);

    PATOMIC_UNSET_OPCAT_SARI_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_SARI_F(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_UARI_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_UARI_F(ops, opcats, ||);

    PATOMIC_UNSET_OPCAT_SPEC(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_FLAG(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_RAW(ops, opcats, ||);

    return opcats;
}


#define PATOMIC_CHK_UNSET_OPKIND(fp_op, kinds, kind) \
    if (fp_op != NULL) kinds &= ~((unsigned int) patomic_opkind_##kind)

#define PATOMIC_UNSET_OPKINDS_LDST(ops, kinds)           \
    PATOMIC_CHK_UNSET_OPKIND(ops->fp_load, kinds, LOAD); \
    PATOMIC_CHK_UNSET_OPKIND(ops->fp_store, kinds, STORE)

#define PATOMIC_UNSET_OPKINDS_XCHG(ops, kinds)                                    \
    PATOMIC_CHK_UNSET_OPKIND(ops->xchg_ops.fp_exchange, kinds, EXCHANGE);         \
    PATOMIC_CHK_UNSET_OPKIND(ops->xchg_ops.fp_cmpxchg_weak, kinds, CMPXCHG_WEAK); \
    PATOMIC_CHK_UNSET_OPKIND(ops->xchg_ops.fp_cmpxchg_strong, kinds, CMPXCHG_STRONG)

#define PATOMIC_UNSET_OPKINDS_BIT(ops, kinds)                                    \
    PATOMIC_CHK_UNSET_OPKIND(ops->bitwise_ops.fp_test, kinds, TEST);             \
    PATOMIC_CHK_UNSET_OPKIND(ops->bitwise_ops.fp_test_set, kinds, TEST_SET);     \
    PATOMIC_CHK_UNSET_OPKIND(ops->bitwise_ops.fp_test_reset, kinds, TEST_RESET); \
    PATOMIC_CHK_UNSET_OPKIND(ops->bitwise_ops.fp_test_compl, kinds, TEST_COMPL)  \

#define PATOMIC_UNSET_OPKINDS_BIN(ops, kinds, fp_sep)                      \
    PATOMIC_CHK_UNSET_OPKIND(ops->binary_ops.fp##fp_sep##or, kinds, OR);   \
    PATOMIC_CHK_UNSET_OPKIND(ops->binary_ops.fp##fp_sep##xor, kinds, XOR); \
    PATOMIC_CHK_UNSET_OPKIND(ops->binary_ops.fp##fp_sep##and, kinds, AND); \
    PATOMIC_CHK_UNSET_OPKIND(ops->binary_ops.fp##fp_sep##not, kinds, NOT)

#define PATOMIC_UNSET_OPKINDS_BIN_V(ops, kinds) \
    PATOMIC_UNSET_OPKINDS_BIN(ops, kinds, _)

#define PATOMIC_UNSET_OPKINDS_BIN_F(ops, kinds) \
    PATOMIC_UNSET_OPKINDS_BIN(ops, kinds, _fetch_)

#define PATOMIC_UNSET_OPKINDS_ARI(ops, kinds, fp_sep, uns) \
    PATOMIC_CHK_UNSET_OPKIND(ops->uns##igned_ops.fp##fp_sep##add, kinds, ADD); \
    PATOMIC_CHK_UNSET_OPKIND(ops->uns##igned_ops.fp##fp_sep##sub, kinds, SUB); \
    PATOMIC_CHK_UNSET_OPKIND(ops->uns##igned_ops.fp##fp_sep##inc, kinds, INC); \
    PATOMIC_CHK_UNSET_OPKIND(ops->uns##igned_ops.fp##fp_sep##dec, kinds, DEC); \
    PATOMIC_CHK_UNSET_OPKIND(ops->uns##igned_ops.fp##fp_sep##neg, kinds, NEG)

#define PATOMIC_UNSET_OPKINDS_SARI_V(ops, kinds) \
    PATOMIC_UNSET_OPKINDS_ARI(ops, kinds, _, s)

#define PATOMIC_UNSET_OPKINDS_SARI_F(ops, kinds) \
    PATOMIC_UNSET_OPKINDS_ARI(ops, kinds, _fetch_, s)

#define PATOMIC_UNSET_OPKINDS_UARI_V(ops, kinds) \
    PATOMIC_UNSET_OPKINDS_ARI(ops, kinds, _, uns)

#define PATOMIC_UNSET_OPKINDS_UARI_F(ops, kinds) \
    PATOMIC_UNSET_OPKINDS_ARI(ops, kinds, _fetch_, uns)

#define PATOMIC_UNSET_OPKINDS_SPEC(ops, kinds)                                 \
    PATOMIC_CHK_UNSET_OPKIND(ops->special_ops.fp_double_cmpxchg_weak, kinds,   \
                             DCMPXCHG_WEAK);                                   \
    PATOMIC_CHK_UNSET_OPKIND(ops->special_ops.fp_double_cmpxchg_strong, kinds, \
                             DCMPXCHG_STRONG);                                 \
    PATOMIC_CHK_UNSET_OPKIND(ops->special_ops.fp_n_cmpxchg_weak, kinds,        \
                             NCMPXCHG_WEAK);                                   \
    PATOMIC_CHK_UNSET_OPKIND(ops->special_ops.fp_n_cmpxchg_strong, kinds,      \
                             NCMPXCHG_STRONG);                                 \
    PATOMIC_CHK_UNSET_OPKIND(ops->special_ops.fp_generic, kinds, GENERIC);     \
    PATOMIC_CHK_UNSET_OPKIND(ops->special_ops.fp_generic_wfb, kinds, GENERIC_WFB)

#define PATOMIC_UNSET_OPKINDS_FLAG(ops, kinds)                            \
    PATOMIC_CHK_UNSET_OPKIND(ops->flag_ops.fp_test, kinds, TEST);         \
    PATOMIC_CHK_UNSET_OPKIND(ops->flag_ops.fp_test_set, kinds, TEST_SET); \
    PATOMIC_CHK_UNSET_OPKIND(ops->flag_ops.fp_clear, kinds, CLEAR)

#define PATOMIC_UNSET_OPKINDS_RAW(ops, kinds)                          \
    PATOMIC_CHK_UNSET_OPKIND(ops->raw_ops.fp_tbegin, kinds, TBEGIN);   \
    PATOMIC_CHK_UNSET_OPKIND(ops->raw_ops.fp_tabort, kinds, TABORT);   \
    PATOMIC_CHK_UNSET_OPKIND(ops->raw_ops.fp_tcommit, kinds, TCOMMIT); \
    PATOMIC_CHK_UNSET_OPKIND(ops->raw_ops.fp_ttest, kinds, TTEST)

#define PATOMIC_CASE_UNSET_OPKINDS(cat, ops, kinds) \
    case patomic_opcat_##cat:                       \
        PATOMIC_UNSET_OPKINDS_##cat(ops, opkinds);  \
        break


PATOMIC_FORCE_INLINE static int
patomic_is_pow2_or_zero(
    unsigned int val
)
{
    return (val & (val - 1)) == 0;
}

unsigned int
patomic_feature_check_leaf(
    patomic_ops_t const *const ops,
    patomic_opcat_t opcat,
    unsigned int opkinds
)
{
    if ((opcat == 0) || (opkinds == 0)) { return opkinds; }
    patomic_assert_always(patomic_is_pow2_or_zero(opcat));

    switch (opcat)
    {
        PATOMIC_CASE_UNSET_OPKINDS(LDST, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(XCHG, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIT, ops, opkinds);

        PATOMIC_CASE_UNSET_OPKINDS(BIN_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIN_F, ops, opkinds);

        PATOMIC_CASE_UNSET_OPKINDS(SARI_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(SARI_F, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(UARI_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(UARI_F, ops, opkinds);

        case patomic_opcat_BIN:
        case patomic_opcat_SARI:
        case patomic_opcat_UARI:
        case patomic_opcat_ARI:
        case patomic_opcat_IMPLICIT:
        /* case patomic_opcat_EXPLICIT (==IMPLICIT) */
        case patomic_opcat_TRANSACTION:
            patomic_assert_always("combined opcats" && 0);
            break;

        case patomic_opcat_SPEC:
        case patomic_opcat_FLAG:
        case patomic_opcat_RAW:
        case patomic_opcat_NONE:
        default: break;
    }

    return opkinds;
}

unsigned int
patomic_feature_check_leaf_explicit(
    patomic_ops_explicit_t const *const ops,
    patomic_opcat_t opcat,
    unsigned int opkinds
)
{
    if ((opcat == 0) || (opkinds == 0)) { return opkinds; }
    patomic_assert_always(patomic_is_pow2_or_zero(opcat));

    switch (opcat)
    {
        PATOMIC_CASE_UNSET_OPKINDS(LDST, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(XCHG, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIT, ops, opkinds);

        PATOMIC_CASE_UNSET_OPKINDS(BIN_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIN_F, ops, opkinds);

        PATOMIC_CASE_UNSET_OPKINDS(SARI_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(SARI_F, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(UARI_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(UARI_F, ops, opkinds);

        case patomic_opcat_BIN:
        case patomic_opcat_SARI:
        case patomic_opcat_UARI:
        case patomic_opcat_ARI:
        case patomic_opcat_IMPLICIT:
        /* case patomic_opcat_EXPLICIT (==IMPLICIT) */
        case patomic_opcat_TRANSACTION:
            patomic_assert_always("combined opcats" && 0);
            break;

        case patomic_opcat_SPEC:
        case patomic_opcat_FLAG:
        case patomic_opcat_RAW:
        case patomic_opcat_NONE:
        default: break;
    }

    return opkinds;
}

unsigned int
patomic_feature_check_leaf_transaction(
    patomic_ops_transaction_t const *const ops,
    patomic_opcat_t opcat,
    unsigned int opkinds
)
{
    if ((opcat == 0) || (opkinds == 0)) { return opkinds; }
    patomic_assert_always(patomic_is_pow2_or_zero(opcat));

    switch (opcat)
    {
        PATOMIC_CASE_UNSET_OPKINDS(LDST, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(XCHG, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIT, ops, opkinds);

        PATOMIC_CASE_UNSET_OPKINDS(BIN_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIN_F, ops, opkinds);

        PATOMIC_CASE_UNSET_OPKINDS(SARI_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(SARI_F, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(UARI_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(UARI_F, ops, opkinds);

        PATOMIC_CASE_UNSET_OPKINDS(SPEC, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(FLAG, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(RAW, ops, opkinds);

        case patomic_opcat_BIN:
        case patomic_opcat_SARI:
        case patomic_opcat_UARI:
        case patomic_opcat_ARI:
        case patomic_opcat_IMPLICIT:
        /* case patomic_opcat_EXPLICIT (==IMPLICIT) */
        case patomic_opcat_TRANSACTION:
            patomic_assert_always("combined opcats" && 0);
            break;

        case patomic_opcat_NONE:
        default: break;
    }

    return opkinds;
}
