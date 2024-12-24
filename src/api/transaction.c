/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception */

#include <patomic/api/transaction.h>


patomic_transaction_exit_code_t
patomic_transaction_status_exit_code(
    const unsigned long status
)
{
    return PATOMIC_TRANSACTION_STATUS_EXIT_CODE(status);
}


unsigned int
patomic_transaction_status_exit_info(
    const unsigned long status
)
{
    return PATOMIC_TRANSACTION_STATUS_EXIT_INFO(status);
}


unsigned char
patomic_transaction_status_abort_reason(
    const unsigned long status
)
{
    return PATOMIC_TRANSACTION_STATUS_ABORT_REASON(status);
}
