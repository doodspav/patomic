#include <patomic/api/feature_check.h>

#include <patomic/macros/unreachable.h>
#include <patomic/stdlib/assert.h>
#include <patomic/stdlib/math.h>


#define PATOMIC_UNSET_OPKIND(fp_op, kinds, kind)              \
    do {                                                      \
        if (fp_op != NULL)                                    \
        {                                                     \
            kinds &= ~((unsigned int) patomic_opkind_##kind); \
        }                                                     \
    }                                                         \
    while (0)


#define PATOMIC_UNSET_OPKINDS_LDST(ops, kinds)        \
    PATOMIC_UNSET_OPKIND(ops->fp_load,  kinds, LOAD); \
    PATOMIC_UNSET_OPKIND(ops->fp_store, kinds, STORE)


#define PATOMIC_UNSET_OPKINDS_XCHG(ops, kinds)                                  \
    PATOMIC_UNSET_OPKIND(ops->xchg_ops.fp_exchange,       kinds, EXCHANGE);     \
    PATOMIC_UNSET_OPKIND(ops->xchg_ops.fp_cmpxchg_weak,   kinds, CMPXCHG_WEAK); \
    PATOMIC_UNSET_OPKIND(ops->xchg_ops.fp_cmpxchg_strong, kinds, CMPXCHG_STRONG)


#define PATOMIC_UNSET_OPKINDS_BIT(ops, kinds)                                \
    PATOMIC_UNSET_OPKIND(ops->bitwise_ops.fp_test,       kinds, TEST);       \
    PATOMIC_UNSET_OPKIND(ops->bitwise_ops.fp_test_compl, kinds, TEST_COMPL); \
    PATOMIC_UNSET_OPKIND(ops->bitwise_ops.fp_test_set,   kinds, TEST_SET);   \
    PATOMIC_UNSET_OPKIND(ops->bitwise_ops.fp_test_reset, kinds, TEST_RESET)


#define PATOMIC_UNSET_OPKINDS_BIN(ops, kinds, fp_sep)                  \
    PATOMIC_UNSET_OPKIND(ops->binary_ops.fp##fp_sep##or,  kinds, OR);  \
    PATOMIC_UNSET_OPKIND(ops->binary_ops.fp##fp_sep##xor, kinds, XOR); \
    PATOMIC_UNSET_OPKIND(ops->binary_ops.fp##fp_sep##and, kinds, AND); \
    PATOMIC_UNSET_OPKIND(ops->binary_ops.fp##fp_sep##not, kinds, NOT)

#define PATOMIC_UNSET_OPKINDS_BIN_V(ops, kinds) \
    PATOMIC_UNSET_OPKINDS_BIN(ops, kinds, _)

#define PATOMIC_UNSET_OPKINDS_BIN_F(ops, kinds) \
    PATOMIC_UNSET_OPKINDS_BIN(ops, kinds, _fetch_)


#define PATOMIC_UNSET_OPKINDS_ARI(ops, kinds, fp_sep)                      \
    PATOMIC_UNSET_OPKIND(ops->arithmetic_ops.fp##fp_sep##add, kinds, ADD); \
    PATOMIC_UNSET_OPKIND(ops->arithmetic_ops.fp##fp_sep##sub, kinds, SUB); \
    PATOMIC_UNSET_OPKIND(ops->arithmetic_ops.fp##fp_sep##inc, kinds, INC); \
    PATOMIC_UNSET_OPKIND(ops->arithmetic_ops.fp##fp_sep##dec, kinds, DEC); \
    PATOMIC_UNSET_OPKIND(ops->arithmetic_ops.fp##fp_sep##neg, kinds, NEG)

#define PATOMIC_UNSET_OPKINDS_ARI_V(ops, kinds) \
    PATOMIC_UNSET_OPKINDS_ARI(ops, kinds, _)

#define PATOMIC_UNSET_OPKINDS_ARI_F(ops, kinds) \
    PATOMIC_UNSET_OPKINDS_ARI(ops, kinds, _fetch_)


#define PATOMIC_UNSET_OPKINDS_TSPEC(ops, kinds)                                      \
    PATOMIC_UNSET_OPKIND(ops->special_ops.fp_double_cmpxchg, kinds, DOUBLE_CMPXCHG); \
    PATOMIC_UNSET_OPKIND(ops->special_ops.fp_multi_cmpxchg,  kinds, MULTI_CMPXCHG);  \
    PATOMIC_UNSET_OPKIND(ops->special_ops.fp_generic,        kinds, GENERIC);        \
    PATOMIC_UNSET_OPKIND(ops->special_ops.fp_generic_wfb,    kinds, GENERIC_WFB)


#define PATOMIC_UNSET_OPKINDS_TFLAG(ops, kinds)                       \
    PATOMIC_UNSET_OPKIND(ops->flag_ops.fp_test,     kinds, TEST);     \
    PATOMIC_UNSET_OPKIND(ops->flag_ops.fp_test_set, kinds, TEST_SET); \
    PATOMIC_UNSET_OPKIND(ops->flag_ops.fp_clear,    kinds, CLEAR)


#define PATOMIC_UNSET_OPKINDS_TRAW(ops, kinds)                     \
    PATOMIC_UNSET_OPKIND(ops->raw_ops.fp_tbegin,  kinds, TBEGIN);  \
    PATOMIC_UNSET_OPKIND(ops->raw_ops.fp_tabort,  kinds, TABORT);  \
    PATOMIC_UNSET_OPKIND(ops->raw_ops.fp_tcommit, kinds, TCOMMIT); \
    PATOMIC_UNSET_OPKIND(ops->raw_ops.fp_ttest,   kinds, TTEST)


#define PATOMIC_CASE_UNSET_OPKINDS(cat, ops, kinds) \
    case patomic_opcat_##cat:                       \
        PATOMIC_UNSET_OPKINDS_##cat(ops, opkinds);  \
        break


unsigned int
patomic_feature_check_leaf(
    const patomic_ops_t *const ops,
    const patomic_opcat_t opcat,
    unsigned int opkinds
)
{
    /* ensure that a single opcat is being checked */
    patomic_assert_always(patomic_unsigned_is_pow2(opcat));

    /* short circuit if possible */
    if (opkinds == 0)
    {
        return opkinds;
    }

    /* check which opcat's opkinds we want to unset */
    switch (opcat)
    {
        /* unset bit for each opkind where op is present */
        PATOMIC_CASE_UNSET_OPKINDS(LDST,  ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(XCHG,  ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIT,   ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIN_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIN_F, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(ARI_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(ARI_F, ops, opkinds);

        /* unsupported opcats do not modify anything */
        case patomic_opcat_TSPEC:
        case patomic_opcat_TFLAG:
        case patomic_opcat_TRAW:
        default:
            break;

        /* opcats with zero or multiple bits set should be unreachable */
        case patomic_opcat_NONE:
        case patomic_opcats_BIN:
        case patomic_opcats_ARI:
        case patomic_opcats_IMPLICIT:
        /* case patomic_opcats_EXPLICIT: (==IMPLICIT) */
        case patomic_opcats_TRANSACTION:
            PATOMIC_UNREACHABLE();  /* GCOVR_EXCL_LINE */
#if PATOMIC_UNREACHABLE_IS_VCZ
            break;
#endif
    }

    /* return updated opkinds */
    return opkinds;
}


unsigned int
patomic_feature_check_leaf_explicit(
    const patomic_ops_explicit_t *const ops,
    const patomic_opcat_t opcat,
    unsigned int opkinds
)
{
    /* ensure that a single opcat is being checked */
    patomic_assert_always(patomic_unsigned_is_pow2(opcat));

    /* short circuit if possible */
    if (opkinds == 0)
    {
        return opkinds;
    }

    /* check which opcat's opkinds we want to unset */
    switch (opcat)
    {
        /* unset bit for each opkind where op is present */
        PATOMIC_CASE_UNSET_OPKINDS(LDST,  ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(XCHG,  ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIT,   ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIN_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIN_F, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(ARI_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(ARI_F, ops, opkinds);

        /* unsupported opcats do not modify anything */
        case patomic_opcat_TSPEC:
        case patomic_opcat_TFLAG:
        case patomic_opcat_TRAW:
        default:
            break;

        /* opcats with zero or multiple bits set should be unreachable */
        case patomic_opcat_NONE:
        case patomic_opcats_BIN:
        case patomic_opcats_ARI:
        case patomic_opcats_IMPLICIT:
        /* case patomic_opcats_EXPLICIT: (==IMPLICIT) */
        case patomic_opcats_TRANSACTION:
            PATOMIC_UNREACHABLE();  /* GCOV_EXCL_LINE */
#if PATOMIC_UNREACHABLE_IS_VCZ
            break;
#endif
    }

    /* return updated opkinds */
    return opkinds;
}


unsigned int
patomic_feature_check_leaf_transaction(
    const patomic_ops_transaction_t *const ops,
    const patomic_opcat_t opcat,
    unsigned int opkinds
)
{
    /* ensure that a single opcat is being checked */
    patomic_assert_always(patomic_unsigned_is_pow2(opcat));

    /* short circuit if possible */
    if (opkinds == 0)
    {
        return opkinds;
    }

    /* check which opcat's opkinds we want to unset */
    switch (opcat)
    {
        /* unset bit for each opkind where op is present */
        PATOMIC_CASE_UNSET_OPKINDS(LDST,  ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(XCHG,  ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIT,   ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIN_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(BIN_F, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(ARI_V, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(ARI_F, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(TSPEC, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(TFLAG, ops, opkinds);
        PATOMIC_CASE_UNSET_OPKINDS(TRAW,  ops, opkinds);

        /* unsupported opcats do not modify anything */
        default:
            break;

        /* opcats with zero or multiple bits set should be unreachable */
        case patomic_opcat_NONE:
        case patomic_opcats_BIN:
        case patomic_opcats_ARI:
        case patomic_opcats_IMPLICIT:
        /* case patomic_opcats_EXPLICIT: (==IMPLICIT) */
        case patomic_opcats_TRANSACTION:
            PATOMIC_UNREACHABLE();  /* LCOV_EXCL_LINE */
#if PATOMIC_UNREACHABLE_IS_VCZ
            break;
#endif
    }

    /* return updated opkinds */
    return opkinds;
}
