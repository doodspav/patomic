#include <patomic/types/feature_check.h>

#include <test/common/make_ops.hpp>

#include <gtest/gtest.h>

#include <vector>


/// @brief Test fixture.
class BtTypesFeatureCheckAnyAll : public testing::Test
{
public:
    const patomic_ops_t implicit_nonnull {
        test::make_ops_nonnull_implicit()
    };

    const patomic_ops_explicit_t explicit_nonnull {
        test::make_ops_nonnull_explicit()
    };

    const patomic_ops_transaction_t transaction_nonnull {
        test::make_ops_nonnull_transaction()
    };

    const std::vector<patomic_opcat_t> solo_opcats {
        patomic_opcat_NONE,
        patomic_opcat_LDST,
        patomic_opcat_XCHG,
        patomic_opcat_BIT,
        patomic_opcat_BIN_V,
        patomic_opcat_BIN_F,
        patomic_opcat_ARI_V,
        patomic_opcat_ARI_F,
        patomic_opcat_TSPEC,
        patomic_opcat_TFLAG,
        patomic_opcat_TRAW
    };

    const std::vector<patomic_opcat_t> combined_opcats {
        patomic_opcats_BIN,
        patomic_opcats_ARI,
        patomic_opcats_IMPLICIT,
        patomic_opcats_EXPLICIT,
        patomic_opcats_TRANSACTION
    };
};


/// @brief All "opcat" opcats have exactly zero or one bits set.
TEST_F(BtTypesFeatureCheckAnyAll, opcat_have_zero_or_one_bits_set)
{}

/// @brief All "opcat" values are unique.
TEST_F(BtTypesFeatureCheckAnyAll, opcat_are_unique)
{}

/// @brief All "opcats" opcats have multiple bits set.
TEST_F(BtTypesFeatureCheckAnyAll, opcats_have_multiple_bits_set)
{}

/// @brief All "opcats" opcats have expected combination of bits.
TEST_F(BtTypesFeatureCheckAnyAll, opcats_have_expected_bits)
{}

/// @brief Each "opcats" opcats consist only of known "opcat" opcat bits.
TEST_F(BtTypesFeatureCheckAnyAll, opcats_only_contain_known_opcat_bits)
{}

/// @brief The values of patomic_opcats_IMPLICIT and patomic_opcats_EXPLICIT
///        are the same.
TEST_F(BtTypesFeatureCheckAnyAll, opcats_implicit_eq_explicit)
{}

/// @brief The bits set in patomic_opcats_IMPLICIT and patomic_opcats_EXPLICIT
///        are all set in patomic_opcats_TRANSACTION.
TEST_F(BtTypesFeatureCheckAnyAll, opcats_implicit_explicit_subset_of_transaction)
{}

/// @brief Invalid opcat bits remain set in the return value of any/all feature
///        check.
TEST_F(BtTypesFeatureCheckAnyAll, check_invalid_opcats_unmodified)
{}

/// @brief All bits unset for patomic_ops*_t which only supports LDST ops
///        exactly matches all bits set in patomic_opcat_LDST.
TEST_F(BtTypesFeatureCheckAnyAll, check_ldst_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports XCHG ops
///        exactly matches all bits set in patomic_opcat_XCHG.
TEST_F(BtTypesFeatureCheckAnyAll, check_xchg_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports BIT ops
///        exactly matches all bits set in patomic_opcat_BIT.
TEST_F(BtTypesFeatureCheckAnyAll, check_bit_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports BIN_V ops
///        exactly matches all bits set in patomic_opcat_BIN_V.
TEST_F(BtTypesFeatureCheckAnyAll, check_bin_v_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports BIN_F ops
///        exactly matches all bits set in patomic_opcat_BIN_F.
TEST_F(BtTypesFeatureCheckAnyAll, check_bin_f_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports ARI_V ops
///        exactly matches all bits set in patomic_opcat_ARI_V.
TEST_F(BtTypesFeatureCheckAnyAll, check_ari_v_bits_expected)
{}

/// @brief All bits unset for patomic_ops*_t which only supports ARI_F ops
///        exactly matches all bits set in patomic_opcat_ARI_F.
TEST_F(BtTypesFeatureCheckAnyAll, check_ari_f_bits_expected)
{}

/// @brief All bits unset for patomic_ops_transaction_t which only supports
///        TSPEC ops exactly matches all bits set in patomic_opcat_TSPEC.
TEST_F(BtTypesFeatureCheckAnyAll, check_tspec_bits_expected)
{}

/// @brief All bits unset for patomic_ops_transaction_t which only supports
///        TFLAG ops exactly matches all bits set in patomic_opcat_TFLAG.
TEST_F(BtTypesFeatureCheckAnyAll, check_tflag_bits_expected)
{}

/// @brief All bits unset for patomic_ops_transaction_t which only supports
///        TRAW ops exactly matches all bits set in patomic_opcat_TRAW.
TEST_F(BtTypesFeatureCheckAnyAll, check_traw_bits_expected)
{}
