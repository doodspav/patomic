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
            patomic_TABORT_DEBUG,
            patomic_TABORT_NESTED
        };
    }
};

/// @brief Success exit code is zero.
TEST_F(BtApiTransaction, exit_code_success_is_zero)
{
    EXPECT_EQ(0, patomic_TSUCCESS);
}

/// @brief All exit codes are non-negative have no more than 8 significant bits.
TEST_F(BtApiTransaction, exit_code_is_8_bits_nonnegative)
{
    // go through all supported exit codes
    for (auto code : supported_exit_codes())
    {
        EXPECT_GE(code, 0);
        EXPECT_LE(code, 255);
    }
}

/// @brief Only first 8 bits of status is provided for exit code.
TEST_F(BtApiTransaction, exit_code_is_first_8_bits_of_status)
{
    // go through all combinations of 8 bits
    for (unsigned long status = 0ul; status < 0xFFul; ++status)
    {
        EXPECT_EQ(status, patomic_transaction_status_exit_code(status));
        EXPECT_EQ(status, PATOMIC_TRANSACTION_STATUS_EXIT_CODE(status));
    }

    // go into 9th bit with first 8 bits zeroed
    unsigned long status = 0x100ul;
    EXPECT_EQ(0ul, patomic_transaction_status_exit_code(status));
    EXPECT_EQ(0ul, PATOMIC_TRANSACTION_STATUS_EXIT_CODE(status));
}

/// @brief Reason returned is 0 if status is not patomic_TABORT_EXPLICIT.
TEST_F(BtApiTransaction, reason_is_zero_if_not_explicit_abort)
{
    // create other statuses
    // new status kinds may be added, but they are guaranteed to fit in 8 bits
    std::vector<unsigned long> statuses;
    for (unsigned long kind = 0ul; kind < 0xFFul; ++kind)
    {
        // check we don't test explicit abort
        if (kind == patomic_TABORT_EXPLICIT)
        {
            continue;
        }

        // combine status with a non-zero reason
        statuses.push_back(kind | (0xFFul << 8ul));
    }

    // check that the reason for all of these is zero
    for (const unsigned long status : statuses)
    {
        EXPECT_EQ(0u, patomic_transaction_status_abort_reason(status));
        EXPECT_EQ(0u, PATOMIC_TRANSACTION_STATUS_ABORT_REASON(status));
    }
}

/// @brief Reason is returned if status is patomic_TABORT_EXPLICIT.
TEST_F(BtApiTransaction, reason_returned_if_explicit_abort)
{
    // create reasons
    const std::vector<unsigned long> reasons {
        0x55ul,
        0xAAul,
        0x0Ful,
        0xF0ul,
        0xFFul,
        0x00ul
    };

    // check that reason is returned from status
    for (const unsigned long reason : reasons)
    {
        const unsigned long status = (reason << 8u) | patomic_TABORT_EXPLICIT;
        EXPECT_EQ(reason, patomic_transaction_status_abort_reason(status));
        EXPECT_EQ(reason, PATOMIC_TRANSACTION_STATUS_ABORT_REASON(status));
    }
}

/// @brief Check that only first 8 bits of reason are provided.
TEST_F(BtApiTransaction, reason_only_saves_first_8_bits)
{
    // create status with extended reason (more than 8 bits)
    constexpr unsigned long extended_reason = 0xFFFul;
    constexpr unsigned long status = (extended_reason << 8u) | patomic_TABORT_EXPLICIT;

    // check that reason is truncated
    EXPECT_NE(extended_reason, patomic_transaction_status_abort_reason(status));
    EXPECT_NE(extended_reason, PATOMIC_TRANSACTION_STATUS_ABORT_REASON(status));
    EXPECT_EQ(extended_reason & 0xFFu, patomic_transaction_status_abort_reason(status));
    EXPECT_EQ(extended_reason & 0xFFu, PATOMIC_TRANSACTION_STATUS_ABORT_REASON(status));
}
