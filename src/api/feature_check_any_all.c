#include <patomic/api/feature_check.h>
#include <patomic/internal/feature_check.h>


#define PATOMIC_UNSET_OPCAT_LDST(ops, cats, and_or) \
    do {                                            \
        if ((cats & patomic_opcat_LDST)             \
            && ((ops->fp_load  != NULL) and_or      \
                (ops->fp_store != NULL)))           \
        {                                           \
            cats ^= patomic_opcat_LDST;             \
        }                                           \
    }                                               \
    while (0)


#define PATOMIC_UNSET_OPCAT_XCHG(ops, cats, and_or)              \
    do {                                                         \
        if ((cats & patomic_opcat_XCHG)                          \
            && ((ops->xchg_ops.fp_exchange       != NULL) and_or \
                (ops->xchg_ops.fp_cmpxchg_weak   != NULL) and_or \
                (ops->xchg_ops.fp_cmpxchg_strong != NULL)))      \
        {                                                        \
            cats ^= patomic_opcat_XCHG;                          \
        }                                                        \
    }                                                            \
    while (0)


#define PATOMIC_UNSET_OPCAT_BIT(ops, cats, and_or)              \
    do {                                                        \
        if ((cats & patomic_opcat_BIT)                          \
            && ((ops->bitwise_ops.fp_test       != NULL) and_or \
                (ops->bitwise_ops.fp_test_compl != NULL) and_or \
                (ops->bitwise_ops.fp_test_set   != NULL) and_or \
                (ops->bitwise_ops.fp_test_reset != NULL)))      \
        {                                                       \
            cats ^= patomic_opcat_BIT;                          \
        }                                                       \
    }                                                           \
    while (0)


#define PATOMIC_UNSET_OPCAT_BIN(ops, cats, and_or, vf, fp_sep)   \
    do {                                                         \
        if ((cats & patomic_opcat_BIN_##vf)                      \
            && ((ops->binary_ops.fp##fp_sep##or  != NULL) and_or \
                (ops->binary_ops.fp##fp_sep##xor != NULL) and_or \
                (ops->binary_ops.fp##fp_sep##and != NULL) and_or \
                (ops->binary_ops.fp##fp_sep##not != NULL)))      \
        {                                                        \
            cats ^= patomic_opcat_BIN_##vf;                      \
        }                                                        \
    }                                                            \
    while (0)

#define PATOMIC_UNSET_OPCAT_BIN_V(ops, cats, and_or) \
    PATOMIC_UNSET_OPCAT_BIN(ops, cats, and_or, V, _)

#define PATOMIC_UNSET_OPCAT_BIN_F(ops, cats, and_or) \
    PATOMIC_UNSET_OPCAT_BIN(ops, cats, and_or, F, _fetch_)


#define PATOMIC_UNSET_OPCAT_ARI(ops, cats, and_or, vf, fp_sep)       \
    do {                                                             \
        if ((cats & patomic_opcat_ARI_##vf)                          \
            && ((ops->arithmetic_ops.fp##fp_sep##add != NULL) and_or \
                (ops->arithmetic_ops.fp##fp_sep##sub != NULL) and_or \
                (ops->arithmetic_ops.fp##fp_sep##inc != NULL) and_or \
                (ops->arithmetic_ops.fp##fp_sep##dec != NULL) and_or \
                (ops->arithmetic_ops.fp##fp_sep##neg != NULL)))      \
        {                                                            \
            cats ^= patomic_opcat_ARI_##vf;                          \
        }                                                            \
    }                                                                \
    while (0)

#define PATOMIC_UNSET_OPCAT_ARI_V(ops, cats, and_or) \
    PATOMIC_UNSET_OPCAT_ARI(ops, cats, and_or, V, _)

#define PATOMIC_UNSET_OPCAT_ARI_F(ops, cats, and_or) \
    PATOMIC_UNSET_OPCAT_ARI(ops, cats, and_or, F, _fetch_)


#define PATOMIC_UNSET_OPCAT_TSPEC(ops, cats, and_or)                \
    do {                                                            \
        if ((cats & patomic_opcat_TSPEC)                            \
            && ((ops->special_ops.fp_double_cmpxchg != NULL) and_or \
                (ops->special_ops.fp_multi_cmpxchg  != NULL) and_or \
                (ops->special_ops.fp_generic        != NULL) and_or \
                (ops->special_ops.fp_generic_wfb    != NULL)))      \
        {                                                           \
            cats ^= patomic_opcat_TSPEC;                            \
        }                                                           \
    }                                                               \
    while (0)


#define PATOMIC_UNSET_OPCAT_TFLAG(ops, cats, and_or)       \
    do {                                                   \
        if ((cats & patomic_opcat_TFLAG)                   \
            && ((ops->flag_ops.fp_test     != NULL) and_or \
                (ops->flag_ops.fp_test_set != NULL) and_or \
                (ops->flag_ops.fp_clear    != NULL)))      \
        {                                                  \
            cats ^= patomic_opcat_TFLAG;                   \
        }                                                  \
    }                                                      \
    while (0)


#define PATOMIC_UNSET_OPCAT_TRAW(ops, cats, and_or)      \
    do {                                                 \
        if ((cats & patomic_opcat_TRAW)                  \
            && ((ops->raw_ops.fp_tbegin  != NULL) and_or \
                (ops->raw_ops.fp_tabort  != NULL) and_or \
                (ops->raw_ops.fp_tcommit != NULL) and_or \
                (ops->raw_ops.fp_ttest != NULL)))        \
        {                                                \
            cats ^= patomic_opcat_TRAW;                  \
        }                                                \
    }                                                    \
    while (0)


unsigned int
patomic_feature_check_all(
    const patomic_ops_t *const ops,
    unsigned int opcats
)
{
    /* short circuit if possible */
    if (opcats == 0)
    {
        return opcats;
    }

    /* unset bits for opcats where all ops are present */
    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_ARI_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_ARI_F(ops, opcats, &&);

    /* return updated opcats */
    return opcats;
}


unsigned int
patomic_feature_check_all_explicit(
    const patomic_ops_explicit_t *const ops,
    unsigned int opcats
)
{
    /* short circuit if possible */
    if (opcats == 0)
    {
        return opcats;
    }

    /* unset bits for opcats where all ops are present */
    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_ARI_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_ARI_F(ops, opcats, &&);

    /* return updated opcats */
    return opcats;
}


unsigned int
patomic_feature_check_all_transaction(
    const patomic_ops_transaction_t *const ops,
    unsigned int opcats
)
{
    /* short circuit if possible */
    if (opcats == 0)
    {
        return opcats;
    }

    /* unset bits for opcats where all ops are present */
    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_ARI_V(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_ARI_F(ops, opcats, &&);
    /* transaction specific opcats */
    PATOMIC_UNSET_OPCAT_TSPEC(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_TFLAG(ops, opcats, &&);
    PATOMIC_UNSET_OPCAT_TRAW(ops, opcats, &&);

    /* return updated opcats */
    return opcats;
}


unsigned int
patomic_internal_feature_check_any(
    const patomic_ops_t *const ops,
    unsigned int opcats
)
{
    /* short circuit if possible */
    if (opcats == 0)
    {
        return opcats;
    }

    /* unset bits for opcats where all ops are present */
    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_ARI_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_ARI_F(ops, opcats, ||);

    /* return updated opcats */
    return opcats;
}


unsigned int
patomic_feature_check_any(
    const patomic_ops_t *const ops,
    const unsigned int opcats
)
{
    /* defer to internal implementation */
    return patomic_internal_feature_check_any(ops, opcats);
}


unsigned int
patomic_internal_feature_check_any_explicit(
    const patomic_ops_explicit_t *const ops,
    unsigned int opcats
)
{
    /* short circuit if possible */
    if (opcats == 0)
    {
        return opcats;
    }

    /* unset bits for opcats where all ops are present */
    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_ARI_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_ARI_F(ops, opcats, ||);

    /* return updated opcats */
    return opcats;
}


unsigned int
patomic_feature_check_any_explicit(
    const patomic_ops_explicit_t *const ops,
    const unsigned int opcats
)
{
    /* defer to internal implementation */
    return patomic_internal_feature_check_any_explicit(ops, opcats);
}


unsigned int
patomic_internal_feature_check_any_transaction(
    const patomic_ops_transaction_t *const ops,
    unsigned int opcats
)
{
    /* short circuit if possible */
    if (opcats == 0)
    {
        return opcats;
    }

    /* unset bits for opcats where all ops are present */
    PATOMIC_UNSET_OPCAT_LDST(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_XCHG(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIT(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIN_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_BIN_F(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_ARI_V(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_ARI_F(ops, opcats, ||);
    /* transaction specific opcats */
    PATOMIC_UNSET_OPCAT_TSPEC(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_TFLAG(ops, opcats, ||);
    PATOMIC_UNSET_OPCAT_TRAW(ops, opcats, ||);

    /* return updated opcats */
    return opcats;
}


unsigned int
patomic_feature_check_any_transaction(
    const patomic_ops_transaction_t *const ops,
    const unsigned int opcats
)
{
    /* defer to internal implementation */
    return patomic_internal_feature_check_any_transaction(ops, opcats);
}
