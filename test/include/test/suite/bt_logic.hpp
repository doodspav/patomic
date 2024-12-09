#ifndef PATOMIC_TEST_SUITE_BT_LOGIC_HPP
#define PATOMIC_TEST_SUITE_BT_LOGIC_HPP

#include <patomic/patomic.h>

#include <test/common/params.hpp>

#include <gtest/gtest.h>


/// @brief
///   Test fixture for implicit logic operations.
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
///   Test fixture for explicit logic operations.
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
    patomic_ops_transaction_t m_ops {};
};


#endif  // PATOMIC_TEST_SUITE_BT_LOGIC_HPP
