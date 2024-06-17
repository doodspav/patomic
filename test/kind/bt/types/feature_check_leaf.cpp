#include <patomic/types/feature_check.h>

#include <test/common/make_ops.hpp>

#include <gtest/gtest.h>

#include <string>
#include <type_traits>


/// @brief Test fixture.
class BtTypesFeatureCheckLeaf : public testing::Test
{};

/// @brief Templated test fixture.
template <class T>
class BtTypesFeatureCheckLeafT : public testing::Test
{
public:
    static constexpr test::ops_domain domain = T::value;
    using OpsTypes = test::ops_types<domain>;
};

using BtTypesFeatureCheckLeafT_Types = ::testing::Types<
    std::integral_constant<test::ops_domain, test::ops_domain::IMPLICIT>,
    std::integral_constant<test::ops_domain, test::ops_domain::EXPLICIT>,
    std::integral_constant<test::ops_domain, test::ops_domain::TRANSACTION>
>;

namespace
{

/// @brief Helper type for templated test fixture.
class TTestHelper
{
public:
    template <class T>
    static std::string
    GetName(int);

    template <>
    std::string
    GetName<std::integral_constant<test::ops_domain, test::ops_domain::IMPLICIT>>(int)
    {
        return "implicit";
    }

    template <>
    std::string
    GetName<std::integral_constant<test::ops_domain, test::ops_domain::EXPLICIT>>(int)
    {
        return "explicit";
    }

    template <>
    std::string
    GetName<std::integral_constant<test::ops_domain, test::ops_domain::TRANSACTION>>(int)
    {
        return "transaction";
    }

    static unsigned int
    check_leaf(const patomic_ops_t& ops, patomic_opcat_t opcat,
               unsigned int opkinds) noexcept
    {
        return patomic_feature_check_leaf(&ops, opcat, opkinds);
    }

    static unsigned int
    check_leaf(const patomic_ops_explicit_t& ops, patomic_opcat_t opcat,
               unsigned int opkinds) noexcept
    {
        return patomic_feature_check_leaf_explicit(&ops, opcat, opkinds);
    }

    static unsigned int
    check_leaf(const patomic_ops_transaction_t& ops, patomic_opcat_t opcat,
               unsigned int opkinds) noexcept
    {
        return patomic_feature_check_leaf_transaction(&ops, opcat, opkinds);
    }
};

}  // namespace

TYPED_TEST_SUITE(
    BtTypesFeatureCheckLeafT,
    BtTypesFeatureCheckLeafT_Types,
    TTestHelper
);
