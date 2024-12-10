#include <patomic/api/transaction.h>


patomic_transaction_exit_status_t
patomic_transaction_exit_status(
    const unsigned int status
)
{
    return PATOMIC_TRANSACTION_EXIT_STATUS(status);
}


unsigned char
patomic_transaction_abort_reason(
    const unsigned int status
)
{
    return PATOMIC_TRANSACTION_ABORT_REASON(status);
}
