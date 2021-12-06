#ifndef PATOMIC_FEATURE_CHECK_H
#define PATOMIC_FEATURE_CHECK_H

#include <patomic/patomic_export.h>
#include <patomic/types/ops.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * OPCAT
 *
 * - operation category
 *
 * - S/U(ARI): signed/unsigned version of (arithmetic) operations
 * - _F: fetch version of operations
 * - _V: non-fetch (void) version of operations
 *
 * - IMPLICIT: all operations in patomic_ops_t
 * - EXPLICIT: all operations in patomic_explicit_ops_t
 * - TRANSACTION: all operations in patomic_transaction_ops_t
 */

typedef enum {
    patomic_opcat_NONE = 0x0
    ,patomic_opcat_LDST = 0x1
    ,patomic_opcat_XCHG = 0x2
    ,patomic_opcat_BIT = 0x4
    ,patomic_opcat_BIN_V = 0x8
    ,patomic_opcat_BIN_F = 0x10
    ,patomic_opcat_SARI_V = 0x20
    ,patomic_opcat_SARI_F = 0x40
    ,patomic_opcat_UARI_V = 0x80
    ,patomic_opcat_UARI_F = 0x100
    ,patomic_opcat_SPEC = 0x200
    ,patomic_opcat_FLAG = 0x400
    ,patomic_opcat_RAW = 0x800
    ,patomic_opcat_BIN  = patomic_opcat_BIN_V  | patomic_opcat_BIN_F
    ,patomic_opcat_SARI = patomic_opcat_SARI_V | patomic_opcat_SARI_F
    ,patomic_opcat_UARI = patomic_opcat_UARI_V | patomic_opcat_UARI_F
    ,patomic_opcat_ARI  = patomic_opcat_SARI   | patomic_opcat_UARI
    ,patomic_opcat_IMPLICIT = patomic_opcat_LDST |
                              patomic_opcat_XCHG |
                              patomic_opcat_BIT  |
                              patomic_opcat_BIN  |
                              patomic_opcat_ARI
    ,patomic_opcat_EXPLICIT = patomic_opcat_IMPLICIT
    ,patomic_opcat_TRANSACTION = patomic_opcat_EXPLICIT |
                                 patomic_opcat_SPEC     |
                                 patomic_opcat_FLAG     |
                                 patomic_opcat_RAW
} patomic_opcat_t;


/*
 * OPKIND
 *
 * - operation kind (within an opcat)
 * - there are no fetch opkinds; instead use the regular opkind version with a
 *   fetch opcat (_F)
 *
 * - Note: multiple opkinds not within the same category may have the same value
 * - the corresponding opcat must also be provided for context
 */

typedef enum {
    patomic_opkind_NONE = 0x0
    /* base (opcat_LDST) */
    ,patomic_opkind_LOAD  = 0x1
    ,patomic_opkind_STORE = 0x2
    /* xchg */
    ,patomic_opkind_EXCHANGE = 0x1
    ,patomic_opkind_CMPXCHG_WEAK   = 0x2
    ,patomic_opkind_CMPXCHG_STRONG = 0x4
    /* bitwise and flag (except compl) */
    ,patomic_opkind_TEST = 0x1
    ,patomic_opkind_TEST_SET   = 0x2
    ,patomic_opkind_TEST_RESET = 0x4
    ,patomic_opkind_TEST_COMPL = 0x8
    /* binary */
    ,patomic_opkind_OR  = 0x1
    ,patomic_opkind_XOR = 0x2
    ,patomic_opkind_AND = 0x4
    ,patomic_opkind_NOT = 0x8
    /* arithmetic */
    ,patomic_opkind_ADD = 0x1
    ,patomic_opkind_SUB = 0x2
    ,patomic_opkind_INC = 0x4
    ,patomic_opkind_DEC = 0x8
    ,patomic_opkind_NEG = 0x10
    /* special */
    ,patomic_opkind_DCMPXCHG_WEAK   = 0x1
    ,patomic_opkind_DCMPXCHG_STRONG = 0x2
    ,patomic_opkind_NCMPXCHG_WEAK   = 0x4
    ,patomic_opkind_NCMPXCHG_STRONG = 0x8
    ,patomic_opkind_GENERIC     = 0x10
    ,patomic_opkind_GENERIC_WFB = 0x20
    /* raw */
    ,patomic_opkind_TBEGIN  = 0x1
    ,patomic_opkind_TABORT  = 0x2
    ,patomic_opkind_TCOMMIT = 0x4
    ,patomic_opkind_TTEST   = 0x8
} patomic_opkind_t;


/*
 * FEATURE ALL
 *
 * - opcats: one or more opcat_t flags combined
 *
 * - checks if all the operations in all the categories set in opcats are
 *   supported
 * - unsets all the categories set in opcats which don't have all their
 *   operations supported, and returns the result (i.e. 0 if everything in each
 *   set category is supported)
 * - invalid categories are ignored and remain set
 */

PATOMIC_EXPORT unsigned int
patomic_feature_check_all(
    const patomic_ops_t *ops,
    unsigned int opcats
);

PATOMIC_EXPORT unsigned int
patomic_feature_check_all_explicit(
    const patomic_ops_explicit_t *ops,
    unsigned int opcats
);

PATOMIC_EXPORT unsigned int
patomic_feature_check_all_transaction(
    const patomic_ops_transaction_t *ops,
    unsigned int opcats
);


/*
 * FEATURE ANY
 *
 * - opcats: one or more opcat_t flags combined
 *
 * - checks if any of the operations in each of the categories set in opcats is
 *   supported
 * - unsets all the categories set in opcats which don't have at least one of
 *   their operations supported, and returns the result (i.e. 0 if at least one
 *   operation in each set category is supported)
 * - invalid categories are ignored and remain set
 */

PATOMIC_EXPORT unsigned int
patomic_feature_check_any(
        const patomic_ops_t *ops,
        unsigned int opcats
);

PATOMIC_EXPORT unsigned int
patomic_feature_check_any_explicit(
        const patomic_ops_explicit_t *ops,
        unsigned int opcats
);

PATOMIC_EXPORT unsigned int
patomic_feature_check_any_transaction(
        const patomic_ops_transaction_t *ops,
        unsigned int opcats
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_FEATURE_CHECK_H */
