#include <patomic/api/combine.h>

#include <test/common/compare.hpp>
#include <test/common/make_ops.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <string>
#include <type_traits>


/// @brief Templated test fixture.
template <class T>
class BtApiCombineT : public testing::Test
{
public:
    static constexpr test::ops_domain domain = T::value;
    using OpsTypes = test::ops_types<domain>;
};

using BtApiCombineT_Types = ::testing::Types<
    std::integral_constant<test::ops_domain, test::ops_domain::IMPLICIT>,
    std::integral_constant<test::ops_domain, test::ops_domain::EXPLICIT>
>;

namespace
{

/// @brief Helper type for templated test fixture.
class TTestHelper
{
public:
    template <class T>
    static std::string
    GetName(int)
    {
        auto name = test::to_string(T::value);
        std::transform(name.begin(), name.end(), name.begin(),
                       [](unsigned char c) noexcept -> char {
            return static_cast<char>(std::tolower(c));
        });
        return name;
    }

    static void
    combine(patomic_t& priority, const patomic_t& other) noexcept
    {
        return patomic_combine(&priority, &other);
    }

    static void
    combine(patomic_explicit_t& priority, const patomic_explicit_t& other) noexcept
    {
        return patomic_combine_explicit(&priority, &other);
    }
};

/// @brief Used to obtain a distinct non-null function pointer.
void fn_a() noexcept {}

/// @brief Used to obtain a distinct non-null function pointer.
void fn_b() noexcept {}

/// @brief Combine align "from" into "to".
patomic_align_t
combine_align(const patomic_align_t& to, const patomic_align_t& from) noexcept
{
    patomic_align_t ret;
    ret.recommended = std::max(to.recommended, from.recommended);
    ret.minimum = std::max(to.minimum, from.minimum);
    if (to.size_within == 0)
    {
        ret.size_within = from.size_within;
    }
    else if (from.size_within == 0)
    {
        ret.size_within = to.size_within;
    }
    else
    {
        ret.size_within = std::min(to.size_within, from.size_within);
    }
    return ret;
}

}  // namespace

TYPED_TEST_SUITE(
    BtApiCombineT,
    BtApiCombineT_Types,
    TTestHelper
);


/// @brief Calling combine with two operands where both have all ops non-null
///        and compare equal and the copied-from alignment is weaker does not
///        modify the alignment.
TYPED_TEST(BtApiCombineT, combine_equal_nonnull_ops_does_not_optimize_align)
{
    // setup
    constexpr test::ops_domain D = TestFixture::domain;
    using BaseT = typename TestFixture::OpsTypes::base_t;
    constexpr patomic_align_t weak_align { 1, 1, 0 };
    constexpr patomic_align_t strong_align { 64, 64, 64 };
    BaseT copied_to { test::make_ops_all_nonnull<D>(), strong_align };
    const BaseT copied_from { test::make_ops_all_nonnull<D>(), weak_align };

    // test
    // check alignment is weaker on copied-from
    EXPECT_LT(copied_from.align.recommended, copied_to.align.recommended);
    EXPECT_LT(copied_from.align.minimum, copied_to.align.minimum);
    EXPECT_EQ(0, copied_from.align.size_within);
    EXPECT_NE(0, copied_to.align.size_within);
    // check ops compare equal and are non-null (if one op is, they all should be)
    EXPECT_EQ(copied_from.ops.fp_store, copied_to.ops.fp_store);
    EXPECT_NE(nullptr, copied_from.ops.fp_store);
    // check that alignment doesn't change
    EXPECT_EQ(copied_to.align, strong_align);
    TTestHelper::combine(copied_to, copied_from);
    EXPECT_EQ(copied_to.align, strong_align);
}

/// @brief Calling combine with two operands where both have all ops null and
///        the copied-from alignment is weaker does not modify the alignment.
TYPED_TEST(BtApiCombineT, combine_equal_null_ops_does_not_optimize_align)
{
    // setup
    constexpr test::ops_domain D = TestFixture::domain;
    using BaseT = typename TestFixture::OpsTypes::base_t;
    constexpr patomic_align_t weak_align { 1, 1, 0 };
    constexpr patomic_align_t strong_align { 64, 64, 64 };
    BaseT copied_to { test::make_ops_all_nonnull<D>(nullptr), strong_align };
    const BaseT copied_from { test::make_ops_all_nonnull<D>(nullptr), weak_align };

    // test
    // check alignment is weaker on copied-from
    EXPECT_LT(copied_from.align.recommended, copied_to.align.recommended);
    EXPECT_LT(copied_from.align.minimum, copied_to.align.minimum);
    EXPECT_EQ(0, copied_from.align.size_within);
    EXPECT_NE(0, copied_to.align.size_within);
    // check ops compare equal and are null (if one op is, they all should be)
    EXPECT_EQ(copied_from.ops.fp_store, copied_to.ops.fp_store);
    EXPECT_EQ(nullptr, copied_from.ops.fp_store);
    // check that alignment doesn't change
    EXPECT_EQ(copied_to.align, strong_align);
    TTestHelper::combine(copied_to, copied_from);
    EXPECT_EQ(copied_to.align, strong_align);
}

/// @brief The size_within member is considered less strict when its value is
///        zero than when its value is non-zero for all ops.
TYPED_TEST(BtApiCombineT, combine_size_within_zero_less_strict_than_non_zero)
{
    // setup
    constexpr test::ops_domain D = TestFixture::domain;
    using BaseT = typename TestFixture::OpsTypes::base_t;
    BaseT copied_to {
        test::make_ops_all_nonnull<D>(nullptr),
        patomic_align_t { 1, 1, 0 }
    };
    const BaseT copied_from {
        test::make_ops_all_nonnull<D>(),
        patomic_align_t { 1, 1, 1 }
    };

    // test
    // check alignment is equal but only copied-to has zero size_within
    EXPECT_EQ(copied_from.align.recommended, copied_to.align.recommended);
    EXPECT_EQ(copied_from.align.minimum, copied_to.align.minimum);
    EXPECT_NE(0, copied_from.align.size_within);
    EXPECT_EQ(0, copied_to.align.size_within);
    // check ops are null for copied-to, and non-null for copied-from
    EXPECT_EQ(nullptr, copied_to.ops.fp_store);
    EXPECT_NE(nullptr, copied_from.ops.fp_store);
    // check that size_within is updated to be non-zero
    TTestHelper::combine(copied_to, copied_from);
    EXPECT_NE(0, copied_to.align.size_within);
}

/// @brief Calling combine with all combinations of LDST ops set in both
///        operands, with all combinations of alignment (stronger, equal,
///        weaker), copies over the correct ops and adjusts the alignment
///        correctly. Non-null ops compare unequal.
TYPED_TEST(BtApiCombineT, combine_all_ldst_combinations_correct_result)
{
    // setup
    constexpr test::ops_domain D = TestFixture::domain;
    using BaseT = typename TestFixture::OpsTypes::base_t;
    // alignments
    constexpr patomic_align_t strong_align { 64, 64, 16 };
    constexpr patomic_align_t normal_align { 32, 32, 32 };
    constexpr patomic_align_t weak_align { 16, 16, 64 };
    constexpr std::array<patomic_align_t, 3> aligns {
        weak_align,
        normal_align,
        strong_align
    };


    for (const auto& ldst_from : test::make_ops_ldst_combinations<D>(&fn_a))
    {
    for (const auto& ldst_to : test::make_ops_ldst_combinations<D>(&fn_b))
    {
    for (const auto align_from : aligns)
    {
        // patomic*_t objects
        BaseT combined { ldst_to.ops, normal_align };
        const BaseT copied_to = combined;
        const BaseT copied_from { ldst_from.ops, align_from };
        // arrays of relevant ops
        const auto arr_to = test::make_ops_ldst_array<D>(copied_to.ops);
        const auto arr_from = test::make_ops_ldst_array<D>(copied_from.ops);

        // do combine
        TTestHelper::combine(combined, copied_from);
        const auto arr_combined = test::make_ops_ldst_array<D>(combined.ops);

        // go through result
        bool any_ops_copied = false;
        for (std::size_t i = 0; i < arr_to.size(); ++i)
        {
            // test
            if (arr_to[i] == nullptr && arr_from[i] != nullptr)
            {
                // if "to" is null, then we took "from"'s value
                EXPECT_EQ(arr_combined[i], arr_from[i]);
                any_ops_copied = true;
            }
            else
            {
                // if "to" is not null, then it didn't change
                EXPECT_EQ(arr_combined[i], arr_to[i]);
            }
        }

        // check alignment is copied correctly
        if (any_ops_copied)
        {
            const auto combined_align =
                combine_align(copied_to.align, copied_from.align);
            EXPECT_EQ(combined.align, combined_align);
        }
        else
        {
            EXPECT_EQ(combined.align, copied_to.align);
        }
    }}}
}

/// @brief Calling combine with all combinations of XCHG ops set in both
///        operands, with all combinations of alignment (stronger, equal,
///        weaker), copies over the correct ops and adjusts the alignment
///        correctly. Non-null ops compare unequal.
TYPED_TEST(BtApiCombineT, combine_all_xchg_combinations_correct_result)
{}

/// @brief Calling combine with all combinations of BIT ops set in both
///        operands, with all combinations of alignment (stronger, equal,
///        weaker), copies over the correct ops and adjusts the alignment
///        correctly. Non-null ops compare unequal.
TYPED_TEST(BtApiCombineT, combine_all_bitwise_combinations_correct_result)
{}

/// @brief Calling combine with all combinations of BIN ops set in both
///        operands, with all combinations of alignment (stronger, equal,
///        weaker), copies over the correct ops and adjusts the alignment
///        correctly. Non-null ops compare unequal.
TYPED_TEST(BtApiCombineT, combine_all_binary_combinations_correct_result)
{}

/// @brief Calling combine with all combinations of ARI ops set in both
///        operands, with all combinations of alignment (stronger, equal,
///        weaker), copies over the correct ops and adjusts the alignment
///        correctly. Non-null ops compare unequal.
TYPED_TEST(BtApiCombineT, combine_all_arithmetic_combinations_correct_result)
{}














/// @brief Calling combine with all combinations of LDST function pointers set
///        in priority and other copies over the correct operations.
TYPED_TEST(BtApiCombineT, hii)
{
    // setup
    constexpr auto D = TestFixture::domain;
    typename TestFixture::OpsTypes::base_t combined {};
    typename TestFixture::OpsTypes::base_t other {};
    for (const auto& priority_ldst : test::make_ops_ldst_combinations<D>(&fn_a))
    {
    for (const auto& other_ldst : test::make_ops_ldst_combinations<D>(&fn_b))
    {
        combined.ops = priority_ldst.ops;
        other.ops = other_ldst.ops;
        TTestHelper::combine(combined, other);
        auto priority_fps = test::make_ops_ldst_array<D>(priority_ldst.ops);
        auto other_fps = test::make_ops_ldst_array<D>(other_ldst.ops);
        auto combined_fps = test::make_ops_ldst_array<D>(combined.ops);

        // test
        ASSERT_EQ(combined_fps.size(), priority_fps.size());
        ASSERT_EQ(combined_fps.size(), other_fps.size());
        for (std::size_t i = 0; i < combined_fps.size(); ++i)
        {
            if (priority_fps[i] != nullptr)
            {
                EXPECT_EQ(combined_fps[i], priority_fps[i]);
            }
            else
            {
                EXPECT_EQ(combined_fps[i], other_fps[i]);
            }
        }
    }}
}

/// @brief Calling combine with no operations being copied over does not affect
///        the alignment requirements.
TYPED_TEST(BtApiCombineT, hii2)
{
    // setup
    constexpr auto D = TestFixture::domain;
    // only combined has non-null ops, so nothing copied over from other
    typename TestFixture::OpsTypes::base_t combined {};
    typename TestFixture::OpsTypes::base_t other {};
    const typename TestFixture::OpsTypes::ldst_t nonnull = test::make_ops_all_nonnull<D>();
    combined.ops.fp_store = nonnull.fp_store;
    combined.ops.fp_load = nonnull.fp_load;
    // other has stricter alignment requirements than combined
    combined.align = patomic_align_t { 1, 1, 64 };
    other.align = patomic_align_t { 8, 8, 32 };
    // combine should do nothing
    TTestHelper::combine(combined, other);

    // testy
    EXPECT_EQ(combined.align.recommended, 1);
    EXPECT_EQ(combined.align.minimum, 1);
    EXPECT_EQ(combined.align.size_within, 64);
}
