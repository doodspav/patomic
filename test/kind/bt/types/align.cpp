#include <patomic/types/align.h>

#include <gtest/gtest.h>


/// @brief Test fixture.
class BtTypesAlign : public testing::Test
{};

/// @brief Test fixture for experimental tests, testing functionality which is
///        not required by the public API.
class BtTypesAlign_Experimental : BtTypesAlign
{};


/// @brief PATOMIC_MAX_CACHE_LINE_SIZE is a power of 2.
TEST_F(BtTypesAlign, max_cache_line_size_macro_is_pow2)
{}

/// @brief PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE is a power of 2.
TEST_F(BtTypesAlign, max_cache_line_size_macro_abi_unstable_is_pow2)
{}

/// @brief PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE compares less than or equal
///        to PATOMIC_MAX_CACHE_LINE_SIZE.
TEST_F(BtTypesAlign, max_cache_line_size_macro_cmp_ge_unstable)
{}


/// @brief Return value of patomic_max_cache_line_size() is a power of 2.
TEST_F(BtTypesAlign, max_cache_line_size_fn_is_pow2)
{}

/// @brief Return value of patomic_max_cache_line_size() compares less than or
///        equal to PATOMIC_MAX_CACHE_LINE_SIZE.
TEST_F(BtTypesAlign, max_cache_line_size_fn_cmp_le_macro)
{}

/// @brief Return value of patomic_max_cache_line_size() compares less than or
///        equal to PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE.
TEST_F(BtTypesAlign, max_cache_line_size_fn_cmp_le_unstable_macro)
{}


/// @brief The check patomic_align_meets_recommended(...) fails when
///        "recommended" is zero.
TEST_F(BtTypesAlign, meets_recommended_fails_recommended_is_zero)
{}

/// @brief The check patomic_align_meets_recommended(...) fails when
///        "recommended" is not a power of 2.
TEST_F(BtTypesAlign, meets_recommended_fails_recommended_non_pow2)
{}

/// @brief The check patomic_align_meets_recommended(...) fails when
///        the given pointer's alignment is less than "recommended".
TEST_F(BtTypesAlign, meets_recommended_fails_cmp_gt_pointer_align)
{}

/// @brief The check patomic_align_meets_recommended(...) succeeds when
///        the given pointer's alignment equals "recommended".
TEST_F(BtTypesAlign, meets_recommended_succeeds_cmp_eq_pointer_align)
{}

/// @brief The check patomic_align_meets_recommended(...) succeeds when
///        the given pointer's alignment exceeds "recommended".
TEST_F(BtTypesAlign, meets_recommended_succeeds_cmp_lt_pointer_align)
{}

/// @brief The check patomic_align_meets_recommended(...) always succeeds when
///        the given pointer is null.
///
/// @note  This test is experimental, and not an actual requirement of the API.
TEST_F(BtTypesAlign_Experimental, meets_recommended_succeeds_pointer_is_null)
{}


/// @brief The check patomic_align_meets_minimum(...) fails when "minimum" is
///        zero.
TEST_F(BtTypesAlign, meets_minimum_fails_minimum_is_zero)
{}

/// @brief The check patomic_align_meets_minimum(...) fails when "minimum" is
///        not a power of 2.
TEST_F(BtTypesAlign, meets_minimum_fails_minimum_non_pow2)
{}

/// @brief The check patomic_align_meets_minimum(...) fails when the given
///        pointer's alignment is less than "minimum".
TEST_F(BtTypesAlign, meets_minimum_fails_cmp_gt_pointer_align)
{}

/// @brief The check patomic_align_meets_minimum(...) succeeds when the given
///        pointer's alignment equals "minimum", and "size_within" is zero.
TEST_F(BtTypesAlign, meets_minimum_succeeds_cmp_eq_pointer_align)
{}

/// @brief The check patomic_align_meets_minimum(...) succeeds when the given
///        pointer's alignment exceeds "minimum", and "size_within" is zero.
TEST_F(BtTypesAlign, meets_minimum_succeeds_cmp_lt_pointer_align)
{}

/// @brief The check patomic_align_meets_minimum(...) succeeds when the pointer
///        fulfills "minimum" and the buffer's size is zero for any
///        "size_within" value.
TEST_F(BtTypesAlign, meets_minimum_succeeds_zero_size_buffer_any_size_within)
{}

/// @brief The check patomic_align_meets_minimum(...) succeeds when the pointer
///        fulfills "minimum" and the buffer fits within a non-zero
///        "size_within" with extra space remaining.
TEST_F(BtTypesAlign, meets_minimum_succeeds_buffer_smaller_fits_in_size_within)
{}

/// @brief The check patomic_align_meets_minimum(...) succeeds when the pointer
///        fulfills "minimum" and the buffer fits within a non-zero
///        "size_within" buffer exactly.
TEST_F(BtTypesAlign, meets_minimum_succeeds_buffer_exactly_fits_in_size_within)
{}

/// @brief The check patomic_align_meets_minimum(...) fails when the pointer
///        fulfills "minimum" but the buffer's size is larger than a non-zero
///        "size_within".
TEST_F(BtTypesAlign, meets_minimum_fails_buffer_larger_than_size_within)
{}

/// @brief The check patomic_align_meets_minimum(...) fails when the pointer
///        fulfills "minimum" and the buffer's size is smaller than a non-zero
///        "size_within", but the buffer does not fit within the "size_within"
///        due to alignment constraints.
TEST_F(BtTypesAlign, meets_minimum_fails_buffer_fits_but_misaligned_for_size_within)
{}

/// @brief The check patomic_align_meets_minimum(...) always succeeds when the
///        given pointer is null.
///
/// @note  This test is experimental, and not an actual requirement of the API.
TEST_F(BtTypesAlign_Experimental, meets_minimum_succeeds_pointer_is_null)
{}
