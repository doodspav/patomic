#include <patomic/types/transaction.h>


unsigned char
patomic_transaction_abort_reason(
    const unsigned int status
)
{
    /* declarations */
    unsigned int kind;
    unsigned int reason;

    /* first 8 bits are the kind of status */
    /* check that explicit abort happened */
    kind = status & 0xffU;
    if (kind != patomic_TABORT_EXPLICIT)
    {
        return 0;
    }

    /* next 8 bits are the abort reason */
    reason = (status >> 8U) & 0xffU;
    return (unsigned char) reason;
}
