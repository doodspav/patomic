extern "C" {
#include <patomic/internal/align.h>
}

#include <gtest/gtest.h>


/// @brief Test fixture.
class UtInternalAlign : public testing::Test
{};


/// @brief Comparing patomic_align_t prioritizes recommend over all other
///        members. Larger recommended is stricter (and compares greater).
TEST_F(UtInternalAlign, compare_align_recommended_ne)
{
    // setup
    // larger recommended is stricter
    patomic_align_t lt { 1, 16, 16 };
    patomic_align_t gt { 2, 8, 8 };

    // test
    // check values
    EXPECT_LT(lt.recommended, gt.recommended);
    EXPECT_GT(lt.minimum, gt.minimum);
    EXPECT_GT(lt.size_within, gt.size_within);
    // do comparison
    EXPECT_LT(patomic_internal_compare_align(lt, gt), 0);
    EXPECT_GT(patomic_internal_compare_align(gt, lt), 0);
}

/// @brief Comparing patomic_align_t prioritizes minimum over all other members
///        if recommend compares equal. Larger minimum is stricter (and
///        compares greater).
TEST_F(UtInternalAlign, compare_align_minimum_ne_recommended_eq)
{
    // setup
    // larger recommended is stricter
    patomic_align_t lt { 64, 1, 16 };
    patomic_align_t gt { 64, 2, 8 };

    // test
    // check values
    EXPECT_EQ(lt.recommended, gt.recommended);
    EXPECT_LT(lt.minimum, gt.minimum);
    EXPECT_GT(lt.size_within, gt.size_within);
    // do comparison
    EXPECT_LT(patomic_internal_compare_align(lt, gt), 0);
    EXPECT_GT(patomic_internal_compare_align(gt, lt), 0);
}

/// @brief Comparing patomic_align_t prioritizes size_within after all other
///        members. Larger size_within is less strict (and compares lesser),
///        except for 0 which is the least strict.
TEST_F(UtInternalAlign, compare_align_size_within_any_minimum_eq_recommended_eq)
{
    // setup
    // smaller size_within is stricter (except zero which is least strict)
    patomic_align_t zero { 64, 32, 0 };
    patomic_align_t lt { 64, 32, 2 };
    patomic_align_t gt { 64, 32, 1 };

    // test
    // check values
    EXPECT_EQ(0, zero.size_within);
    EXPECT_EQ(lt.recommended, gt.recommended);
    EXPECT_EQ(lt.minimum, gt.minimum);
    EXPECT_GT(lt.size_within, gt.size_within);
    // do comparison
    EXPECT_EQ(patomic_internal_compare_align(zero, zero), 0);
    EXPECT_EQ(patomic_internal_compare_align(lt, lt), 0);
    EXPECT_EQ(patomic_internal_compare_align(gt, gt), 0);
    EXPECT_LT(patomic_internal_compare_align(zero, lt), 0);
    EXPECT_LT(patomic_internal_compare_align(lt, gt), 0);
    EXPECT_GT(patomic_internal_compare_align(gt, lt), 0);
    EXPECT_GT(patomic_internal_compare_align(lt, zero), 0);
}
