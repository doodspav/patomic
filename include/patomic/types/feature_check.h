#ifndef PATOMIC_FEATURE_CHECK_H
#define PATOMIC_FEATURE_CHECK_H

#include "ops.h"

#include <patomic/types/export.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup feature_check
 *
 * @brief
 *   Enum constants representing operation categories, corresponding to groups
 *   of operations found in ops structs.
 */
typedef enum {

    /** @brief No operations. May be used with any opkind. */
    patomic_opcat_NONE = 0x0,

    /** @brief Load and store operations. */
    patomic_opcat_LDST = 0x1,

    /** @brief Exchange and compare-exchange (xchg) operations. */
    patomic_opcat_XCHG = 0x2,

    /** @brief Bitwise operations. */
    patomic_opcat_BIT = 0x4,

    /** @brief Binary void (non-fetch) operations. */
    patomic_opcat_BIN_V = 0x8,

    /** @brief Binary fetch operations. */
    patomic_opcat_BIN_F = 0x10,

    /** @brief Arithmetic void (non-fetch) operations. */
    patomic_opcat_ARI_V = 0x20,

    /** @brief Arithmetic fetch operations. */
    patomic_opcat_ARI_F = 0x40,

    /** @brief Transaction specific special extended operations. */
    patomic_opcat_TSPEC = 0x80,

    /** @brief Transaction specific flag operations. */
    patomic_opcat_TFLAG = 0x100,

    /** @brief Transaction specific raw operations. */
    patomic_opcat_TRAW = 0x200,

    /** @brief Binary operations. */
    patomic_opcats_BIN = patomic_opcat_BIN_V | patomic_opcat_BIN_F,

    /** @brief Arithmetic operations. */
    patomic_opcats_ARI = patomic_opcat_ARI_V | patomic_opcat_ARI_F,

    /** @brief All implicit operations. */
    patomic_opcats_IMPLICIT = patomic_opcat_LDST |
                              patomic_opcat_XCHG |
                              patomic_opcat_BIT  |
                              patomic_opcats_BIN |
                              patomic_opcats_ARI,

    /** @brief All explicit operations. */
    patomic_opcats_EXPLICIT = patomic_opcats_IMPLICIT,

    /** @brief All transaction operations. */
    patomic_opcats_TRANSACTION = patomic_opcats_EXPLICIT |
                                 patomic_opcat_TSPEC     |
                                 patomic_opcat_TFLAG     |
                                 patomic_opcat_TRAW

} patomic_opcat_t;


/**
 * @addtogroup feature_check
 *
 * @brief
 *   Enum constants representing operation kinds, corresponding to a specific
 *   operation found in an ops struct.
 *
 * @note
 *   Multiple op kinds from different categories may have the same value, and
 *   are meaningless in isolation. They must be combined with an op category to
 *   have proper meaning.
 *
 * @note
 *   There is no differentiation between fetch and void (non-fetch) op kinds.
 *   Instead, the differentiation is made by whether the op kind is used with
 *   a fetch or void op category.
 */
typedef enum {

    /** @brief No operation. May be used with any opcat. */
    patomic_opkind_NONE = 0x0,

    /** @brief opcat_LDST: load operation. */
    patomic_opkind_LOAD = 0x1,

    /** @brief opcat_LDST: store operation. */
    patomic_opkind_STORE = 0x2,

    /** @brief opcat_LDST: all operations. */
    patomic_opkinds_LDST = patomic_opkind_LOAD | patomic_opkind_STORE,

    /** @brief opcat_XCHG: exchange operation. */
    patomic_opkind_EXCHANGE = 0x1,

    /** @brief opcat_XCHG: compare-exchange weak operation. */
    patomic_opkind_CMPXCHG_WEAK = 0x2,

    /** @brief opcat_XCHG: compare-exchange strong operation. */
    patomic_opkind_CMPXCHG_STRONG = 0x4,

    /** @brief opcat_XCHG: all operations. */
    patomic_opkinds_XCHG = patomic_opkind_EXCHANGE     |
                           patomic_opkind_CMPXCHG_WEAK |
                           patomic_opkind_CMPXCHG_STRONG,

    /** @brief opcat_BIT or opcat_TFLAG: test operation. */
    patomic_opkind_TEST = 0x1,

    /** @brief opcat_BIT or opcat_TFLAG: test and set operation. */
    patomic_opkind_TEST_SET = 0x2,

    /** @brief opcat_BIT: test and reset operation. */
    patomic_opkind_TEST_RESET = 0x4,

    /** @brief opcat_BIT: test and complement operation. */
    patomic_opkind_TEST_COMPL = 0x8,

    /** @brief opcat_BIT or opcat_TFLAG: clear operation. */
    patomic_opkind_CLEAR = 0x10,

    /** @brief opcat_BIT: all operations. */
    patomic_opkinds_BIT = patomic_opkind_TEST       |
                          patomic_opkind_TEST_SET   |
                          patomic_opkind_TEST_RESET |
                          patomic_opkind_TEST_COMPL,

    /** @brief opcat_TFLAG: all operations. */
    patomic_opkinds_TFLAG = patomic_opkind_TEST     |
                            patomic_opkind_TEST_SET |
                            patomic_opkind_CLEAR,

    /** @brief opcats_BIN: binary OR operation. */
    patomic_opkind_OR = 0x1,

    /** @brief opcats_BIN: binary XOR operation. */
    patomic_opkind_XOR = 0x2,

    /** @brief opcats_BIN: binary AND operation. */
    patomic_opkind_AND = 0x4,

    /** @brief opcats_BIN: binary NOT operation. */
    patomic_opkind_NOT = 0x8,

    /** @brief opcats_BIN: all operations. */
    patomic_opkinds_BIN = patomic_opkind_OR  |
                          patomic_opkind_XOR |
                          patomic_opkind_AND |
                          patomic_opkind_NOT,

    /** @brief opcats_ARI: addition operation. */
    patomic_opkind_ADD = 0x1,

    /** @brief opcats_ARI: subtraction operation. */
    patomic_opkind_SUB = 0x2,

    /** @brief opcats_ARI: increment operation. */
    patomic_opkind_INC = 0x4,

    /** @brief opcats_ARI: decrement operation. */
    patomic_opkind_DEC = 0x8,

    /** @brief opcats_ARI: negation operation. */
    patomic_opkind_NEG = 0x10,

    /** @brief opcats_ARI: all operations. */
    patomic_opkinds_ARI = patomic_opkind_ADD |
                          patomic_opkind_SUB |
                          patomic_opkind_INC |
                          patomic_opkind_DEC |
                          patomic_opkind_NEG,

    /** @brief opcat_TSPEC: double compare-exchange operation. */
    patomic_opkind_DOUBLE_CMPXCHG = 0x1,

    /** @brief opcat_TSPEC: multi compare-exchange operation. */
    patomic_opkind_MULTI_CMPXCHG = 0x2,

    /** @brief opcat_TSPEC: generic operation. */
    patomic_opkind_GENERIC = 0x4,

    /** @brief opcat_TSPEC: generic operation with fallback. */
    patomic_opkind_GENERIC_WFB = 0x8,

    /** @brief opcat_TSPEC: all operations. */
    patomic_opkinds_TSPEC = patomic_opkind_DOUBLE_CMPXCHG |
                            patomic_opkind_MULTI_CMPXCHG  |
                            patomic_opkind_GENERIC        |
                            patomic_opkind_GENERIC_WFB,

    /** @brief opcat_TRAW: transaction start operation. */
    patomic_opkind_TBEGIN = 0x1,

    /** @brief opcat_TRAW: transaction abort operation. */
    patomic_opkind_TABORT = 0x2,

    /** @brief opcat_TRAW: transaction commit operation. */
    patomic_opkind_TCOMMIT = 0x4,

    /** @brief opcat_TRAW: transaction test operation. */
    patomic_opkind_TTEST = 0x8,

    /** @brief opcat_TRAW: all operations. */
    patomic_opkinds_TRAW = patomic_opkind_TBEGIN  |
                           patomic_opkind_TABORT  |
                           patomic_opkind_TCOMMIT |
                           patomic_opkind_TTEST

} patomic_opkind_t;


/**
 * @addtogroup feature_check
 *
 * @brief
 *   Checks if all the operations in all the categories in a set of opcats are
 *   supported by the given implicit ops struct.
 *
 * @param ops
 *   Pointer to a set of atomic operations with implicit memory ordering.
 *
 * @param opcats
 *   One or more patomic_opcat_t flags combined.
 *
 * @returns
 *   The input "opcats" value where each bit corresponding to a supported opcat
 *   has been cleared. An opcat is supported if all of its operations are
 *   supported. A return value of 0 means that everything requested is
 *   supported.
 *
 * @note
 *   Invalid bits in "opcats" which do not correspond to a patomic_opcat_t
 *   label are ignored and remain set in the return value.
 */
PATOMIC_EXPORT unsigned int
patomic_feature_check_all(
    const patomic_ops_t *ops,
    unsigned int opcats
);


/**
 * @addtogroup feature_check
 *
 * @brief
 *   Checks if all the operations in all the categories in a set of opcats are
 *   supported by the given explicit ops struct.
 *
 * @param ops
 *   Pointer to a set of atomic operations with explicit memory ordering.
 *
 * @param opcats
 *   One or more patomic_opcat_t flags combined.
 *
 * @returns
 *   The input "opcats" value where each bit corresponding to a supported opcat
 *   has been cleared. An opcat is supported if all of its operations are
 *   supported. A return value of 0 means that everything requested is
 *   supported.
 *
 * @note
 *   Invalid bits in "opcats" which do not correspond to a patomic_opcat_t
 *   label are ignored and remain set in the return value.
 */
PATOMIC_EXPORT unsigned int
patomic_feature_check_all_explicit(
    const patomic_ops_explicit_t *ops,
    unsigned int opcats
);


/**
 * @addtogroup feature_check
 *
 * @brief
 *   Checks if all the operations in all the categories in a set of opcats are
 *   supported by the given transaction ops struct.
 *
 * @param ops
 *   Pointer to a set of atomic operations implemented using a sequentially
 *   consistent transaction, as well as transaction specific operations
 *
 * @param opcats
 *   One or more patomic_opcat_t flags combined.
 *
 * @returns
 *   The input "opcats" value where each bit corresponding to a supported opcat
 *   has been cleared. An opcat is supported if all of its operations are
 *   supported. A return value of 0 means that everything requested is
 *   supported.
 *
 * @note
 *   Invalid bits in "opcats" which do not correspond to a patomic_opcat_t
 *   label are ignored and remain set in the return value.
 */
PATOMIC_EXPORT unsigned int
patomic_feature_check_all_transaction(
    const patomic_ops_transaction_t *ops,
    unsigned int opcats
);


/**
 * @addtogroup feature_check
 *
 * @brief
 *   Checks if any of the operations in all the categories in a set of opcats
 *   are supported by the given implicit ops struct.
 *
 * @param ops
 *   Pointer to a set of atomic operations with implicit memory ordering.
 *
 * @param opcats
 *   One or more patomic_opcat_t flags combined.
 *
 * @returns
 *   The input "opcats" value where each bit corresponding to a supported opcat
 *   has been cleared. An opcat is supported if any of its operations are
 *   supported. A return value of 0 means that everything requested is
 *   supported.
 *
 * @note
 *   Invalid bits in "opcats" which do not correspond to a patomic_opcat_t
 *   label are ignored and remain set in the return value.
 */
PATOMIC_EXPORT unsigned int
patomic_feature_check_any(
    const patomic_ops_t *ops,
    unsigned int opcats
);


/**
 * @addtogroup feature_check
 *
 * @brief
 *   Checks if any of the operations in all the categories in a set of opcats
 *   are supported by the given explicit ops struct.
 *
 * @param ops
 *   Pointer to a set of atomic operations with explicit memory ordering.
 *
 * @param opcats
 *   One or more patomic_opcat_t flags combined.
 *
 * @returns
 *   The input "opcats" value where each bit corresponding to a supported opcat
 *   has been cleared. An opcat is supported if any of its operations are
 *   supported. A return value of 0 means that everything requested is
 *   supported.
 *
 * @note
 *   Invalid bits in "opcats" which do not correspond to a patomic_opcat_t
 *   label are ignored and remain set in the return value.
 */
PATOMIC_EXPORT unsigned int
patomic_feature_check_any_explicit(
    const patomic_ops_explicit_t *ops,
    unsigned int opcats
);


/**
 * @addtogroup feature_check
 *
 * @brief
 *   Checks if any of the operations in all the categories in a set of opcats
 *   are supported by the given transaction ops struct.
 *
 * @param ops
 *   Pointer to a set of atomic operations implemented using a sequentially
 *   consistent transaction, as well as transaction specific operations
 *
 * @param opcats
 *   One or more patomic_opcat_t flags combined.
 *
 * @returns
 *   The input "opcats" value where each bit corresponding to a supported opcat
 *   has been cleared. An opcat is supported if any of its operations are
 *   supported. A return value of 0 means that everything requested is
 *   supported.
 *
 * @note
 *   Invalid bits in "opcats" which do not correspond to a patomic_opcat_t
 *   label are ignored and remain set in the return value.
 */
PATOMIC_EXPORT unsigned int
patomic_feature_check_any_transaction(
    const patomic_ops_transaction_t *ops,
    unsigned int opcats
);


/**
 * @addtogroup feature_check
 *
 * @brief
 *   Checks if any operations in the set of opkinds in the category opcat are
 *   supported by the given implicit ops struct.
 *
 * @param ops
 *   Pointer to a set of atomic operations with implicit memory ordering.
 *
 * @param opcat
 *   Any single patomic_opcat_t flag that has a single bit set.
 *
 * @param opkinds
 *   One or more patomic_opkind_t flags combined.
 *
 * @returns
 *   The input "opkinds" value interpreted according to "opcat" where each bit
 *   corresponding to a supported opkind has been cleared. A return value of 0
 *   means that everything requested is supported.
 *
 * @note
 *   Invalid bits in "opkinds" which do not correspond to a patomic_opkind_t
 *   label are ignored and remain set in the return value.
 *
 * @note
 *   If the "opcat" provided does not apply to the "ops" (e.g. it is
 *   transaction specific), then "opkinds" is returned directly unmodified.
 *
 * @warning
 *   The "opcat" value MUST have exactly 1 valid bit set. This means that labels
 *   such as patomic_opcat_NONE and patomic_opcats_* are not allowed. This will
 *   always be asserted (even if NDEBUG is defined).
 */
PATOMIC_EXPORT unsigned int
patomic_feature_check_leaf(
    const patomic_ops_t *ops,
    patomic_opcat_t opcat,
    unsigned int opkinds
);


/**
 * @addtogroup feature_check
 *
 * @brief
 *   Checks if any operations in the set of opkinds in the category opcat are
 *   supported by the given explicit ops struct.
 *
 * @param ops
 *   Pointer to a set of atomic operations with explicit memory ordering.
 *
 * @param opcat
 *   Any single patomic_opcat_t flag that has a single bit set.
 *
 * @param opkinds
 *   One or more patomic_opkind_t flags combined.
 *
 * @returns
 *   The input "opkinds" value interpreted according to "opcat" where each bit
 *   corresponding to a supported opkind has been cleared. A return value of 0
 *   means that everything requested is supported.
 *
 * @note
 *   Invalid bits in "opkinds" which do not correspond to a patomic_opkind_t
 *   label are ignored and remain set in the return value.
 *
 * @note
 *   If the "opcat" provided does not apply to the "ops" (e.g. it is
 *   transaction specific), then "opkinds" is returned directly unmodified.
 *
 * @warning
 *   The "opcat" value MUST have exactly 1 valid bit set. This means that labels
 *   such as patomic_opcat_NONE and patomic_opcats_* are not allowed. This will
 *   always be asserted (even if NDEBUG is defined).
 */
PATOMIC_EXPORT unsigned int
patomic_feature_check_leaf_explicit(
    const patomic_ops_explicit_t *ops,
    patomic_opcat_t opcat,
    unsigned int opkinds
);


/**
 * @addtogroup feature_check
 *
 * @brief
 *   Checks if any operations in the set of opkinds in the category opcat are
 *   supported by the given transaction ops struct.
 *
 * @param ops
 *   Pointer to a set of atomic operations implemented using a sequentially
 *   consistent transaction, as well as transaction specific operations
 *
 * @param opcat
 *   Any single patomic_opcat_t flag that has a single bit set.
 *
 * @param opkinds
 *   One or more patomic_opkind_t flags combined.
 *
 * @returns
 *   The input "opkinds" value interpreted according to "opcat" where each bit
 *   corresponding to a supported opkind has been cleared. A return value of 0
 *   means that everything requested is supported.
 *
 * @note
 *   Invalid bits in "opkinds" which do not correspond to a patomic_opkind_t
 *   label are ignored and remain set in the return value.
 *
 * @warning
 *   The "opcat" value MUST have exactly 1 valid bit set. This means that labels
 *   such as patomic_opcat_NONE and patomic_opcats_* are not allowed. This will
 *   always be asserted (even if NDEBUG is defined).
 */
PATOMIC_EXPORT unsigned int
patomic_feature_check_leaf_transaction(
    const patomic_ops_transaction_t *ops,
    patomic_opcat_t opcat,
    unsigned int opkinds
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_FEATURE_CHECK_H */
