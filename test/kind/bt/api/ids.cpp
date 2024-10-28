#include <patomic/api/ids.h>

#include <test/common/death.hpp>
#include <test/common/math.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <map>
#include <set>
#include <vector>


/// @brief Test fixture.
class BtApiIds : public testing::Test
{
public:
    const std::map<patomic_id_t, patomic_kind_t> impls_id_to_kind {
        { patomic_id_NULL, patomic_kind_UNKN },
        { patomic_id_STDC, patomic_kind_BLTN}
    };

    const std::vector<patomic_id_t> ids {
        patomic_id_NULL,
        patomic_id_STDC
    };

    const std::vector<patomic_kind_t> kinds {
        patomic_kind_UNKN,
        patomic_kind_DYN,
        patomic_kind_OS,
        patomic_kind_LIB,
        patomic_kind_BLTN,
        patomic_kind_ASM
    };
};

/// @brief Test fixture for death tests.
class BtApiIds_DeathTest : public testing::Test
{};


/// @brief All ids are unique.
TEST_F(BtApiIds, all_ids_are_unique)
{
    // setup
    const std::set<patomic_id_t> ids_set { ids.begin(), ids.end() };

    // test
    EXPECT_EQ(ids.size(), impls_id_to_kind.size());
    EXPECT_EQ(ids.size(), ids_set.size());
}

/// @brief All kinds are unique.
TEST_F(BtApiIds, all_kinds_are_unique)
{
    // setup
    const std::set<patomic_kind_t> kinds_set { kinds.begin(), kinds.end() };

    // test
    EXPECT_EQ(kinds.size(), kinds_set.size());
}

/// @brief All ids have exactly zero or one bits set (except for ALL).
TEST_F(BtApiIds, all_ids_have_zero_or_one_bits_set)
{
    // setup
    for (const auto impl_it : impls_id_to_kind)
    {
        const unsigned long id = impl_it.first;

        // test
        if (id == patomic_id_NULL)
        {
            EXPECT_EQ(0, id);
        }
        else
        {
            EXPECT_TRUE(test::is_positive_pow2(id));
        }
    }
}

/// @brief All kinds have exactly zero or one bits set (except for ALL).
TEST_F(BtApiIds, all_kinds_have_zero_or_one_bits_set)
{
    // test
    for (const patomic_kind_t kind : kinds)
    {
        if (kind == patomic_kind_UNKN)
        {
            EXPECT_EQ(0, kind);
        }
        else
        {
            EXPECT_TRUE(test::is_positive_pow2(static_cast<int>(kind)));
        }
    }
}

/// @brief Each id's kind has exactly zero or one bits set.
TEST_F(BtApiIds, each_ids_kind_has_zero_or_one_bits_set)
{
    // setup
    for (const auto impl_it : impls_id_to_kind)
    {
        const patomic_kind_t kind = impl_it.second;

        // test
        if (kind == patomic_kind_UNKN)
        {
            EXPECT_EQ(0, kind);
        }
    }
}

/// @brief The value of patomic_kinds_ALL is exactly the value of all the other
///        kinds combined.
TEST_F(BtApiIds, all_kinds_matches_exactly_each_kind)
{
    // setup
    int all_kinds {};
    for (const patomic_kind_t kind : kinds)
    {
        all_kinds |= kind;
    }

    // test
    EXPECT_EQ(patomic_kinds_ALL, all_kinds);
}

/// @brief The value of patomic_ids_ALL is at least the value of all the other
///        ids combined.
TEST_F(BtApiIds, all_ids_matches_at_least_each_id)
{
    // setup
    unsigned long all_ids {};
    for (const auto impl_it : impls_id_to_kind)
    {
        all_ids |= impl_it.first;
    }

    // test
    EXPECT_EQ(patomic_ids_ALL & all_ids, all_ids);
}

/// @brief All expected ids match a bit set in patomic_kinds_ALL.
TEST_F(BtApiIds, all_expected_ids_are_provided)
{
    // setup
    const unsigned long all_ids = patomic_get_ids(patomic_kinds_ALL);
    for (const auto impl_it : impls_id_to_kind)
    {
        const unsigned long id = impl_it.first;

        // test
        EXPECT_EQ(all_ids&  id, id);
    }
}

/// @brief There are no bits set in patomic_kinds_ALL which do not match an expected id.
TEST_F(BtApiIds, no_unexpected_ids_are_provided)
{
    // setup
    unsigned long all_ids = patomic_get_ids(patomic_kinds_ALL);
    for (const auto impl_it : impls_id_to_kind)
    {
        const unsigned long id = impl_it.first;
        all_ids ^= id;
    }

    // test
    EXPECT_EQ(0, all_ids);
}

/// @brief All combinations of valid kinds yields the expected combinations of
///        ids.
TEST_F(BtApiIds, get_ids_gives_correct_ids_for_all_kind_combinations)
{
    // calculate all kind combinations
    std::vector<int> all_kind_combos;
    all_kind_combos.resize(1ull << kinds.size());
    for (std::size_t i = 0; i < all_kind_combos.size(); ++i)
    {
        int kind_combo = 0;
        for (std::size_t j = 0; j < kinds.size(); ++j)
        {
            if (i & (1ull << j))
            {
                kind_combo |= kinds[j];
            }
        }
        all_kind_combos[i] = kind_combo;
    }

    // skip empty set
    all_kind_combos.erase(all_kind_combos.begin());


    // calculate resulting ids
    std::vector<unsigned long> all_id_results;
    for (const int kind_combo: all_kind_combos)
    {
        unsigned long id_combo = 0;
        for (const auto& impl_it : impls_id_to_kind)
        {
            if (kind_combo&  impl_it.second)
            {
                id_combo |= impl_it.first;
            }
        }
        all_id_results.push_back(id_combo);
    }

    // test
    ASSERT_EQ(all_kind_combos.size(), all_id_results.size());
    for (std::size_t i = 0; i < all_kind_combos.size(); ++i)
    {
        const int kinds = all_kind_combos[i];
        const unsigned long ids = all_id_results[i];
        EXPECT_EQ(ids, patomic_get_ids(kinds));
    }
}

/// @brief Invalid kinds are ignored and if no valid kinds are provided, the
///        NULL implementation id is returned.
TEST_F(BtApiIds, get_ids_ignores_invalid_kinds)
{
    // setup
    ASSERT_FALSE(kinds.empty());
    const auto invalid_kind = kinds.back() << 1;
    EXPECT_EQ(kinds.end(), std::find(kinds.begin(), kinds.end(), invalid_kind));
    const auto stdc_kind = impls_id_to_kind.at(patomic_id_STDC);

    // test
    EXPECT_EQ(patomic_id_NULL, patomic_get_ids(invalid_kind));
    EXPECT_EQ(patomic_id_STDC, patomic_get_ids(invalid_kind | stdc_kind));
}

/// @brief The corresponding kind is returned for each valid id.
TEST_F(BtApiIds, get_kind_gives_correct_kind_for_all_ids)
{
    // setup
    for (const auto impl_it : impls_id_to_kind)
    {
        const unsigned long id = impl_it.first;
        const patomic_kind_t kind = impl_it.second;

        // test
        EXPECT_EQ(kind, patomic_get_kind(id));
    }
}

/// @brief The UNKN kind is returned for any invalid id.
TEST_F(BtApiIds, get_kind_gives_kind_unkn_for_invalid_id)
{
    // setup
    unsigned long id = 1;
    while (id != 0 && impls_id_to_kind.count(id) != 0)
    {
        id <<= 1;
    }

    // test
    if (id != 0)
    {
        ASSERT_TRUE(test::is_positive_pow2(id));
        EXPECT_EQ(patomic_get_kind(id), patomic_kind_UNKN);
    }
}


/// @brief Passing multiple ids (multiple bits set) is fatally asserted.
TEST_F(BtApiIds_DeathTest, get_kinds_asserts_on_multiple_ids)
{
    // setup
    constexpr unsigned long multiple_bits_set = 3;

    // test
    EXPECT_GT(multiple_bits_set, 0);
    EXPECT_FALSE(test::is_positive_pow2(multiple_bits_set));
    EXPECT_EXIT(
        patomic_get_kind(multiple_bits_set),
        test::KilledByAbort(),
        ".*"
    );
}
