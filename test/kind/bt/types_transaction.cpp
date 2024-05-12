#include <patomic/types/transaction.h>

#include <gtest/gtest.h>

#include <limits>
#include <vector>


/// @brief Reason returned is 0 if status is not patomic_TABORT_EXPLICIT.
TEST(TypesTransaction, ReasonIsZeroIfNotExplicitAbort)
{
    // create other statuses
    // new status kinds may be added, but they are guaranteed to fit in 8 bits
    std::vector<unsigned long> statuses;
    for (unsigned long kind = 0UL; kind < 0xffUL; ++kind)
    {
        // check we don't test explicit abort
        if (kind == patomic_TABORT_EXPLICIT)
        {
            continue;
        }

        // combine status with a non-zero reason
        statuses.push_back(kind | (0xffUL << 8));
    }

    // check that the reason for all of these is zero
    for (unsigned long status : statuses)
    {
        unsigned char reason = patomic_transaction_abort_reason(status);
        EXPECT_EQ(0U, reason);
    }
}

/// @brief Reason is returned if status is patomic_TABORT_EXPLICIT.
TEST(TypesTransaction, ReasonReturnedIfExplicitAbort)
{
    // create reasons
    std::vector<unsigned long> reasons {
        0x55UL,
        0xaaUL,
        0x0fUL,
        0xf0UL,
        0xffUL,
        0UL
    };

    // check that reason is returned from status
    for (unsigned long reason : reasons)
    {
        unsigned long status = (reason << 8) | patomic_TABORT_EXPLICIT;
        EXPECT_EQ(reason, patomic_transaction_abort_reason(status));
    }
}

/// @brief Check that only first 8 bits of reason are provided.
TEST(TypesTransaction, ReasonDoesNotExceedEightBits)
{
    // create status with extended reason (more than 8 bits)
    unsigned long extended_reason = 0xfffUL;
    unsigned long status = (extended_reason << 8) | patomic_TABORT_EXPLICIT;

    // check that reason is truncated
    EXPECT_NE(extended_reason, patomic_transaction_abort_reason(status));
    EXPECT_EQ(extended_reason & 0xFF, patomic_transaction_abort_reason(status));
}
