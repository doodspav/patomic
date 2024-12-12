#include <patomic/api/transaction.h>


patomic_transaction_exit_code_t
patomic_transaction_status_exit_code(
    const unsigned long status
)
{
    return PATOMIC_TRANSACTION_STATUS_EXIT_CODE(status);
}


unsigned char
patomic_transaction_status_abort_reason(
    const unsigned long status
)
{
    return PATOMIC_TRANSACTION_STATUS_ABORT_REASON(status);
}
