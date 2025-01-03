// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <patomic/api/align.h>

#include <test/common/align.hpp>
#include <test/common/math.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>


/// @brief Helper type that is over aligned.
struct OverAlignedBuffer
{
    static constexpr auto align =
        std::max(alignof(std::max_align_t), static_cast<std::size_t>(64u));
    alignas(align) unsigned char data[align] {};
    size_t size { align };
};


/// @brief Test fixture.
class BtApiAlign : public testing::Test
{};

/// @brief Test fixture for experimental tests, testing functionality which is
///        not required by the public API.
class BtApiAlign_Experimental : public testing::Test
{};


/// @brief PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE is a positive power of 2.
TEST_F(BtApiAlign, max_cache_line_size_macro_abi_unstable_is_pow2)
{
    // setup
    constexpr auto macro_unstable = PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE;

    // test
    EXPECT_TRUE(test::is_positive_pow2(macro_unstable));
}


/// @brief Return value of patomic_max_cache_line_size() is a positive power
///        of 2.
TEST_F(BtApiAlign, max_cache_line_size_fn_is_pow2)
{
    // setup
    const auto fnval = patomic_cache_line_size();

    // test
    EXPECT_TRUE(test::is_positive_pow2(fnval));
}

/// @brief Return value of patomic_max_cache_line_size() compares less than or
///        equal to PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE.
TEST_F(BtApiAlign, max_cache_line_size_fn_cmp_le_unstable_macro)
{
    // setup
    constexpr auto macro_unstable = PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE;
    const auto fnval = patomic_cache_line_size();

    // test
    EXPECT_LE(fnval, macro_unstable);
}


/// @brief The check patomic_align_meets_recommended(...) fails when
///        "recommended" is zero.
TEST_F(BtApiAlign, meets_recommended_fails_recommended_is_zero)
{
    // setup
    constexpr OverAlignedBuffer buf;
    constexpr patomic_align_t align {};
    const void *ptr = buf.data;

    // test
    EXPECT_EQ(0, align.recommended);
    EXPECT_FALSE(patomic_align_meets_recommended(ptr, align));
}

/// @brief The check patomic_align_meets_recommended(...) fails when
///        "recommended" is not a positive power of 2.
TEST_F(BtApiAlign, meets_recommended_fails_recommended_non_pow2)
{
    // setup
    constexpr OverAlignedBuffer buf;
    constexpr patomic_align_t align { 3, 0, 0 };
    const void *ptr = test::aligned_pointer(
        buf.data, buf.size, align.recommended, 1);

    // test
    // recommended is positive but not a power of 2
    EXPECT_NE(0, align.recommended);
    EXPECT_FALSE(test::is_positive_pow2(align.recommended));
    // pointer is aligned to recommended (don't use runtime_alignof because non-pow2)
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(0, reinterpret_cast<std::uintptr_t>(ptr) % align.recommended);
    // check fails
    EXPECT_FALSE(patomic_align_meets_recommended(ptr, align));
}

/// @brief The check patomic_align_meets_recommended(...) fails when
///        the given pointer's alignment is less than "recommended".
TEST_F(BtApiAlign, meets_recommended_fails_cmp_gt_pointer_align)
{
    // setup
    constexpr OverAlignedBuffer buf;
    const void *ptr = buf.data;
    const patomic_align_t align { test::runtime_alignof(ptr) * 2, 0, 0 };

    // test
    // recommended is valid
    EXPECT_TRUE(test::is_positive_pow2(align.recommended));
    // pointer is aligned less than recommended
    EXPECT_LT(test::runtime_alignof(ptr), align.recommended);
    // check fails
    EXPECT_FALSE(patomic_align_meets_recommended(ptr, align));

}

/// @brief The check patomic_align_meets_recommended(...) succeeds when
///        the given pointer's alignment equals "recommended".
TEST_F(BtApiAlign, meets_recommended_succeeds_cmp_eq_pointer_align)
{
    // setup
    constexpr OverAlignedBuffer buf;
    const void *ptr = buf.data;
    const patomic_align_t align { test::runtime_alignof(ptr), 0, 0 };

    // test
    // recommended is valid
    EXPECT_TRUE(test::is_positive_pow2(align.recommended));
    // pointer is aligned to exactly recommended
    EXPECT_EQ(test::runtime_alignof(ptr), align.recommended);
    // check succeeds
    EXPECT_TRUE(patomic_align_meets_recommended(ptr, align));
}

/// @brief The check patomic_align_meets_recommended(...) succeeds when
///        the given pointer's alignment exceeds "recommended".
TEST_F(BtApiAlign, meets_recommended_succeeds_cmp_lt_pointer_align)
{
    // setup
    constexpr OverAlignedBuffer buf;
    const void *ptr = buf.data;
    const patomic_align_t align { test::runtime_alignof(ptr) / 2, 0, 0 };

    // test
    // recommended is valid
    EXPECT_TRUE(test::is_positive_pow2(align.recommended));
    // pointer is aligned more than recommended
    EXPECT_GT(test::runtime_alignof(ptr), align.recommended);
    // check succeeds
    EXPECT_TRUE(patomic_align_meets_recommended(ptr, align));
}

/// @brief The check patomic_align_meets_recommended(...) always succeeds when
///        the given pointer is null.
///
/// @note  This test is experimental, and not an actual requirement of the API.
TEST_F(BtApiAlign_Experimental, meets_recommended_succeeds_pointer_is_null)
{
    // setup
    constexpr patomic_align_t align { 32768u, 0, 0 };
    const void *ptr = nullptr;

    // test
    EXPECT_TRUE(test::is_positive_pow2(align.recommended));
    EXPECT_TRUE(patomic_align_meets_recommended(ptr, align));
}


/// @brief The check patomic_align_meets_minimum(...) fails when "minimum" is
///        zero.
TEST_F(BtApiAlign, meets_minimum_fails_minimum_is_zero)
{
    // setup
    constexpr OverAlignedBuffer buf;
    constexpr patomic_align_t align {};

    // test
    EXPECT_EQ(0, align.minimum);
    EXPECT_EQ(0, align.size_within);
    EXPECT_FALSE(patomic_align_meets_minimum(buf.data, align, 1));
}

/// @brief The check patomic_align_meets_minimum(...) fails when "minimum" is
///        not a positive power of 2.
TEST_F(BtApiAlign, meets_minimum_fails_minimum_non_pow2)
{
    // setup
    constexpr OverAlignedBuffer buf;
    constexpr patomic_align_t align { 0, 3, 0 };
    const void *ptr = test::aligned_pointer(
        buf.data, buf.size, align.minimum, 1);

    // test
    // minimum is positive but not a power of 2
    EXPECT_NE(0, align.minimum);
    EXPECT_FALSE(test::is_positive_pow2(align.minimum));
    EXPECT_EQ(0, align.size_within);
    // pointer is aligned to minimum (don't use runtime_alignof because non-pow2)
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(0, reinterpret_cast<std::uintptr_t>(ptr) % align.minimum);
    // check fails
    EXPECT_FALSE(patomic_align_meets_minimum(ptr, align, 1));
}

/// @brief The check patomic_align_meets_minimum(...) fails when the given
///        pointer's alignment is less than "minimum".
TEST_F(BtApiAlign, meets_minimum_fails_cmp_gt_pointer_align)
{
    // setup
    constexpr OverAlignedBuffer buf;
    const void *ptr = buf.data;
    const patomic_align_t align { 0, test::runtime_alignof(ptr) * 2, 0 };

    // test
    // minimum is valid
    EXPECT_TRUE(test::is_positive_pow2(align.minimum));
    EXPECT_EQ(0, align.size_within);
    // pointer is aligned to less than minimum
    EXPECT_LT(test::runtime_alignof(ptr), align.minimum);
    // check fails
    EXPECT_FALSE(patomic_align_meets_minimum(ptr, align, 1));
}

/// @brief The check patomic_align_meets_minimum(...) succeeds when the given
///        pointer's alignment equals "minimum", and "size_within" is zero.
TEST_F(BtApiAlign, meets_minimum_succeeds_cmp_eq_pointer_align)
{
    // setup
    constexpr OverAlignedBuffer buf;
    const void *ptr = buf.data;
    const patomic_align_t align { 0, test::runtime_alignof(ptr), 0 };

    // test
    // minimum is valid
    EXPECT_TRUE(test::is_positive_pow2(align.minimum));
    EXPECT_EQ(0, align.size_within);
    // pointer is aligned to at least minimum
    EXPECT_EQ(test::runtime_alignof(ptr), align.minimum);
    // check succeeds
    EXPECT_TRUE(patomic_align_meets_minimum(ptr, align, 1));
}

/// @brief The check patomic_align_meets_minimum(...) succeeds when the given
///        pointer's alignment exceeds "minimum", and "size_within" is zero.
TEST_F(BtApiAlign, meets_minimum_succeeds_cmp_lt_pointer_align)
{
    // setup
    constexpr OverAlignedBuffer buf;
    const void *ptr = buf.data;
    const patomic_align_t align { 0, test::runtime_alignof(ptr) / 2, 0 };

    // test
    // minimum is valid
    EXPECT_TRUE(test::is_positive_pow2(align.minimum));
    EXPECT_EQ(0, align.size_within);
    // pointer is aligned to at least minimum
    EXPECT_GT(test::runtime_alignof(ptr), align.minimum);
    // check succeeds
    EXPECT_TRUE(patomic_align_meets_minimum(ptr, align, 1));
}

/// @brief The check patomic_align_meets_minimum(...) succeeds when the pointer
///        fulfills "minimum" and the buffer's size is zero for any
///        "size_within" value.
TEST_F(BtApiAlign, meets_minimum_succeeds_zero_size_buffer_any_size_within)
{
    // setup
    constexpr OverAlignedBuffer buf;
    const void *ptr = buf.data;
    patomic_align_t align { 0, test::runtime_alignof(ptr), 0 };

    // test
    // minimum is valid
    EXPECT_TRUE(test::is_positive_pow2(align.minimum));
    EXPECT_EQ(0, align.size_within);
    // pointer is aligned to at least minimum
    EXPECT_GE(test::runtime_alignof(ptr), align.minimum);
    // check succeeds for successive values of size_within with a zero sized buffer
    // should check zero and non-zero values for size_within
    for (int i = 0; i < 20; ++i)
    {
        EXPECT_TRUE(patomic_align_meets_minimum(ptr, align, 0));
        ++align.size_within;
    }
}

/// @brief The check patomic_align_meets_minimum(...) succeeds when the pointer
///        fulfills "minimum" and the buffer fits within a non-zero
///        "size_within" with extra space remaining.
TEST_F(BtApiAlign, meets_minimum_succeeds_buffer_smaller_fits_in_size_within)
{
    // setup
    constexpr OverAlignedBuffer buf;
    const void *ptr = buf.data + 1;
    constexpr patomic_align_t align { 0, 1, 8 };

    // test
    // minimum is valid
    EXPECT_TRUE(test::is_positive_pow2(align.minimum));
    EXPECT_GT(align.size_within, 2);
    // buffer is aligned to at least minimum and size_within
    // check this because pointer is offset from buffer
    EXPECT_GE(test::runtime_alignof(buf.data), align.minimum);
    EXPECT_GE(test::runtime_alignof(buf.data), align.size_within);
    // pointer is aligned to at least minimum
    EXPECT_GE(test::runtime_alignof(ptr), align.minimum);
    // check succeeds
    EXPECT_TRUE(patomic_align_meets_minimum(ptr, align, align.size_within - 2));
}

/// @brief The check patomic_align_meets_minimum(...) succeeds when the pointer
///        fulfills "minimum" and the buffer fits within a non-zero
///        "size_within" buffer exactly.
TEST_F(BtApiAlign, meets_minimum_succeeds_buffer_exactly_fits_in_size_within)
{
    // setup
    constexpr OverAlignedBuffer buf;
    const void *ptr = buf.data;
    constexpr patomic_align_t align { 0, 1, 8 };

    // test
    // minimum is valid
    EXPECT_TRUE(test::is_positive_pow2(align.minimum));
    EXPECT_NE(0, align.size_within);
    // pointer is aligned to at least minimum and size_within
    EXPECT_GE(test::runtime_alignof(ptr), align.minimum);
    EXPECT_GE(test::runtime_alignof(ptr), align.size_within);
    // check succeeds
    EXPECT_TRUE(patomic_align_meets_minimum(ptr, align, align.size_within));

}

/// @brief The check patomic_align_meets_minimum(...) fails when the pointer
///        fulfills "minimum" but the buffer's size is larger than a non-zero
///        "size_within".
TEST_F(BtApiAlign, meets_minimum_fails_buffer_larger_than_size_within)
{
    // setup
    constexpr OverAlignedBuffer buf;
    const void *ptr = buf.data;
    constexpr patomic_align_t align { 0, 1, 8 };

    // test
    // minimum is valid
    EXPECT_TRUE(test::is_positive_pow2(align.minimum));
    EXPECT_NE(0, align.size_within);
    // pointer is aligned to at least minimum and size_within
    EXPECT_GE(test::runtime_alignof(ptr), align.minimum);
    EXPECT_GE(test::runtime_alignof(ptr), align.size_within);
    // check fails
    EXPECT_FALSE(patomic_align_meets_minimum(ptr, align, align.size_within + 1));
}

/// @brief The check patomic_align_meets_minimum(...) fails when the pointer
///        fulfills "minimum" and the buffer's size is smaller than a non-zero
///        "size_within", but the buffer does not fit within the "size_within"
///        due to alignment constraints.
TEST_F(BtApiAlign, meets_minimum_fails_buffer_fits_but_misaligned_for_size_within)
{
    // setup
    // we need a pointer that is 16 bytes offset from a 64 byte aligned address
    // the buffer is 32 bytes, crossing the 64 byte alignment boundary
    alignas(64) constexpr char buffer[80] {};
    const void *ptr = buffer + (64 - 16);
    constexpr std::size_t size = 32;
    constexpr patomic_align_t align { 0, 16, 64 };

    // test
    // minimum is valid
    EXPECT_TRUE(test::is_positive_pow2(align.minimum));
    // size_within is 64 bytes but pointer is only aligned to 16 bytes
    EXPECT_EQ(64, align.size_within);
    EXPECT_EQ(16, align.minimum);
    EXPECT_EQ(16, test::runtime_alignof(ptr));
    // pointer is 16 bytes from a 64 byte alignment boundary and crosses it
    EXPECT_LE(64, test::runtime_alignof(static_cast<const char *>(ptr) + 16));
    EXPECT_GT(size, 16);
    // check fails
    EXPECT_FALSE(patomic_align_meets_minimum(ptr, align, size));
}

/// @brief The check patomic_align_meets_minimum(...) always succeeds when the
///        given pointer is null.
///
/// @note  This test is experimental, and not an actual requirement of the API.
TEST_F(BtApiAlign_Experimental, meets_minimum_succeeds_pointer_is_null)
{
    // setup
    constexpr patomic_align_t align { 0, 32768u, 8 };
    const void *ptr = nullptr;

    // test
    EXPECT_TRUE(test::is_positive_pow2(align.minimum));
    EXPECT_NE(0, align.size_within);
    EXPECT_TRUE(patomic_align_meets_minimum(ptr, align, 0));
}
