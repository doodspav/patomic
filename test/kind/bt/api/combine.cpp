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

/// @brief Templated test fixture for "slow" tests.
template <class T>
class BtApiCombineT_SlowTest : public testing::Test
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

// helper constants
constexpr patomic_align_t strong_align { 64, 64, 16 };
constexpr patomic_align_t normal_align { 32, 32, 32 };
constexpr patomic_align_t weak_align { 16, 16, 64 };
constexpr std::array<patomic_align_t, 3> aligns {
    weak_align,
    normal_align,
    strong_align
};

}  // namespace

TYPED_TEST_SUITE(
    BtApiCombineT,
    BtApiCombineT_Types,
    TTestHelper
);

TYPED_TEST_SUITE(
    BtApiCombineT_SlowTest,
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
    BaseT copied_to { test::make_ops_all_nonnull<D>(), strong_align };
    const BaseT copied_from { test::make_ops_all_nonnull<D>(), weak_align };

    // test
    // check alignment is weaker on copied-from
    EXPECT_LT(copied_from.align.recommended, copied_to.align.recommended);
    EXPECT_LT(copied_from.align.minimum, copied_to.align.minimum);
    EXPECT_GT(copied_from.align.size_within, copied_to.align.size_within);
    EXPECT_GT(copied_to.align.size_within, 0);
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
    BaseT copied_to { test::make_ops_all_nonnull<D>(nullptr), strong_align };
    const BaseT copied_from { test::make_ops_all_nonnull<D>(nullptr), weak_align };

    // test
    // check alignment is weaker on copied-from
    EXPECT_LT(copied_from.align.recommended, copied_to.align.recommended);
    EXPECT_LT(copied_from.align.minimum, copied_to.align.minimum);
    EXPECT_GT(copied_from.align.size_within, copied_to.align.size_within);
    EXPECT_GT(copied_to.align.size_within, 0);
    // check ops compare equal and are null (if one op is, they all should be)
    EXPECT_EQ(copied_from.ops.fp_store, copied_to.ops.fp_store);
    EXPECT_EQ(nullptr, copied_from.ops.fp_store);
    // check that alignment doesn't change
    EXPECT_EQ(copied_to.align, strong_align);
    TTestHelper::combine(copied_to, copied_from);
    EXPECT_EQ(copied_to.align, strong_align);
}

/// @brief The size_within member is considered less strict when its value is
///        zero (copied-to) than when its value is non-zero (copied-from) for
///        all ops.
TYPED_TEST(BtApiCombineT, combine_size_within_to_zero_less_strict_than_from_non_zero)
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

/// @brief The size_within member is considered less strict when its value is
///        zero (copied-from) than when its value is non-zero (copied-to) for
///        all ops.
TYPED_TEST(BtApiCombineT, combine_size_within_from_zero_less_strict_than_to_non_zero)
{
    // setup
    constexpr test::ops_domain D = TestFixture::domain;
    using BaseT = typename TestFixture::OpsTypes::base_t;
    BaseT copied_to {
        test::make_ops_all_nonnull<D>(nullptr),
        patomic_align_t { 1, 1, 1 }
    };
    const BaseT copied_from {
        test::make_ops_all_nonnull<D>(),
        patomic_align_t { 1, 1, 0 }
    };

    // test
    // check alignment is equal but only copied-from has zero size_within
    EXPECT_EQ(copied_from.align.recommended, copied_to.align.recommended);
    EXPECT_EQ(copied_from.align.minimum, copied_to.align.minimum);
    EXPECT_EQ(0, copied_from.align.size_within);
    EXPECT_NE(0, copied_to.align.size_within);
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
{
    // setup
    constexpr test::ops_domain D = TestFixture::domain;
    using OpsT = typename TestFixture::OpsTypes::ldst_t;
    using BaseT = typename TestFixture::OpsTypes::base_t;

    for (const auto& xchg_from : test::make_ops_xchg_combinations<D>(&fn_a))
    {
    for (const auto& xchg_to : test::make_ops_xchg_combinations<D>(&fn_b))
    {
    for (const auto align_from : aligns)
    {
        // patomic_ops*_t objects
        OpsT ops_to {};
        ops_to.xchg_ops = xchg_to.ops;
        OpsT ops_from {};
        ops_from.xchg_ops = xchg_from.ops;
        // patomic*_t objects
        BaseT combined { ops_to, normal_align };
        const BaseT copied_to = combined;
        const BaseT copied_from { ops_from, align_from };
        // arrays of relevant ops
        const auto arr_to = test::make_ops_xchg_array<D>(copied_to.ops.xchg_ops);
        const auto arr_from = test::make_ops_xchg_array<D>(copied_from.ops.xchg_ops);

        // do combine
        TTestHelper::combine(combined, copied_from);
        const auto arr_combined = test::make_ops_xchg_array<D>(combined.ops.xchg_ops);

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

/// @brief Calling combine with all combinations of BIT ops set in both
///        operands, with all combinations of alignment (stronger, equal,
///        weaker), copies over the correct ops and adjusts the alignment
///        correctly. Non-null ops compare unequal.
TYPED_TEST(BtApiCombineT, combine_all_bitwise_combinations_correct_result)
{
    // setup
    constexpr test::ops_domain D = TestFixture::domain;
    using OpsT = typename TestFixture::OpsTypes::ldst_t;
    using BaseT = typename TestFixture::OpsTypes::base_t;

    for (const auto& bitwise_from : test::make_ops_bitwise_combinations<D>(&fn_a))
    {
    for (const auto& bitwise_to : test::make_ops_bitwise_combinations<D>(&fn_b))
    {
    for (const auto align_from : aligns)
    {
        // patomic_ops*_t objects
        OpsT ops_to {};
        ops_to.bitwise_ops = bitwise_to.ops;
        OpsT ops_from {};
        ops_from.bitwise_ops = bitwise_from.ops;
        // patomic*_t objects
        BaseT combined { ops_to, normal_align };
        const BaseT copied_to = combined;
        const BaseT copied_from { ops_from, align_from };
        // arrays of relevant ops
        const auto arr_to = test::make_ops_bitwise_array<D>(copied_to.ops.bitwise_ops);
        const auto arr_from = test::make_ops_bitwise_array<D>(copied_from.ops.bitwise_ops);

        // do combine
        TTestHelper::combine(combined, copied_from);
        const auto arr_combined = test::make_ops_bitwise_array<D>(combined.ops.bitwise_ops);

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

/// @brief Calling combine with all combinations of BIN ops set in both
///        operands, with all combinations of alignment (stronger, equal,
///        weaker), copies over the correct ops and adjusts the alignment
///        correctly. Non-null ops compare unequal.
TYPED_TEST(BtApiCombineT_SlowTest, combine_all_binary_combinations_correct_result)
{
    // setup
    constexpr test::ops_domain D = TestFixture::domain;
    using OpsT = typename TestFixture::OpsTypes::ldst_t;
    using BaseT = typename TestFixture::OpsTypes::base_t;

    for (const auto& binary_from : test::make_ops_binary_combinations<D>(&fn_a))
    {
    for (const auto& binary_to : test::make_ops_binary_combinations<D>(&fn_b))
    {
    for (const auto align_from : aligns)
    {
        // patomic_ops*_t objects
        OpsT ops_to {};
        ops_to.binary_ops = binary_to.ops;
        OpsT ops_from {};
        ops_from.binary_ops = binary_from.ops;
        // patomic*_t objects
        BaseT combined { ops_to, normal_align };
        const BaseT copied_to = combined;
        const BaseT copied_from { ops_from, align_from };
        // arrays of relevant ops
        const auto arr_to = test::make_ops_binary_array<D>(copied_to.ops.binary_ops);
        const auto arr_from = test::make_ops_binary_array<D>(copied_from.ops.binary_ops);

        // do combine
        TTestHelper::combine(combined, copied_from);
        const auto arr_combined = test::make_ops_binary_array<D>(combined.ops.binary_ops);

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

/// @brief Calling combine with all combinations of ARI ops set in both
///        operands, with all combinations of alignment (stronger, equal,
///        weaker), copies over the correct ops and adjusts the alignment
///        correctly. Non-null ops compare unequal.
TYPED_TEST(BtApiCombineT_SlowTest, combine_all_arithmetic_combinations_correct_result)
{
    // setup
    constexpr test::ops_domain D = TestFixture::domain;
    using OpsT = typename TestFixture::OpsTypes::ldst_t;
    using BaseT = typename TestFixture::OpsTypes::base_t;

    for (const auto& arithmetic_from : test::make_ops_arithmetic_combinations<D>(&fn_a))
    {
    for (const auto& arithmetic_to : test::make_ops_arithmetic_combinations<D>(&fn_b))
    {
    for (const auto align_from : aligns)
    {
        // patomic_ops*_t objects
        OpsT ops_to {};
        ops_to.arithmetic_ops = arithmetic_to.ops;
        OpsT ops_from {};
        ops_from.arithmetic_ops = arithmetic_from.ops;
        // patomic*_t objects
        BaseT combined { ops_to, normal_align };
        const BaseT copied_to = combined;
        const BaseT copied_from { ops_from, align_from };
        // arrays of relevant ops
        const auto arr_to = test::make_ops_arithmetic_array<D>(copied_to.ops.arithmetic_ops);
        const auto arr_from = test::make_ops_arithmetic_array<D>(copied_from.ops.arithmetic_ops);

        // do combine
        TTestHelper::combine(combined, copied_from);
        const auto arr_combined = test::make_ops_arithmetic_array<D>(combined.ops.arithmetic_ops);

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
