#include <patomic/types/feature_check.h>

#include <gtest/gtest.h>


/// @brief Test fixture.
class BtTypesFeatureCheck : public testing::Test
{};

/*
 *
 * all opcat have 1 bit set, except NONE which has 0
 * all opcat are unique
 * no unexpected opcat
 * all opcats consist of known opcat values
 *
 * all opkind have 1 bit set, except NONE which has 0
 * all opkind are unique within their group
 * no unexpected opkind (might need per opcat)
 * all opkinds consist of known and expected opkind values
 *
 * invalid bits remain set
 * each bit corresponds properly to the category (might need per opcat)
 *
 * invalid bits remain set
 * each bit corresponds properly to the category (might need per opcat)
 *
 * invalid kind bits remain set
 * invalid opcat bit is fatally asserted
 * multiple opcat bits are fatally asserted
 * each kind bit corresponds properly to the operation (needs per opcat)
 *
 */


/// @brief All opcats are unique.
TEST_F(BtTypesFeatureCheck, all_opcats_are_unique)
{}

/// @brief All "opcat" opcats have exactly zero or one bits set.

/// @brief All "opcats" opcats have multiple bits set.

/// @brief Each "opcats" opcats consist only of known "opcat" opcat bits.

/// @brief Invalid opcat bits passed to "all" remain set in the return value.

/// @brief Invalid opcat bits passed to "any" remain set in the return value.

/// @brief All bits unset for patomic_ops*_t which only supports LDST ops
///        exactly matches all bits set in patomic_opcat_LDST.

/// @brief All bits unset for patomic_ops*_t which only supports XCHG ops
///        exactly matches all bits set in patomic_opcat_XCHG.

/// @brief All bits unset for patomic_ops*_t which only supports BIT ops
///        exactly matches all bits set in patomic_opcat_BIT.

/// @brief All bits unset for patomic_ops*_t which only supports BIN_V ops
///        exactly matches all bits set in patomic_opcat_BIN_V.

/// @brief All bits unset for patomic_ops*_t which only supports BIN_F ops
///        exactly matches all bits set in patomic_opcat_BIN_F.

/// @brief All bits unset for patomic_ops*_t which only supports ARI_V ops
///        exactly matches all bits set in patomic_opcat_ARI_V.

/// @brief All bits unset for patomic_ops*_t which only supports ARI_F ops
///        exactly matches all bits set in patomic_opcat_ARI_F.

/// @brief All bits unset for patomic_ops_transaction_t which only supports
///        TSPEC ops exactly matches all bits set in patomic_opcat_TSPEC.

/// @brief All bits unset for patomic_ops_transaction_t which only supports
///        TFLAG ops exactly matches all bits set in patomic_opcat_TFLAG.

/// @brief All bits unset for patomic_ops_transaction_t which only supports
///        TRAW ops exactly matches all bits set in patomic_opcat_TRAW.
