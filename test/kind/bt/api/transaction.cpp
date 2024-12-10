#include <patomic/api/transaction.h>

#include <gtest/gtest.h>

#include <vector>


/// @brief Test fixture.
class BtApiTransaction : public testing::Test
{};


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
        const unsigned char reason = patomic_transaction_abort_reason(status);
        EXPECT_EQ(0u, reason);
        EXPECT_EQ(0u, PATOMIC_TRANSACTION_ABORT_REASON(status));
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
        EXPECT_EQ(reason, patomic_transaction_abort_reason(status));
        EXPECT_EQ(reason, PATOMIC_TRANSACTION_ABORT_REASON(status));
    }
}

/// @brief Check that only first 8 bits of reason are provided.
TEST_F(BtApiTransaction, reason_only_saves_first_8_bits)
{
    // create status with extended reason (more than 8 bits)
    constexpr unsigned long extended_reason = 0xFFFul;
    constexpr unsigned long status = (extended_reason << 8u) | patomic_TABORT_EXPLICIT;

    // check that reason is truncated
    EXPECT_NE(extended_reason, patomic_transaction_abort_reason(status));
    EXPECT_NE(extended_reason, PATOMIC_TRANSACTION_ABORT_REASON(status));
    EXPECT_EQ(extended_reason & 0xFFu, patomic_transaction_abort_reason(status));
    EXPECT_EQ(extended_reason & 0xFFu, PATOMIC_TRANSACTION_ABORT_REASON(status));
}
