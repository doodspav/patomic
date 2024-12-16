#ifndef PATOMIC_INTERNAL_TRANSACTION
#define PATOMIC_INTERNAL_TRANSACTION

#include <patomic/api/transaction.h>


/**
 * @addtogroup internal
 *
 * @brief
 *   Create a status from exit code, exit info, and abort reason.
 *
 * @param code
 *   An expression of type patomic_transaction_exit_code_t.
 *
 * @param info
 *   Zero or more combined patomic_transaction_exit_info_t values.
 *
 * @param reason
 *   A non-negative 8bit integer value.
 */
#define PATOMIC_INTERNAL_TRANSACTION_STATUS_CREATE(code, info, reason) \
    ( ((unsigned long) (code)) |                                       \
      ((unsigned long) (info)) |                                       \
      ((unsigned long) (reason)) )


#endif  /* PATOMIC_INTERNAL_TRANSACTION */
