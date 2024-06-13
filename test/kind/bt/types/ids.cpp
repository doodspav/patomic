#include <patomic/types/ids.h>

#include <gtest/gtest.h>

#include <map>


/// @brief Test fixture.
class BtTypesIds : public testing::Test
{
public:
    const std::map<patomic_id_t, patomic_kind_t> impls_id_to_kind {
        { patomic_id_NULL, patomic_kind_UNKN }
    };
};

/// @brief Test fixture for death tests.
class BtTypesIds_DeathTest : public testing::Test
{};


/// @brief The value of patomic_kinds_ALL is exactly the value of all the other
///        kinds combined.
TEST_F(BtTypesIds, all_kinds_matches_exactly_each_kind)
{}

/// @brief The value of patomic_ids_ALL is at least the value of all the other
///        ids combined.
TEST_F(BtTypesIds, all_ids_matches_at_least_each_id)
{}

/// @brief All ids match an an expected id, and each expected id is present in
///        the set of all ids.
TEST_F(BtTypesIds, all_ids_are_expected)
{}

/// @brief All combinations of valid kinds yields the expected combinations of
///        ids.
TEST_F(BtTypesIds, get_ids_gives_correct_ids_for_all_kind_combinations)
{}

/// @brief Invalid kinds are ignored and if no valid kinds are provided, the
///        NULL implementation id is returned.
TEST_F(BtTypesIds, get_ids_ignores_invalid_kinds)
{}

/// @brief The corresponding kind is returned for each valid id.
TEST_F(BtTypesIds, get_kind_gives_correct_kind_for_all_ids)
{}

/// @brief The UNKN kind is returned for any invalid id.
TEST_F(BtTypesIds, get_kind_gives_kind_unkn_for_invalid_id)
{}

/// @brief Passing multiple ids (multiple bits set) is fatally asserted.
TEST_F(BtTypesIds_DeathTest, get_kinds_asserts_on_multiple_ids)
{}
