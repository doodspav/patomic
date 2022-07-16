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
 * - opcat: single bit set (or zero bits for NONE)
 * - opcats: multiple bits set
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
    /* opcat */
    patomic_opcat_NONE = 0x0
    ,patomic_opcat_LDST = 0x1
    ,patomic_opcat_XCHG = 0x2
    ,patomic_opcat_BIT  = 0x4
    ,patomic_opcat_BIN_V = 0x8
    ,patomic_opcat_BIN_F = 0x10
    ,patomic_opcat_SARI_V = 0x20
    ,patomic_opcat_SARI_F = 0x40
    ,patomic_opcat_UARI_V = 0x80
    ,patomic_opcat_UARI_F = 0x100
    ,patomic_opcat_SPEC = 0x200
    ,patomic_opcat_FLAG = 0x400
    ,patomic_opcat_RAW  = 0x800
    /* opcats */
    ,patomic_opcats_BIN  = patomic_opcat_BIN_V  | patomic_opcat_BIN_F
    ,patomic_opcats_SARI = patomic_opcat_SARI_V | patomic_opcat_SARI_F
    ,patomic_opcats_UARI = patomic_opcat_UARI_V | patomic_opcat_UARI_F
    ,patomic_opcats_ARI  = patomic_opcats_SARI  | patomic_opcats_UARI
    ,patomic_opcats_IMPLICIT    = patomic_opcat_LDST |
                                  patomic_opcat_XCHG |
                                  patomic_opcat_BIT  |
                                  patomic_opcats_BIN |
                                  patomic_opcats_ARI
    ,patomic_opcats_EXPLICIT    = patomic_opcats_IMPLICIT
    ,patomic_opcats_TRANSACTION = patomic_opcats_EXPLICIT |
                                  patomic_opcat_SPEC      |
                                  patomic_opcat_FLAG      |
                                  patomic_opcat_RAW
} patomic_opcat_t;


/*
 * OPKIND
 *
 * - operation kind (within an opcat)
 * - there are no fetch opkinds; instead use the regular opkind version with a
 *   fetch opcat (_F)
 *
 * Note:
 * - multiple opkinds not within the same category may have the same value
 * - the corresponding opcat must also be provided for context
 */

typedef enum {
    patomic_opkind_NONE = 0x0
    /* base (opcat_LDST) */
    ,patomic_opkind_LOAD  = 0x1
    ,patomic_opkind_STORE = 0x2
    ,patomic_opkinds_LDST = patomic_opkind_LOAD |
                            patomic_opkind_STORE
    /* xchg */
    ,patomic_opkind_EXCHANGE = 0x1
    ,patomic_opkind_CMPXCHG_WEAK   = 0x2
    ,patomic_opkind_CMPXCHG_STRONG = 0x4
    ,patomic_opkinds_XCHG = patomic_opkind_EXCHANGE     |
                            patomic_opkind_CMPXCHG_WEAK |
                            patomic_opkind_CMPXCHG_STRONG
    /* bitwise and flag */
    ,patomic_opkind_TEST = 0x1
    ,patomic_opkind_TEST_SET   = 0x2
    ,patomic_opkind_TEST_RESET = 0x4
    ,patomic_opkind_TEST_COMPL = 0x8
    ,patomic_opkind_CLEAR = 0x10
    ,patomic_opkinds_BIT  = patomic_opkind_TEST       |
                            patomic_opkind_TEST_SET   |
                            patomic_opkind_TEST_RESET |
                            patomic_opkind_TEST_COMPL
    ,patomic_opkinds_FLAG = patomic_opkind_TEST     |
                            patomic_opkind_TEST_SET |
                            patomic_opkind_CLEAR
    /* binary */
    ,patomic_opkind_OR  = 0x1
    ,patomic_opkind_XOR = 0x2
    ,patomic_opkind_AND = 0x4
    ,patomic_opkind_NOT = 0x8
    ,patomic_opkinds_BIN = patomic_opkind_OR  |
                           patomic_opkind_XOR |
                           patomic_opkind_AND |
                           patomic_opkind_NOT
    /* arithmetic */
    ,patomic_opkind_ADD = 0x1
    ,patomic_opkind_SUB = 0x2
    ,patomic_opkind_INC = 0x4
    ,patomic_opkind_DEC = 0x8
    ,patomic_opkind_NEG = 0x10
    ,patomic_opkinds_ARI = patomic_opkind_ADD |
                           patomic_opkind_SUB |
                           patomic_opkind_INC |
                           patomic_opkind_DEC |
                           patomic_opkind_NEG
    /* special */
    ,patomic_opkind_DOUBLE_CMPXCHG = 0x1
    ,patomic_opkind_MULTI_CMPXCHG  = 0x2
    ,patomic_opkind_GENERIC        = 0x4
    ,patomic_opkind_GENERIC_WFB    = 0x8
    ,patomic_opkinds_SPEC = patomic_opkind_DOUBLE_CMPXCHG |
                            patomic_opkind_MULTI_CMPXCHG  |
                            patomic_opkind_GENERIC        |
                            patomic_opkind_GENERIC_WFB
    /* raw */
    ,patomic_opkind_TBEGIN  = 0x1
    ,patomic_opkind_TABORT  = 0x2
    ,patomic_opkind_TCOMMIT = 0x4
    ,patomic_opkind_TTEST   = 0x8
    ,patomic_opkinds_RAW = patomic_opkind_TBEGIN  |
                           patomic_opkind_TABORT  |
                           patomic_opkind_TCOMMIT |
                           patomic_opkind_TTEST
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


/*
 * FEATURE LEAF
 *
 * - opcat: any single opcat_t flag that has a single bit set
 * - opkinds: one or more opkind_t flags combined
 *
 * - unsets all the operations set in opkinds which aren't available in the set
 *   opcat, and returns the result (i.e. 0 if all the operations in the category
 *   are supported)
 * - invalid operations are ignored and remain set
 *
 * WARNING:
 * - opcat MUST NOT have more than 1 bit set
 * - this means you cannot pass opcat_t labels which combine multiple other
 *   labels, such as BIT, ARI, IMPLICIT, etc...
 * - this will always be asserted (even if NDEBUG is defined)
 */

PATOMIC_EXPORT unsigned int
patomic_feature_check_leaf(
    const patomic_ops_t *ops,
    unsigned int opcat,
    unsigned int opkinds
);

PATOMIC_EXPORT unsigned int
patomic_feature_check_leaf_explicit(
    const patomic_ops_explicit_t *ops,
    unsigned int opcat,
    unsigned int opkinds
);

PATOMIC_EXPORT unsigned int
patomic_feature_check_leaf_transaction(
    const patomic_ops_transaction_t *ops,
    unsigned int opcat,
    unsigned int opkinds
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_FEATURE_CHECK_H */
