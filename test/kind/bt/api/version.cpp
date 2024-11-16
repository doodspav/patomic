#include <patomic/api/version.h>

#include <gtest/gtest.h>

#include <regex>
#include <string>


/// @brief Test fixture.
class BtApiVersion : public testing::Test
{
public:
    const std::regex semver_regex {
        R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"
    };
};


/// @brief Return value of patomic_version_string() compares equal to
///        PATOMIC_VERSION_STRING.
TEST_F(BtApiVersion, version_string_fn_cmp_eq_macro)
{
    // setup
    constexpr auto macro = PATOMIC_VERSION_STRING;
    const auto fnval = patomic_version_string();

    // test
    EXPECT_STREQ(macro, fnval);
}

/// @brief Return value of patomic_version_major() compares equal to
///        PATOMIC_VERSION_MAJOR.
TEST_F(BtApiVersion, version_major_fn_cmp_eq_macro)
{
    // setup
    constexpr auto macro = PATOMIC_VERSION_MAJOR;
    const auto fnval = patomic_version_major();

    // test
    EXPECT_EQ(macro, fnval);
}

/// @brief Return value of patomic_version_minor() compares equal to
///        PATOMIC_VERSION_MINOR.
TEST_F(BtApiVersion, version_minor_fn_cmp_eq_macro)
{
    // setup
    constexpr auto macro = PATOMIC_VERSION_MINOR;
    const auto fnval = patomic_version_minor();

    // test
    EXPECT_EQ(macro, fnval);
}

/// @brief Return value of patomic_version_patch() compares equal to
///        PATOMIC_VERSION_PATCH.
TEST_F(BtApiVersion, version_patch_fn_cmp_eq_macro)
{
    // setup
    constexpr auto macro = PATOMIC_VERSION_PATCH;
    const auto fnval = patomic_version_patch();

    // test
    EXPECT_EQ(macro, fnval);
}

/// @brief PATOMIC_VERSION_STRING matches SemVer regex.
TEST_F(BtApiVersion, version_string_matches_semver_regex)
{
    // test
    EXPECT_TRUE(std::regex_match(PATOMIC_VERSION_STRING, semver_regex));
}

/// @brief PATOMIC_VERSION_STRING major component compares equal to
///        PATOMIC_VERSION_MAJOR.
TEST_F(BtApiVersion, version_string_major_component_matches_version_major)
{
    // get major component as int
    std::cmatch sub_matches;
    ASSERT_TRUE(std::regex_match(
        PATOMIC_VERSION_STRING, sub_matches, semver_regex));
    const int major = std::stoi(sub_matches[1].str());

    // test
    EXPECT_EQ(major, PATOMIC_VERSION_MAJOR);
}

/// @brief PATOMIC_VERSION_STRING minor component compares equal to
///        PATOMIC_VERSION_MINOR.
TEST_F(BtApiVersion, version_string_minor_component_matches_version_minor)
{
    // get major component as int
    std::cmatch sub_matches;
    ASSERT_TRUE(std::regex_match(
        PATOMIC_VERSION_STRING, sub_matches, semver_regex));
    const int minor = std::stoi(sub_matches[2].str());

    // test
    EXPECT_EQ(minor, PATOMIC_VERSION_MINOR);
}

/// @brief PATOMIC_VERSION_STRING major component compares equal to
///        PATOMIC_VERSION_MAJOR.
TEST_F(BtApiVersion, version_string_patch_component_matches_version_patch)
{
    // get major component as int
    std::cmatch sub_matches;
    ASSERT_TRUE(std::regex_match(
        PATOMIC_VERSION_STRING, sub_matches, semver_regex));
    const int patch = std::stoi(sub_matches[3].str());

    // test
    EXPECT_EQ(patch, PATOMIC_VERSION_PATCH);
}

/// @brief Library is not compatible with major versions that do not compare
///        equal to PATOMIC_VERSION_MAJOR.
TEST_F(BtApiVersion, version_not_compatible_major_ne)
{
    // setup
    constexpr int major = PATOMIC_VERSION_MAJOR;
    for (const int bad_major : { major - 1, major + 1 })
    {

        // test
        EXPECT_FALSE(PATOMIC_VERSION_COMPATIBLE_WITH(bad_major, 0, 0));
        EXPECT_FALSE(patomic_version_compatible_with(bad_major, 0, 0));
    }
}

/// @brief Library is not compatible with minor versions that compare greater
///        than PATOMIC_VERSION_MINOR when the major version compares equal
///        to PATOMIC_VERSION_MAJOR.
TEST_F(BtApiVersion, version_not_compatible_major_eq_minor_gt)
{
    // setup
    constexpr int major = PATOMIC_VERSION_MAJOR;
    constexpr int minor_gt = PATOMIC_VERSION_MINOR + 1;

    // test
    EXPECT_FALSE(PATOMIC_VERSION_COMPATIBLE_WITH(major, minor_gt, 0));
    EXPECT_FALSE(patomic_version_compatible_with(major, minor_gt, 0));
}

/// @brief Library is not compatible with patch versions that compare greater
///        than PATOMIC_VERSION_PATCH when the major and minor versions compare
///        equal to PATOMIC_VERSION_MAJOR and PATOMIC_VERSION_MINOR
///        respectively.
TEST_F(BtApiVersion, version_not_compatible_major_eq_minor_eq_patch_gt)
{
    // setup
    constexpr int major = PATOMIC_VERSION_MAJOR;
    constexpr int minor = PATOMIC_VERSION_MINOR;
    constexpr int patch_gt = PATOMIC_VERSION_PATCH + 1;

    // test
    EXPECT_FALSE(PATOMIC_VERSION_COMPATIBLE_WITH(major, minor, patch_gt));
    EXPECT_FALSE(patomic_version_compatible_with(major, minor, patch_gt));
}

/// @brief Library is compatible with minor versions that compare less than
///        PATOMIC_VERSION_MINOR when the major version compares equal to
///        PATOMIC_VERSION_MAJOR and the patch version has any value.
TEST_F(BtApiVersion, version_compatible_major_eq_minor_lt_patch_any)
{
    // setup
    constexpr int patch = PATOMIC_VERSION_PATCH;
    for (const int patch_any : { -1, 0, patch - 1, patch, patch + 1 })
    {
        constexpr int major = PATOMIC_VERSION_MAJOR;
        constexpr int minor_lt = PATOMIC_VERSION_MINOR - 1;

        // test
        EXPECT_TRUE(PATOMIC_VERSION_COMPATIBLE_WITH(major, minor_lt, patch_any));
        EXPECT_TRUE(patomic_version_compatible_with(major, minor_lt, patch_any));
    }
}

/// @brief Library is compatible with minor versions that compare equal to
///        PATOMIC_VERSION_MINOR when the major version compares equal to
///        PATOMIC_VERSION_MAJOR and the patch version compares less than or
///        equal to PATOMIC_VERSION_PATCH.
TEST_F(BtApiVersion, version_compatible_major_eq_minor_eq_patch_le)
{
    // setup
    constexpr int patch = PATOMIC_VERSION_PATCH;
    for (const int patch_le : { patch - 1, patch })
    {
        constexpr int major = PATOMIC_VERSION_MAJOR;
        constexpr int minor = PATOMIC_VERSION_MINOR;

        // test
        EXPECT_TRUE(PATOMIC_VERSION_COMPATIBLE_WITH(major, minor, patch_le));
        EXPECT_TRUE(patomic_version_compatible_with(major, minor, patch_le));
    }
}
