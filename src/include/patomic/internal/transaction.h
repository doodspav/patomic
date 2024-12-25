/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

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
 *   A single patomic_transaction_exit_code_t value.
 *
 * @param info
 *   Zero or more combined patomic_transaction_exit_info_t values.
 *
 * @param reason
 *   A non-negative 8bit integer value.
 */
#define PATOMIC_INTERNAL_TRANSACTION_STATUS_CREATE(code, info, reason) \
    ( (((unsigned long) (code)) & 0xFFul)           |                  \
      ((((unsigned long) (info)) & 0xFFul) << 16ul) |                  \
      ((((unsigned long) (reason)) & 0xFFul) << 8ul) )


#endif  /* PATOMIC_INTERNAL_TRANSACTION */
