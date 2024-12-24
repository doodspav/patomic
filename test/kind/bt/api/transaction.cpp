// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

#include <test/common/math.hpp>
#include <test/common/support.hpp>

#include <patomic/api/transaction.h>

#include <gtest/gtest.h>

#include <vector>


/// @brief Test fixture.
class BtApiTransaction : public testing::Test
{
public:
    static std::vector<patomic_transaction_exit_code_t>
    supported_exit_codes()
    {
        return {
            patomic_TSUCCESS,
            patomic_TABORT_UNKNOWN,
            patomic_TABORT_EXPLICIT,
            patomic_TABORT_CONFLICT,
            patomic_TABORT_CAPACITY,
            patomic_TABORT_DEBUG
        };
    }

    static std::vector<patomic_transaction_exit_info_t>
    supported_exit_infos()
    {
        return {
            patomic_TINFO_NONE,
            patomic_TINFO_ZERO_ATTEMPTS,
            patomic_TINFO_FLAG_SET,
            patomic_TINFO_RETRY,
            patomic_TINFO_NESTED
        };
    }
};


/// @brief Success exit code is zero.
TEST_F(BtApiTransaction, exit_code_success_is_zero)
{
    EXPECT_EQ(0, patomic_TSUCCESS);
}

/// @brief All exit codes are non-negative and have no more than 8 significant
///        bits.
TEST_F(BtApiTransaction, exit_code_is_8_bits_nonnegative)
{
    // go through all supported exit codes
    for (auto code : supported_exit_codes())
    {
        EXPECT_GE(code, 0);
        EXPECT_LE(code, 255);
    }
}

/// @brief Only first 8 bits of status are provided for exit code.
TEST_F(BtApiTransaction, exit_code_is_first_8_bits_of_status)
{
    // go through all combinations of 8 bits
    for (unsigned long code = 0ul; code < 0xFFul; ++code)
    {
        unsigned long status = code;
        EXPECT_EQ(code, patomic_transaction_status_exit_code(status));
        EXPECT_EQ(code, PATOMIC_TRANSACTION_STATUS_EXIT_CODE(status));
    }

    // go into 9th bit with first 8 bits zeroed
    unsigned long status = 1ul << 8ul;
    EXPECT_EQ(0ul, patomic_transaction_status_exit_code(status));
    EXPECT_EQ(0ul, PATOMIC_TRANSACTION_STATUS_EXIT_CODE(status));
}


/// @brief None exit info is zero.
TEST_F(BtApiTransaction, exit_info_none_is_zero)
{
    EXPECT_EQ(0, patomic_TINFO_NONE);
}

/// @brief All exit infos are non-negative, have no more than 8 significant
///        bits, and have exactly 1 bit set (except for NONE).
TEST_F(BtApiTransaction, exit_info_is_8_bits_nonnegative_1_bit_set)
{
    // go through all supported exit infos
    for (auto info : supported_exit_infos())
    {
        if (info == patomic_TINFO_NONE)
        {
            continue;
        }
        using E = std::underlying_type_t<decltype(info)>;
        EXPECT_GT(info, 0);
        EXPECT_LE(info, 255);
        EXPECT_TRUE(test::is_positive_pow2(static_cast<E>(info)));
    }
}

/// @brief Only third 8 bits of status are provided for exit info.
TEST_F(BtApiTransaction, exit_info_is_third_8_bits_of_status)
{
    // go through all combinations of 8 bits (offset by 16)
    for (unsigned long info = 0ul; info < 0xFFul; ++info)
    {
        unsigned long status = info << 16ul;
        EXPECT_EQ(info, patomic_transaction_status_exit_info(status));
        EXPECT_EQ(info, PATOMIC_TRANSACTION_STATUS_EXIT_INFO(status));
    }

    // go into 9th bit with first 8 bits zeroed (offset by 16)
    unsigned long status = 1ul << 24ul;
    EXPECT_EQ(0ul, patomic_transaction_status_exit_info(status));
    EXPECT_EQ(0ul, PATOMIC_TRANSACTION_STATUS_EXIT_INFO(status));
}


/// @brief Only second 8 bits of status are provided for abort reason.
TEST_F(BtApiTransaction, abort_reason_is_second_8_bits_of_status)
{
    // go through all combinations of 8 bits (offset by 8)
    for (unsigned long reason = 0ul; reason < 0xFFul; ++reason)
    {
        unsigned long status = reason << 8ul;
        EXPECT_EQ(reason, patomic_transaction_status_abort_reason(status));
        EXPECT_EQ(reason, PATOMIC_TRANSACTION_STATUS_ABORT_REASON(status));
    }

    // go into 9th bit with first 9 bits zeroed (offset by 8)
    unsigned long status = 1ul << 16ul;
    EXPECT_EQ(0ul, patomic_transaction_status_abort_reason(status));
    EXPECT_EQ(0ul, PATOMIC_TRANSACTION_STATUS_ABORT_REASON(status));
}
