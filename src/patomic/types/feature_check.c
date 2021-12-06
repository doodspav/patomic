#include <patomic/types/feature_check.h>


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

#define PATOMIC_UNSET_OPCAT_FLAG(ops, cats, and_or)            \
    do {                                                       \
        if ((cats & patomic_opcat_FLAG)                        \
            && ((ops->flag_ops.fp_test != NULL)         and_or \
                (ops->flag_ops.fp_test_and_set != NULL) and_or \
                (ops->flag_ops.fp_clear)))                     \
        { cats ^= patomic_opcat_FLAG; }                        \
    }                                                          \
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
        const patomic_ops_t *ops,
        unsigned int opcats
)
{
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
        const patomic_ops_explicit_t *ops,
        unsigned int opcats
)
{
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
        const patomic_ops_transaction_t *ops,
        unsigned int opcats
)
{
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
        const patomic_ops_t *ops,
        unsigned int opcats
)
{
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
        const patomic_ops_explicit_t *ops,
        unsigned int opcats
)
{
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
        const patomic_ops_transaction_t *ops,
        unsigned int opcats
)
{
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
