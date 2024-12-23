#ifndef PATOMIC_TEST_SUITE_BT_LOGIC_HPP
#define PATOMIC_TEST_SUITE_BT_LOGIC_HPP

#include <patomic/api/core.h>

#include <test/common/params.hpp>

#include <gtest/gtest.h>


/// @brief
///   Test fixture for implicit logic operations with all memory orders.
class BtLogicImplicit : public testing::TestWithParam<test::ParamsImplicit>
{
public:
    /// @brief
    ///   Run once before every test case in suite.
    void
    SetUp() override;

protected:
    // helper members
    patomic_ops_t m_ops {};
    patomic_align_t m_align {};
};


/// @brief
///   Test fixture for implicit logic operations with store memory orders.
class BtLogicImplicitStore : public BtLogicImplicit
{};


/// @brief
///   Test fixture for implicit logic operations with load memory orders.
class BtLogicImplicitLoad : public BtLogicImplicit
{};


/// @brief
///   Test fixture for explicit logic operations with all memory orders.
class BtLogicExplicit : public testing::TestWithParam<test::ParamsExplicit>
{
public:
    /// @brief
    ///   Run once before every test case in suite.
    void
    SetUp() override;

protected:
    // helper members
    patomic_ops_explicit_t m_ops {};
    patomic_align_t m_align {};
};


/// @brief
///   Test fixture for explicit logic operations with store memory orders.
class BtLogicExplicitStore : public BtLogicExplicit
{};


/// @brief
///   Test fixture for explicit logic operations with load memory orders.
class BtLogicExplicitLoad : public BtLogicExplicit
{};


/// @brief
///   Test fixture for transaction logic operations.
class BtLogicTransaction : public testing::TestWithParam<test::ParamsTransaction>
{
public:
    /// @brief
    ///   Run once before every test case in suite.
    void
    SetUp() override;

protected:
    // helper members
    std::vector<std::size_t> m_widths {};
    patomic_ops_transaction_t m_ops {};
    patomic_transaction_config_t m_config {};
    patomic_transaction_config_wfb_t m_config_wfb {};
};


#endif  // PATOMIC_TEST_SUITE_BT_LOGIC_HPP
